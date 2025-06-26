// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Installer {
  private:
    ColorPrinter color;
    Checker checker;
  public:
    bool install_sys_files(SdCard& sd) {
        Serial.println("=== System Installer module ===");
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

        // Создание директорий
        bool all_operations_ok_dr = true;
        for (size_t i = 0; i < sys_dir_count; i++) {
            bool result = true;
            Serial.printf("Creating %s directory on SD Card...   ", sys_main_dirs[i]);
            if (sd.is_path_exists(sys_main_dirs[i])) result = true;
            else result = sd.createDir(SD, sys_main_dirs[i]);
            color.print_result(result, true);
            all_operations_ok_dr &= result;
        }

        // Создание файлов
        bool all_operations_ok_files = true;
        for (size_t i = 0; i < sys_files_count; i++) {
            bool result = true;
            Serial.printf("Creating %s file on SD Card...   ", sys_main_files[i]);
            if (sd.is_path_exists(sys_main_files[i])) result = true;
            else result = sd.create_empty_file(sys_main_files[i]);
            color.print_result(result, true);
            all_operations_ok_files &= result;
        }
        bool all_operations_ok = false;
        if (all_operations_ok_dr && all_operations_ok_files) all_operations_ok = true;
        if (checker.check_sys_dirs_noexists(sd).size() != 0 || checker.check_sys_files_noexists(sd).size() != 0) all_operations_ok = false;

        // Итоговый результат
        Serial.print("System Installer module finished   ");
        if (all_operations_ok) {
            color.print_success("OK", true);
            return true;
        } else {
            color.print_error("ERROR", true);
            color.print_error("Some operations failed. System may be in inconsistent state.", true);
            delay(1000);
            return false;
        }
    }
};