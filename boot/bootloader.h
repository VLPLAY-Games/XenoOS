class Bootloader {
  private:
    bool boot_success = false;
    SdCard& sd;    
    Wifi& wifi;
    Esp& esp;
    Timer& timer;
    Spiffs spiffs;

    // Инициализация загрузчика
    void initialize_bootloader() {
      timer.println_with_timer("Bootloader Information:");
      timer.println_with_timer(String("  Name: ") + bl_name);
      timer.println_with_timer(String("  Version: ") + bl_version);
      timer.println_with_timer(String("  Build Date: ") + bl_date);
      timer.println_with_timer("Bootloader initialization completed.");
    }

    void log_esp_info() {
      timer.println_with_timer("=== ESP32 Information ===");
      timer.println_with_timer("Chip Model: " + String(esp.chip_model()));
      timer.println_with_timer("Chip Revision: " + String(esp.chip_revision()));
      timer.println_with_timer("Chip ID: " + String(esp.chip_id()));
      timer.println_with_timer("CPU Frequency: " + String(esp.cpu_freq()) + " MHz");
      timer.println_with_timer("Chip Cores: " + String(esp.chip_cores()));
      timer.println_with_timer("Total Program Memory: " + String(esp.total_program_memory()) + " KB");
      timer.println_with_timer("Total RAM: " + String(esp.total_ram()) + " KB");
      timer.println_with_timer("===========================");
    }

    // Инициализация оборудования
    void initialize_hardware() {
      timer.println_with_timer("Initializing Hardware...");
      timer.println_with_timer("Chip Information:");
      log_esp_info();
      timer.println_with_timer("Hardware initialization successful.");
    }

    // Инициализация системы
    void initialize_system() {
      timer.println_with_timer("System Information:");
      timer.println_with_timer(String("  OS Name: ") + os_name);
      timer.println_with_timer(String("  OS Version: ") + os_version);
      timer.println_with_timer(String("  Build Date: ") + os_date);
    }

    // Инициализация ядра
    void initialize_core() {
      timer.println_with_timer("Core Information:");
      timer.println_with_timer(String("  Core Name: ") + core_name);
      timer.println_with_timer(String("  Core Version: ") + core_version);
      timer.println_with_timer(String("  Build Date: ") + core_date);
    }

    // Загрузка модулей
    void load_modules() {
      timer.println_with_timer("Loading Modules...");
      timer.println_with_timer("Loading SD Card module...");

      if (sd.init(timer)){
        boot_success = true;
      }
    }

    // Режим восстановления
    void enter_recovery_mode() {
      timer.println_with_timer("A fatal error occurred during startup.");
      timer.println_with_timer("Entering Recovery Mode...");
      RecoveryMode rm(esp, timer, spiffs, sd);
      rm.recovery();
    }

    // Логирование причин сброса процессоров
    void print_cpu_reason() {
      timer.println_with_timer("CPU Reset Reasons:");
      log_reset_reason(0); // CPU 0
      log_reset_reason(1); // CPU 1
    }

    // Логирование причины сброса для указанного процессора
    void log_reset_reason(uint8_t cpu) {
      timer.println_with_timer(String("CPU") + String(cpu) + ": ");
      timer.print_time();
      esp.print_reset_reason(rtc_get_reset_reason(cpu));
      timer.print_time();
      esp.verbose_print_reset_reason(rtc_get_reset_reason(cpu));
    }

  public:
    // Конструктор
    Bootloader(SdCard& sd_instance, Wifi& wifi_instance, Esp& esp_instance, Timer& timer_instance, Spiffs& spiffs_instance)
      : sd(sd_instance), wifi(wifi_instance), esp(esp_instance), timer(timer_instance), spiffs(spiffs_instance) {
      timer.println_with_timer("Initializing Bootloader...");
      initialize_bootloader();
      print_cpu_reason();  
      timer.println_with_timer("Bootloader initialization successful.");
    }

    // Основной метод загрузки
    void boot() {
      timer.println_with_timer("Starting Hardware Initialization...");
      initialize_hardware();

      timer.println_with_timer("Starting Core Initialization...");
      initialize_core();

      timer.println_with_timer("Starting System Initialization...");
      initialize_system();

      timer.println_with_timer("Starting Module Loading...");
      load_modules();
      if (boot_success) {
        timer.println_with_timer("System initialization successful.");
        Serial.println();
        Serial.print(current_directory + " $ ");
      } else {
        enter_recovery_mode();
      }
      system_started = true;
    }
};