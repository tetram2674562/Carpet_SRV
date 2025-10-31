//
// Created by tetram26 on 31/07/25.
//
#include "packet/Buffer.h"

#include <cstring>
#include <iostream>
#include <bits/stdint-uintn.h>
#include <sys/types.h>

#include "utils/ConsoleUtils.h"
using namespace std;
namespace packet {
/** Read a char from the buffer
     *
     * @return A char
     */
    unsigned char Buffer::readByte(){
        return this->data[this->index++];
    }

    /** Read a short from the buffer
     *
     * @return A short
     */
    short Buffer::readShort(){
        unsigned char part1 = this->data[this->index++];
        unsigned char part2 = this->data[this->index++];
        return (short)(part1 << 8 | part2);
    }

    /** Read a char from the buffer
     *
     * @return A char
     */
    char Buffer::readChar() {
        return (char)readShort();
    }

    /** Read a string from the buffer
     *
     * @return The string
     */
    string Buffer::readString(int maxSize) {
        const short stringLen = this->readShort();
        string result;
        if (stringLen > maxSize)
        {
            utils::ConsoleUtils::getInstance().printerr("Received string length longer than maximum allowed (> "+utils::ConsoleUtils::toString(maxSize)+")");
        }
        else if (stringLen < 0) {
            utils::ConsoleUtils::getInstance().printerr("Received string length negative (< 0)");
        }
        else {
            for (uint i = 0; i < stringLen; i++) {
                result += readChar();
            }
        }
        return result;
    }

    double Buffer::readDouble(){
        if (index + 8 > this->data.size()) {
            utils::ConsoleUtils::getInstance().printerr("Not enough chars to read a double");
            return 0.0;
        }

        uint64_t raw = 0;
        for (int i = 0; i < 8; ++i) {
            raw = (raw << 8) | static_cast<uint8_t>(this->data[index + i]);
        }
        index += 8;
        double value;
        std::memcpy(&value, &raw, sizeof(double));
        return value;
    }

    void Buffer::writeDouble(double number) {
        uint64_t raw;
        std::memcpy(&raw, &number, sizeof(double));

        for (int i = 7; i >= 0; --i) {
            this->data.push_back(static_cast<char>((raw >> (i * 8)) & 0xFF));
        }
    }

    /** Write a char into the buffer
     *
     * @param number A char
     */
    void Buffer::writeByte(const unsigned char byte) {
        this->data.push_back(byte);
    }

    /** Write a short into the buffer
     *
     * @param number A short
     */
    void Buffer::writeShort(const short number) {
        this->data.push_back(static_cast<unsigned char>((number >> 8) & 0xFF));
        this->data.push_back(static_cast<unsigned char>(number & 0xFF));
    }

    /** Write a char to the buffer
     *
     * @param c A char
     */
    void Buffer::writeChar(char c) {
        this->data.push_back(static_cast<char>(0x00)); // high char
        this->data.push_back(static_cast<char>(c)); // low char
    }

    /** Write a string to the buffer
     *
     * @param str A string
     */
    void Buffer::writeString(const string &str) {
        writeShort(static_cast<short>(str.size()));
        for (int i = 0; i < str.size(); i++) {
            writeChar(str[i]);
        }
    }

    void Buffer::writeUTF16String(const basic_string<unsigned short> &str) {
        writeShort(str.size());

        for (int i = 0; i < str.size(); i++) {
            writeUTF16Char(str[i]);
        }
    }
    void Buffer::writeUTF16Char(unsigned short ch) {
        this->data.push_back(static_cast<unsigned char>(ch >> 8));     // high char
        this->data.push_back(static_cast<unsigned char>(ch & 0xFF));
    }

    void Buffer::writeBytes(const vector<unsigned char> &bytes) {
        //cout << chars.size() << endl;
        writeShort(static_cast<short>(bytes.size()));
        for (int i = 0; i < bytes.size(); i++) {
            writeByte(bytes[i]);
        }
    }

    vector<unsigned char> Buffer::readBytes() {
        int16_t len = readShort();
        if (len < 0) {
            throw std::runtime_error("Negative char array length in packet.");
        }

        if (static_cast<size_t>(index) + len > data.size()) {
            throw std::runtime_error("Not enough chars left in packet to read char array.");
        }

        vector<unsigned char> result(len);
        std::memcpy(result.data(), &data[index], len);
        index += len;
        return result;
    }

    void Buffer::writeInt(const int number) {
        data.push_back((char)((number >> 24) & 0xFF));
        data.push_back((char)((number >> 16) & 0xFF));
        data.push_back((char)((number >>  8) & 0xFF));
        data.push_back((char)(number & 0xFF));
    }

    Buffer::Buffer(std::vector<unsigned char>& bytesArray) : index(0), data(bytesArray) {
    }

    Buffer::Buffer() : index(0){
    }

    void Buffer::clearBuffer() {
        this->index = 0;
        this->data.clear();
    }

    vector<unsigned char>& Buffer::getDataBuffer() {
        return this->data;
    }


    int Buffer::readInt() {
        return (readByte() << 24) + (readByte() << 16) + (readByte() << 8) + readByte();
    }


    bool Buffer::readBool() {
        return readByte() != 0;
    }



}