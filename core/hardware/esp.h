#include <Update.h>
#include <FS.h>
#include "esp_mac.h" 

#if CONFIG_IDF_TARGET_ESP32  // ESP32/PICO-D4
#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C2
#include "esp32c2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C6
#include "esp32c6/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32H2
#include "esp32h2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32P4
#include "esp32p4/rom/rtc.h"
#else
#error Target CONFIG_IDF_TARGET is not supported
#endif

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */

class Esp {
  private:
    uint32_t free_ram_kb;
    uint32_t total_ram_kb;
    uint32_t used_ram_kb;
    uint32_t total_program_memory_kb;
    const char* chip_model_name;
    uint16_t chip_revision_id;
    uint32_t chip_unique_id;
    uint32_t cpu_frequency_mhz;
    uint8_t chip_core_count;

    // Вспомогательная функция для расчета уникального ID чипа
    uint32_t calculate_chip_id() {
      uint32_t chipId = 0;
      for (int i = 0; i < 17; i += 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
      }
      return chipId;
    }

  public:
    // Конструктор для инициализации данных
    Esp() {
      free_ram_kb = ESP.getFreeHeap() / 1024;
      total_ram_kb = ESP.getHeapSize() / 1024;
      used_ram_kb = (ESP.getHeapSize() - ESP.getFreeHeap()) / 1024;
      total_program_memory_kb = ESP.getFlashChipSize() / 1024;
      chip_model_name = ESP.getChipModel();
      chip_revision_id = ESP.getChipRevision();
      chip_unique_id = calculate_chip_id();
      cpu_frequency_mhz = ESP.getCpuFreqMHz();
      chip_core_count = ESP.getChipCores();
    }

    // Методы для получения и обновления значений
    uint32_t free_ram() {
      free_ram_kb = ESP.getFreeHeap() / 1024; // Обновляем переменную
      return free_ram_kb; // Возвращаем значение
    }

    uint32_t total_ram() {
      total_ram_kb = ESP.getHeapSize() / 1024; // Обновляем переменную
      return total_ram_kb; // Возвращаем значение
    }

    uint32_t used_ram() {
      used_ram_kb = (ESP.getHeapSize() - ESP.getFreeHeap()) / 1024; // Обновляем переменную
      return used_ram_kb; // Возвращаем значение
    }

    uint32_t total_program_memory() {
      total_program_memory_kb = ESP.getFlashChipSize() / 1024; // Обновляем переменную
      return total_program_memory_kb; // Возвращаем значение
    }

    const char* chip_model() {
      chip_model_name = ESP.getChipModel(); // Обновляем переменную
      return chip_model_name; // Возвращаем значение
    }

    uint16_t chip_revision() {
      chip_revision_id = ESP.getChipRevision(); // Обновляем переменную
      return chip_revision_id; // Возвращаем значение
    }

    uint32_t chip_id() {
      chip_unique_id = calculate_chip_id(); // Обновляем переменную
      return chip_unique_id; // Возвращаем значение
    }

    uint32_t cpu_freq() {
      cpu_frequency_mhz = ESP.getCpuFreqMHz(); // Обновляем переменную
      return cpu_frequency_mhz; // Возвращаем значение
    }

    uint8_t chip_cores() {
      chip_core_count = ESP.getChipCores(); // Обновляем переменную
      return chip_core_count; // Возвращаем значение
    }

    // Метод для вывода всей информации
    void print_all_info() {
      Serial.println("=== System Information ===");
      Serial.printf("Free RAM: %u KB\n", free_ram());
      Serial.printf("Total RAM: %u KB\n", total_ram());
      Serial.printf("Used RAM: %u KB\n", used_ram());
      Serial.printf("Total Program Memory: %u KB\n", total_program_memory());
      Serial.printf("Chip Model: %s\n", chip_model());
      Serial.printf("Chip Revision: %u\n", chip_revision());
      Serial.printf("Chip ID: %u\n", chip_id());
      Serial.printf("CPU Frequency: %u MHz\n", cpu_freq());
      Serial.printf("Chip Cores: %u\n", chip_cores());
      Serial.println("===========================");
    }

    // Метод для вывода cpu информации
    void print_cpu_info() {
      Serial.println("=== CPU Information ===");
      Serial.printf("CPU Frequency: %u MHz\n", cpu_freq());
      Serial.printf("Chip Cores: %u\n", chip_cores());
      Serial.println("===========================");
    }

    // Метод для вывода cpu информации
    void print_ram_info() {
      Serial.println("=== Memory Information ===");
      Serial.printf("Free RAM: %u KB\n", free_ram());
      Serial.printf("Total RAM: %u KB\n", total_ram());
      Serial.printf("Used RAM: %u KB\n", used_ram());
      Serial.printf("Total Program Memory: %u KB\n", total_program_memory());
      Serial.println("===========================");
    }

    // Метод для перезагрузки системы
    void restart() {
      ESP.restart();
    }

