class Mac {
  private:
    const char* mac_commands[2] = {"info", "help"}; // Массив строк с командами
    Help help;
  public:

    void handle_mac_commands(const std::vector<String>& command, Esp esp) {
        if (command.size() < 2) {
          Serial.println("Incomplete mac command");
          return;
        }

        // Обработка команды "info"
        if (command[1] == "info") {
          Serial.println("MAC Address:");
          Serial.println(esp.getDefaultMacAddress());
        }
        // Обработка команды "help"
        else if (command[1] == "help") {
          Serial.print("Available mac commands: ");
          help.print_help(mac_commands, sizeof(mac_commands) / sizeof(mac_commands[0]));
        }
        // Неизвестная команда
        else {
          Serial.println("Unknown mac command");
        }
    }
};