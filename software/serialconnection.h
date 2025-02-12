#include <vector>

class SerialConnection {
  private:
    String input;
    std::vector<String> command;

  public:
    void read_serial() {
      while (Serial.available()) {
        delay(2);
        char c = Serial.read();

        // Обработка клавиши Backspace
        if (c == 0x08 || c == 0x7F) {  // Код для Backspace
            if (this->input.length() > 0) {
                this->input.remove(this->input.length() - 1);  // Удаляем последний символ из строки
                Serial.print("\b \b");  // Убираем символ на экране
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