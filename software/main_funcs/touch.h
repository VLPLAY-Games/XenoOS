// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Touch {
  private:
    const char* touch_commands[1] = {"help"};  // Обновленный массив команд
    const char* touch_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    void handle_touch_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete touch command. Usage: touch <file_path>");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available touch commands: ");
        help.print_help(touch_commands, sizeof(touch_commands) / sizeof(touch_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(touch_dependencies, sizeof(touch_dependencies) / sizeof(touch_dependencies[0]));
        return;
      }

      // Получаем путь из команды
      String file_path = sd.normalize_path(command[1]);

      // Создаём пустой файл, используя функцию из SdCard
      sd.create_empty_file(file_path.c_str());
    }
};
