#include <WiFi.h>

class Wifi {
  public:
    // Подключение к WiFi
    void connect_wifi(const char* ssid, const char* password) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Already connected to a WiFi network. Disconnecting...");
        WiFi.disconnect();
        delay(1000);
      }
      
      Serial.println("Connecting to WiFi...");
      WiFi.begin(ssid, password);

      Timer timer;

      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);

        // Проверка времени подключения
        if (timer.get_sec() > 10.0f) { // 10 секунд
          Serial.println("");
          Serial.print(timer.get_sec_str());
          Serial.println("Error: Failed to connect to WiFi: Timeout");
          WiFi.disconnect();
          return; 
        }
      }
      
      Serial.print("\n");
      timer.print_time();
      Serial.println("Connected to the WiFi network");
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());
    }

    // Отключение от WiFi
    void disconnect_wifi() {
      if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
        Serial.println("Disconnected from WiFi");
      }
      else {
        Serial.println("WiFi is not connected");
      }
    }

    // Переподключение к WiFi
    void reconnect_wifi() {
      WiFi.reconnect();
      Serial.println("Reconnecting to WiFi...");
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
      }
      Serial.println("\nReconnected to WiFi");
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());
    }

    // Статус подключения
    void wifi_status() {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi is connected");
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("RSSI: ");
        Serial.println(WiFi.RSSI());
      } else {
        Serial.println("WiFi is not connected");
      }
    }

    // Получение локального IP-адреса
    void get_ip_address() {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Local ESP32 IP: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("WiFi is not connected");
      }
    }

    // Сканирование доступных сетей
    void scan_wifi() {
      Serial.println("WiFi Scan start");
      uint8_t n = WiFi.scanNetworks();
      Serial.println("Scan done");
      if (n == 0) {
        Serial.println("No networks found");
      } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (uint8_t i = 0; i < n; ++i) {
          Serial.printf("%2d", i + 1);
          Serial.print(" | ");
          Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
          Serial.print(" | ");
          Serial.printf("%4ld", WiFi.RSSI(i));
          Serial.print(" | ");
          Serial.printf("%2ld", WiFi.channel(i));
          Serial.print(" | ");
          switch (WiFi.encryptionType(i)) {
            case WIFI_AUTH_OPEN:            Serial.print("open"); break;
            case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
            case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
            case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
            case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
            case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
            case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
            case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
            case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
            default:                        Serial.print("unknown");
          }
          Serial.println();
          delay(10);
        }
      }
      Serial.println("");
      WiFi.scanDelete();
    }

    // Метод для проверки подключения к WiFi
    bool check_wifi() {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Error: Not connected to WiFi. Connect first.");
        return false;
      }
      return true;
    }
};