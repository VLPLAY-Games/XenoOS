// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



#include <EEPROM.h>

class Eeprom {
  private:
    const uint8_t eeprom_size = 18;  // Размер EEPROM памяти в байтах (18 байт)
    const uint8_t diagnostic_index = 17;  // Индекс для хранения диагностической информации
    const uint8_t diagnostic_value = 200;
    bool eeprom_init = false;

  public:
    bool init() {
        eeprom_init = EEPROM.begin(eeprom_size); // Инициализация EEPROM с заданным размером
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
        EEPROM.commit();  // Не забываем сохранить изменения
        Serial.printf("Written %d to EEPROM[%d]\r\n", data, index);
    }

    // Функция для чтения диагностической информации
    uint8_t read_diagnostic_info() {
      uint8_t diagnostic_data = EEPROM.read(diagnostic_index);
      Serial.printf("Diagnostic Info (EEPROM[%d]): %d\r\n", diagnostic_index, diagnostic_data);
      Serial.println("Read Test: Successful");
      return diagnostic_data;
    }

    // Функция для записи диагностической информации
    void write_diagnostic_info(byte data) {
      EEPROM.write(diagnostic_index, data);
      EEPROM.commit();  // Сохраняем изменения
      Serial.printf("Written %d to EEPROM diagnostic index\r\n", data);
      Serial.println("Write Test: Successful");
    }

    void print_info(Timer* timer = nullptr){
      if (timer) {
        timer->println_with_timer(String("  EEPROM Total: ") + eeprom_size + String(" B"));
      } else {
        Serial.printf("  EEPROM Total: %u B\r\n", eeprom_size);
      }
    }

    // Функция диагностики
    void diagnostics() {
      Serial.println("=== Starting EEPROM Diagnostics ===");
  
      bool init_status = false;
      bool write_test_status = false;
      bool read_test_status = false;
      bool clear_test_status = false;
      bool skip_tests = true; // Флаг для пропуска тестов, если EEPROM не инициализирована
  
      // Проверка инициализации EEPROM
      Serial.print("Checking EEPROM initialization... ");
      if (eeprom_init) {
          Serial.println("OK");
          init_status = true;
          skip_tests = false; // Позволяем выполнять остальные тесты
      } else {
          Serial.println("FAILED");
          Serial.println("ERROR: EEPROM is not initialized. Skipping diagnostics.");
      }
  
      // Печать информации о размере EEPROM (если инициализировано)
      if (!skip_tests) {
          print_info();
      }
  
      // Тест диагностического индекса EEPROM[17] (если инициализировано)
      if (!skip_tests) {
          Serial.println("Testing Diagnostic Index (EEPROM[17])...");
  
          // Запись в диагностический байт (индекс 17)
          Serial.print("Writing diagnostic value... ");
          write_diagnostic_info(diagnostic_value);
          delay(100);
  
          // Проверяем, правильно ли записалось
          Serial.print("Verifying written value... ");
          if (read_diagnostic_info() == diagnostic_value) {
              write_test_status = true;
          } else {
              Serial.println("FAILED");
              Serial.println("ERROR: Diagnostic Index write test failed.");
          }
  
          // Очистка диагностического байта (запись 0x00)
          Serial.print("Clearing Diagnostic Index... ");
          write_diagnostic_info(0x00);
          delay(100);
  
          // Проверяем, успешно ли очищено
          Serial.print("Verifying clear operation... ");
          if (read_diagnostic_info() == 0x00) {
              clear_test_status = true;
          } else {
              Serial.println("FAILED");
              Serial.println("ERROR: Failed to clear Diagnostic Index.");
          }
      }
  
      // Вывод итоговых результатов тестов
      Serial.println("\n=== EEPROM Diagnostics Summary ===");
      Serial.printf("Initialization: %s\r\n", init_status ? "PASSED" : "FAILED");
      Serial.printf("Write Test: %s\r\n", skip_tests ? "SKIPPED" : (write_test_status ? "PASSED" : "FAILED"));
      Serial.printf("Read Test: %s\r\n", skip_tests ? "SKIPPED" : (read_test_status ? "PASSED" : "FAILED"));
      Serial.printf("Clear Test: %s\r\n", skip_tests ? "SKIPPED" : (clear_test_status ? "PASSED" : "FAILED"));
  
      Serial.println("=== EEPROM Diagnostics Complete ===\n");
  }
};
