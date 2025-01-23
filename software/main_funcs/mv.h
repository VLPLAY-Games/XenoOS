class Mv {
  public:
    // Метод для обработки команды mv
    void handle_mv_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 3) {
        Serial.println("Incomplete mv command. Usage: mv <source_path> <destination_path>");
        return;
      }

      // Получаем пути из команды
      String source_path = sd.normalize_path(command[1]);
      String destination_path = sd.normalize_path(command[2]);

      // Перемещение файла, используя функцию из SdCard
      sd.move_file(source_path.c_str(), destination_path.c_str());
    }
};