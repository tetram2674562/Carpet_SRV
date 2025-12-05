//
// Created by tetram26 on 31/07/25.
//
#include <iostream>
#include <unistd.h>
#include <network/Connection.h>
#include <sys/socket.h>

#include "entity/player/Player.h"
#include "packet/handshake/ClientProtocolPacket.h"
#include "packet/handshake/KickPacket.h"
#include "packet/handshake/ServerPingPacket.h"
#include "packet/login/LoginPacket.h"
#include "packet/login/ServerAuthDataPacket.h"
#include "packet/login/SharedKeyPacket.h"
#include "packet/play/KeepAlivePacket.h"
#include "packet/play/player/ClientInfoPacket.h"
#include "packet/play/player/PlayerPositionPacket.h"
#include "utils/ConsoleUtils.h"
#include "utils/LockGuard.h"

#include "utils/UTF16String.h"

using namespace std;
using namespace utils;

namespace network {
    Connection::Connection(const int client_FD,entity::Player& player): client_FD(client_FD), lastActivity(time(NULL)), player(&player) {
        this->cipher = NULL;
    }

    Connection::~Connection() {
        if (client_FD != -1) close(client_FD);
    }

    void Connection::addPacketToQueue(packet::Packet *packet) {
        LockGuard qlg(this->queueMutex);
        this->queue.push_back(packet);
    }

    void Connection::sendAndFlushQueue() {
        std::vector<packet::Packet*> localQueue;
        {
            LockGuard qlg(this->queueMutex);
            localQueue.swap(this->queue);
        }

        for (size_t i = 0; i < localQueue.size(); ++i) {
            packet::Packet* p = localQueue[i];
            if (p == NULL) continue;

            packet::Buffer tmpBuf;
            p->writeData(tmpBuf);

            int fd_local;
            crypto::AESCipher* cipher_local;
            {
                LockGuard sl(this->stateMutex);
                fd_local = this->client_FD;
                cipher_local = this->cipher;
            }

            if (fd_local != -1) {
                if (cipher_local != NULL) {
                    vector<unsigned char> encryptedData = cipher_local->encrypt(tmpBuf.getDataBuffer());
                    send(fd_local, encryptedData.data(), encryptedData.size(), 0);
                } else {
                    const std::vector<unsigned char>& db = tmpBuf.getDataBuffer();
                    if (!db.empty()) {
                        send(fd_local, db.data(), db.size(), 0);
                    }
                }
            }

            delete p;
        }
    }

    bool Connection::handleConnection(entity::Player &player) {
        this->player = &player;
        bool login = performLoginSequence();
        if (!login) {
            disconnect(utils::UTF16String("Login failed"));
        }
        return login;
    }

    void Connection::handlePackets() {
        vector<unsigned char> rawData;
        vector<unsigned char> decryptedData;
        rawData.clear();
        decryptedData.clear();
        if (recvPacket(rawData)) {
            if (!rawData.empty()) {
                // Avoid data race (I hate you data race)
                crypto::AESCipher* cipher_local;
                entity::Player* player_local;
                {
                    LockGuard sl(this->stateMutex);
                    cipher_local = this->cipher;
                    player_local = this->player;
                }
                if (cipher_local == NULL) return;
                decryptedData = cipher_local->decrypt(rawData);
                packet::Buffer packetBuffer = packet::Buffer(decryptedData);
                unsigned char pid = packetBuffer.readByte();
                switch (pid) {
                    // client info
                    case 0xCC: {
                        packet::ClientInfoPacket infoPacket;
                        infoPacket.readData(packetBuffer);
                        if (player_local) {
                            player_local->setLanguage(infoPacket.getLanguage());
                            player_local->setShowCape(infoPacket.getShowCape());
                            player_local->setRenderDistance(infoPacket.getRenderDistance());
                        }
                        break;
                    }
                    case 0x00: {
                        packet::KeepAlivePacket* keepAlivePacket = new packet::KeepAlivePacket();
                        addPacketToQueue(keepAlivePacket);
                        break;
                    }
                    case 0x0B: {
                        // player position packet
                        packet::PlayerPositionPacket positionPacket;
                        positionPacket.readData(packetBuffer);
                        if (player_local) {
                            player_local->setPosition(positionPacket.getX(), positionPacket.getY(), positionPacket.getZ());
                        }
                        break;
                    }
                    default:
                        utils::ConsoleUtils::getInstance().printerr("Unknown packet ID: " + utils::ConsoleUtils::toString(pid));
                        break;
                }

                sendAndFlushQueue();
            }

            if (time(NULL) - lastActivity > 20) {
                disconnect(utils::UTF16String("Timed out"));
            }
        }
    }


