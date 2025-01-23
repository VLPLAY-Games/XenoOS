class Hash {
  public:
    // Метод для обработки команды mv
    void handle_hash_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete hash command. Usage: hash <file_path>");
        return;
      }

      // Получаем пути из команды
      String file_path = sd.normalize_path(command[1]);
      
      Md5 md5;
      md5.calculate_md5_file(file_path.c_str());
      
    }
};