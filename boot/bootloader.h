class Bootloader{
  private:
    bool boot_success = false;
    SdCard sd;
    Wifi wifi;
    Esp esp;
    Timer timer;
  public:
    // Constructor
    Bootloader(SdCard& sd_instance, Wifi& wifi_instance, Esp& esp_instance, Timer& timer_instance) : sd(sd_instance), wifi(wifi_instance), esp(esp_instance), timer(timer_instance) {
      timer.print_time();
      Serial.println("Initializing bootloader");
      timer.print_time();
      Serial.println("");
      timer.print_time();
      Serial.println("CPU0 reset reason:");
      timer.print_time();
      esp.print_reset_reason(rtc_get_reset_reason(0));
      timer.print_time();
      esp.verbose_print_reset_reason(rtc_get_reset_reason(0));
      timer.print_time();
      Serial.println("CPU1 reset reason:");
      timer.print_time();
      esp.print_reset_reason(rtc_get_reset_reason(1));
      timer.print_time();
      esp.verbose_print_reset_reason(rtc_get_reset_reason(1));
      timer.print_time();
      Serial.println("");
      timer.print_time();
      Serial.println("Initialize bootloader successful");
    }

    void boot(){
      timer.print_time();
      Serial.println("Initializing hardware");

      timer.print_time();
      Serial.println("");

      timer.print_time();
      Serial.print("Chip model ");
      Serial.println(esp.chip_model());

      timer.print_time();
      Serial.print("Chip revision ");
      Serial.println(esp.chip_revision());

      timer.print_time();
      Serial.print("Chip ID ");
      Serial.println(esp.chip_id());

      timer.print_time();
      Serial.print("CPU Frequency ");
      Serial.print(esp.cpu_freq());
      Serial.println(" MHz");

      timer.print_time();
      Serial.print("CPU Cores ");
      Serial.println(esp.chip_cores());

      timer.print_time();
      Serial.print("Total chip memory ");
      Serial.print(esp.total_program_memory());
      Serial.println(" KB");

      timer.print_time();
      Serial.print("Total RAM ");
      Serial.print(esp.total_ram());
      Serial.println(" KB");
      
      timer.print_time();
      Serial.println("");

      timer.print_time();
      Serial.println("Initialize hardware successful");
      timer.print_time();
      Serial.println("Initializing system");
      timer.print_time();
      Serial.println("");
      timer.print_time();
      Serial.print(os_name);
      Serial.print(" version: ");
      Serial.println(os_version);
      timer.print_time();
      Serial.print("OS Date ");
      Serial.println(os_date);
      timer.print_time();
      Serial.println("");
      timer.print_time();
      Serial.println("Loading modules");
      timer.print_time();
      Serial.println("Loading SD Card");
      timer.print_time();
      Serial.println("");
      sd.init(timer);
      timer.print_time();
      Serial.println("");
      if (card_init){
        boot_success = true;
      }
      if (boot_success){
        timer.print_time();
        Serial.println("Initialization successfull");
        Serial.println();
        Serial.print(current_directory + " $ ");
      }
      else {
        timer.print_time();
        Serial.println("A fatal error occurred while starting");
        timer.print_time();
        Serial.println("Entering to Recovery Mode");
        RecoveryMode rm(esp, timer); // Create RecoveryMode with Esp instance
        rm.recovery();
      }
      system_started = true;
    }
};