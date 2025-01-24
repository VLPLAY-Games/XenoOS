class Bootloader {
  private:
    bool boot_success = false;   // Флаг успешной загрузки
    SdCard& sd;                  // Ссылка на экземпляр SdCard
    Wifi& wifi;                  // Ссылка на экземпляр Wifi
    Esp& esp;                    // Ссылка на экземпляр Esp
    Timer& timer;                // Ссылка на экземпляр Timer

    // Инициализация загрузчика
    void initialize_bootloader() {
      timer.log_with_timer("Bootloader Information:");
      timer.log_with_timer(String("  Name: ") + bl_name);
      timer.log_with_timer(String("  Version: ") + bl_version);
      timer.log_with_timer(String("  Build Date: ") + bl_date);
      timer.log_with_timer("Bootloader initialization completed.");
    }

    void log_esp_info() {
      timer.log_with_timer("=== ESP32 Information ===");
      timer.log_with_timer("Chip Model: " + String(esp.chip_model()));
      timer.log_with_timer("Chip Revision: " + String(esp.chip_revision()));
      timer.log_with_timer("Chip ID: " + String(esp.chip_id()));
      timer.log_with_timer("CPU Frequency: " + String(esp.cpu_freq()) + " MHz");
      timer.log_with_timer("Chip Cores: " + String(esp.chip_cores()));
      timer.log_with_timer("Total Program Memory: " + String(esp.total_program_memory()) + " KB");
      timer.log_with_timer("Total RAM: " + String(esp.total_ram()) + " KB");
      timer.log_with_timer("===========================");
    }

    // Инициализация оборудования
    void initialize_hardware() {
      timer.log_with_timer("Initializing Hardware...");
      timer.log_with_timer("Chip Information:");
      log_esp_info(); // Передаём timer для логирования
      timer.log_with_timer("Hardware initialization successful.");
    }

    // Инициализация системы
    void initialize_system() {
      timer.log_with_timer("System Information:");
      timer.log_with_timer(String("  OS Name: ") + os_name);
      timer.log_with_timer(String("  OS Version: ") + os_version);
      timer.log_with_timer(String("  Build Date: ") + os_date);
    }

    // Инициализация ядра
    void initialize_core() {
      timer.log_with_timer("Core Information:");
      timer.log_with_timer(String("  Core Name: ") + core_name);
      timer.log_with_timer(String("  Core Version: ") + core_version);
      timer.log_with_timer(String("  Build Date: ") + core_date);
    }

    // Загрузка модулей
    void load_modules() {
      timer.log_with_timer("Loading Modules...");
      timer.log_with_timer("Loading SD Card module...");
      sd.init(timer);

      if (card_init){
        boot_success = true;
      }
    }

    // Режим восстановления
    void enter_recovery_mode() {
      timer.log_with_timer("A fatal error occurred during startup.");
      timer.log_with_timer("Entering Recovery Mode...");
      RecoveryMode rm(esp, timer); // Создаём объект RecoveryMode
      rm.recovery();
    }

    // Логирование причин сброса процессоров
    void print_cpu_reason() {
      timer.log_with_timer("CPU Reset Reasons:");
      log_reset_reason(0); // CPU 0
      log_reset_reason(1); // CPU 1
    }

    // Логирование причины сброса для указанного процессора
    void log_reset_reason(uint8_t cpu) {
      timer.log_with_timer(String("CPU") + String(cpu) + ":");
      timer.print_time();
      esp.print_reset_reason(rtc_get_reset_reason(cpu));
      timer.print_time();
      esp.verbose_print_reset_reason(rtc_get_reset_reason(cpu));
    }

  public:
    // Конструктор
    Bootloader(SdCard& sd_instance, Wifi& wifi_instance, Esp& esp_instance, Timer& timer_instance)
      : sd(sd_instance), wifi(wifi_instance), esp(esp_instance), timer(timer_instance) {
      timer.log_with_timer("Initializing Bootloader...");
      initialize_bootloader(); // Вывод информации о загрузчике
      print_cpu_reason();      // Вывод причин сброса
      timer.log_with_timer("Bootloader initialization successful.");
    }

    // Основной метод загрузки
    void boot() {
      timer.log_with_timer("Starting Hardware Initialization...");
      initialize_hardware();

      timer.log_with_timer("Starting Core Initialization...");
      initialize_core();

      timer.log_with_timer("Starting System Initialization...");
      initialize_system();

      timer.log_with_timer("Starting Module Loading...");
      load_modules();
      if (boot_success) {
        timer.log_with_timer("System initialization successful.");
        Serial.println();
        Serial.print(current_directory + " $ ");
      } else {
        enter_recovery_mode();
      }
      system_started = true;
    }
};