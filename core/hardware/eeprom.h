// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <EEPROM.h>

extern bool eeprom_init = false;

class Eeprom {
  private:
    const uint8_t eeprom_size = 18;  // Размер EEPROM памяти в байтах (18 байт)
    const uint8_t diagnostic_index = 17;  // Индекс для хранения диагностической информации
    const uint8_t diagnostic_value = 200;
    
    ColorPrinter color;

  public:
    bool init() {
        eeprom_init = EEPROM.begin(eeprom_size);
        return eeprom_init;
    }

    uint8_t get_eeprom_size() {
        return eeprom_size;
    }

    // Чтение данных из EEPROM по индексу (для первых 16 байт)
    void read_data(int index) {
        if (index < 0 || index >= 16) {  // Проверка на допустимый индекс (индексы 0-15)
            Serial.println("Access denied: You cannot read from the first 16 bytes of EEPROM.");
            return;
        }
        
        byte data = EEPROM.read(index);
        Serial.printf("EEPROM[%d]: %d\r\n", index, data);
    }

    // Запись данных в EEPROM по индексу (для первых 16 байт)
    void write_data(int index, byte data) {
        if (index < 0 || index >= 16) {  // Проверка на допустимый индекс (индексы 0-15)
            Serial.println("Access denied: You cannot write to the first 16 bytes of EEPROM.");
            return;
        }

        EEPROM.write(index, data);
        EEPROM.commit();
        Serial.printf("Written %d to EEPROM[%d]\r\n", data, index);
    }

    // Функция для чтения диагностической информации
    uint8_t read_diagnostic_info() {
      uint8_t diagnostic_data = EEPROM.read(diagnostic_index);
      Serial.printf("Diagnostic Info (EEPROM[%d]): %d\r\n", diagnostic_index, diagnostic_data);
      return diagnostic_data;
    }

    // Функция для записи диагностической информации
    void write_diagnostic_info(byte data) {
      EEPROM.write(diagnostic_index, data);
      EEPROM.commit();
      Serial.printf("Written %d to EEPROM diagnostic index\r\n", data);
    }

    void print_info(Timer* timer = nullptr){
      if (timer) {
        timer->println_with_timer(String("  EEPROM Total: ") + eeprom_size + String(" B"));
      } else {
        Serial.printf("  EEPROM Total: %u B\r\n", eeprom_size);
      }
    }

    // Функция диагностики
    bool diagnostics() {
      color.print_log("=== Starting EEPROM Diagnostics ===", true);
  
      bool init_status = false;
      bool write_test_status = false;
      bool clear_test_status = false;
      bool skip_tests = true;  // Флаг для пропуска тестов, если EEPROM не инициализирована
  
      // Проверка инициализации EEPROM
      color.print_info("Checking EEPROM initialization... ");
      if (eeprom_init) {
          color.print_success("OK", true);
          init_status = true;
          skip_tests = false;  // Позволяем выполнять остальные тесты
      } else {
          color.print_error("FAILED", true);
          color.print_error("ERROR: EEPROM is not initialized. Skipping diagnostics.", true);
      }
  
      // Печать информации о размере EEPROM (если инициализировано)
      if (!skip_tests) {
          print_info();
      }
  
      // Тест диагностического индекса EEPROM[17] (если инициализировано)
      if (!skip_tests) {
          color.print_info("Testing Diagnostic Index (EEPROM[17])...", true);
  
          // Запись в диагностический байт (индекс 17)
          color.print_info("Writing diagnostic value... ");
          write_diagnostic_info(diagnostic_value);
          delay(100);
  
          // Проверяем, правильно ли записалось
          color.print_info("Verifying written value... ");
          if (read_diagnostic_info() == diagnostic_value) {
              color.print_success("PASSED", true);
              write_test_status = true;
          } else {
              color.print_error("FAILED", true);
              color.print_error("ERROR: Diagnostic Index write test failed.", true);
          }
  
          // Очистка диагностического байта (запись 0x00)
          color.print_info("Clearing Diagnostic Index... ");
          write_diagnostic_info(0x00);
          delay(100);
  
          // Проверяем, успешно ли очищено
          color.print_info("Verifying clear operation... ");
          if (read_diagnostic_info() == 0x00) {
              color.print_success("PASSED", true);
              clear_test_status = true;
          } else {
              color.print_error("FAILED", true);
              color.print_error("ERROR: Failed to clear Diagnostic Index.", true);
          }
      }
  
      // Вывод итоговых результатов тестов
      color.print_log("\n=== EEPROM Diagnostics Summary ===", true);
      color.print_info("Initialization: ");
      init_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true);
  
      color.print_info("Write Test: ");
      skip_tests ? color.print_warning("SKIPPED", true) : (write_test_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true));
  
      color.print_info("Clear Test: ");
      skip_tests ? color.print_warning("SKIPPED", true) : (clear_test_status ? color.print_success("PASSED", true) : color.print_error("FAILED", true));
  
      color.print_log("=== EEPROM Diagnostics Complete ===\n", true);

      if (init_status && write_test_status && clear_test_status) {
        return true;
      } else {
        return false;
      }
    }
};
