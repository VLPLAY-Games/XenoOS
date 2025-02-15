// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class DF {
  private:
    Esp esp;
    Spiffs spiffs;
    SdCard sd;
    const char* df_commands[2] = {"info", "help"}; // Массив строк с командами
    const char* df_dependencies[1] = {"Help"}; // Список зависимостей
    Help help; // Экземпляр класса помощи

  public:
    // Конструктор
    DF(Esp& esp_instance, Spiffs& spiffs_instance, SdCard& sd_instance) 
      : esp(esp_instance), spiffs(spiffs_instance), sd(sd_instance) {}

    // Метод для вывода всей информации о памяти в красивом виде
    void print_memory_info() {
      Serial.println("=================================");
      Serial.println("Memory Information (in KB):");
      Serial.println("=================================");
      
      // Оперативная память
      Serial.print("Free RAM: ");
      Serial.print(esp.free_ram());
      Serial.println(" KB");
      Serial.print("Total RAM: ");
      Serial.print(esp.total_ram());
      Serial.println(" KB");
      Serial.print("Used RAM: ");
      Serial.print(esp.used_ram());
      Serial.println(" KB");
      
      // Программная память
      Serial.print("Total Program Memory: ");
      Serial.print(esp.total_program_memory());
      Serial.println(" KB");
      
      // SD карта
      Serial.println("---------------------------------");
      Serial.println("SD Card Information:");
      Serial.print("Card Size: ");
      Serial.print(sd.get_card_size());
      Serial.println(" KB");
      Serial.print("Card Usage: ");
      Serial.print(sd.get_card_usage());
      Serial.println(" KB");
      Serial.print("Card Free: ");
      Serial.print(sd.get_card_free());
      Serial.println(" KB");
      
      // SPIFFS
      Serial.println("---------------------------------");
      Serial.println("SPIFFS Memory:");
      Serial.print("Total SPIFFS Memory: ");
      Serial.print(spiffs.total_memory());
      Serial.println(" KB");
      Serial.print("Used SPIFFS Memory: ");
      Serial.print(spiffs.used_memory());
      Serial.println(" KB");
      Serial.print("Free SPIFFS Memory: ");
      Serial.print(spiffs.free_memory());
      Serial.println(" KB");
      
      Serial.println("=================================");
    }

    void handle_df_commands(const std::vector<String>& command) {
        if (command.size() < 2) {
          Serial.println("Incomplete df command");
          return;
        }

        // Обработка команды "help"
        if (command[1] == "help") {
          Serial.print("Available df commands: ");
          help.print_help(df_commands, sizeof(df_commands) / sizeof(df_commands[0]));
          Serial.print("Dependencies: ");
          help.print_help(df_dependencies, sizeof(df_dependencies) / sizeof(df_dependencies[0]));
          return;
        } else if (command[1] == "info") {
          print_memory_info();
        } else {
            Serial.print("Unknown df command");
        }
    }
};