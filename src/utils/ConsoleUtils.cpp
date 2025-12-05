//
// Created by tetram26 on 04/08/25.
//
#include <sstream>
#include <iostream>
#include <string>
#include <utils/ConsoleUtils.h>
using namespace std;

namespace utils {
    ConsoleUtils ConsoleUtils::instance;

    void ConsoleUtils::printMessage(const std::string &message) {
        consoleMutex.lock();
        std::cout << message << std::endl;
        consoleMutex.unlock();
    }

    void ConsoleUtils::printerr(const std::string &error) {
        consoleMutex.lock();
        std::cerr << error << std::endl;
        consoleMutex.unlock();
    }

    void ConsoleUtils::getLine(string &variable) {
        inputMutex.lock();
        std::getline(std::cin, variable);
        inputMutex.unlock();
    }

    std::string ConsoleUtils::toString(const int num) {
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }

    utils::UTF16String ConsoleUtils::createUTF16String(const std::string &msg) {
        utils::UTF16String us;

        for (int i = 0; i < msg.length(); i++) {
            us.append((unsigned short) msg[i]);
        }
        // now us contains the same text, widened
        return us;
    }

    ConsoleUtils &ConsoleUtils::getInstance() {
        return instance;
    }

    ConsoleUtils::ConsoleUtils() {
    }
}
