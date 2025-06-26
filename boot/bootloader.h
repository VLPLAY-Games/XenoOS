// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.

#include "buildTime.h"

class Bootloader {
  private:
    bool boot_success = false;
    SdCard& sd;    
    Wifi& wifi;
    Esp& esp;
    Timer& timer;
    Spiffs spiffs;
    Eeprom eeprom;
    Checker checker;

    // Инициализация загрузчика
    void initialize_bootloader() {
      timer.println_with_timer("");
      timer.println_with_timer("       Bootloader Info", "log");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer(String("  Name: ") + bl_name, "info");
      timer.println_with_timer(String("  Version: ") + bl_version, "info");
      timer.println_with_timer(String("  Build Date: ") + bl_date, "info");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("");
    }

    // Логирование информации о чипе
    void log_esp_info() {
      timer.println_with_timer("");
      timer.println_with_timer("      ESP32 Information", "log");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("Chip Model: " + String(esp.chip_model()), "info");
      timer.println_with_timer("Chip Revision: " + String(esp.chip_revision()), "info");
      timer.println_with_timer("Chip ID: " + String(esp.chip_id()), "info");
      timer.println_with_timer("CPU Frequency: " + String(esp.cpu_freq()) + " MHz", "info");
      timer.println_with_timer("Chip Cores: " + String(esp.chip_cores()), "info");
      timer.println_with_timer("Total Program Memory: " + String(esp.total_program_memory()) + " KB", "info");
      timer.println_with_timer("Total RAM: " + String(esp.total_ram()) + " KB", "info");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("");
    }

    // Инициализация оборудования
    void initialize_hardware() {
      timer.println_with_timer("Initializing Hardware", "log");
      log_esp_info();
      timer.println_with_timer("Hardware initialization successful", "success");
      timer.println_with_timer("");
    }

    // Инициализация системы
    void initialize_system() {
      timer.println_with_timer("      System Information", "log");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer(String("  OS Name: ") + os_name, "info");
      timer.println_with_timer(String("  OS Version: ") + os_version, "info");
      timer.println_with_timer(String("  Build Date: ") + os_date, "info");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("");
    }

    // Инициализация ядра
    void initialize_core() {
      timer.println_with_timer("      Core Information", "log");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer(String("  Core Name: ") + core_name, "info");
      timer.println_with_timer(String("  Core Version: ") + core_version, "info");
      timer.println_with_timer(String("  Build Date: ") + core_date, "info");
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("");
    }

    // Загрузка модулей
    void load_modules() {
      timer.println_with_timer("Loading Modules", "log");

      timer.println_with_timer("");
      timer.println_with_timer("Loading SD Card module...", "log");
      sd.mount_sd(&timer, true);
      timer.println_with_timer("");
      timer.println_with_timer("     SD Card Information", "log");
      timer.println_with_timer("===========================", "log");
      bool sd_init = sd.init(timer);

      bool system_files_ok = checker.sys_checker(sd, &timer);

      if (sd_init && system_files_ok) {
        timer.println_with_timer("SD Card module loaded successfully.", "success");
      } else {
        timer.println_with_timer("Failed to load SD Card module.", "error");
      }

      timer.println_with_timer("Checking SD Card Free space...", "log");
      if (sd.get_card_free() < 256) {
        timer.println_with_timer("CRITICAL: Not enough free space on SD card. At least 256 KB required.", "error");
        sd_init = false;
      } else {
        timer.println_with_timer("SD Card Free space checked", "success");
      }

      timer.println_with_timer("");
      timer.println_with_timer("Loading SPIFFS module...", "log");
      timer.print_time();
      bool spiffs_init = spiffs.begin();
      if (spiffs_init) {
        timer.println_with_timer("");
        timer.println_with_timer("     SPIFFS Information", "log");
        timer.println_with_timer("===========================", "log");
        spiffs.print_info(&timer);
        timer.println_with_timer("===========================", "log");
        timer.println_with_timer("");
        timer.println_with_timer("SPIFFS module loaded successfully.", "success");
      } else {
        timer.println_with_timer("Failed to load SPIFFS module.", "error");
      }

      timer.println_with_timer("");
      timer.println_with_timer("Loading EEPROM module...", "log");
      timer.print_time();
      bool eeprom_init = eeprom.init();

      if (eeprom_init) {
        Serial.println();
        timer.println_with_timer("     EEPROM Information", "log");
        timer.println_with_timer("===========================", "log");
        eeprom.print_info(&timer);
        timer.println_with_timer("===========================", "log");
        timer.println_with_timer("");
        timer.println_with_timer("EEPROM module loaded successfully.", "success");
      } else {
        timer.println_with_timer("Failed to load EEPROM module.", "error");
      }


      timer.println_with_timer("");

      if (sd_init && system_files_ok && spiffs_init && eeprom_init) {
        boot_success = true;
      }
    }

    // Режим восстановления
    void enter_recovery_mode() {
      timer.println_with_timer("   Entering Recovery Mode", "warning");
      timer.println_with_timer("");
      RecoveryMode rm(esp, timer, spiffs, sd, eeprom);
      rm.recovery();
    }

    // Логирование причин сброса процессоров
    void print_cpu_reason() {
      timer.println_with_timer("     CPU Reset Reasons", "log");
      timer.println_with_timer("===========================", "log");
      log_reset_reason(0); // CPU 0
      log_reset_reason(1); // CPU 1
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("");
    }

    // Логирование причины сброса для указанного процессора
    void log_reset_reason(uint8_t cpu) {
      timer.print_with_timer(String("  CPU") + String(cpu) + ": ", "info");
      esp.print_reset_reason(rtc_get_reset_reason(cpu));
      Serial.print(", ");
      esp.verbose_print_reset_reason(rtc_get_reset_reason(cpu));
      Serial.println("");
    }

    void log_build_info() {
      timer.println_with_timer("");
      timer.println_with_timer("      Build Information", "log");
      timer.println_with_timer("===========================", "log");
      
      // Выводим стандартную информацию о дате и времени сборки
      timer.println_with_timer("Build Date: " + String(__DATE__), "info");
      timer.println_with_timer("Build Time: " + String(__TIME__), "info");
      
      timer.println_with_timer("===========================", "log");
      timer.println_with_timer("");
    }

  public:
    // Конструктор
    Bootloader(SdCard& sd_instance, Wifi& wifi_instance, Esp& esp_instance, Timer& timer_instance, Spiffs& spiffs_instance, Eeprom& eeprom_instance)
      : sd(sd_instance), wifi(wifi_instance), esp(esp_instance), timer(timer_instance), spiffs(spiffs_instance), eeprom(eeprom_instance) {
      timer.println_with_timer("");
      timer.println_with_timer("Initializing Bootloader", "log");
      initialize_bootloader();
      print_cpu_reason();
      timer.println_with_timer("Initialized Bootloader Successful", "success");
    }

    // Основной метод загрузки
    void boot() {
      timer.println_with_timer("Starting Boot Process", "log");
      log_build_info();
      initialize_hardware();
      initialize_core();
      initialize_system();

      timer.println_with_timer("Starting Module Loading", "log");
      load_modules();

      if (boot_success) {
        timer.println_with_timer("===========================");
        timer.println_with_timer("  System Initialization OK", "success");
        Serial.println();
        Serial.print(current_directory + " $ ");
      } else {
        timer.println_with_timer("===========================");
        timer.println_with_timer("  System Initialization Failed", "error");
        enter_recovery_mode();
      }
    }
};