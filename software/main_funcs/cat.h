class Cat {
  private:
    SdCard &sdcard;           // Ссылка на объект SdCard для взаимодействия с файловой системой
    Help help;                // Объект для управления справкой
    const char* cat_commands[2] = {"<path_to_file>", "help"}; // Список доступных команд для cat
    const char* cat_dependencies[1] = {"Help"}; // Список зависимостей

  public:
    // Конструктор
    Cat(SdCard &sd) : sdcard(sd) {}

    // Чтение содержимого файла и вывод в консоль
    void read_file(const String &path) {
      // Нормализуем путь к файлу
      String resolved_path = sdcard.resolve_path(path);

      // Проверяем, существует ли файл
      if (!SD.exists(resolved_path)) {
        Serial.printf("File not found: %s\r\n", resolved_path.c_str());
        return;
      }

      // Используем метод readFile из sdcard
      sdcard.readFile(SD, resolved_path.c_str());
    }

    // Обработка команд cat
    void handle_cat_commands(const std::vector<String> &command) {
      if (command.size() < 2) {
        Serial.println("Usage: cat <file_path>");
        return;
      }

      // Команда "help" для вывода справки
      if (command[1] == "help") {
        Serial.print("Available cat commands: ");
        help.print_help(cat_commands, sizeof(cat_commands) / sizeof(cat_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(cat_dependencies, sizeof(cat_dependencies) / sizeof(cat_dependencies[0]));
        return;
      }

      // Чтение файла
      read_file(command[1]);
    }
};
