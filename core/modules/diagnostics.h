// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Diagnostics {
    private:
        Esp esp;
        Spiffs spiffs;
        SdCard sd;
        Eeprom eeprom;

    public:

        // Конструктор
        Diagnostics(Esp& esp_instance, Spiffs& spiffs_instance, SdCard& sd_instance, Eeprom& eeprom_instance) 
        : esp(esp_instance), spiffs(spiffs_instance), sd(sd_instance), eeprom(eeprom_instance) {}


        // Функция диагностики
        void diagnostics() {
            Serial.println("=== System Diagnostics ===");
            Serial.println();

            // Диагностика ESP
            Serial.println("Checking ESP...");
            esp.diagnostics();
            Serial.println();

            // Диагностика SPIFFS
            Serial.println("Checking SPIFFS...");
            spiffs.diagnostics();
            Serial.println();

            // Диагностика SD-карты
            Serial.println("Checking SD Card...");
            sd.diagnostics();
            Serial.println();

            // Диагностика EEPROM
            Serial.println("Checking EEPROM...");
            eeprom.diagnostics();
            Serial.println();

            Serial.println("=== Diagnostics Complete ===");
        }
};