#include "SD.h"
#include <HTTPClient.h>

class MainFuncs{
  private:
    Help help;
  public:
    void update(){}
    void restart(Esp esp){
      esp.restart();
    }
    void free(Esp esp){
      esp.print_ram_info();
    }
    void clear(){
      Serial.write(27);       // ESC command
      Serial.print("[2J");    // clear screen command
      Serial.write(27);
      Serial.print("[H");     // cursor to home command
    }
    void info(Esp esp){
      Serial.println(os_name);
      Serial.print("Version ");
      Serial.println(os_version);
      Serial.println(made_by);
      Serial.print("OS Date ");
      Serial.println(os_date);
      esp.print_all_info();
      Serial.println(os_github);
    }
    void help_commands(){
      Serial.println("Available commands:");
      help.print_help(commands, sizeof(commands) / sizeof(commands[0]));
    }
};

class Cd {
  public:
    void handle_cd_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete cd command");
        return;
      }

      String new_path = sd.normalize_path(command[1]);
      new_path = sd.resolve_path(new_path);

      if (SD.exists(new_path)) {
        current_directory = new_path;
        Serial.println("Directory changed to: " + current_directory);
      } else {
        Serial.println("Directory does not exist: " + new_path);
      }

      Serial.println("Current directory: " + current_directory);
    }
};

class Mkdir {
  public:
    void handle_mkdir_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete mkdir command");
        return;
      }

      String path = sd.normalize_path(command[1]);
      path = sd.resolve_path(path);

      if (SD.exists(path)) {
        Serial.println("Directory already exists: " + path);
        return;
      }

      sd.createDir(SD, path.c_str());
    }
};

class Rm {
  public:
    void handle_rm_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete rm command");
        return;
      }

      String path = sd.normalize_path(command[1]);
      path = sd.resolve_path(path);

      if (SD.exists(path)) {
        if (sd.is_directory(path)) {
          sd.removeDir(SD, path.c_str());
        } else {
          sd.deleteFile(SD, path.c_str());
        }
      } else {
        Serial.println("Path does not exist: " + path);
      }
    }
};

class Ls {
  public:
    void handle_ls_commands(const std::vector<String>& command, SdCard& sd) {
      String target_dir = current_directory;
      if (command.size() > 1) {
        target_dir = sd.normalize_path(command[1]);
        target_dir = sd.resolve_path(target_dir);
      }

      if (SD.exists(target_dir)) {
        sd.listDir(SD, target_dir.c_str(), 0);
      } else {
        Serial.println("Directory does not exist: " + target_dir);
      }
    }
};

class Cpu {
  private:
    const char* cpu_commands[2] = {"info", "help"}; // Массив строк с командами
    Help help;
  public:

    void handle_cpu_commands(const std::vector<String>& command, Esp esp) {
        if (command.size() < 2) {
          Serial.println("Incomplete cpu command");
          return;
        }

        // Обработка команды "info"
        if (command[1] == "info") {
          esp.print_cpu_info();
        }
        // Обработка команды "help"
        else if (command[1] == "help") {
          Serial.print("Available cpu commands: ");
          help.print_help(cpu_commands, sizeof(cpu_commands) / sizeof(cpu_commands[0]));
        }
        // Неизвестная команда
        else {
          Serial.println("Unknown cpu command");
        }
    }
};

class Wget {
  private:
    Wifi wifi; // Экземпляр класса Wifi для управления подключением
    SdCard sdcard; // Экземпляр класса SdCard для работы с SD-картой
    Help help;
    String default_download_path = "/downloads"; // Путь по умолчанию для загрузки
    const char* wget_commands[2] = {"<url> [<path>]", "help"}; // Массив строк с командами

    // Метод для проверки существования директории
    bool validate_directory(const String& path) {
      if (!sdcard.is_directory(path)) {
        Serial.printf("Error: Path does not exist: %s\n", path.c_str());
        return false;
      }
      return true;
    }

