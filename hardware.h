#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Update.h>

extern String current_directory = "/";
extern uint8_t cardType = 0;
extern uint64_t cardSize = 0;
extern uint64_t cardUsage = 0;
extern bool card_init = false;

class SdCard{
  private:
    Help help;
    const char* sdcard_commands[2] = {"info", "help"}; // Массив строк с командами
  public:
    void init(Timer timer){
      if(!SD.begin()){
        timer.print_time();
        Serial.println("Card Mount Failed");
        return;
      }
      cardType = SD.cardType();

      if(cardType == CARD_NONE){
        timer.print_time();
        Serial.println("No SD card attached");
        return;
      }
      timer.print_time();
      Serial.print("SD Card Type: ");
      if(cardType == CARD_MMC){
        Serial.println("MMC");
      } else if(cardType == CARD_SD){
        Serial.println("SDSC");
      } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
      } else {
        Serial.println("UNKNOWN");
      }
      cardSize = SD.cardSize() / (1024 * 1024);
      cardUsage = SD.usedBytes() / (1024 * 1024);
      timer.print_time();
      Serial.printf("SD Card Size: %lluMB\n", cardSize);
      timer.print_time();
      Serial.printf("SD Card Used space: %lluMB\n", cardUsage);
      current_directory = "/";
      timer.print_time();
      Serial.println("Changed current directory to /");
      card_init = true;
      timer.print_time();
      Serial.println("SD Card Loaded");
    }


    void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
      Serial.printf("Listing directory: %s\n", dirname);

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
          Serial.println(file.size());
        }
        file = root.openNextFile();
      }
    }

    void createDir(fs::FS &fs, const char *path) {
      Serial.printf("Creating Dir: %s\n", path);
      if (fs.mkdir(path)) {
        Serial.println("Dir created");
      } else {
        Serial.println("mkdir failed");
      }
    }


    void removeDir(fs::FS &fs, const char *path) {
      Serial.printf("Removing Dir: %s\n", path);
      if (fs.rmdir(path)) {
        Serial.println("Dir removed");
      } else {
        Serial.println("rmdir failed");
      }
    }

    void readFile(fs::FS &fs, const char *path) {
      Serial.printf("Reading file: %s\n", path);

      File file = fs.open(path);
      if (!file) {
        Serial.println("Failed to open file for reading");
        return;
      }

      Serial.print("Read from file: ");
      while (file.available()) {
        Serial.write(file.read());
      }
      file.close();
    }


    void writeFile(fs::FS &fs, const char *path, const char *message) {
      Serial.printf("Writing file: %s\n", path);

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

    void appendFile(fs::FS &fs, const char *path, const char *message) {
      Serial.printf("Appending to file: %s\n", path);

      File file = fs.open(path, FILE_APPEND);
      if (!file) {
        Serial.println("Failed to open file for appending");
        return;
      }
      if (file.print(message)) {
        Serial.println("Message appended");
      } else {
        Serial.println("Append failed");
      }
      file.close();
    }

    void renameFile(fs::FS &fs, const char *path1, const char *path2) {
      Serial.printf("Renaming file %s to %s\n", path1, path2);
      if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
      } else {
        Serial.println("Rename failed");
      }
    }

    void deleteFile(fs::FS &fs, const char *path) {
      Serial.printf("Deleting file: %s\n", path);
      if (fs.remove(path)) {
        Serial.println("File deleted");
      } else {
        Serial.println("Delete failed");
      }
    }
    

    String get_cardtype(){
      cardType = SD.cardType();
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
      Serial.printf("SD Card Size: %lluMB\n", cardSize);
      Serial.printf("SD Card Used space: %lluMB\n", cardUsage);
      Serial.println("===========================");
    }

    void handle_sdcard_commands(const std::vector<String>& command) {
      if (command.size() < 2) {
        Serial.println("Incomplete sdcard command");
        return;
      }

      if (command[1] == "info") {
        print_info();
      }
      // Обработка команды "help"
      else if (command[1] == "help") {
        Serial.print("Available sdcard commands: ");
        help.print_help(sdcard_commands, sizeof(sdcard_commands) / sizeof(sdcard_commands[0]));
      } 
      else {
        Serial.println("Unknown sdcard command");
      }
    }

};


class Esp {
  private:
    uint32_t free_ram_kb;
    uint32_t total_ram_kb;
    uint32_t used_ram_kb;
    uint32_t total_program_memory_kb;
    const char* chip_model_name;
    uint16_t chip_revision_id;
    uint32_t chip_unique_id;
    uint32_t cpu_frequency_mhz;
    uint8_t chip_core_count;

    // Вспомогательная функция для расчета уникального ID чипа
    uint32_t calculate_chip_id() {
      uint32_t chipId = 0;
      for (int i = 0; i < 17; i += 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
      }
      return chipId;
    }

