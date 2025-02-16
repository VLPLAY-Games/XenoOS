// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <SHA1Builder.h>
#include <SD.h>

class SHA1Hasher {
  public:
    // Метод для вычисления SHA1 хеша строки
    String calculate_sha1_string(const String& input) {
      Serial.println("Started calculating hash SHA1");
      SHA1Builder sha1;  // Создаем объект SHA1Builder
      sha1.begin();      // Инициализируем SHA1

      sha1.add(input.c_str());  // Добавляем строку в вычисления

      sha1.calculate();  // Завершаем вычисления

      // Получаем результат в виде строки
      String hashString = sha1.toString();
      hashString.toUpperCase();  // Преобразуем строку в верхний регистр

      return hashString;
    }

    // Метод для вычисления SHA1 хеша файла с карты памяти
    String calculate_sha1_file(const char* file_path) {
      File file = SD.open(file_path, FILE_READ);  // Открываем файл для чтения
      if (!file) {
        Serial.printf("Failed to open file: %s\r\n", file_path);
        return "";  // Если файл не открыт, возвращаем пустую строку
      }
      
      Serial.println("Started calculating hash SHA1");
      SHA1Builder sha1;  // Создаем объект SHA1Builder
      sha1.begin();      // Инициализируем SHA1

      byte buffer[512];
      size_t bytesRead;

      // Чтение файла и добавление данных в хеш
      while ((bytesRead = file.read(buffer, sizeof(buffer))) > 0) {
        sha1.add(buffer, bytesRead);  // Добавляем прочитанные данные в хеш
      }

      file.close();  // Закрываем файл

      sha1.calculate();  // Завершаем вычисления

      // Получаем результат в виде строки
      String hashString = sha1.toString();
      hashString.toUpperCase();  // Преобразуем строку в верхний регистр

      return hashString;
    }

    // Метод для вычисления SHA1 хеша данных в виде массива байтов
    String calculate_sha1_data(const uint8_t* data, size_t length) {
      Serial.println("Started calculating hash SHA1");
      SHA1Builder sha1;  // Создаем объект SHA1Builder
      sha1.begin();      // Инициализируем SHA1

      sha1.add(data, length);  // Добавляем данные в хеш

      sha1.calculate();  // Завершаем вычисления

      // Получаем результат в виде строки
      String hashString = sha1.toString();
      hashString.toUpperCase();  // Преобразуем строку в верхний регистр

      return hashString;
    }
};