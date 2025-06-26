// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Freset {
  private:
    ColorPrinter color;
    Checker checker;
  public:
    bool factory_reset(SdCard& sd) {
        Serial.println("=== Factory reset module ===");
        Serial.println();

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
        for (size_t i = 0; i < sys_dir_count; i++) {
            Serial.printf("Removing %s directory of SD Card...   ", sys_main_dirs[i]);
            bool result = false;
            if (sd.is_path_exists(sys_main_dirs[i])) result = sd.removeDir(SD, sys_main_dirs[i]);
            else result = true;
            color.print_result(result, true);
            all_operations_ok &= result;
        }
        if (checker.check_sys_dirs_noexists(sd, true).size() != 0 || checker.check_sys_files_noexists(sd, true).size() != 0) all_operations_ok = false;

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