    utils::UTF16String serializeServerInfo(const std::string &version,
                                       const std::string &motd,
                                       const int currentPlayers,
                                       const int maxPlayers) {
        std::vector<utils::UTF16String> data;
        data.push_back(utils::UTF16String("1"));
        data.push_back(utils::UTF16String("51"));
        data.push_back(utils::UTF16String(version));
        data.push_back(utils::UTF16String(motd));
        data.push_back(utils::UTF16String(utils::ConsoleUtils::toString(currentPlayers)));
        data.push_back(utils::UTF16String(utils::ConsoleUtils::toString(maxPlayers)));

        utils::UTF16String joined;
        joined.append(0x00A7);

        for (int i = 0; i < data.size(); i++) {
            joined += data[i];
            joined.append(0x0000);
        }

        return joined;
    }

    bool network::Connection::performLoginSequence() {

        // 1. Receive handshake (0x02)
        vector<unsigned char> rawData;
        if (!recvPacket(rawData)) {
            utils::ConsoleUtils::getInstance().printerr("Failed to receive handshake.");
            return false;
        }

        // Current packet id
        packet::Buffer buffer(rawData);
        unsigned char pid = buffer.readByte();

        // If the packet id is 254 -> server ping
        if (pid == 0xFE) {
            packet::ServerPingPacket serverPing;
            serverPing.readData(buffer);
            packet::KickPacket pingResponsePacket(serializeServerInfo("1.4.7", "A server running Carpet SRV", 0, 20));
            buffer.clearBuffer();
            pingResponsePacket.writeData(buffer);
            int fd_local;
            {
                LockGuard sl(this->stateMutex);
                fd_local = this->client_FD;
            }
            send(fd_local, buffer.getDataBuffer().data(), buffer.getDataBuffer().size(), 0);

            return false;
        }

        if (pid != 0x02) {
            utils::ConsoleUtils::getInstance().printerr("Expected handshake (0x02), got: " + utils::ConsoleUtils::toString(pid));
            return false;
        }

        packet::ClientProtocolPacket clientProtocolPacket;
        clientProtocolPacket.readData(buffer);
        if (clientProtocolPacket.getProtocolVersion() != 51) {
            utils::ConsoleUtils::getInstance().printerr("Client protocol version doesn't fit 1.4.7...");
            return false;
        }

        // set username under state lock
        {
            LockGuard sl(this->stateMutex);
            if (this->player) this->player->setUsername(clientProtocolPacket.getUsername());
        }
        utils::ConsoleUtils::getInstance().printMessage("[Server] Got connection from " + clientProtocolPacket.getServerHost() + " with username \\\"" + this->player->getName() + "\\\"");

        // 2. Send encryption key request (0xFD)
        packet::ServerAuthDataPacket serverAuthDataPacket;
        buffer.clearBuffer();
        serverAuthDataPacket.writeData(buffer);
        {
            LockGuard sl(this->stateMutex);
            send(this->client_FD, buffer.getDataBuffer().data(), buffer.getDataBuffer().size(), 0);
        }

        // 3. Receive encrypted shared key (0xFC)
        rawData.clear();
        if (!recvPacket(rawData)) {
            utils::ConsoleUtils::getInstance().printerr("Failed to receive encryption response packet.");
            return false;
        }

        buffer = packet::Buffer(rawData);
        pid = buffer.readByte();
        if (static_cast<int>(pid) != 0xFC) {
            utils::ConsoleUtils::getInstance().printerr("Expected shared key packet (0xFC), got: " + utils::ConsoleUtils::toString(pid));
            return false;
        }

        packet::SharedKeyPacket sharedKeyPacket = packet::SharedKeyPacket();
        // 4. Decrypt shared key and verify
        sharedKeyPacket.readData(buffer);
        if (serverAuthDataPacket.getVerifyToken() != sharedKeyPacket.getVerifyToken()) {
            utils::ConsoleUtils::getInstance().printMessage("Excepted an exact same verify token");
            return false;
        }

        //cout << "5. Set up AES cipher" << endl;
        {
            LockGuard sl(this->stateMutex);
            this->cipher = new crypto::AESCipher(sharedKeyPacket.getSharedSecret());
        }
        // 6. Send encryption response
        buffer.clearBuffer();
        sharedKeyPacket.writeData(buffer);
        {
            LockGuard sl(this->stateMutex);
            send(this->client_FD, buffer.getDataBuffer().data(), buffer.getDataBuffer().size(), 0);
        }

        // 7. Send login success (0x01 or server auth data)
        buffer.clearBuffer();
        packet::LoginPacket loginPacket;
        loginPacket.writeData(buffer);
        vector<unsigned char> packetData;
        {
            LockGuard sl(this->stateMutex);
            packetData = this->cipher->encrypt(buffer.getDataBuffer());
            if (!packetData.empty()) send(this->client_FD, packetData.data(), packetData.size(), 0);
        }

        // 8. Ready to PLAY.
        return true;
    }

