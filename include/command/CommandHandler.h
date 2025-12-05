//
// Created by tetram26 on 30/07/25.
//
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "utils/ThreadUtils.h"

namespace command {
    class CommandHandler {
        public:
            bool isRunning() const;
            static void *CommandListener(void *);
            static void initCommandHandler();
            void shutdown();
            static CommandHandler& getInstance();


        private:
            CommandHandler();
            bool running;
            static CommandHandler instance;
            utils::Thread command_listener_thread;
    };
}
#endif //COMMANDHANDLER_H
