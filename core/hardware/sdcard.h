// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <FS.h>
#include <SD.h>
#include <SPI.h>

extern String current_directory = "/";
extern uint8_t cardType = 0;
extern String cardTypeStr = "";
extern uint64_t cardSize = 0;
extern uint64_t cardUsage = 0;
extern uint64_t cardFree = 0;

class SdCard{ 
  private:
    ColorPrinter color;

  public:
    bool mount_sd(Timer* timer = nullptr, bool print = false) {
      if (!SD.begin()) {
        if (print){
          if (timer) {
            timer->println_with_timer("Card Mount Failed", "error");
          } else {
            Serial.println("Card Mount Failed");
          }
        }
        return false;
      }

      cardType = SD.cardType();

      if (cardType == CARD_NONE) {
        if (print){
          if (timer) {
            timer->println_with_timer("No SD card attached", "error");
          } else {
            Serial.println("No SD card attached");
          }
        }
        return false;
      }

      if (print){
        if (timer) {
          timer->println_with_timer("SD Card Mounted successfully", "success");
        } else {
          Serial.println("SD Card Mounted successfully");
        }
      }
      return true;
    }

    // Функция для возврата типа карты SD
    uint8_t get_cardtype() {
      cardType = SD.cardType();
      return cardType;
    }

    String get_cardtypeStr(){
      cardType = get_cardtype();
      if(cardType == CARD_MMC){
        return "MMC";
      } else if(cardType == CARD_SD){
        return "SDSC";
      } else if(cardType == CARD_SDHC){
        return "SDHC";
      } else {
        return "UNKNOWN";
      }
    }

    void update_info(){
      cardTypeStr = get_cardtypeStr();
      cardSize = SD.cardSize() / 1024;
      cardUsage = SD.usedBytes() / 1024;
      cardFree = cardSize - cardUsage;
    }

    // Функция для возврата общего размера карты
    uint64_t get_card_size() {
      cardSize = SD.cardSize() / 1024;
      return cardSize;
    }

    // Функция для возврата использованного пространства на карте
    uint64_t get_card_usage() {
      cardUsage = SD.usedBytes() / 1024;
      return cardUsage;
    }

    // Функция для возврата использованного пространства на карте
    uint64_t get_card_free() {
      cardFree = get_card_size() - get_card_usage();
      return cardFree;
    }

    bool init(Timer timer){
      if (mount_sd(&timer)) {
        timer.print_with_timer("  SD Card Type: ");
        Serial.println(get_cardtypeStr());
        update_info();
        timer.print_time();
        Serial.printf("  SD Card Size: %llu KB\r\n", get_card_size());
        timer.print_time();
        Serial.printf("  SD Card Used space: %llu KB\r\n", get_card_usage());
        timer.print_time();
        Serial.printf("  SD Card Free space: %llu KB\r\n", get_card_free());
        timer.println_with_timer("===========================");
        timer.println_with_timer("");
        current_directory = "/";
        timer.println_with_timer("Changed current directory to /", "info");
        timer.println_with_timer("SD Card Loaded", "success");
        return true;
      }
      else {
        timer.println_with_timer("SD Card Not Loaded", "error");
        return false;
      }
    }


    void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
      Serial.printf("Listing directory: %s\r\n", dirname);

