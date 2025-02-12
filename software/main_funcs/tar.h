#include <ESP32-targz.h>
#include <SD.h>

class Tar {
  private:
    Help help; // Экземпляр класса помощи
    const char* tar_commands[2] = {"extract", "help"}; // Доступные команды

    // Проверяет, существует ли директория, и создает её, если нужно
    bool ensure_directory(const char* directory) {
      if (!SD.exists(directory)) {
        if (!SD.mkdir(directory)) {
          Serial.printf("Failed to create directory: %s\n", directory);
          return false;
        }
      }
      return true;
    }

    // Распаковка TAR-файлов
    void extract_tar(const char* tar_file, const char* extract_to) {
      if (!ensure_directory(extract_to)) return;

      File tarFile = SD.open(tar_file, FILE_READ);
      if (!tarFile) {
        Serial.printf("Failed to open TAR file: %s\r\n", tar_file);
        return;
      }

      TarUnpacker *TARUnpacker = new TarUnpacker();
      TARUnpacker->haltOnError(true); // Остановка при ошибке
      TARUnpacker->setTarVerify(true); // Проверка целостности
      TARUnpacker->setupFSCallbacks(targzTotalBytesFn, targzFreeBytesFn); // Проверка свободного места
      TARUnpacker->setLoggerCallback(BaseUnpacker::targzPrintLoggerCallback); // Логирование
      TARUnpacker->setTarProgressCallback(BaseUnpacker::defaultProgressCallback); // Прогресс распаковки
      TARUnpacker->setTarStatusProgressCallback(BaseUnpacker::defaultTarStatusProgressCallback); // Прогресс статуса
      TARUnpacker->setTarMessageCallback(BaseUnpacker::targzPrintLoggerCallback); // Сообщения TAR

      Serial.printf("Extracting TAR file: %s to %s\r\n", tar_file, extract_to);
      if (!TARUnpacker->tarStreamExpander(&tarFile, tarFile.size(), SD, extract_to)) {
        Serial.printf("Extraction failed with error code: %d\r\n", TARUnpacker->tarGzGetError());
      } else {
        Serial.println("TAR extraction completed successfully.");
      }

      tarFile.close();
      delete TARUnpacker;
    }

    // Распаковка TAR.GZ-файлов
    void extract_targz(const char* targz_file, const char* extract_to) {
      if (!ensure_directory(extract_to)) return;

      File targzFile = SD.open(targz_file, FILE_READ);
      if (!targzFile) {
        Serial.printf("Failed to open TAR.GZ file: %s\r\n", targz_file);
        return;
      }

      TarGzUnpacker *TARGZUnpacker = new TarGzUnpacker();
      TARGZUnpacker->haltOnError(true); // Остановка при ошибке
      TARGZUnpacker->setTarVerify(true); // Проверка целостности
      TARGZUnpacker->setupFSCallbacks(targzTotalBytesFn, targzFreeBytesFn); // Проверка свободного места
      TARGZUnpacker->setGzProgressCallback(BaseUnpacker::defaultProgressCallback); // Прогресс для GZ
      TARGZUnpacker->setLoggerCallback(BaseUnpacker::targzPrintLoggerCallback); // Логирование GZ
      TARGZUnpacker->setTarProgressCallback(BaseUnpacker::defaultProgressCallback); // Прогресс TAR
      TARGZUnpacker->setTarStatusProgressCallback(BaseUnpacker::defaultTarStatusProgressCallback); // Прогресс статуса
      TARGZUnpacker->setTarMessageCallback(BaseUnpacker::targzPrintLoggerCallback); // Сообщения TAR

      Serial.printf("Extracting TAR.GZ file: %s to %s\r\n", targz_file, extract_to);
      if (!TARGZUnpacker->tarGzStreamExpander(&targzFile, SD)) {
        Serial.printf("Extraction failed with error code: %d\r\n", TARGZUnpacker->tarGzGetError());
      } else {
        Serial.println("TAR.GZ extraction completed successfully.");
      }

      targzFile.close();
      delete TARGZUnpacker;
    }

  public:
    // Обработка команд TAR
    void handle_tar_commands(const std::vector<String>& command) {
      if (command.size() < 2) {
        Serial.println("Incomplete tar command");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available tar commands: ");
        help.print_help(tar_commands, sizeof(tar_commands) / sizeof(tar_commands[0]));
      } else if (command[1] == "extract") {
        if (command.size() < 4) {
          Serial.println("Usage: tar extract <file> <extract_to>");
          return;
        }

        const char* file = command[2].c_str();
        const char* extract_to = command[3].c_str();

        if (!SD.exists(file)) {
          Serial.printf("File not found: %s\r\n", file);
          return;
        }

        // Определяем, какой тип файла обрабатывать
        String filename = file;
        if (filename.endsWith(".tar")) {
          extract_tar(file, extract_to);
        } else if (filename.endsWith(".tar.gz") || filename.endsWith(".tgz")) {
          extract_targz(file, extract_to);
        } else {
          Serial.printf("Unsupported file type: %s\r\n", file);
        }
      } else {
        Serial.println("Invalid tar command");
      }
    }
};
