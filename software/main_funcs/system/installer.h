// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.

#pragma once

#include "esp_app_format.h"
#include "esp_image_format.h"
#include "esp_partition.h"
#include "FS.h"

class Installer {
  private:
    ColorPrinter color;
    Checker checker;
    SdCard* sd;

  public:
    Installer(SdCard* sdcard) : sd(sdcard) {}

    // Установка системных директорий и файлов
    bool install_sys_files() {
        Serial.println("=== System Installer module ===");
        bool sd_ok = sd->mount_sd();
        color.print_result(sd_ok, true);
        if (!sd_ok) {
            color.print_error("Can't mount SD card. Abort.", true);
            return false;
        }

        bool all_dirs_ok = true;
        for (size_t i = 0; i < sys_dir_count; i++) {
            bool exists = sd->is_path_exists(sys_main_dirs[i]);
            bool result = exists ? true : sd->createDir(SD, sys_main_dirs[i]);
            color.print_result(result, true);
            all_dirs_ok &= result;
        }

        bool all_files_ok = true;
        for (size_t i = 0; i < sys_files_count; i++) {
            bool exists = sd->is_path_exists(sys_main_files[i]);
            bool result = exists ? true : sd->create_empty_file(sys_main_files[i]);
            color.print_result(result, true);
            all_files_ok &= result;
        }

        bool all_ok = all_dirs_ok && all_files_ok &&
                      checker.check_sys_dirs_noexists(*sd).empty() &&
                      checker.check_sys_files_noexists(*sd).empty();

        if (all_ok) {
            color.print_success("System Installer: OK", true);
            return true;
        } else {
            color.print_error("System Installer: ERROR", true);
            return false;
        }
    }
};