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
        Checker checker;
        Installer installer;

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

            // Диагностика системных файлов
            Serial.println("Checking System Integrity...");
            bool sys_integrity_diagnostic = checker.sys_checker(sd);
            bool installer_success = sys_integrity_diagnostic;
            if (!sys_integrity_diagnostic) {
                color.print_error("System Integrity needs to be recovered", true);
                installer_success = installer.install_sys_files(sd);
            }
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

            // Проверка системных файлов
            color.print_info("System Integrity: ");
            if (sys_integrity_diagnostic) color.print_success("PASSED", true);
            else if (installer_success) color.print_warning("RECOVERED", true);
            else color.print_error("FAILED", true);

            // Проверка EEPROM
            color.print_info("EEPROM: ");
            eeprom_diagnostic ? color.print_success("PASSED", true) : color.print_error("FAILED", true);
            
            Serial.println();
            // Итоговый результат диагностики
            if (esp_diagnostic && spiffs_diagnostic && sd_diagnostic && eeprom_diagnostic && (sys_integrity_diagnostic || installer_success)) {
                color.print_success("Diagnostic: PASSED", true);
            } else {
                color.print_error("Diagnostic: FAILED", true);
            }

            Serial.println();
            Serial.println("=== Diagnostics Complete ===");
        }
};