  public:
    // Конструктор для инициализации данных
    Esp() {
      free_ram_kb = ESP.getFreeHeap() / 1024;
      total_ram_kb = ESP.getHeapSize() / 1024;
      used_ram_kb = (ESP.getHeapSize() - ESP.getFreeHeap()) / 1024;
      total_program_memory_kb = ESP.getFlashChipSize() / 1024;
      chip_model_name = ESP.getChipModel();
      chip_revision_id = ESP.getChipRevision();
      chip_unique_id = calculate_chip_id();
      cpu_frequency_mhz = ESP.getCpuFreqMHz();
      chip_core_count = ESP.getChipCores();
    }

    // Методы для получения и обновления значений
    uint32_t free_ram() {
      free_ram_kb = ESP.getFreeHeap() / 1024; // Обновляем переменную
      return free_ram_kb; // Возвращаем значение
    }

    uint32_t total_ram() {
      total_ram_kb = ESP.getHeapSize() / 1024; // Обновляем переменную
      return total_ram_kb; // Возвращаем значение
    }

    uint32_t used_ram() {
      used_ram_kb = (ESP.getHeapSize() - ESP.getFreeHeap()) / 1024; // Обновляем переменную
      return used_ram_kb; // Возвращаем значение
    }

    uint32_t total_program_memory() {
      total_program_memory_kb = ESP.getFlashChipSize() / 1024; // Обновляем переменную
      return total_program_memory_kb; // Возвращаем значение
    }

    const char* chip_model() {
      chip_model_name = ESP.getChipModel(); // Обновляем переменную
      return chip_model_name; // Возвращаем значение
    }

    uint16_t chip_revision() {
      chip_revision_id = ESP.getChipRevision(); // Обновляем переменную
      return chip_revision_id; // Возвращаем значение
    }

    uint32_t chip_id() {
      chip_unique_id = calculate_chip_id(); // Обновляем переменную
      return chip_unique_id; // Возвращаем значение
    }

    uint32_t cpu_freq() {
      cpu_frequency_mhz = ESP.getCpuFreqMHz(); // Обновляем переменную
      return cpu_frequency_mhz; // Возвращаем значение
    }

    uint8_t chip_cores() {
      chip_core_count = ESP.getChipCores(); // Обновляем переменную
      return chip_core_count; // Возвращаем значение
    }

    // Метод для вывода всей информации
    void print_all_info() {
      Serial.println("=== System Information ===");
      Serial.printf("Free RAM: %u KB\n", free_ram());
      Serial.printf("Total RAM: %u KB\n", total_ram());
      Serial.printf("Used RAM: %u KB\n", used_ram());
      Serial.printf("Total Program Memory: %u KB\n", total_program_memory());
      Serial.printf("Chip Model: %s\n", chip_model());
      Serial.printf("Chip Revision: %u\n", chip_revision());
      Serial.printf("Chip ID: %u\n", chip_id());
      Serial.printf("CPU Frequency: %u MHz\n", cpu_freq());
      Serial.printf("Chip Cores: %u\n", chip_cores());
      Serial.println("===========================");
    }

    // Метод для вывода cpu информации
    void print_cpu_info() {
      Serial.println("=== CPU Information ===");
      Serial.printf("CPU Frequency: %u MHz\n", cpu_freq());
      Serial.printf("Chip Cores: %u\n", chip_cores());
      Serial.println("===========================");
    }

    // Метод для вывода cpu информации
    void print_ram_info() {
      Serial.println("=== Memory Information ===");
      Serial.printf("Free RAM: %u KB\n", free_ram());
      Serial.printf("Total RAM: %u KB\n", total_ram());
      Serial.printf("Used RAM: %u KB\n", used_ram());
      Serial.printf("Total Program Memory: %u KB\n", total_program_memory());
      Serial.println("===========================");
    }

    // Метод для перезагрузки системы
    void restart() {
      ESP.restart();
    }

    // perform the actual update from a given stream
    void performUpdate(Stream &updateSource, size_t updateSize) {
      if (Update.begin(updateSize)) {
        size_t written = Update.writeStream(updateSource);
        if (written == updateSize) {
          Serial.println("Written : " + String(written) + " successfully");
        } else {
          Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
        }
        if (Update.end()) {
          Serial.println("OTA done!");
          if (Update.isFinished()) {
            Serial.println("Update successfully completed. Rebooting.");
          } else {
            Serial.println("Update not finished? Something went wrong!");
          }
        } else {
          Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        }

      } else {
        Serial.println("Not enough space to begin OTA");
      }
    }

    // check given FS for valid update.bin and perform update if available
    void updateFromFS(fs::FS &fs) {
      File updateBin = fs.open("/upd/update.bin");
      if (updateBin) {
        if (updateBin.isDirectory()) {
          Serial.println("Error, /upd/update.bin is not a file");
          updateBin.close();
          return;
        }

        size_t updateSize = updateBin.size();

        if (updateSize > 0) {
          Serial.println("Try to start update");
          performUpdate(updateBin, updateSize);
        } else {
          Serial.println("Error, file is empty");
        }

        updateBin.close();

        // when finished remove the binary from sd card to indicate end of the process
        fs.remove("/upd/update.bin");
      } else {
        Serial.println("Could not load update.bin from sd root");
      }
    }
};
