// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Hash {
  private:
    const char* hash_commands[1] = {"help"};  // Массив строк с командами
    const char* hash_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    void handle_hash_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete hash command. Usage: hash <file_path>");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available hash commands: ");
        help.print_help(hash_commands, sizeof(hash_commands) / sizeof(hash_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(hash_dependencies, sizeof(hash_dependencies) / sizeof(hash_dependencies[0]));
        return;
      }

      // Получаем пути из команды
      String file_path = sd.normalize_path(command[1]);
      Md5 md5;
      md5.calculate_md5_file(file_path.c_str());
    }
};