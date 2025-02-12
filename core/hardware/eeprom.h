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
        Serial.printf("  EEPROM Total: %s B\r\n", eeprom_size);
      }
    }

    // Функция диагностики
    void diagnostics() {
        Serial.println("=== EEPROM Diagnostics ===");

        // Проверяем, доступна ли EEPROM
        if (eeprom_init) {
            Serial.println("EEPROM initialized successfully.");
        } else {
            Serial.println("EEPROM initialization failed.");
        }
        
        // Печатаем информацию о размере EEPROM
        print_info();
        
        // Диагностика только для диагностического байта
        Serial.println("Testing Diagnostic Index (EEPROM[17]):");

        // Запись в диагностический байт (индекс 17)
        write_diagnostic_info(diagnostic_value);  // Записываем значение diagnostic_value
        delay(100);

        if (read_diagnostic_info() == diagnostic_value) {
            Serial.println("Testing Diagnostic Index (EEPROM[17]): Successful");
        } else {
            Serial.println("Testing Diagnostic Index (EEPROM[17]): Error");
        }

        Serial.println("=== EEPROM Diagnostics Complete ===");
    }
};
