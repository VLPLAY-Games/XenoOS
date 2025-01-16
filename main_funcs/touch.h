class Touch {
  public:
    // Метод для обработки команды touch
    void handle_touch_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete touch command. Usage: touch <file_path>");
        return;
      }

      // Получаем путь из команды
      String file_path = sd.normalize_path(command[1]);

      // Создаём пустой файл, используя функцию из SdCard
      sd.create_empty_file(file_path.c_str());
    }
};