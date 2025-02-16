// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Diagnostics {
    private:
        Esp esp;
        Spiffs spiffs;
        SdCard sd;
        Eeprom eeprom;
        ColorPrinter color;

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
            bool esp_diagnostic = esp.diagnostics();
            Serial.println();

            // Диагностика SPIFFS
            Serial.println("Checking SPIFFS...");
            bool spiffs_diagnostic = spiffs.diagnostics();
            Serial.println();

            // Диагностика SD-карты
            Serial.println("Checking SD Card...");
            bool sd_diagnostic = sd.diagnostics();
            Serial.println();

            // Диагностика EEPROM
            Serial.println("Checking EEPROM...");
            bool eeprom_diagnostic = eeprom.diagnostics();
            Serial.println();
            Serial.println();
            // Итоговый результат
            color.print_log("=== Diagnostics Total ===", true);

            // Проверка ESP32
            color.print_info("ESP32: ");
            esp_diagnostic ? color.print_success("PASSED", true) : color.print_error("FAILED", true);

            // Проверка SPIFFS
            color.print_info("SPIFFS: ");
            spiffs_diagnostic ? color.print_success("PASSED", true) : color.print_error("FAILED", true);

            // Проверка SD Card
            color.print_info("SD Card: ");
            sd_diagnostic ? color.print_success("PASSED", true) : color.print_error("FAILED", true);

            // Проверка EEPROM
            color.print_info("EEPROM: ");
            eeprom_diagnostic ? color.print_success("PASSED", true) : color.print_error("FAILED", true);
            
            Serial.println();
            // Итоговый результат диагностики
            if (esp_diagnostic && spiffs_diagnostic && sd_diagnostic && eeprom_diagnostic) {
                color.print_success("Diagnostic: PASSED", true);  // Все прошли успешно
            } else {
                color.print_error("Diagnostic: FAILED", true);  // Есть ошибки
            }

            Serial.println();
            Serial.println("=== Diagnostics Complete ===");
        }
};