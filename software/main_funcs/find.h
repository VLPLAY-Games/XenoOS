class Find {
  private:
    const char* find_commands[1] = {"help"}; // Массив строк с командами
    Help help; // Экземпляр класса помощи

  public:
    // Функция для обработки команд поиска
    void handle_find_commands(const std::vector<String>& command, SdCard& sd) {
        if (command.size() < 2) {
          Serial.println("Incomplete find command");
          return;
        }

        // Обработка команды "help"
        if (command[1] == "help") {
          Serial.print("Available find commands: ");
          help.print_help(find_commands, sizeof(find_commands) / sizeof(find_commands[0]));
        }
        // Обработка команды поиска
        else {
          const char* file_name = command[1].c_str();
          String directory = "/"; // Поиск в корневом каталоге по умолчанию

          // Если передана директория, используем её
          if (command.size() > 2) {
            file_name = command[1].c_str(); // Первая строка после "find" будет файлом
            directory = command[2]; // Следующая строка будет директорией
          }

          // Выполнение поиска
          sd.search_file(directory, file_name);
        }
    }
};