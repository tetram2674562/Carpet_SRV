#include <csignal>
#include <iostream>
#include <sys/socket.h>

#include "server/VanillaMinecraftServer.h"
void cleanup() {
    if (server::VanillaMinecraftServer::getServer().isRunning()) {
        std::cout << "[Server] Server stopped!\n";
        server::VanillaMinecraftServer::getServer().shutdown();
        close(server::VanillaMinecraftServer::getServer().getServerSocket());
        OPENSSL_cleanup();
        std::cout << "[Done]" << std::endl;
    }
}
void handleSignal(int sig) {
    std::cout << "\n[Signal] Caught signal: " << sig << ", cleaning up...\n";
    cleanup();
    std::_Exit(0);
}


int main() {
    // very messy, should rework that...
    utils::ConsoleUtils::getInstance().printMessage("Initializing...");
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);
    atexit(cleanup);
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    signal(SIGPIPE, SIG_IGN);
    utils::ConsoleUtils::getInstance().printMessage("Emergency saving and encryption successfully initialized!");
    utils::ConsoleUtils::getInstance().printMessage("Starting server...");
    server::VanillaMinecraftServer::startServer();

    return 0;
}


