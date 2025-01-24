#include <ESPping.h>

class Wifi_ping {
  private:
    Wifi wifi;  // Включаем класс Wifi для работы с подключением Wi-Fi

  public:
    // Метод для пинга хоста или IP-адреса
    void ping(const String& input) {
      if (!wifi.check_wifi()) {
        Serial.println("Cannot ping, WiFi not connected.");
        return;
      }

      Serial.printf("Pinging: %s\n", input.c_str());
      IPAddress ip;

      // Проверка, является ли строка допустимым IP-адресом
      if (ip.fromString(input)) {
        // Если это IP, пингуем его напрямую
        Serial.print("Resolved IP: ");
        Serial.println(ip);
        if (Ping.ping(ip) > 0) {
          Serial.printf("Ping successful! Time: %d ms\n", Ping.minTime());
        } else {
          Serial.println("Ping failed");
        }
      } else {
        // Если это не IP, пробуем преобразовать в домен и пингуем
        Serial.println("Attempting to resolve host...");
        if (WiFi.hostByName(input.c_str(), ip)) {
          Serial.print("Resolved IP: ");
          Serial.println(ip);
          if (Ping.ping(ip) > 0) {
            Serial.printf("Ping successful! Time: %d ms\n", Ping.minTime());
          } else {
            Serial.println("Ping failed");
          }
        } else {
          Serial.println("Failed to resolve host.");
        }
      }
    }

    // Метод для обработки команд пинга
    void handle_ping_commands(const std::vector<String>& command) {
      if (command.size() < 2) {
        Serial.println("Please provide an IP address or hostname to ping");
        return;
      }

      ping(command[1]);
    }
};