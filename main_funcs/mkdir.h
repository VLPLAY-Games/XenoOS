class Mkdir {
  public:
    void handle_mkdir_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete mkdir command");
        return;
      }

      String path = sd.normalize_path(command[1]);
      path = sd.resolve_path(path);

      if (SD.exists(path)) {
        Serial.println("Directory already exists: " + path);
        return;
      }

      sd.createDir(SD, path.c_str());
    }
};