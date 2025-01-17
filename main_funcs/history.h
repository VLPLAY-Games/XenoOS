class History {
  private:
    Help help;                // Объект для управления справкой
    const char* history_commands[2] = {"clear", "help"};
  public:
    void handle_history_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() > 2) {
        Serial.println("Wrong history command");
        return;
      }

      if (command.size() == 1) {
        Cat cat(sd);
        cat.read_file(historylog_path);
        return;
      }
      
      // Команда "help" для вывода справки
      if (command[1] == "help") {
        Serial.print("Available history commands: ");
        help.print_help(history_commands, sizeof(history_commands) / sizeof(history_commands[0]));
        return;
      } else if (command[1] == "clear") {
        sd.deleteFile(SD, historylog_path);
        sd.create_empty_file(historylog_path);
        return;
      }

      
    }

    void write_history(const char *message, SdCard& sd) {
        sd.appendFile(SD, historylog_path, message, true);
    }
};