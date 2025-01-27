class RecoveryMode{
  private:
    Esp esp;
    Timer timer;
    Spiffs spiffs;
    SdCard sd;

    // Инициализация загрузчика
    void initialize_recovery() {
      timer.println_with_timer("Initializing recovery");
      timer.println_with_timer("Recovery Information:");
      timer.println_with_timer(String("  Name: ") + rec_name);
      timer.println_with_timer(String("  Version: ") + rec_version);
      timer.println_with_timer(String("  Build Date: ") + rec_date);
      timer.println_with_timer("Recovery initialization completed.");
    }

  public:
    // Конструктор
    RecoveryMode(Esp& esp_instance, Timer& timer_instance, Spiffs& spiffs_instance, SdCard& sd_instance) 
      : esp(esp_instance), timer(timer_instance), spiffs(spiffs_instance), sd(sd_instance) {
        initialize_recovery();
    }

    

    void recovery() {
      Diagnostics diagnostic(esp, spiffs, sd);
      timer.println_with_timer("Performing system diagnostics...");
      diagnostic.diagnostics();

      timer.println_with_timer("Restarting system in 10 seconds...");
      delay(10000);
      esp.restart();
    }
};