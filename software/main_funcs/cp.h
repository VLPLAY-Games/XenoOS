// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Cp {
  public:
    // Метод для обработки команды cp
    void handle_cp_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 3) {
        Serial.println("Incomplete cp command. Usage: cp <source_path> <destination_path>");
        return;
      }

      // Получаем пути из команды
      String source_path = sd.normalize_path(command[1]);
      String destination_path = sd.normalize_path(command[2]);

      // Копирование файла, используя функцию из SdCard
      sd.copy_file(source_path.c_str(), destination_path.c_str());
    }
};
