// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.


class Bootloader {
  private:
    bool boot_success = false;
    SdCard& sd;    
    Wifi& wifi;
    Esp& esp;
    Timer& timer;
    Spiffs spiffs;
    Eeprom eeprom;

    // Инициализация загрузчика
    void initialize_bootloader() {
      timer.println_with_timer("");
      timer.println_with_timer("       Bootloader Info");
      timer.println_with_timer("===========================");
      timer.println_with_timer(String("  Name: ") + bl_name);
      timer.println_with_timer(String("  Version: ") + bl_version);
      timer.println_with_timer(String("  Build Date: ") + bl_date);
      timer.println_with_timer("===========================");
      timer.println_with_timer("");
    }

    // Логирование информации о чипе
    void log_esp_info() {
      timer.println_with_timer("");
      timer.println_with_timer("      ESP32 Information");
      timer.println_with_timer("===========================");
      timer.println_with_timer("Chip Model: " + String(esp.chip_model()));
      timer.println_with_timer("Chip Revision: " + String(esp.chip_revision()));
      timer.println_with_timer("Chip ID: " + String(esp.chip_id()));
      timer.println_with_timer("CPU Frequency: " + String(esp.cpu_freq()) + " MHz");
      timer.println_with_timer("Chip Cores: " + String(esp.chip_cores()));
      timer.println_with_timer("Total Program Memory: " + String(esp.total_program_memory()) + " KB");
      timer.println_with_timer("Total RAM: " + String(esp.total_ram()) + " KB");
      timer.println_with_timer("===========================");
      timer.println_with_timer("");
    }

    // Инициализация оборудования
    void initialize_hardware() {
      timer.println_with_timer("Initializing Hardware");
      log_esp_info();
      timer.println_with_timer("Hardware initialization successful");
      timer.println_with_timer("");
    }

    // Инициализация системы
    void initialize_system() {
      timer.println_with_timer("      System Information");
      timer.println_with_timer("===========================");
      timer.println_with_timer(String("  OS Name: ") + os_name);
      timer.println_with_timer(String("  OS Version: ") + os_version);
      timer.println_with_timer(String("  Build Date: ") + os_date);
      timer.println_with_timer("===========================");
      timer.println_with_timer("");
    }

    // Инициализация ядра
    void initialize_core() {
      timer.println_with_timer("      Core Information");
      timer.println_with_timer("===========================");
      timer.println_with_timer(String("  Core Name: ") + core_name);
      timer.println_with_timer(String("  Core Version: ") + core_version);
      timer.println_with_timer(String("  Build Date: ") + core_date);
      timer.println_with_timer("===========================");
      timer.println_with_timer("");
    }

    // Загрузка модулей
    void load_modules() {
      timer.println_with_timer("Loading Modules");

      timer.println_with_timer("");
      timer.println_with_timer("Loading SD Card module...");
      sd.mount_sd(&timer, true);
      timer.println_with_timer("");
      timer.println_with_timer("     SD Card Information");
      timer.println_with_timer("===========================");
      bool sd_init = sd.init(timer);
      if (sd_init) {
        timer.println_with_timer("SD Card module loaded successfully.");
      } else {
        timer.println_with_timer("Failed to load SD Card module.");
      }

      timer.println_with_timer("Checking SD Card Free space...");
      if (sd.get_card_free() < 256) {
        timer.println_with_timer("CRITICAL: Not enough free space on SD card. At least 256 KB required.");
        sd_init = false;
      } else {
        timer.println_with_timer("SD Card Free space checked");
      }

      timer.println_with_timer("");
      timer.println_with_timer("Loading SPIFFS module...");
      timer.print_time();
      bool spiffs_init = spiffs.begin();
      if (spiffs_init) {
        timer.println_with_timer("");
        timer.println_with_timer("     SPIFFS Information");
        timer.println_with_timer("===========================");
        spiffs.print_info(&timer);
        timer.println_with_timer("===========================");
        timer.println_with_timer("");
        timer.println_with_timer("SPIFFS module loaded successfully.");
      } else {
        timer.println_with_timer("Failed to load SPIFFS module.");
      }

      timer.println_with_timer("");
      timer.println_with_timer("Loading EEPROM module...");
      timer.print_time();
      bool eeprom_init = eeprom.init();

      if (eeprom_init) {
        Serial.println();
        timer.println_with_timer("     EEPROM Information");
        timer.println_with_timer("===========================");
        eeprom.print_info(&timer);
        timer.println_with_timer("===========================");
        timer.println_with_timer("");
        timer.println_with_timer("EEPROM module loaded successfully.");
      } else {
        timer.println_with_timer("Failed to load EEPROM module.");
      }


      timer.println_with_timer("");

      if (sd_init && spiffs_init && eeprom_init) {
        boot_success = true;
      }
    }

    // Режим восстановления
    void enter_recovery_mode() {
      timer.println_with_timer("   Entering Recovery Mode");
      timer.println_with_timer("");
      RecoveryMode rm(esp, timer, spiffs, sd, eeprom);
      rm.recovery();
    }

    // Логирование причин сброса процессоров
    void print_cpu_reason() {
      timer.println_with_timer("     CPU Reset Reasons");
      timer.println_with_timer("===========================");
      log_reset_reason(0); // CPU 0
      log_reset_reason(1); // CPU 1
      timer.println_with_timer("===========================");
      timer.println_with_timer("");
    }

    // Логирование причины сброса для указанного процессора
    void log_reset_reason(uint8_t cpu) {
      timer.print_with_timer(String("  CPU") + String(cpu) + ": ");
      esp.print_reset_reason(rtc_get_reset_reason(cpu));
      Serial.print(", ");
      esp.verbose_print_reset_reason(rtc_get_reset_reason(cpu));
      Serial.println("");
    }

  public:
    // Конструктор
    Bootloader(SdCard& sd_instance, Wifi& wifi_instance, Esp& esp_instance, Timer& timer_instance, Spiffs& spiffs_instance, Eeprom& eeprom_instance)
      : sd(sd_instance), wifi(wifi_instance), esp(esp_instance), timer(timer_instance), spiffs(spiffs_instance), eeprom(eeprom_instance) {
      timer.println_with_timer("");
      timer.println_with_timer("Initializing Bootloader");
      initialize_bootloader();
      print_cpu_reason();
      timer.println_with_timer("Initialized Bootloader Successful");
    }

    // Основной метод загрузки
    void boot() {
      timer.println_with_timer("Starting Boot Process");

      initialize_hardware();
      initialize_core();
      initialize_system();

      timer.println_with_timer("Starting Module Loading");
      load_modules();

      if (boot_success) {
        timer.println_with_timer("===========================");
        timer.println_with_timer("  System Initialization OK");
        Serial.println();
        Serial.print(current_directory + " $ ");
      } else {
        timer.println_with_timer("===========================");
        timer.println_with_timer("  System Initialization Failed");
        enter_recovery_mode();
      }
    }
};