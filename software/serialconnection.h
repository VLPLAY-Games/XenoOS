// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <vector>

class SerialConnection {
  private:
    String input;
    History history;
    std::vector<String> command;
    String current_directory = "/";  // Пример начальной директории для подсказки
    int command_history_index = -1;

  public:
    void read_serial(SdCard& sd) {
      while (Serial.available()) {
          delay(2);
          char c = Serial.read();

          // Обработка клавиши Backspace
          if (c == 0x08 || c == 0x7F) {  // Код для Backspace
              if (this->input.length() > 0) {
                  // Проверяем, не является ли последний символ составным (многобайтовым)
                  int len = this->input.length();
                  if (this->input.charAt(len - 1) & 0xC0) { // Если старшие два бита 10, значит это составной символ
                      this->input.remove(len - 2, 2);  // Удаляем два байта для составного символа
                  } else {
                      this->input.remove(len - 1);  // Удаляем один символ
                  }
                  Serial.print("\b \b");  // Убираем символ на экране
              }
          } 
          // Обработка последовательностей для стрелок
          else if (c == 0x1B) {  // Начало escape-последовательности
              delay(10);  // Ждем, чтобы получить следующий символ
              if (Serial.available()) {
                  char nextChar = Serial.read();
                  if (nextChar == 0x5B) {  // Код для '['
                      delay(10);
                      if (Serial.available()) {
                          char arrowKey = Serial.read();
                          if (arrowKey == 0x41) {  // Стрелка вверх (0x1B 0x5B 0x41)
                              handle_arrow_up(sd);
                          } else if (arrowKey == 0x42) {  // Стрелка вниз (0x1B 0x5B 0x42)
                              handle_arrow_down(sd);
                          }
                      }
                  }
              }
          }
          // Обработка обычных символов
          else if (c != '\n' && c != '\r') {
              Serial.print(c);
              this->input += c;
          } else {
              Serial.println();
              this->input.trim();  // Удаляем пробелы в начале и конце строки
              if (this->input.isEmpty()) {
                  Serial.print(current_directory + " $ ");
              } else {
                  // Когда строка закончилась, разделяем её на слова
                  split_input_to_command();
              }
          }
      }
    }

    void handle_arrow_up(SdCard& sd) {
      // Стрелка вверх - перемещаемся к последней команде в истории
      if (command_history_index == -1) {
          command_history_index = history.get_history_size(sd) - 1;  // Начинаем с последней команды
      } else if (command_history_index > 0) {
          command_history_index--;  // Двигаемся назад по истории
      }
  
      if (command_history_index >= 0) {
          input = history.get_command_by_index(command_history_index, sd);  // Получаем команду по индексу
  
          // Очистка строки и вывод нового запроса
          Serial.print("\r");  // Перемещаем курсор в начало строки
          Serial.print("                        ");  // Заполняем строку пробелами (чтобы стереть старую команду)
          Serial.print("\r");  // Перемещаем курсор в начало снова
          Serial.print(current_directory + " $ ");  // Выводим запрос
          Serial.print(input);  // Выводим команду
      }
    }
    
    void handle_arrow_down(SdCard& sd) {
        // Стрелка вниз - перемещаемся по истории вперед
        if (command_history_index < history.get_history_size(sd) - 1) {
            command_history_index++;  // Двигаемся вперед по истории
            input = history.get_command_by_index(command_history_index, sd);  // Получаем команду по индексу
    
            // Очистка строки и вывод нового запроса
            Serial.print("\r");  // Перемещаем курсор в начало строки
            Serial.print("                        ");  // Заполняем строку пробелами (чтобы стереть старую команду)
            Serial.print("\r");  // Перемещаем курсор в начало снова
            Serial.print(current_directory + " $ ");  // Выводим запрос
            Serial.print(input);  // Выводим команду
        } else if (command_history_index == history.get_history_size(sd) - 1) {
            // Если достигнут конец истории, очистим строку
            command_history_index++;  // Переход к "пустому" состоянию
            input = "";  // Очистка ввода
    
            // Очистка строки и вывод нового запроса
            Serial.print("\r");  // Перемещаем курсор в начало строки
            Serial.print("                        ");  // Заполняем строку пробелами (чтобы стереть старую команду)
            Serial.print("\r");  // Перемещаем курсор в начало снова
            Serial.print(current_directory + " $ ");  // Выводим запрос
        }
    }
  

    void split_input_to_command() {
      String word = "";
      for (size_t i = 0; i < this->input.length(); i++) {
        char c = this->input[i];
        if (c == ' ') {
          if (word.length() > 0) {
            command.push_back(word); // Добавляем слово в массив
            word = ""; // Очищаем временное слово
          }
        } else {
          word += c; // Добавляем символ к слову
        }
      }
      if (word.length() > 0) {
        command.push_back(word); // Добавляем последнее слово
      }
    }

    std::vector<String> get_command() {
      return this->command;
    }

    void empty_command() {
      this->command.clear(); // Очистка массива слов
    }

    String get_input() {
      return this->input;
    }

    void empty_input() {
      this->input = "";
    }
};
