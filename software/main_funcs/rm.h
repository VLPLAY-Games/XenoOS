// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Rm {
  private:
    const char* rm_commands[1] = {"help"};  // Обновленный массив команд
    const char* rm_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    void handle_rm_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete rm command. Usage: rm <path>");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available rm commands: ");
        help.print_help(rm_commands, sizeof(rm_commands) / sizeof(rm_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(rm_dependencies, sizeof(rm_dependencies) / sizeof(rm_dependencies[0]));
        return;
      }

      String path = sd.normalize_path(command[1]);
      path = sd.resolve_path(path);

      if (SD.exists(path)) {
        if (sd.is_directory(path)) {
          sd.removeDir(SD, path.c_str());
        } else {
          sd.deleteFile(SD, path.c_str());
        }
      } else {
        Serial.println("Path does not exist: " + path);
      }
    }
};
