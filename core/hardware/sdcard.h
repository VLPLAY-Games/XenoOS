#include <FS.h>
#include <SD.h>
#include <SPI.h>

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
      Serial.print("  SD Card Type: ");
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
      Serial.printf("  SD Card Size: %lluMB\n", cardSize);
      timer.print_time();
      Serial.printf("  SD Card Used space: %lluMB\n", cardUsage);
      current_directory = "/";
      timer.print_time();
      Serial.println("Changed current directory to /");
      card_init = true;
      timer.print_time();
      Serial.println("");
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
          Serial.print(file.size());
          Serial.println("B");
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
      Serial.println("Read from file: ");
      Serial.println();
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

    void appendFile(fs::FS &fs, const char *path, const char *message, bool background=false) {
      if (!background) {
        Serial.printf("Appending to file: %s\n", path);
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

    bool is_exists(const char* path){
      if (SD.exists(path)){
        return true;
      }
      return false;
    }

    // Новая функция: Изменение текущего каталога
    void change_directory(const String& path) {
      String resolved_path = resolve_path(path);
      if (is_directory(resolved_path)) {
        current_directory = resolved_path;
        Serial.print("Current directory changed to: ");
        Serial.println(current_directory);
      } else {
        Serial.print("Failed to change directory: Directory doesn't exists ");
        Serial.println(resolved_path);
      }
    }

    // Новая функция: Перемещение файла
    void move_file(const char* source_path, const char* destination_path) {
      Serial.printf("Moving file from %s to %s\n", source_path, destination_path);

      // Проверяем, существует ли исходный файл
      if (!SD.exists(source_path)) {
        Serial.println("Source file does not exist");
        return;
      }

      // Копируем файл в новую директорию
      copy_file(source_path, destination_path);

      // Удаляем исходный файл после успешного копирования
      if (SD.remove(source_path)) {
        Serial.println("File moved successfully");
      } else {
        Serial.println("Failed to remove source file after copying");
      }
    }

    // Новая функция: Копирование файла
    void copy_file(const char* source_path, const char* destination_path) {
      File source = SD.open(source_path, FILE_READ);
      if (!source) {
        Serial.println("Failed to open source file for copying");
        return;
      }

      File destination = SD.open(destination_path, FILE_WRITE);
      if (!destination) {
        Serial.println("Failed to open destination file for copying");
        source.close();
        return;
      }

      while (source.available()) {
        destination.write(source.read());
      }

      Serial.printf("File copied from %s to %s\n", source_path, destination_path);
      source.close();
      destination.close();
    }

    // Новая функция: Подсчет файлов в директории
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

    // Новая функция: Чтение строки из файла
    String read_line(File& file) {
      String line = "";
      while (file.available()) {
        char c = file.read();
        if (c == '\n') break;
        line += c;
      }
      return line;
    }

    // Новая функция: Поиск файла
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
    
    // Новая функция: Получение размера файла
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

    void create_empty_file(const char* path) {
      Serial.printf("Creating empty file: %s\n", path);

      // Проверяем, существует ли файл
      if (SD.exists(path)) {
        Serial.println("File already exists");
        return;
      }

      // Открываем файл в режиме записи, создавая его
      File file = SD.open(path, FILE_WRITE);
      if (!file) {
        Serial.println("Failed to create file");
        return;
      }

      Serial.println("Empty file created successfully");
      file.close();
    }
};