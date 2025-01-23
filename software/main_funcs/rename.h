class Rename {
  private:
    SdCard &sdcard;           // Ссылка на объект SdCard для взаимодействия с файловой системой
    Help help;                // Объект для управления справкой
    const char* rename_commands[2] = {"<old_file_path> <new_file_path>", "help"}; // Список доступных команд для rename

  public:
    // Конструктор
    Rename(SdCard &sd) : sdcard(sd) {}

    // Переименование файла
    void rename_file(const String &old_name, const String &new_name) {
      // Нормализуем пути к файлам
      String resolved_old_name = sdcard.resolve_path(old_name);
      String resolved_new_name = sdcard.resolve_path(new_name);

      // Проверяем, существует ли файл с исходным именем
      if (!SD.exists(resolved_old_name)) {
        Serial.printf("File not found: %s\n", resolved_old_name.c_str());
        return;
      }

      // Переименование файла
      sdcard.renameFile(SD, resolved_old_name.c_str(), resolved_new_name.c_str());
    }

    // Обработка команд rename
    void handle_rename_commands(const std::vector<String> &command) {
      if (command.size() < 3 && command[1] != "help") {
        Serial.println("Usage: rename <old_file_path> <new_file_path>");
        return;
      }

      // Команда "help" для вывода справки
      if (command[1] == "help") {
        Serial.print("Available rename commands: ");
        help.print_help(rename_commands, sizeof(rename_commands) / sizeof(rename_commands[0]));
        return;
      }

      // Переименование файла
      rename_file(command[1], command[2]);
    }
};