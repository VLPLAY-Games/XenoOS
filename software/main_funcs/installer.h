// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Installer {
  private:
    ColorPrinter color;
  public:
    bool install_sys_files(SdCard& sd, Esp esp) {
        Serial.println("=== System Installer module ===");
        Serial.println();

        const char* dirs_to_create[] = {
            "/cfg",
            "/downloads",
            "/sys",
            "/upd"
        };
        const char* files_to_create[] = {
            "/cfg/date.cfg",
            "/cfg/sys.cfg",
            "/sys/history.log",
        };
        const size_t dir_count = sizeof(dirs_to_create) / sizeof(dirs_to_create[0]);
        const size_t files_count = sizeof(files_to_create) / sizeof(files_to_create[0]);

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
        for (size_t i = 0; i < dir_count; i++) {
            Serial.printf("Creating %s directory on SD Card...   ", dirs_to_create[i]);
            bool result = sd.createDir(SD, dirs_to_create[i]);
            color.print_result(result, true);
            all_operations_ok_dr &= result;
        }

        // Создание файлов
        bool all_operations_ok_files = true;
        for (size_t i = 0; i < files_count; i++) {
            Serial.printf("Creating %s file on SD Card...   ", files_to_create[i]);
            bool result = sd.create_empty_file(files_to_create[i]);
            color.print_result(result, true);
            all_operations_ok_files &= result;
        }

        // Итоговый результат
        Serial.print("System Installer module finished   ");
        if (all_operations_ok_dr && all_operations_ok_files) {
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