      File root = fs.open(dirname);
      if (!root) {
        Serial.println("Failed to open directory");
        return;
      }
      if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
      }

      File file = root.openNextFile();
      while (file) {
        if (file.isDirectory()) {
          Serial.print("  DIR : ");
          Serial.println(file.name());
          if (levels) {
            listDir(fs, file.path(), levels - 1);
          }
        } else {
          Serial.print("  FILE: ");
          Serial.print(file.name());
          Serial.print("  SIZE: ");
          Serial.print(file.size());
          Serial.println("B");
        }
        file = root.openNextFile();
      }
    }

    bool createDir(fs::FS &fs, const char *path) {
      String normalized_path = normalize_path(path);
      String resolved_path = resolve_path(normalized_path);
      Serial.printf("Creating Dir: %s\r\n", path);
      if (fs.mkdir(resolved_path)) {
        Serial.println("Dir created");
        return true;
      } else {
        Serial.println("mkdir failed");
        return false;
      }
    }


    bool removeDir(fs::FS &fs, const char *path) {
      String normalized_path = normalize_path(path);
      String resolved_path = resolve_path(normalized_path);
      
      Serial.printf("Directory delete: %s\r\n", resolved_path.c_str());

      // Открываем директорию
      File dir = fs.open(resolved_path.c_str());
      if (!dir || !dir.isDirectory()) {
          Serial.println("Error opening directory");
          return false;
      }

      // Рекурсивно удаляем все содержимое
      File file = dir.openNextFile();
      while (file) {
          String filePath = resolved_path + "/" + String(file.name());
          
          if (file.isDirectory()) {
              // Рекурсивный вызов для поддиректории
              if (!removeDir(SD, filePath.c_str())) {
                  file.close();
                  dir.close();
                  return false;
              }
          } else {
              // Удаление файла
              deleteFile(SD, filePath.c_str());
          }
          file = dir.openNextFile();
      }

      // Закрываем директорию
      dir.close();

      // Удаляем саму пустую директорию
      if (fs.rmdir(resolved_path.c_str())) {
          Serial.println("Directory delete success");
          return true;
      } else {
          Serial.println("Directory delete fail");
          return false;
      }
    }


    void readFile(fs::FS &fs, const char *path) {
      Serial.printf("Reading file: %s\r\n", path);

      File file = fs.open(path);
      if (!file) {
        Serial.println("Failed to open file for reading");
        return;
      }
      Serial.println("Read from file: ");
      Serial.println();
      while (file.available()) {
        Serial.write(file.read());
      }
      file.close();
    }


    void writeFile(fs::FS &fs, const char *path, const char *message) {
      Serial.printf("Writing file: %s\r\n", path);

      File file = fs.open(path, FILE_WRITE);
      if (!file) {
        Serial.println("Failed to open file for writing");
        return;
      }
      if (file.print(message)) {
        Serial.println("File written");
      } else {
        Serial.println("Write failed");
      }
      file.close();
    }

    void appendFile(fs::FS &fs, const char *path, const char *message, bool background=false) {
      if (!background) {
        Serial.printf("Appending to file: %s\r\n", path);
      }

      File file = fs.open(path, FILE_APPEND);
      if (!file) {
        if (!background) {
          Serial.println("Failed to open file for appending");
        }
        return;
      }
      if (file.print(message)) {
        if (!background) {
          Serial.println("Message appended");
        }
      } else {
        if (!background) {
          Serial.println("Append failed");
        }
      }
      file.close();
    }

    void renameFile(fs::FS &fs, const char *path1, const char *path2) {
      Serial.printf("Renaming file %s to %s\r\n", path1, path2);
      if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
      } else {
        Serial.println("Rename failed");
      }
    }

    void deleteFile(fs::FS &fs, const char *path) {
      Serial.printf("Deleting file: %s\r\n", path);
      if (fs.remove(path)) {
        Serial.println("File deleted");
      } else {
        Serial.println("Delete failed");
      }
    }

    // Helper to normalize paths
    String normalize_path(const String& path) {
      if (path.startsWith("/")) {
        return path; // Absolute path, no change needed
      } else {
        if (current_directory == "/") {
          return "/" + path;
        } else {
          return current_directory + "/" + path;
        }
      }
    }

    // Helper to remove redundant slashes and resolve `.` and `..`
    String resolve_path(const String& path) {
      std::vector<String> parts;
      String resolved;

      int start = 0;
      for (int i = 0; i <= path.length(); i++) {
        if (i == path.length() || path[i] == '/') {
          String part = path.substring(start, i);
          start = i + 1;

          if (part == "" || part == ".") {
            continue; // Skip empty and current directory
          } else if (part == "..") {
            if (!parts.empty()) {
              parts.pop_back(); // Go up one directory
            }
          } else {
            parts.push_back(part);
          }
        }
      }

      for (const auto& part : parts) {
        resolved += "/" + part;
      }

      return resolved.length() == 0 ? "/" : resolved;
    }

    bool is_directory(const String& path) {
      File file = SD.open(path);
      bool result = file.isDirectory();
      file.close();
      return result;
    }

    void print_info(){
      Serial.println("=== SD Card Information ===");
      Serial.print("SD Card Type: ");
      Serial.println(get_cardtype());
      Serial.printf("SD Card Size: %lluKB\r\n", get_card_size());
      Serial.printf("SD Card Used space: %lluKB\r\n", get_card_usage());
      Serial.println("===========================");
    }

    bool is_exists(const char* path){
      if (SD.exists(path)){
        return true;
      }
      return false;
    }

    // Изменение текущего каталога
    void change_directory(const String& path) {
      // Нормализуем и разрешаем путь
      String normalized_path = normalize_path(path);
      String resolved_path = resolve_path(normalized_path);

      // Проверяем существование директории
      if (is_directory(resolved_path)) {
        current_directory = resolved_path;
        Serial.print("Current directory changed to: ");
        Serial.println(current_directory);
      } else {
        Serial.print("Failed to change directory: Directory doesn't exist ");
        Serial.println(resolved_path);
      }
    }

    // Перемещение файла с проверкой размера
    // Функция для копирования файла с возвращаемым значением успешности
    bool copy_file(const char* source_path, const char* destination_path) {
      Serial.printf("Copying file from %s to %s\r\n", source_path, destination_path);
      
      // Открываем исходный файл для чтения
      File source = SD.open(source_path, FILE_READ);
      if (!source) {
          Serial.println("Failed to open source file for copying");
          return false;  // Ошибка открытия исходного файла
      }

      // Получаем размер исходного файла
      uint32_t totalSize = source.size();
      uint32_t copiedSize = 0;
      int barsPrinted = 0;
      const int totalBars = 50;  // Количество символов прогресса
      const int nextBarThreshold = totalSize / totalBars;

      // Открываем файл назначения для записи
      File destination = SD.open(destination_path, FILE_WRITE);
      if (!destination) {
          Serial.println("Failed to open destination file for copying");
          source.close();
          return false;  // Ошибка открытия целевого файла
      }

      Serial.print("Copying file: [");

      // Копирование файла с отображением прогресса
      while (source.available()) {
          char buffer[128];  // Буфер для чтения данных (char вместо byte)
          size_t size = source.readBytes(buffer, sizeof(buffer));
          destination.write(reinterpret_cast<const uint8_t*>(buffer), size);  // Преобразуем buffer в uint8_t* перед записью
          copiedSize += size;

          // Проверяем, достигнут ли порог для добавления нового символа '#'
          while (copiedSize >= (barsPrinted + 1) * nextBarThreshold && barsPrinted < totalBars) {
              Serial.print("#");  // Печатаем новый символ '#'
              barsPrinted++;
          }
      }

      // Завершаем строку прогресса
      while (barsPrinted < totalBars) {
          Serial.print("#");
          barsPrinted++;
      }

      Serial.println("]");

      source.close();
      destination.close();

      // Проверка, что размер исходного и целевого файлов совпадает
      File checkFile = SD.open(destination_path, FILE_READ);
      if (!checkFile) {
          Serial.println("Failed to open destination file for checking size");
          return false;  // Ошибка открытия файла для проверки размера
      }

      uint32_t destinationSize = checkFile.size();
      checkFile.close();

      // Если размеры не совпадают, удаляем файл назначения и выводим ошибку
      if (totalSize != destinationSize) {
          Serial.println("Error: File sizes do not match.");
          SD.remove(destination_path);  // Удаляем файл назначения
          Serial.println("Destination file removed due to size mismatch.");
          return false;  // Ошибка копирования из-за несоответствия размеров
      }

      Serial.printf("File copied successfully: %s to %s\r\n", source_path, destination_path);
      return true;  // Успешное копирование
    }

    // Функция для перемещения файла с проверкой успешности копирования
    void move_file(const char* source_path, const char* destination_path) {
      Serial.printf("Moving file from %s to %s\r\n", source_path, destination_path);

      // Проверяем, существует ли исходный файл
      if (!SD.exists(source_path)) {
          Serial.println("Source file does not exist");
          return;
      }

      // Копируем файл в новую директорию
      bool copy_success = copy_file(source_path, destination_path);
      if (!copy_success) {
          Serial.println("File move failed due to copy error");
          return;  // Прерываем выполнение, если копирование не удалось
      }

      // Удаляем исходный файл после успешного копирования
      if (SD.remove(source_path)) {
          Serial.println("File moved successfully");
      } else {
          Serial.println("Failed to remove source file after copying");
      }
    }
    // Подсчет файлов в директории
    size_t count_files(const char* directory_path) {
      size_t file_count = 0;
      File dir = SD.open(directory_path);
      if (!dir || !dir.isDirectory()) {
        Serial.println("Invalid directory for counting files");
        return 0;
      }

      File file = dir.openNextFile();
      while (file) {
        if (!file.isDirectory()) {
          file_count++;
        }
        file = dir.openNextFile();
      }
      return file_count;
    }

    // Чтение строки из файла
    String read_line(File& file) {
      String line = "";
      while (file.available()) {
        char c = file.read();
        if (c == '\n') break;
        line += c;
      }
      return line;
    }

    // Поиск файла
    bool find_file(const char* directory_path, const char* filename) {
      File dir = SD.open(directory_path);
      if (!dir || !dir.isDirectory()) {
        Serial.println("Invalid directory for searching file");
        return false;
      }

      File file = dir.openNextFile();
      while (file) {
        if (!file.isDirectory() && strcmp(file.name(), filename) == 0) {
          return true;
        }
        file = dir.openNextFile();
      }
      return false;
    }
    
    // Получение размера файла
    size_t get_file_size(const char* file_path) {
      File file = SD.open(file_path);
      if (!file) {
        Serial.println("Failed to open file for getting size");
        return 0;
      }
      size_t size = file.size();
      file.close();
      return size;
    }

    bool create_empty_file(const char* path) {
      String normalized_path = normalize_path(path);
      String resolved_path = resolve_path(normalized_path);
      Serial.printf("Creating empty file: %s\r\n", resolved_path);

      // Проверяем, существует ли файл
      if (SD.exists(resolved_path)) {
        Serial.println("File already exists");
        return false;
      }

      // Открываем файл в режиме записи, создавая его
      File file = SD.open(resolved_path, FILE_WRITE);
      if (!file) {
        Serial.println("Failed to create file");
        return false;
      }

      Serial.println("Empty file created successfully");
      file.close();
      return true;
    }

    // Проверка типа файла
    String get_file_type(const char* file_path) {
      File file = SD.open(file_path);
      if (!file) {
        Serial.println("Failed to open file for checking type");
        return "";
      }
      
      String fileType = "";
      // Проверяем, является ли это каталогом или обычным файлом
      if (file.isDirectory()) {
        fileType = "Directory";
      } else {
        fileType = "File";
      }

      file.close();
      return fileType;
    }

    // Функция для поиска файла в директории и её подкаталогах
    void search_file(const String& directory, const char* filename) {
      String normalized_directory = normalize_path(directory); // Нормализуем путь
      String resolved_directory = resolve_path(normalized_directory); // Разрешаем путь
      Serial.printf("Searching for file: %s in directory: %s\r\n", filename, resolved_directory.c_str());
      bool found = search_in_directory(resolved_directory, filename); // Ищем файл в разрешённой директории
      if (found) {
        Serial.println("File found!");
      } else {
        Serial.println("File not found");
      }
    }

    // Рекурсивная функция для поиска файла в директории и её подкаталогах
    bool search_in_directory(const String& directory, const char* filename) {
      File dir = SD.open(directory);  // Открываем указанную директорию на SD-карте
      if (!dir || !dir.isDirectory()) {
        Serial.println("Invalid directory for searching file");
        return false;
      }

      File file = dir.openNextFile();
      while (file) {
        // Если файл найден, вернуть true и напечатать путь
        if (!file.isDirectory() && strcmp(file.name(), filename) == 0) {
          Serial.print("Found in directory: ");
          Serial.println(directory);  // Печатаем путь директории, где найден файл
          Serial.print("File: ");
          Serial.println(file.name());
          return true;
        }
        // Если это директория, рекурсивно ищем в ней
        if (file.isDirectory()) {
          if (search_in_directory(file.path(), filename)) {  // Рекурсивный вызов для вложенных папок
            return true;
          }
        }
        file = dir.openNextFile();
      }
      return false;
    }


    // Функция диагностики
    bool diagnostics() {
      color.print_log("=== Starting SD Card Diagnostics ===", true);

      bool init_status = false;
      bool memory_status = false;
      bool space_status = false;
      bool filesystem_status = false;
      bool write_test_status = false;
      bool skip_tests = true; // Флаг для пропуска тестов, если SD-карта не инициализирована

      // Проверка инициализации
      color.print_info("Checking SD Card initialization... ");
      SD.end();
      if (mount_sd()) {
          color.print_success("PASSED", true);
          init_status = true;
          skip_tests = false;
      } else {
          color.print_error("FAILED", true);
          color.print_error("ERROR: SD Card is not initialized", true);
      }

      // Проверка объёма памяти
      if (!skip_tests) {
          update_info();
          color.print_info("Checking SD Card memory...", true);
          color.print_log("  Total Space: " + String(cardSize) + " KB", true);
          color.print_log("  Used Space: " + String(cardUsage) + " KB", true);

          if (cardSize > 0 && cardUsage <= cardSize) {
              memory_status = true;
          } else {
              color.print_error("FAILED", true);
              color.print_error("ERROR: Invalid memory values detected on SD Card.", true);
          }
      }

      // Проверка свободного места
      if (!skip_tests) {
          color.print_info("Checking available space... ");
          if (get_card_free() < 256) {
              color.print_error("FAILED", true);
              color.print_error("CRITICAL: Not enough free space on SD card. At least 256 KB required.", true);
          } else {
              color.print_success("PASSED", true);
              space_status = true;
          }
      }

      // Проверка файловой системы
      if (!skip_tests) {
          color.print_info("Checking filesystem... ");
          if (SD.exists("/")) {
              color.print_success("PASSED", true);
              filesystem_status = true;
          } else {
              color.print_error("FAILED", true);
              color.print_error("ERROR: Filesystem not found or corrupted.", true);
          }
      }

      // Проверка возможности записи
      if (!skip_tests) {
          color.print_info("Testing write capability... ");
          const char *test_file_path = "/test.txt";
          const char *test_message = "SD Card Write Test";
          writeFile(SD, test_file_path, test_message);

          if (SD.exists(test_file_path)) {
              SD.remove(test_file_path);
              color.print_success("PASSED", true);
              write_test_status = true;
          } else {
              color.print_error("FAILED", true);
              color.print_error("ERROR: Write Test Failed.", true);
          }
      }

      // Вывод итоговых результатов тестов
      color.print_log("\n=== SD Card Diagnostics Summary ===", true);
      
      color.print_info("Initialization: ");
      init_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true);

      color.print_info("Memory Check: ");
      skip_tests ? color.print_warning("SKIPPED", true) : (memory_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true));

      color.print_info("Available Space: ");
      skip_tests ? color.print_warning("SKIPPED", true) : (space_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true));

      color.print_info("Filesystem Check: ");
      skip_tests ? color.print_warning("SKIPPED", true) : (filesystem_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true));

      color.print_info("Write Test: ");
      skip_tests ? color.print_warning("SKIPPED", true) : (write_test_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true));

      color.print_log("=== SD Card Diagnostics Complete ===", true);
      color.print_info("SD Card Diagnostics finished.", true);

      if (init_status && memory_status && space_status && filesystem_status && write_test_status) {
        return true;
      } else {
        return false;
      }
  }
};
