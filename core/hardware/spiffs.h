#include <FS.h>
#include <SPIFFS.h>

class Spiffs {
  public:
    // Инициализация SPIFFS
    bool begin() {
      if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return false;
      }
      Serial.println("SPIFFS Mounted successfully");
      return true;
    }

    // Завершение работы с SPIFFS
    void end() {
      SPIFFS.end();
      Serial.println("SPIFFS unmounted");
    }

    // Чтение файла
    String readFile(const char* path) {
      File file = SPIFFS.open(path, "r");
      String content = "";
      if (!file) {
        Serial.printf("Failed to open file %s for reading\r\n", path);
        return "";
      }

      while (file.available()) {
        content += (char)file.read();
      }

      file.close();
      return content;
    }

    // Запись в файл
    bool writeFile(const char* path, const char* message) {
      File file = SPIFFS.open(path, "w");
      if (!file) {
        Serial.printf("Failed to open file %s for writing\r\n", path);
        return false;
      }

      file.print(message);
      file.close();
      Serial.printf("File %s written successfully\r\n", path);
      return true;
    }

    // Дописать в файл
    bool appendFile(const char* path, const char* message) {
      File file = SPIFFS.open(path, "a");
      if (!file) {
        Serial.printf("Failed to open file %s for appending\r\n", path);
        return false;
      }

      file.print(message);
      file.close();
      Serial.printf("Message appended to %s\r\n", path);
      return true;
    }

    // Удаление файла
    bool deleteFile(const char* path) {
      if (SPIFFS.remove(path)) {
        Serial.printf("File %s deleted successfully\r\n", path);
        return true;
      } else {
        Serial.printf("Failed to delete file %s\r\n", path);
        return false;
      }
    }

    // Переименование файла
    bool renameFile(const char* oldPath, const char* newPath) {
      if (SPIFFS.rename(oldPath, newPath)) {
        Serial.printf("File %s renamed to %s\r\n", oldPath, newPath);
        return true;
      } else {
        Serial.printf("Failed to rename file %s\r\n", oldPath);
        return false;
      }
    }

    // Проверка существования файла
    bool fileExists(const char* path) {
      if (SPIFFS.exists(path)) {
        Serial.printf("File %s exists\r\n", path);
        return true;
      } else {
        Serial.printf("File %s does not exist\r\n", path);
        return false;
      }
    }

    // Получить размер файла
    size_t getFileSize(const char* path) {
      File file = SPIFFS.open(path, "r");
      if (!file) {
        Serial.printf("Failed to open file %s for size check\r\n", path);
        return 0;
      }
      size_t size = file.size();
      file.close();
      return size;
    }

    // Список файлов в каталоге
    void listDir(const char* dirPath) {
      Serial.printf("Listing directory: %s\r\n", dirPath);
      File root = SPIFFS.open(dirPath);
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
        } else {
          Serial.print("  FILE: ");
          Serial.print(file.name());
          Serial.print("  SIZE: ");
          Serial.println(file.size());
        }
        file = root.openNextFile();
      }
    }

    // Создание директории
    bool createDir(const char* path) {
      if (SPIFFS.mkdir(path)) {
        Serial.printf("Directory %s created\r\n", path);
        return true;
      } else {
        Serial.printf("Failed to create directory %s\r\n", path);
        return false;
      }
    }
    // Удаление директории
    bool removeDir(const char* path) {
      if (SPIFFS.rmdir(path)) {
        Serial.printf("Directory %s removed\r\n", path);
        return true;
      } else {
        Serial.printf("Failed to remove directory %s\r\n", path);
        return false;
      }
    }

    // Очистка всего содержимого SPIFFS
    void format() {
      SPIFFS.format();
      Serial.println("SPIFFS formatted");
    }

    // Проверка, доступна ли файловая система
    bool isMounted() {
      return SPIFFS.begin(true);
    }

    // Функция для получения общего объема памяти
    uint32_t total_memory() {
      return SPIFFS.totalBytes();
    }

    // Функция для получения использованного объема памяти
    uint32_t used_memory() {
      return SPIFFS.usedBytes();
    }

    // Функция для получения оставшегося объема памяти
    uint32_t free_memory() {
      return total_memory() - used_memory();
    }

    void print_info(Timer* timer = nullptr){
      if (timer) {
        timer->println_with_timer(String("  SPIFFS Total: ") + (total_memory() / 1024) + String(" KB"));
        timer->println_with_timer(String("  SPIFFS Used: ") + (used_memory() / 1024) + String(" KB"));
        timer->println_with_timer(String("  SPIFFS Free: ") + (free_memory() / 1024) + String(" KB"));
      } else {
        Serial.printf("  SPIFFS Total: %s KB\r\n", total_memory() / 1024);
        Serial.printf("  SPIFFS Used: %s KB\r\n", used_memory() / 1024);
        Serial.printf("  SPIFFS Free: %s KB\r\n", free_memory() / 1024);
      }
    }

    // Функция диагностики SPIFFS
    void diagnostics() {
      Serial.println("=== Starting SPIFFS Diagnostics ===");
  
      bool init_status = false;
      bool memory_status = false;
      bool mount_status = false;
      bool write_test_status = false;
      bool read_test_status = false;
      bool delete_test_status = false;
      bool skip_tests = true; // Флаг для пропуска тестов, если SPIFFS не инициализирован
  
      // Проверка инициализации
      Serial.print("Checking SPIFFS initialization... ");
      if (SPIFFS.begin(true)) {
          Serial.println("OK");
          init_status = true;
          skip_tests = false; // Позволяем выполнять остальные тесты
      } else {
          Serial.println("FAILED");
          Serial.println("ERROR: SPIFFS not initialized.");
      }
  
      // Получение информации о памяти (если инициализировано)
      if (!skip_tests) {
          Serial.print("Checking SPIFFS memory... ");
          size_t totalBytes = SPIFFS.totalBytes();
          size_t usedBytes = SPIFFS.usedBytes();
          Serial.printf("\n  Total Space: %u KB\r\n", totalBytes / 1024);
          Serial.printf("  Used Space: %u KB\r\n", usedBytes / 1024);
          if (totalBytes > 0 && usedBytes <= totalBytes) {
              memory_status = true;
          } else {
              Serial.println("FAILED");
              Serial.println("ERROR: Invalid memory values detected in SPIFFS.");
          }
      }
  
      // Проверка файловой системы (если инициализировано)
      if (!skip_tests) {
          Serial.print("Checking filesystem mount... ");
          if (isMounted()) {
              Serial.println("OK");
              mount_status = true;
          } else {
              Serial.println("FAILED");
              Serial.println("ERROR: Filesystem not mounted.");
          }
      }
  
      // Тест создания, записи, чтения и удаления файла (если инициализировано)
      if (!skip_tests && mount_status) {
          Serial.println("Performing file operations for diagnostics...");
          const char* testFilePath = "/spiffs_test.txt";
          const char* testMessage = "SPIFFS Diagnostics Test Message";
  
          // Тест записи
          Serial.print("Testing write operation... ");
          if (writeFile(testFilePath, testMessage)) {
              write_test_status = true;
  
              // Тест чтения
              Serial.print("Testing read operation... ");
              String content = readFile(testFilePath);
              if (content == testMessage) {
                  Serial.println("OK");
                  read_test_status = true;
              } else {
                  Serial.println("FAILED");
                  Serial.println("ERROR: Read Test Failed. Content mismatch.");
              }
  
              // Удаление тестового файла
              Serial.print("Testing file deletion... ");
              if (deleteFile(testFilePath)) {
                  delete_test_status = true;
              } else {
                  Serial.println("FAILED");
                  Serial.println("ERROR: Delete Test Failed.");
              }
          } else {
              Serial.println("FAILED");
              Serial.println("ERROR: Write Test Failed.");
          }
      }
  
      // Вывод итоговых результатов тестов
      Serial.println("\n=== SPIFFS Diagnostics Summary ===");
      Serial.printf("Initialization: %s\r\n", init_status ? "PASSED" : "FAILED");
      Serial.printf("Memory Check: %s\r\n", skip_tests ? "SKIPPED" : (memory_status ? "PASSED" : "FAILED"));
      Serial.printf("Filesystem Mount: %s\r\n", skip_tests ? "SKIPPED" : (mount_status ? "PASSED" : "FAILED"));
      Serial.printf("Write Test: %s\r\n", skip_tests || !mount_status ? "SKIPPED" : (write_test_status ? "PASSED" : "FAILED"));
      Serial.printf("Read Test: %s\r\n", skip_tests || !mount_status ? "SKIPPED" : (read_test_status ? "PASSED" : "FAILED"));
      Serial.printf("Delete Test: %s\r\n", skip_tests || !mount_status ? "SKIPPED" : (delete_test_status ? "PASSED" : "FAILED"));
  
      Serial.println("=== SPIFFS Diagnostics Complete ===\n");
  }
};