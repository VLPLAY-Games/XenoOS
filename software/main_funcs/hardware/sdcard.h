// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class Sd{
    private:
        Help help;
        const char* sdcard_commands[2] = {"info", "help"}; // Массив строк с командами
        const char* sdcard_dependencies[1] = {"Help"}; // Список зависимостей
    public:
        void handle_sdcard_commands(const std::vector<String>& command, SdCard& sd) {
            if (command.size() < 2) {
                Serial.println("Incomplete sdcard command");
                return;
            }

            if (command[1] == "info") {
                sd.print_info();
            }
            // Обработка команды "help"
            else if (command[1] == "help") {
                Serial.print("Available sdcard commands: ");
                help.print_help(sdcard_commands, sizeof(sdcard_commands) / sizeof(sdcard_commands[0]));
                Serial.print("Dependencies: ");
                help.print_help(sdcard_dependencies, sizeof(sdcard_dependencies) / sizeof(sdcard_dependencies[0]));
                return;
            } 
            else {
                Serial.println("Unknown sdcard command");
            }
        }
};