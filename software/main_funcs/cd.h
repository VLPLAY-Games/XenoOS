class Cd {
  public:
    void handle_cd_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete cd command");
        return;
      }

      // Используем функцию change_directory из SdCard
      sd.change_directory(command[1]);

      // Вывод текущего каталога
      Serial.println("Current directory: " + current_directory);
    }
};