class Cd {
  private:
    const char* cd_commands[2] = {"help"};  // Обновленный массив команд
    Help help;

  public:
    void handle_cd_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete cd command");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available cd commands: ");
        help.print_help(cd_commands, sizeof(cd_commands) / sizeof(cd_commands[0]));
        return;
      }

      sd.change_directory(command[1]);
      Serial.println("Current directory: " + current_directory);
    }
};
