//
// Created by tetram26 on 04/08/25.
//
#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#include "Mutex.h"
#include "UTF16String.h"

namespace utils {
    class ConsoleUtils {
        public:
            static ConsoleUtils& getInstance();
            void printMessage(const std::string &);
            void printerr(const std::string &);
            void getLine(std::string &);
            static std::string toString(int) ;
            static utils::UTF16String createUTF16String(const std::string&);
        private:
            ConsoleUtils();
            Mutex consoleMutex;
            Mutex inputMutex;
            static ConsoleUtils instance;
    };
}
#endif //CONSOLEUTILS_H