// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class SystemUpdate {
  private:
    Wget &wget;                 // Ссылка на объект Wget для загрузки обновлений
    Esp &esp;                   // Ссылка на объект Esp для обновлений
    Wifi &wifi; 
    SdCard &sd;
    Help help;                  // Объект для справки
    const char* update_commands[3] = {"download", "install", "help"}; // Список доступных команд для обновлений
    const char* update_dependencies[1] = {"Help"}; // Список зависимостей

  public:
    // Конструктор
    SystemUpdate(Wget &wget_instance, Esp &esp_instance, Wifi &wifi_instance, SdCard &sd_instance) : wget(wget_instance), esp(esp_instance), wifi(wifi_instance), sd(sd_instance) {}

    // Обработка команд update
    void handle_update_commands(const std::vector<String> &command) {
      if (command.size() < 2) {
        Serial.println("Usage: update download | update install | update help");
        return;
      }

      // Команда "help" для вывода справки
      if (command[2] == "help") {
        Serial.print("Available update commands: ");
        help.print_help(update_commands, sizeof(update_commands) / sizeof(update_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(update_dependencies, sizeof(update_dependencies) / sizeof(update_dependencies[0]));
        return;
      }

      // Команда загрузки обновления
      if (command[2] == "download") {
        if (wifi.check_wifi()) {
          check_and_download_update();
        } else {
          return;
        }
        
      } 
      // Команда установки обновления
      else if (command[2] == "install") {
        esp.updateFromFS(SD);
      } else {
        Serial.println("Invalid update command");
      }
    }

    // Функция для получения информации о версии и хэше
    String get_version_info() {
      HTTPClient http;
      http.begin(sys_upd_url_txt); // URL текстового файла с информацией
      int httpCode = http.GET();
      String version_info = "";

      if (httpCode == HTTP_CODE_OK) {
        version_info = http.getString(); // Считываем файл с сервером
      } else {
        Serial.printf("Error getting version info: %d\n", httpCode);
      }

      http.end();
      return version_info;
    }

    // Функция для скачивания и проверки версии прошивки
    void check_and_download_update() {
      String version_info = get_version_info();
      
      // Если версия из файла (на сервере) пустая, то ошибка при получении данных
      if (version_info == "") {
          Serial.println("Failed to get version info from the server.");
          return;
      }

      // Разделяем строку на номер версии и хэш
      String current_version = version_info.substring(0, 3); // Версия на сервере в формате "019"
      String version_hash = version_info.substring(4); // Хэш-сумма

      // Форматируем версии
      String formatted_current_version = String(current_version[0]) + "." + current_version[1] + "." + current_version[2];
      Serial.printf("Current version: %s, Available version: %s\r\n", os_version, formatted_current_version.c_str());

      // Сравниваем версии как числа
      int current_version_int = current_version.toInt();  // Серверная версия
      int os_version_int = atoi(os_version_upd);              // Локальная версия
      // Если текущая версия системы старее, чем на сервере, обновляем
      if (current_version_int > os_version_int) {
          Serial.println("New version available. Starting download...");

          // Скачиваем прошивку
          download_firmware();

          // Проверка хэш-суммы после скачивания
          if (!check_firmware_hash(version_hash)) {
              Serial.println("Error: Firmware hash does not match!");
              sd.deleteFile(SD, "/upd/firmware.bin");
          } else {
              Serial.println("Firmware update download successful");
          }
      } else {
          Serial.println("No update available");
      }
    }

    // Функция для скачивания прошивки
    void download_firmware() {
      Serial.println("Downloading firmware...");
      wget.handle_wget_commands({"wget", sys_upd_url, "/upd"});
    }

    // Функция для проверки хэш-суммы с использованием вашего метода
    bool check_firmware_hash(String expected_hash) {
      Md5 md5;
      String file_path = "/upd/firmware.bin";  // Путь к скачанному файлу

      // Вычисляем MD5 хэш для скачанного файла
      String downloaded_hash = md5.calculate_md5_file(file_path.c_str());

      Serial.printf("Expected Hash: %s\n", expected_hash.c_str());
      Serial.printf("Downloaded Hash: %s\n", downloaded_hash.c_str());

      // Сравниваем хэши
      return downloaded_hash == expected_hash;
    }
};