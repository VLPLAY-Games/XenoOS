class Wifi_T{
    private:
        const char* wifi_commands[7] = {"connect", "disconnect", "reconnect", "scan", "status", "ip", "help"}; // Массив строк с командами
        const char* wifi_dependencies[1] = {"Help"}; // Список зависимостей
        Help help;
    public:
        // Обработка команд WiFi
        void handle_wifi_commands(const std::vector<String>& command, Wifi& wifi) {
            if (command.size() < 2) {
                Serial.println("Incomplete wifi command");
                return;
            }

            if (command[1] == "connect") {
                if (command.size() < 4) {
                    Serial.println("Incomplete wifi connect command");
                    return;
                }
                wifi.connect_wifi(command[2].c_str(), command[3].c_str());
            } else if (command[1] == "disconnect") {
                wifi.disconnect_wifi();
            } else if (command[1] == "reconnect") {
                wifi.reconnect_wifi();
            } else if (command[1] == "scan") {
                wifi.scan_wifi();
            } else if (command[1] == "status") {
                wifi.wifi_status();
            } else if (command[1] == "ip") {
                wifi.get_ip_address();
            } else if (command[1] == "help") {
                Serial.print("Available wifi commands: ");
                help.print_help(wifi_commands, sizeof(wifi_commands) / sizeof(wifi_commands[0]));
                Serial.print("Dependencies: ");
                help.print_help(wifi_dependencies, sizeof(wifi_dependencies) / sizeof(wifi_dependencies[0]));
                return;
            } else {
                Serial.println("Unknown wifi command");
            }
        }
};