// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <MD5Builder.h>
#include <SD.h>

class Md5 {
    public:
        // Функция для подсчёта MD5-хэша файла
        String calculate_md5_file(const char* file_path) {
            File file = SD.open(file_path, FILE_READ);
            if (!file) {
                Serial.printf("Failed to open file: %s\r\n", file_path);
                return "";
            }
            
            Serial.println("Started calculating hash MD5");

            MD5Builder md5;
            md5.begin();

            uint8_t buffer[512];
            size_t bytes_read;

            // Читаем файл по частям и добавляем данные в MD5
            while ((bytes_read = file.read(buffer, sizeof(buffer))) > 0) {
                md5.add(buffer, bytes_read);
            }

            file.close();

            md5.calculate();
            String hash = md5.toString();

            Serial.printf("MD5 hash of file %s: %s\r\n", file_path, hash.c_str());
            return hash;
        }

        // Функция для подсчёта MD5-хэша строки
        String calculate_md5_string(const String& input) {
            Serial.println("Started calculating hash MD5");
            MD5Builder md5;
            md5.begin();
            md5.add(input.c_str());
            md5.calculate();

            String hash = md5.toString();
            Serial.printf("MD5 hash of string \"%s\": %s\r\n", input.c_str(), hash.c_str());
            return hash;
        }

        // Функция для подсчёта MD5-хэша массива данных
        String calculate_md5_data(const uint8_t* data, size_t length) {
            Serial.println("Started calculating hash MD5");
            MD5Builder md5;
            md5.begin();
            md5.add(data, length);
            md5.calculate();

            String hash = md5.toString();
            Serial.printf("MD5 hash of data (%zu bytes): %s\r\n", length, hash.c_str());
            return hash;
        }
};