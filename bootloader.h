class RecoveryMode{
  private:
    Esp esp;
    Timer timer;
  public:
    // Constructor
    RecoveryMode(Esp& esp_instance, Timer& timer_instance) : esp(esp_instance), timer(timer_instance) {
      timer.print_time();
      Serial.println("Initializing recovery");
      timer.print_time();
      Serial.println("Initialize recovery successful");
    }

    void recovery(){
      while (1){
        delay(10000);
        Serial.println("Restarting system");
        delay(500);
        esp.restart();
      }
    }
};

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
      Serial.println("Initialize bootloader successful");
    }

    void boot(){
      timer.print_time();
      Serial.println("Initializing hardware");

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
      Serial.println("Initialize hardware successful");
      timer.print_time();
      Serial.println("Initializing system");
      timer.print_time();
      Serial.print(os_name);
      Serial.print(" version: ");
      Serial.println(os_version);
      timer.print_time();
      Serial.println("Loading modules");
      timer.print_time();
      Serial.println("Loading SD Card");
      sd.init(timer);
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