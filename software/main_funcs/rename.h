// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Rename {
  private:
    SdCard &sdcard;
    const char* rename_commands[2] = {"help", "<old_file_path> <new_file_path>"};  // Обновленный массив команд
    const char* rename_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    Rename(SdCard &sd) : sdcard(sd) {}

    void rename_file(const String &old_name, const String &new_name) {
      String resolved_old_name = sdcard.resolve_path(old_name);
      String resolved_new_name = sdcard.resolve_path(new_name);

      if (!SD.exists(resolved_old_name)) {
        Serial.printf("File not found: %s\r\n", resolved_old_name.c_str());
        return;
      }

      sdcard.renameFile(SD, resolved_old_name.c_str(), resolved_new_name.c_str());
    }

    void handle_rename_commands(const std::vector<String> &command) {
      if (command.size() < 3 && command[1] != "help") {
        Serial.println("Usage: rename <old_file_path> <new_file_path>");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available rename commands: ");
        help.print_help(rename_commands, sizeof(rename_commands) / sizeof(rename_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(rename_dependencies, sizeof(rename_dependencies) / sizeof(rename_dependencies[0]));
        return;
      }

      rename_file(command[1], command[2]);
    }
};
