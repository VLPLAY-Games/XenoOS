class Rm {
  public:
    void handle_rm_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete rm command");
        return;
      }

      String path = sd.normalize_path(command[1]);
      path = sd.resolve_path(path);

      if (SD.exists(path)) {
        if (sd.is_directory(path)) {
          sd.removeDir(SD, path.c_str());
        } else {
          sd.deleteFile(SD, path.c_str());
        }
      } else {
        Serial.println("Path does not exist: " + path);
      }
    }
};