//
// Created by tetram26 on 30/07/25.
//
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <thread>

namespace command {
class CommandHandler {
public:
  bool isRunning() const;
  static void CommandListener(CommandHandler *);
  static void initCommandHandler();
  void shutdown();
  static CommandHandler &getInstance();

private:
  CommandHandler();
  bool running;
  static CommandHandler instance;
  std::thread command_listener_thread;
};
} // namespace command
#endif // COMMANDHANDLER_H
