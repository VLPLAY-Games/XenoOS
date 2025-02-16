// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <SD.h>

class History {
  private:
    Help help;
    const char* history_commands[3] = {"clear", "help", "read, get"};  // Обновленный массив команд
    const char* history_dependencies[1] = {"Help"}; // Список зависимостей

  public:
    void handle_history_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete history command");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available history commands: ");
        help.print_help(history_commands, sizeof(history_commands) / sizeof(history_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(history_dependencies, sizeof(history_dependencies) / sizeof(history_dependencies[0]));
        return;
      }
      
      if (command[1] == "clear") {
        sd.deleteFile(SD, historylog_path);
        sd.create_empty_file(historylog_path);
        return;
      }

      if (command[1] == "read") {
        Cat cat(sd);
        cat.read_file(historylog_path);
        return;
      }

      if (command[1] == "get") {
        // Получение команды по индексу
        if (command.size() < 3) {
          Serial.println("Please provide an index");
          return;
        }
        
        int index = command[2].toInt();
        String command_from_history = get_command_by_index(index, sd);
        if (command_from_history.length() > 0) {
          Serial.print("Command at index ");
          Serial.print(index);
          Serial.print(": ");
          Serial.println(command_from_history);
        } else {
          Serial.println("Command not found at the given index");
        }
        return;
      }

      Serial.println("Unknown history command");
    }

    // Чтение команды из файла по индексу
    String get_command_by_index(int index, SdCard& sd) {
      File file = SD.open(historylog_path, FILE_READ);  // Открываем файл для чтения
      if (!file) {
        Serial.println("Failed to open history log file");
        return "";
      }

      int current_index = 0;
      String line = "";
      while (file.available()) {
        line = sd.read_line(file);
        if (current_index == index) {
          file.close();
          return line;  // Возвращаем строку, если индекс совпал
        }
        current_index++;
      }

      file.close();
      return "";  // Возвращаем пустую строку, если индекс не найден
    }

    void write_history(const char *message, SdCard& sd) {
        sd.appendFile(SD, historylog_path, message, true);
    }

    // Функция для получения общего числа команд в истории
    int get_history_size(SdCard& sd) {
      File file = SD.open(historylog_path, FILE_READ);  // Открываем файл для чтения
      if (!file) {
        Serial.println("Failed to open history log file");
        return 0;  // Возвращаем 0, если файл не найден
      }

      int count = 0;
      while (file.available()) {
        sd.read_line(file);  // Пропускаем строку
        count++;  // Считаем количество строк в файле
      }

      file.close();
      return count;  // Возвращаем общее количество команд в файле
    }
};