    // perform the actual update from a given stream
    void performUpdate(Stream &updateSource, size_t updateSize) {
      Timer updateTimer;  // Создаем экземпляр таймера для отслеживания времени обновления

      if (Update.begin(updateSize)) {
        size_t written = 0;
        uint8_t buffer[128];  // Буфер для записи данных
        int progress = 0;
        size_t totalWritten = 0;

        // Чтение и запись данных
        while (updateSource.available()) {
          int len = updateSource.readBytes(buffer, sizeof(buffer));
          written = Update.write(buffer, len);
          totalWritten += written;

          // Печать прогресса
          int currentProgress = (totalWritten * 100) / updateSize;  // Вычисляем прогресс в процентах
          if (currentProgress > progress) {
            progress = currentProgress;
            Serial.printf("Writing at 0x%08x... (%d%%)\n", totalWritten, progress);
          }
        }

        // Окончание записи
        float elapsedTime = updateTimer.get_sec();  // Время выполнения обновления в секундах
        float speed = (totalWritten / 1024.0) / elapsedTime;  // Скорость в KB/s

        // Финальный отчет
        Serial.printf("Wrote %d bytes at 0x%08x in %.1f seconds (effective %.1f kbit/s)...\n", totalWritten, 0x10000, elapsedTime, speed * 8); // Выводим информацию о размере и скорости

        if (Update.end()) {
          Serial.println("Update done!");
          if (Update.isFinished()) {
            Serial.println("Updating successful");
            Serial.println("Rebooting...");
            ESP.restart();  // Перезагружаем устройство после успешного обновления
          } else {
            Serial.println("Update not finished. Something went wrong.");
          }
        } else {
          Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        }

      } else {
        Serial.println("Not enough space to begin Update");
      }
    }

    // check given FS for valid update.bin and perform update if available
    void updateFromFS(fs::FS &fs) {
      File updateBin = fs.open("/upd/update.bin");
      if (updateBin) {
        if (updateBin.isDirectory()) {
          Serial.println("Error, /upd/update.bin is not a file");
          updateBin.close();
          return;
        }

        size_t updateSize = updateBin.size();

        if (updateSize > 0) {
          Serial.println("Trying to start update");
          performUpdate(updateBin, updateSize);
        } else {
          Serial.println("Error, file is empty");
        }

        updateBin.close();

        // when finished remove the binary from sd card to indicate end of the process
        fs.remove("/upd/update.bin");
      } else {
        Serial.println("Could not load update.bin from /upd/");
      }
    }

    String getDefaultMacAddress() {
      String mac = "";
      unsigned char mac_base[6] = {0};
      if (esp_efuse_mac_get_default(mac_base) == ESP_OK) {
        char buffer[18];  // 6*2 characters for hex + 5 characters for colons + 1 character for null terminator
        sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
        mac = buffer;
      }
      return mac;
    }


    void print_reset_reason(int reason) {
      switch (reason) {
        case 1:  Serial.println("POWERON_RESET"); break;          /**<1,  Vbat power on reset*/
        case 3:  Serial.println("SW_RESET"); break;               /**<3,  Software reset digital core*/
        case 4:  Serial.println("OWDT_RESET"); break;             /**<4,  Legacy watch dog reset digital core*/
        case 5:  Serial.println("DEEPSLEEP_RESET"); break;        /**<5,  Deep Sleep reset digital core*/
        case 6:  Serial.println("SDIO_RESET"); break;             /**<6,  Reset by SLC module, reset digital core*/
        case 7:  Serial.println("TG0WDT_SYS_RESET"); break;       /**<7,  Timer Group0 Watch dog reset digital core*/
        case 8:  Serial.println("TG1WDT_SYS_RESET"); break;       /**<8,  Timer Group1 Watch dog reset digital core*/
        case 9:  Serial.println("RTCWDT_SYS_RESET"); break;       /**<9,  RTC Watch dog Reset digital core*/
        case 10: Serial.println("INTRUSION_RESET"); break;        /**<10, Instrusion tested to reset CPU*/
        case 11: Serial.println("TGWDT_CPU_RESET"); break;        /**<11, Time Group reset CPU*/
        case 12: Serial.println("SW_CPU_RESET"); break;           /**<12, Software reset CPU*/
        case 13: Serial.println("RTCWDT_CPU_RESET"); break;       /**<13, RTC Watch dog Reset CPU*/
        case 14: Serial.println("EXT_CPU_RESET"); break;          /**<14, for APP CPU, reset by PRO CPU*/
        case 15: Serial.println("RTCWDT_BROWN_OUT_RESET"); break; /**<15, Reset when the vdd voltage is not stable*/
        case 16: Serial.println("RTCWDT_RTC_RESET"); break;       /**<16, RTC Watch dog reset digital core and rtc module*/
        default: Serial.println("NO_MEAN");
      }
    }

    void verbose_print_reset_reason(int reason) {
      switch (reason) {
        case 1:  Serial.println("Vbat power on reset"); break;
        case 3:  Serial.println("Software reset digital core"); break;
        case 4:  Serial.println("Legacy watch dog reset digital core"); break;
        case 5:  Serial.println("Deep Sleep reset digital core"); break;
        case 6:  Serial.println("Reset by SLC module, reset digital core"); break;
        case 7:  Serial.println("Timer Group0 Watch dog reset digital core"); break;
        case 8:  Serial.println("Timer Group1 Watch dog reset digital core"); break;
        case 9:  Serial.println("RTC Watch dog Reset digital core"); break;
        case 10: Serial.println("Instrusion tested to reset CPU"); break;
        case 11: Serial.println("Time Group reset CPU"); break;
        case 12: Serial.println("Software reset CPU"); break;
        case 13: Serial.println("RTC Watch dog Reset CPU"); break;
        case 14: Serial.println("for APP CPU, reset by PRO CPU"); break;
        case 15: Serial.println("Reset when the vdd voltage is not stable"); break;
        case 16: Serial.println("RTC Watch dog reset digital core and rtc module"); break;
        default: Serial.println("NO_MEAN");
      }
    }
};
