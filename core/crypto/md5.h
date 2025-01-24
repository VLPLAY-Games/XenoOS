#include <MD5Builder.h>
#include <SD.h>

class Md5 {
    public:
        // Функция для подсчёта MD5-хэша файла
        String calculate_md5_file(const char* file_path) {
            File file = SD.open(file_path, FILE_READ); // Открываем файл для чтения
            if (!file) {
                Serial.printf("Failed to open file: %s\n", file_path);
                return ""; // Возвращаем пустую строку, если файл открыть не удалось
            }
            
            Serial.println("Started calculating hash MD5");

            MD5Builder md5; // Создаем объект MD5Builder
            md5.begin(); // Инициализируем подсчёт MD5

            uint8_t buffer[512]; // Буфер для чтения данных
            size_t bytes_read;

            // Читаем файл по частям и добавляем данные в MD5
            while ((bytes_read = file.read(buffer, sizeof(buffer))) > 0) {
                md5.add(buffer, bytes_read); // Добавляем прочитанные данные в MD5
            }

            file.close(); // Закрываем файл

            md5.calculate(); // Завершаем подсчёт MD5
            String hash = md5.toString(); // Получаем MD5-хэш в виде строки

            Serial.printf("MD5 hash of file %s: %s\n", file_path, hash.c_str());
            return hash; // Возвращаем хэш
        }

        // Функция для подсчёта MD5-хэша строки
        String calculate_md5_string(const String& input) {
            Serial.println("Started calculating hash MD5");
            MD5Builder md5; // Создаем объект MD5Builder
            md5.begin(); // Инициализируем подсчёт MD5
            md5.add(input.c_str()); // Добавляем строку для обработки
            md5.calculate(); // Завершаем подсчёт MD5

            String hash = md5.toString(); // Получаем MD5-хэш в виде строки
            Serial.printf("MD5 hash of string \"%s\": %s\n", input.c_str(), hash.c_str());
            return hash; // Возвращаем хэш
        }

        // Функция для подсчёта MD5-хэша массива данных
        String calculate_md5_data(const uint8_t* data, size_t length) {
            Serial.println("Started calculating hash MD5");
            MD5Builder md5; // Создаем объект MD5Builder
            md5.begin(); // Инициализируем подсчёт MD5
            md5.add(data, length); // Добавляем массив данных для обработки
            md5.calculate(); // Завершаем подсчёт MD5

            String hash = md5.toString(); // Получаем MD5-хэш в виде строки
            Serial.printf("MD5 hash of data (%zu bytes): %s\n", length, hash.c_str());
            return hash; // Возвращаем хэш
        }
};