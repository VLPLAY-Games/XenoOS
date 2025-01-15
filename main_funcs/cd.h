class Cd {
  public:
    void handle_cd_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete cd command");
        return;
      }

      String new_path = sd.normalize_path(command[1]);
      new_path = sd.resolve_path(new_path);

      if (SD.exists(new_path)) {
        current_directory = new_path;
        Serial.println("Directory changed to: " + current_directory);
      } else {
        Serial.println("Directory does not exist: " + new_path);
      }

      Serial.println("Current directory: " + current_directory);
    }
};