class Cpu {
  private:
    const char* cpu_commands[2] = {"info", "help"}; // Массив строк с командами
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
        }
        // Обработка команды "help"
        else if (command[1] == "help") {
          Serial.print("Available cpu commands: ");
          help.print_help(cpu_commands, sizeof(cpu_commands) / sizeof(cpu_commands[0]));
        }
        // Неизвестная команда
        else {
          Serial.println("Unknown cpu command");
        }
    }
};