    int getExpectedPacketSize(uint8_t packetId) {
        // TODO Implement this.
        switch (packetId) {
            case 0x02: return -1;
            case 0xFD: return -1;
            case 0xFC: return -1;
            case 0x01: return 1;
            default: return -1;
        }
    }

    bool network::Connection::recvPacket(std::vector<unsigned char> &outBuffer) {
        outBuffer.clear();

        int fd_local;
        {
            LockGuard sl(this->stateMutex);
            fd_local = this->client_FD;
        }
        if (fd_local == -1) return false;

        char packetId;
        ssize_t charsRead = recv(fd_local, &packetId, 1, MSG_WAITALL);
        if (charsRead <= 0) return false;

        outBuffer.push_back(packetId);

        int expectedSize = getExpectedPacketSize(static_cast<uint8_t>(packetId));
        if (expectedSize > 0) {
            std::vector<char> rest(expectedSize);
            ssize_t restRead = recv(fd_local, rest.data(), expectedSize, MSG_WAITALL);
            if (restRead <= 0) return false;
            outBuffer.insert(outBuffer.end(), rest.begin(), rest.end());
            return true;
        }

        char tempBuf[1024];
        ssize_t len = recv(fd_local, tempBuf, sizeof(tempBuf), MSG_DONTWAIT);
        if (len > 0) {
            outBuffer.insert(outBuffer.end(), tempBuf, tempBuf + len);
        }

        return true;
    }

    bool Connection::isAlive() {
        // stupid function... I hate data race !
        LockGuard sl(this->stateMutex);
        return this->client_FD != -1;
    }

    void Connection::disconnect(const utils::UTF16String &reason) {
        packet::KickPacket* kickPacket = new packet::KickPacket(reason);
        addPacketToQueue(kickPacket);
        sendAndFlushQueue();

        {
            LockGuard sl(this->stateMutex);
            if (this->client_FD != -1) {
                close(this->client_FD);
                this->client_FD = -1;
            }
        }

        if (this->player) {
            utils::ConsoleUtils::getInstance().printMessage("Disconnected: " + this->player->getName());
        } else {
            utils::ConsoleUtils::getInstance().printMessage("Disconnected: (unknown)");
        }
    }

    void Connection::handlePacket(packet::Buffer &buffer) {
        // Read the packet id
        switch (buffer.readByte()) {
            //case 0xFE: this->handlePacket(packet::ServerPingPacket(buffer));
            default: return;
        };
        // Then call the function again but with the packet
    }

    void Connection::handlePacket(packet::ServerPingPacket &packet) {

    }

}
