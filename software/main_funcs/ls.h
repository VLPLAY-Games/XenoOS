// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Ls {
  private:
    const char* ls_commands[1] = {"help"};  // Обновленный массив команд
    const char* ls_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    void handle_ls_commands(const std::vector<String>& command, SdCard& sd) {
      String target_dir = current_directory;
      if (command.size() == 2 && command[1] != "help") {
        target_dir = sd.normalize_path(command[1]);
        target_dir = sd.resolve_path(target_dir);
      }

      if (SD.exists(target_dir)) {
        sd.listDir(SD, target_dir.c_str(), 0);
        return;
      } else {
        Serial.println("Directory does not exist: " + target_dir);
        return;
      }
      if (command[1] == "help") {
        Serial.print("Available ls commands: ");
        help.print_help(ls_commands, sizeof(ls_commands) / sizeof(ls_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(ls_dependencies, sizeof(ls_dependencies) / sizeof(ls_dependencies[0]));
        return;
      } else {
        Serial.println("Unknown ls command");
      }
    }
};