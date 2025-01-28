class Touch {
  private:
    const char* touch_commands[1] = {"help"};  // Обновленный массив команд
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
        return;
      }

      // Получаем путь из команды
      String file_path = sd.normalize_path(command[1]);

      // Создаём пустой файл, используя функцию из SdCard
      sd.create_empty_file(file_path.c_str());
    }
};
