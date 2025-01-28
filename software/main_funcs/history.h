class History {
  private:
    Help help;
    const char* history_commands[3] = {"clear", "help", "read"};  // Обновленный массив команд

  public:
    void handle_history_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete history command");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available history commands: ");
        help.print_help(history_commands, sizeof(history_commands) / sizeof(history_commands[0]));
        return;
      }
      
      if (command[1] == "clear") {
        sd.deleteFile(SD, historylog_path);
        sd.create_empty_file(historylog_path);
        return;
      }

      if (command[1] == "read") {
        Cat cat(sd);
        cat.read_file(historylog_path);
        return;
      }

      Serial.println("Unknown history command");
    }

    void write_history(const char *message, SdCard& sd) {
        sd.appendFile(SD, historylog_path, message, true);
    }
};