class Cpu {
  private:
    const char* cpu_commands[3] = {"info", "temp", "help"}; // Массив строк с командами
    const char* cpu_dependencies[1] = {"Help"}; // Список зависимостей
    Help help;
  public:

    void handle_cpu_commands(const std::vector<String>& command, Esp esp) {
        if (command.size() < 2) {
          Serial.println("Incomplete cpu command");
          return;
        }

        // Обработка команды "info"
        if (command[1] == "info") {
          esp.print_cpu_info();
        } else if (command[1] == "temp") {
          Serial.print("CPU Temperature: ");
          Serial.print(esp.get_cpu_temp());
          Serial.println(" °C");
        }
        // Обработка команды "help"
        else if (command[1] == "help") {
          Serial.print("Available cpu commands: ");
          help.print_help(cpu_commands, sizeof(cpu_commands) / sizeof(cpu_commands[0]));
          Serial.print("Dependencies: ");
          help.print_help(cpu_dependencies, sizeof(cpu_dependencies) / sizeof(cpu_dependencies[0]));
          return;
        }
        // Неизвестная команда
        else {
          Serial.println("Unknown cpu command");
        }
    }
};