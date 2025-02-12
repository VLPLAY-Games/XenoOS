class Wget {
  private:
    Wifi wifi; // Экземпляр класса Wifi для управления подключением
    SdCard sdcard; // Экземпляр класса SdCard для работы с SD-картой
    Help help;
    String default_download_path = "/downloads"; // Путь по умолчанию для загрузки
    const char* wget_commands[2] = {"<url> [<path>]", "help"}; // Массив строк с командами

    

    // Метод для скачивания файла с отображением прогресса
    void downloadFile(const char* url, const char* path) {
      HTTPClient http;

      // Выполнение HTTP GET запроса к серверу
      http.begin(url);
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        // Получение размера файла
        uint32_t totalSize = http.getSize();
        Serial.printf("File size: %d bytes\r\n", totalSize);

        if (totalSize >= (sdcard.get_card_free() * 1024)){
          Serial.println("ERROR: File size is larger than free space on SD Card");
          return;
        } else {
          // Открываем файл для записи
          File file = SD.open(path, FILE_WRITE);
          if (!file) {
            Serial.println("Failed to open file for writing");
            http.end();
            return;
          }

          // Поток данных для загрузки
          WiFiClient* stream = http.getStreamPtr();
          uint32_t downloadedSize = 0;
          uint8_t buffer[256]; // Буфер для загрузки данных (256 байт)

          // Переменная для отслеживания прогресса
          uint8_t totalBars = 50; // Всего 50 символов '#'
          int nextBarThreshold = totalSize / totalBars; // Байты для добавления 1 символа '#'
          uint8_t barsPrinted = 0;

          // Измерение времени загрузки
          unsigned long startTime = millis();

          // Отображение начальной строки прогресса
          Serial.print("Downloading [");

          // Загрузка файла с отображением прогресса
          while (http.connected() && downloadedSize < totalSize) {
            size_t size = stream->available();
            if (size) {
              size_t readBytes = stream->readBytes(buffer, min(size, sizeof(buffer)));
              file.write(buffer, readBytes); // Записываем данные в файл
              downloadedSize += readBytes;

              // Проверяем, достигнут ли порог для добавления нового символа '#'
              while (downloadedSize >= (barsPrinted + 1) * nextBarThreshold && barsPrinted < totalBars) {
                Serial.print("#"); // Печатаем новый символ '#'
                barsPrinted++;
              }
            }
          }

          // Завершение строки прогресса
          while (barsPrinted < totalBars) {
            Serial.print("#");
            barsPrinted++;
          }
          Serial.println("]");

          // Завершение измерения времени загрузки
          unsigned long endTime = millis();
          float downloadTime = (endTime - startTime) / 1000.0; // Время в секундах
          float averageSpeedKB = (downloadedSize / 1024.0) / downloadTime;  // Средняя скорость (KB/сек)

          // Завершение загрузки
          file.close(); // Закрываем файл

          // Вывод средней скорости
          Serial.printf("Average speed: %.2f KB/sec\r\n", averageSpeedKB);

          // Проверка размера файла
          Serial.println("Checking file size");
          File downloadedFile = SD.open(path, FILE_READ);
          if (downloadedFile) {
            uint32_t localFileSize = downloadedFile.size();
            downloadedFile.close();

            if (localFileSize == totalSize) {
              Serial.printf("File downloaded successfully to: %s\r\n", path);
            } else {
              Serial.printf("Error: File size mismatch. Expected %d bytes, got %d bytes\r\n", totalSize, localFileSize);
            }
          } else {
            Serial.println("Error: Failed to open downloaded file for size verification");
          }
        } 
      } else {
        Serial.println("Failed to download file");
      }
      http.end(); // Завершаем HTTP запрос
    }

  public:
    // Конструктор
    Wget(Wifi& wifi_instance, SdCard& sdcard_instance) 
      : wifi(wifi_instance), sdcard(sdcard_instance) {}

    // Метод для загрузки файла
    void download(const String& url, const String& local_path = "") {
      if (!wifi.check_wifi()) return;

      // Если путь не указан, используем путь по умолчанию
      String resolved_path = local_path.isEmpty() ? default_download_path : local_path;

      // Проверяем, что директория существует
      if (!sdcard.is_directory(resolved_path)) {
          Serial.printf("Error: Path does not exist: %s\r\n", resolved_path.c_str());
          return; // Завершаем выполнение, если путь не существует
      }



      // Извлекаем имя файла из URL
      int last_slash = url.lastIndexOf('/');
      String filename = last_slash != -1 && last_slash < url.length() - 1 ? url.substring(last_slash + 1) : "file.txt";
      
      // Формируем полный путь для сохранения файла
      String file_path = sdcard.resolve_path(resolved_path + "/" + filename);
      if (sdcard.is_exists(file_path.c_str())){
        Serial.printf("Error: file with path and name %s already exists\r\n", file_path.c_str());
        return;
      }
      Serial.printf("Starting download: %s\r\n", url.c_str());

      // Вызываем функцию для загрузки файла
      downloadFile(url.c_str(), file_path.c_str());
    }

    // Обработка команд
    void handle_wget_commands(const std::vector<String>& command) {
      if (command.size() < 2) {
        Serial.println("Incomplete wget command. Use 'wget help' for usage.");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available wget commands: ");
        help.print_help(wget_commands, sizeof(wget_commands) / sizeof(wget_commands[0]));
      } else {
        String url = command[1];
        String path = command.size() >= 3 ? command[2] : "";
        download(url, path); // Запуск загрузки
      }
    }
};