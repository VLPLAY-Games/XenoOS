// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Mac {
  private:
    const char* mac_commands[2] = {"info", "help"}; // Массив строк с командами
    const char* mac_dependencies[1] = {"Help"}; // Список зависимостей
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
          Serial.print("Dependencies: ");
          help.print_help(mac_dependencies, sizeof(mac_dependencies) / sizeof(mac_dependencies[0]));
        }
        // Неизвестная команда
        else {
          Serial.println("Unknown mac command");
        }
    }
};