class Mv {
  private:
    const char* mv_commands[1] = {"help"};  // Обновленный массив команд
    Help help;

  public:
    void handle_mv_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 3) {
        Serial.println("Incomplete mv command. Usage: mv <source_path> <destination_path>");
        return;
      }

      if (command[1] == "help") {
        Serial.print("Available mv commands: ");
        help.print_help(mv_commands, sizeof(mv_commands) / sizeof(mv_commands[0]));
        return;
      }

      String source_path = sd.normalize_path(command[1]);
      String destination_path = sd.normalize_path(command[2]);

      sd.move_file(source_path.c_str(), destination_path.c_str());
    }
};
