// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Freset {
  private:
    ColorPrinter color;
  public:
    bool factory_reset(SdCard& sd, Esp esp) {
        Serial.println("=== Factory reset module ===");
        Serial.println();

        const char* dirs_to_remove[] = {
            "/cfg",
            "/downloads",
            "/sys",
            "/upd"
        };
        const size_t dir_count = sizeof(dirs_to_remove) / sizeof(dirs_to_remove[0]);

        // Проверка SD Card
        Serial.print("Checking SD Card...   ");
        bool sd_ok = sd.mount_sd();
        color.print_result(sd_ok, true);
        
        if (!sd_ok) {
            color.print_error("Can't mount SD card. Abort.", true);
            delay(1000);
            return false;
        }

        // Удаление директорий
        bool all_operations_ok = true;
        for (size_t i = 0; i < dir_count; i++) {
            Serial.printf("Removing %s directory of SD Card...   ", dirs_to_remove[i]);
            bool result = sd.removeDir(SD, dirs_to_remove[i]);
            color.print_result(result, true);
            all_operations_ok &= result;
        }

        // Итоговый результат
        Serial.print("Factory reset module finished   ");
        if (all_operations_ok) {
            color.print_success("OK", true);
            return true;
        } else {
            color.print_error("Error", true);
            color.print_error("Some operations failed. System may be in inconsistent state.", true);
            delay(1000);
            return false;
        }
    }
};