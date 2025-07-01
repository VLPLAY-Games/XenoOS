// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.

#pragma once

#include "esp_app_format.h"
#include "esp_image_format.h"
#include "esp_partition.h"
#include "FS.h"

static const char* MODULES_DIR = "/modules";

class Installer {
  private:
    ColorPrinter color;
    Checker checker;
    SdCard* sd;

  public:
    Installer(SdCard* sdcard) : sd(sdcard) {
        if (!sd->is_path_exists(MODULES_DIR)) {
            sd->createDir(SD, MODULES_DIR);
        }
    }

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

    // Копирование модуля с SD в /modules
    bool install_from_sd(const char* sdPath) {
        if (!sd->is_path_exists(sdPath)) {
            color.print_error("SD file not found", true);
            return false;
        }

        String filename = String(sdPath).substring(String(sdPath).lastIndexOf('/') + 1);
        String dest = String(MODULES_DIR) + "/" + filename;

        if (sd->is_path_exists(dest.c_str()))
            sd->deleteFile(SD, dest.c_str());

        bool ok = sd->copy_file(sdPath, dest.c_str());
        color.print_result(ok, true);
        return ok;
    }

    // Удаление .xmod/.bin модуля из /modules
    bool remove_module(const char* name) {
        String path_x = String(MODULES_DIR) + "/" + name + ".xmod";
        String path_b = String(MODULES_DIR) + "/" + name + ".bin";

        if (sd->is_path_exists(path_x.c_str())) sd->deleteFile(SD, path_x.c_str());
        if (sd->is_path_exists(path_b.c_str())) sd->deleteFile(SD, path_b.c_str());

        color.print_success("Module deleted (if existed)", true);
        return true;
    }

    // Установка модуля с SD в раздел Flash "modules"
    bool install_module_to_flash(const char* sdPath) {
        File f = SD.open(sdPath);
        if (!f || !f.available()) {
            color.print_error("Failed to open module file.", true);
            return false;
        }

        size_t sz = f.size();
        if (sz == 0) return false;

        const esp_partition_t* mod_part = esp_partition_find_first(
            ESP_PARTITION_TYPE_DATA, (esp_partition_subtype_t)0x40, "modules");

        if (!mod_part) {
            color.print_error("Module partition not found!", true);
            return false;
        }

        if (sz > mod_part->size) {
            color.print_error("Module too big for partition!", true);
            return false;
        }

        esp_err_t err = esp_partition_erase_range(mod_part, 0, mod_part->size);
        if (err != ESP_OK) {
            color.print_error("Erase failed!", true);
            return false;
        }

        const size_t BUF_SZ = 4096;
        uint8_t buf[BUF_SZ];
        size_t written = 0;

        while (written < sz) {
            size_t chunk = f.read(buf, BUF_SZ);
            if (chunk <= 0) break;

            err = esp_partition_write(mod_part, written, buf, chunk);
            if (err != ESP_OK) {
                color.print_error("Write failed!", true);
                return false;
            }

            written += chunk;
        }

        f.close();
        color.print_success("Module installed to flash.", true);
        return true;
    }

    // Запуск модуля из раздела Flash
    bool run_module_from_flash(int argc = 0, char** argv = nullptr) {
        const esp_partition_t* mod_part = esp_partition_find_first(
            ESP_PARTITION_TYPE_DATA, (esp_partition_subtype_t)0x40, "modules");

        if (!mod_part) {
            color.print_error("Module partition not found!", true);
            return false;
        }

        esp_image_metadata_t data;
        memset(&data, 0, sizeof(data));

        esp_partition_pos_t part_pos;
        part_pos.offset = mod_part->address;
        part_pos.size = mod_part->size;

        esp_err_t err = esp_image_verify(ESP_IMAGE_VERIFY, &part_pos, &data);
        if (err != ESP_OK) {
            color.print_error("Image verify failed!", true);
            return false;
        }

        typedef void (*module_main_t)(int, char**);

        // Используй поле start_addr вместо entry_addr (проверь в своей версии)
        module_main_t entry = (module_main_t)data.start_addr;

        if (entry == nullptr) {
            color.print_error("Invalid entry point!", true);
            return false;
        }

        color.print_success("Launching module from flash...", true);
        entry(argc, argv);
        return true;
    }
};