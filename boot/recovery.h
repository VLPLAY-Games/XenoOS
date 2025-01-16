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