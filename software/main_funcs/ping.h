// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include <ESPping.h>

class Wifi_ping {
  private:
    Wifi wifi;
    const char* ping_commands[1] = {"help"};  // Обновленный массив команд
    const char* ping_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;

  public:
    void ping(const String& input) {
      if (!wifi.check_wifi()) {
        Serial.println("Cannot ping, WiFi not connected.");
        return;
      }

      Serial.printf("Pinging: %s\r\n", input.c_str());
      IPAddress ip;

      if (ip.fromString(input)) {
        Serial.print("Resolved IP: ");
        Serial.println(ip);
        if (Ping.ping(ip) > 0) {
          Serial.printf("Ping successful! Time: %d ms\r\n", Ping.minTime());
        } else {
          Serial.println("Ping failed");
        }
      } else {
        Serial.println("Attempting to resolve host...");
        if (WiFi.hostByName(input.c_str(), ip)) {
          Serial.print("Resolved IP: ");
          Serial.println(ip);
          if (Ping.ping(ip) > 0) {
            Serial.printf("Ping successful! Time: %d ms\r\n", Ping.minTime());
          } else {
            Serial.println("Ping failed");
          }
        } else {
          Serial.println("Failed to resolve host.");
        }
      }
    }

    void handle_ping_commands(const std::vector<String>& command) {
      if (command.size() < 2) {
        Serial.println("Please provide an IP address or hostname to ping");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available ping commands: ");
        help.print_help(ping_commands, sizeof(ping_commands) / sizeof(ping_commands[0]));
        Serial.print("Dependencies: ");
        help.print_help(ping_dependencies, sizeof(ping_dependencies) / sizeof(ping_dependencies[0]));
        return;
      }

      ping(command[1]);
    }
};
