// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Mkdir {
  private:
    const char* mkdir_commands[1] = {"help"};  // Обновленный массив команд
    const char* mkdir_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    void handle_mkdir_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete mkdir command");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available mkdir commands: ");
        help.print_help(mkdir_commands, sizeof(mkdir_commands) / sizeof(mkdir_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(mkdir_dependencies, sizeof(mkdir_dependencies) / sizeof(mkdir_dependencies[0]));
        return;
      }

      String path = sd.normalize_path(command[1]);
      path = sd.resolve_path(path);

      if (SD.exists(path)) {
        Serial.println("Directory already exists: " + path);
        return;
      }

      sd.createDir(SD, path.c_str());
    }
};