    // Метод для скачивания файла
    void downloadFile(const char* url, const char* path) {
      HTTPClient http;
      // Выполнение HTTP GET запроса к серверу
      http.begin(url);
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        File file = SD.open(path, FILE_WRITE);  // Открываем файл для записи
        if (file) {
          http.writeToStream(&file); // Записываем данные в файл
          file.close();
          Serial.println("File downloaded successfully");
        } else {
          Serial.println("Failed to open file for writing");
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
      if (!validate_directory(resolved_path)) {
          return; // Завершаем выполнение, если путь не существует
      }

      // Извлекаем имя файла из URL
      int last_slash = url.lastIndexOf('/');
      String filename = last_slash != -1 && last_slash < url.length() - 1 ? url.substring(last_slash + 1) : "file.txt";

      // Формируем полный путь для сохранения файла
      String file_path = sdcard.resolve_path(resolved_path + "/" + filename);

      Serial.printf("Starting download: %s\n", url.c_str());

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


class Cat {
  private:
    SdCard &sdcard;           // Ссылка на объект SdCard для взаимодействия с файловой системой
    Help help;                // Объект для управления справкой
    const char* cat_commands[2] = {"<path_to_file>", "help"}; // Список доступных команд для cat

  public:
    // Конструктор
    Cat(SdCard &sd) : sdcard(sd) {}

    // Чтение содержимого файла и вывод в консоль
    void read_file(const String &path) {
      // Нормализуем путь к файлу
      String resolved_path = sdcard.resolve_path(path);

      // Проверяем, существует ли файл
      if (!SD.exists(resolved_path)) {
        Serial.printf("File not found: %s\n", resolved_path.c_str());
        return;
      }

      // Используем метод readFile из sdcard
      sdcard.readFile(SD, resolved_path.c_str());
    }

    // Обработка команд cat
    void handle_cat_commands(const std::vector<String> &command) {
      if (command.size() < 2) {
        Serial.println("Usage: cat <file_path>");
        return;
      }

      // Команда "help" для вывода справки
      if (command[1] == "help") {
        Serial.print("Available cat commands: ");
        help.print_help(cat_commands, sizeof(cat_commands) / sizeof(cat_commands[0]));
        return;
      }

      // Чтение файла
      read_file(command[1]);
    }
};

class Rename {
  private:
    SdCard &sdcard;           // Ссылка на объект SdCard для взаимодействия с файловой системой
    Help help;                // Объект для управления справкой
    const char* rename_commands[2] = {"<old_file_path> <new_file_path>", "help"}; // Список доступных команд для rename

  public:
    // Конструктор
    Rename(SdCard &sd) : sdcard(sd) {}

    // Переименование файла
    void rename_file(const String &old_name, const String &new_name) {
      // Нормализуем пути к файлам
      String resolved_old_name = sdcard.resolve_path(old_name);
      String resolved_new_name = sdcard.resolve_path(new_name);

      // Проверяем, существует ли файл с исходным именем
      if (!SD.exists(resolved_old_name)) {
        Serial.printf("File not found: %s\n", resolved_old_name.c_str());
        return;
      }

      // Переименование файла
      sdcard.renameFile(SD, resolved_old_name.c_str(), resolved_new_name.c_str());
    }

    // Обработка команд rename
    void handle_rename_commands(const std::vector<String> &command) {
      if (command.size() < 3 && command[1] != "help") {
        Serial.println("Usage: rename <old_file_path> <new_file_path>");
        return;
      }

      // Команда "help" для вывода справки
      if (command[1] == "help") {
        Serial.print("Available rename commands: ");
        help.print_help(rename_commands, sizeof(rename_commands) / sizeof(rename_commands[0]));
        return;
      }

      // Переименование файла
      rename_file(command[1], command[2]);
    }
};

class SystemUpdate {
  private:
    Wget &wget;                 // Ссылка на объект Wget для загрузки обновлений
    Esp &esp;                   // Ссылка на объект Esp для обновлений
    Help help;                  // Объект для справки
    const char* update_commands[3] = {"download", "install", "help"}; // Список доступных команд для обновлений
    

  public:
    // Конструктор
    SystemUpdate(Wget &wget_instance, Esp &esp_instance) : wget(wget_instance), esp(esp_instance) {}


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
        return;
      }

      // Команда загрузки обновления
      if (command[2] == "download") {
        wget.handle_wget_commands({"wget", sys_upd_url, "/upd"});
      } 
      // Команда установки обновления
      else if (command[2] == "install") {
        esp.updateFromFS(SD);
      } else {
        Serial.println("Invalid update command");
      }
    }
};