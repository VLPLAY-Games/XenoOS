// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <vector>

class SerialConnection {
  private:
      String input;
      History history;
      std::vector<String> command;
      int command_history_index = -1;
      int cursor_position = 0;
  
  public:
      void read_serial(SdCard& sd) {
          while (Serial.available()) {
              delay(2);
              char c = Serial.read();
  
              // Обработка клавиши Backspace
              if (c == 0x08 || c == 0x7F) {  // Код для Backspace
                if (this->cursor_position > 0) {
                    int len = this->input.length();
                    
                    // Определяем размер символа перед курсором в UTF-8
                    int charSize = 1;
                    while (this->cursor_position - charSize > 0 && (this->input.charAt(this->cursor_position - charSize) & 0xC0) == 0x80) {
                        charSize++; // Подсчитываем все байты составного символа
                    }
            
                    // Удаляем символ перед курсором
                    this->input.remove(this->cursor_position - charSize, charSize);
            
                    // Перемещаем курсор влево на количество байтов удаленного символа
                    for (int i = 0; i < charSize; i++) {
                        Serial.print("\b \b");
                    }
            
                    // Обновляем позицию курсора
                    this->cursor_position -= charSize;
            
                    // Перерисовываем строку после удаления символа
                    Serial.print(this->input.substring(this->cursor_position));
                    Serial.print(" "); // Очищаем последний символ
                    for (int i = 0; i < this->input.length() - this->cursor_position + 1; i++) {
                        Serial.print("\b"); // Возвращаем курсор на место
                    }
                }
              }
              // Обработка последовательностей для стрелок
              else if (c == 0x1B) {  // Начало escape-последовательности
                  delay(10);
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
                              } else if (arrowKey == 0x44) {  // Стрелка влево (0x1B 0x5B 0x44)
                                  handle_arrow_left();
                              } else if (arrowKey == 0x43) {  // Стрелка вправо (0x1B 0x5B 0x43)
                                  handle_arrow_right();
                              }
                          }
                      }
                  }
              }
              // Обработка обычных символов
              else if (c != '\n' && c != '\r') {
                  // Вставляем символ в текущую позицию курсора
                  this->input = this->input.substring(0, this->cursor_position) + c + this->input.substring(this->cursor_position);
                  Serial.print(c); // Выводим символ
                  Serial.print(this->input.substring(this->cursor_position + 1)); // Выводим оставшуюся часть строки
                  for (int i = 0; i < this->input.length() - this->cursor_position - 1; i++) {
                      Serial.print("\b"); // Возвращаем курсор на место
                  }
                  this->cursor_position++;  // Двигаем курсор вправо
              } else {
                  Serial.println();
                  this->input.trim();  // Удаляем пробелы в начале и конце строки
                  if (this->input.isEmpty()) {
                      Serial.print(current_directory + " $ ");
                  } else {
                      // Когда строка закончилась, разделяем её на слова
                      split_input_to_command();
                  }
                  cursor_position = input.length();  // Устанавливаем курсор в конец строки после ввода
              }
          }
      }
  
      // Обработка стрелки влево
      void handle_arrow_left() {
          if (cursor_position > 0) {
              cursor_position--;  // Двигаем курсор влево
              Serial.print("\033[D");  // Перемещаем курсор влево
          }
      }
  
      // Обработка стрелки вправо
      void handle_arrow_right() {
          if (cursor_position < input.length()) {
              cursor_position++;  // Двигаем курсор вправо
              Serial.print("\033[C");  // Перемещаем курсор вправо
          }
      }
  
      // Обработка стрелки вверх
      void handle_arrow_up(SdCard& sd) {
          if (command_history_index == -1) {
              command_history_index = history.get_history_size(sd) - 1;  // Начинаем с последней команды
          } else if (command_history_index > 0) {
              command_history_index--;  // Двигаемся назад по истории
          }
  
          if (command_history_index >= 0) {
              Serial.print("\r");  // начало строки
              for (int i = 0; i < input.length() + current_directory.length() + 3; i++) 
                  Serial.print(" ");
              
              input = history.get_command_by_index(command_history_index, sd);
  
              Serial.print("\r");  // Перемещаем курсор в начало снова
              Serial.print(current_directory + " $ ");  // Выводим запрос
              Serial.print(input);  // Выводим команду
              cursor_position = input.length();  // Устанавливаем курсор в конец строки после обновления
          }
      }
  
      // Обработка стрелки вниз
      void handle_arrow_down(SdCard& sd) {
          if (command_history_index < history.get_history_size(sd) - 1) {
              command_history_index++;  // Двигаемся вперед по истории
  
              Serial.print("\r");  // начало строки
              for (int i = 0; i < input.length() + current_directory.length() + 3; i++) 
                  Serial.print(" ");
  
              input = history.get_command_by_index(command_history_index, sd);  // Получаем команду по индексу
  
              Serial.print("\r");  // Перемещаем курсор в начало снова
              Serial.print(current_directory + " $ ");  // Выводим запрос
              Serial.print(input);  // Выводим команду
              cursor_position = input.length();  // Устанавливаем курсор в конец строки после обновления
          } else if (command_history_index == history.get_history_size(sd) - 1) {
              command_history_index++;  // Переход к "пустому" состоянию
  
              Serial.print("\r");  // начало строки
  
              for (int i = 0; i < input.length() + current_directory.length() + 3; i++) 
                  Serial.print(" ");
  
              input = "";  // Очистка ввода
  
              Serial.print("\r");  // Перемещаем курсор в начало снова
              Serial.print(current_directory + " $ ");  // Выводим запрос
              cursor_position = current_directory.length() + 3;  // Устанавливаем курсор в конец строки после очистки
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
          this->command.clear();
      }
  
      String get_input() {
          return this->input;
      }
  
      void empty_input() {
          this->input = "";
      }
  };