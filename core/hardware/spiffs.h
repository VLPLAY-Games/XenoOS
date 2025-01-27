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
        Serial.printf("Failed to open file %s for reading\n", path);
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
        Serial.printf("Failed to open file %s for writing\n", path);
        return false;
      }

      file.print(message);
      file.close();
      Serial.printf("File %s written successfully\n", path);
      return true;
    }

    // Дописать в файл
    bool appendFile(const char* path, const char* message) {
      File file = SPIFFS.open(path, "a");
      if (!file) {
        Serial.printf("Failed to open file %s for appending\n", path);
        return false;
      }

      file.print(message);
      file.close();
      Serial.printf("Message appended to %s\n", path);
      return true;
    }

    // Удаление файла
    bool deleteFile(const char* path) {
      if (SPIFFS.remove(path)) {
        Serial.printf("File %s deleted successfully\n", path);
        return true;
      } else {
        Serial.printf("Failed to delete file %s\n", path);
        return false;
      }
    }

    // Переименование файла
    bool renameFile(const char* oldPath, const char* newPath) {
      if (SPIFFS.rename(oldPath, newPath)) {
        Serial.printf("File %s renamed to %s\n", oldPath, newPath);
        return true;
      } else {
        Serial.printf("Failed to rename file %s\n", oldPath);
        return false;
      }
    }

    // Проверка существования файла
    bool fileExists(const char* path) {
      if (SPIFFS.exists(path)) {
        Serial.printf("File %s exists\n", path);
        return true;
      } else {
        Serial.printf("File %s does not exist\n", path);
        return false;
      }
    }

    // Получить размер файла
    size_t getFileSize(const char* path) {
      File file = SPIFFS.open(path, "r");
      if (!file) {
        Serial.printf("Failed to open file %s for size check\n", path);
        return 0;
      }
      size_t size = file.size();
      file.close();
      return size;
    }

    // Список файлов в каталоге
    void listDir(const char* dirPath) {
      Serial.printf("Listing directory: %s\n", dirPath);
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
        Serial.printf("Directory %s created\n", path);
        return true;
      } else {
        Serial.printf("Failed to create directory %s\n", path);
        return false;
      }
    }
    // Удаление директории
    bool removeDir(const char* path) {
      if (SPIFFS.rmdir(path)) {
        Serial.printf("Directory %s removed\n", path);
        return true;
      } else {
        Serial.printf("Failed to remove directory %s\n", path);
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

    // Функция диагностики SPIFFS
    void diagnostics() {
      Serial.println("=== SPIFFS Diagnostics ===");

      // Проверка инициализации
      if (!SPIFFS.begin(true)) {
        Serial.println("Error: SPIFFS not initialized.");
        return;
      }
      Serial.println("SPIFFS initialized successfully.");

      // Получение общей и используемой памяти
      size_t totalBytes = SPIFFS.totalBytes();
      size_t usedBytes = SPIFFS.usedBytes();

      Serial.printf("Total Space: %u KB\n", totalBytes / 1024);
      Serial.printf("Used Space: %u KB\n", usedBytes / 1024);

      if (totalBytes > 0 && usedBytes <= totalBytes) {
        Serial.println("SPIFFS memory check passed");
      } else {
        Serial.println("Error: Invalid memory values detected in SPIFFS.");
      }

      // Проверка доступности файловой системы
      if (!isMounted()) {
        Serial.println("Error: Filesystem not mounted.");
        return;
      } else {
        Serial.println("Filesystem mounted successfully.");
      }

      // Тест создания, записи, чтения и удаления файла
      const char* testFilePath = "/spiffs_test.txt";
      const char* testMessage = "SPIFFS Diagnostics Test Message";

      Serial.println("Performing file operations for diagnostics...");

      // Тест записи
      if (writeFile(testFilePath, testMessage)) {
        Serial.println("Write Test: Successful");

        // Тест чтения
        String content = readFile(testFilePath);
        if (content == testMessage) {
          Serial.println("Read Test: Successful");
        } else {
          Serial.println("Error: Read Test Failed. Content mismatch.");
        }

        // Удаление тестового файла
        if (deleteFile(testFilePath)) {
          Serial.println("Delete Test: Successful");
        } else {
          Serial.println("Error: Delete Test Failed.");
        }
      } else {
        Serial.println("Error: Write Test Failed.");
      }

      Serial.println("=== SPIFFS Diagnostics Complete ===");
    }
};