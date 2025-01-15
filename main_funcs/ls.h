class Ls {
  public:
    void handle_ls_commands(const std::vector<String>& command, SdCard& sd) {
      String target_dir = current_directory;
      if (command.size() > 1) {
        target_dir = sd.normalize_path(command[1]);
        target_dir = sd.resolve_path(target_dir);
      }

      if (SD.exists(target_dir)) {
        sd.listDir(SD, target_dir.c_str(), 0);
      } else {
        Serial.println("Directory does not exist: " + target_dir);
      }
    }
};