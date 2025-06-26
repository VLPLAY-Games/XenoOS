// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Checker {
  private:
    ColorPrinter color;
  public:
    bool check_sys_files_exists(SdCard& sd, bool reverse_return = false, Timer* timer = nullptr) {
        bool all_files_ok = true;
        for (size_t i = 0; i < sys_files_count; i++) {
            if (timer) timer->print_time();
            Serial.printf("Checking %s file on SD Card...   ", sys_main_files[i]);
            bool result = sd.is_path_exists(sys_main_files[i]);
            if (reverse_return) result = !result;
            color.print_result(result, true);
            all_files_ok &= result;
        }
        if (reverse_return) return !all_files_ok;
        else return all_files_ok;
    }

    bool check_sys_dirs_exists(SdCard& sd, bool reverse_return = false, Timer* timer = nullptr) {
        bool all_dirs_ok = true;
        for (size_t i = 0; i < sys_dir_count; i++) {
            if (timer) timer->print_time();
            Serial.printf("Checking %s directory on SD Card...   ", sys_main_dirs[i]);
            bool result = sd.is_path_exists(sys_main_dirs[i]);
            if (reverse_return) result = !result;
            color.print_result(result, true);
            all_dirs_ok &= result;
        }
        if (reverse_return) return !all_dirs_ok;
        else return all_dirs_ok;
    }

    std::vector<String> check_sys_files_noexists(SdCard& sd, bool reverse_return = false) {
        std::vector<String> noexist_files;
        for (size_t i = 0; i < sys_files_count; i++) {
            Serial.printf("Checking %s file on SD Card...   ", sys_main_files[i]);
            bool result = sd.is_path_exists(sys_main_files[i]);
            if (reverse_return) result = !result;
            if (!result) noexist_files.push_back(sys_main_files[i]);
            color.print_result(result, true);
        }
        if (noexist_files.size() > 0) return noexist_files;
        else return {};
    }

    std::vector<String> check_sys_dirs_noexists(SdCard& sd, bool reverse_return = false) {
        std::vector<String> noexist_dirs;
        for (size_t i = 0; i < sys_dir_count; i++) {
            Serial.printf("Checking %s directory on SD Card...   ", sys_main_dirs[i]);
            bool result = sd.is_path_exists(sys_main_dirs[i]);
            if (reverse_return) result = !result;
            if (!result) noexist_dirs.push_back(sys_main_dirs[i]);
            color.print_result(result, true);
        }
        if (noexist_dirs.size() > 0) return noexist_dirs;
        else return {};
    }

    bool sys_checker(SdCard& sd, Timer* timer = nullptr) {
        if (timer) timer->print_time();
        Serial.println("=== System Checker module ===");
        if (timer) timer->print_time();
        Serial.println();

        // Проверка SD Card
        if (timer) timer->print_time();
        Serial.print("Checking SD Card...   ");
        bool sd_ok = sd.mount_sd();
        color.print_result(sd_ok, true);

        if (!sd_ok) {
            if (timer) timer->print_time();
            color.print_error("Can't mount SD card. Abort.", true);
            delay(1000);
            return false;
        }

        bool dir_check = false;
        bool file_check = false;
        if (timer) {
            dir_check = check_sys_dirs_exists(sd, false, timer);
            file_check = check_sys_files_exists(sd, false, timer);
        } else {
            dir_check = check_sys_dirs_exists(sd);
            file_check = check_sys_files_exists(sd);
        }
        
        // Итоговый результат
        if (dir_check && file_check) {
            if (timer) timer->print_time();
            Serial.print("System Checker module finished   ");
            color.print_success("OK", true);
            return true;
        } else {
            if (timer) timer->print_time();
            Serial.print("System Checker module finished   ");
            color.print_error("ERROR", true);
            if (timer) timer->print_time();
            color.print_error("Some files or directories not found. Please run 'system installer'", true);
            delay(1000);
            return false;
        }
    }
};
