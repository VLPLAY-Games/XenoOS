// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <Wire.h>

class I2C {
  private:
    uint8_t _sdaPin;   // Пин SDA
    uint8_t _sclPin;   // Пин SCL
    int _address;  // Адрес I2C устройства
    bool _initialized = false;  // Статус инициализации

  public:
    // Конструктор для инициализации с пинами
    I2C(uint8_t sdaPin = 21, uint8_t sclPin = 22) {
      _sdaPin = sdaPin;
      _sclPin = sclPin;
    }

    // Инициализация I2C с указанием пинов
    bool begin() {
      Wire.begin(_sdaPin, _sclPin);
      Wire.setClock(400000); // Устанавливаем скорость шины I2C (400 кГц)
      _initialized = true;
      Serial.println("I2C initialized successfully");
      return true;
    }

    // Проверка инициализации I2C
    bool isInitialized() {
      return _initialized;
    }

    // Установка адреса I2C устройства
    void setAddress(int address) {
      _address = address;
    }

    // Чтение одного байта с устройства
    uint8_t readByte() {
      if (!_initialized) {
        Serial.println("I2C not initialized!");
        return 0;
      }

      Wire.beginTransmission(_address);
      Wire.endTransmission(false);
      Wire.requestFrom(_address, (uint8_t)1);

      if (Wire.available()) {
        return Wire.read();
      } else {
        Serial.println("Error: No data available!");
        return 0;
      }
    }

    // Запись одного байта в устройство
    bool writeByte(uint8_t data) {
      if (!_initialized) {
        Serial.println("I2C not initialized!");
        return false;
      }

      Wire.beginTransmission(_address);
      Wire.write(data);
      uint8_t result = Wire.endTransmission();
      
      if (result == 0) {
        Serial.println("Data written successfully");
        return true;
      } else {
        Serial.println("Error: Failed to write data");
        return false;
      }
    }

    // Чтение нескольких байтов с устройства
    bool readBytes(uint8_t* buffer, size_t length) {
      if (!_initialized) {
        Serial.println("I2C not initialized!");
        return false;
      }

      Wire.beginTransmission(_address);
      Wire.endTransmission(false);
      Wire.requestFrom(_address, length);

      size_t i = 0;
      while (Wire.available() && i < length) {
        buffer[i++] = Wire.read();
      }

      if (i == length) {
        Serial.println("Data read successfully");
        return true;
      } else {
        Serial.println("Error: Failed to read data");
        return false;
      }
    }

    // Запись нескольких байтов в устройство
    bool writeBytes(uint8_t* data, size_t length) {
      if (!_initialized) {
        Serial.println("I2C not initialized!");
        return false;
      }

      Wire.beginTransmission(_address);
      for (size_t i = 0; i < length; i++) {
        Wire.write(data[i]);
      }

      uint8_t result = Wire.endTransmission();
      if (result == 0) {
        Serial.println("Data written successfully");
        return true;
      } else {
        Serial.println("Error: Failed to write data");
        return false;
      }
    }

    // Пинг устройства на I2C (проверка наличия устройства по адресу)
    bool ping() {
      if (!_initialized) {
        Serial.println("I2C not initialized!");
        return false;
      }

      Wire.beginTransmission(_address);
      uint8_t result = Wire.endTransmission();
      if (result == 0) {
        Serial.println("Device found on I2C bus");
        return true;
      } else {
        Serial.println("Device not found on I2C bus");
        return false;
      }
    }

    // Установка частоты I2C
    void setClock(uint32_t clockSpeed) {
      if (!_initialized) {
        Serial.println("I2C not initialized!");
        return;
      }
      Wire.setClock(clockSpeed);
      Serial.printf("I2C clock set to %d Hz\r\n", clockSpeed);
    }

    // Сканирование всех устройств на I2C
    void scan() {
      Serial.println("Scanning I2C bus...");

      for (uint8_t i = 0; i < 128; i++) {
        Wire.beginTransmission(i);
        uint8_t result = Wire.endTransmission();
        if (result == 0) {
          Serial.print("Device found at address 0x");
          Serial.println(i, HEX);
        }
      }

      Serial.println("I2C scan complete");
    }
};