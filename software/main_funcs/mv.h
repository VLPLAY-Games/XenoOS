// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Mv {
  private:
    const char* mv_commands[1] = {"help"};  // Обновленный массив команд
    const char* mv_dependencies[1] = {"Help"}; // Список зависимостей
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
        Serial.print("Dependencies: ");
        help.print_help(mv_dependencies, sizeof(mv_dependencies) / sizeof(mv_dependencies[0]));
        return;
      }

      // Получаем нормализованные пути
      String source_path = sd.normalize_path(command[1]);
      String destination_path = sd.normalize_path(command[2]);

      // Если в destination_path нет имени файла, добавляем имя из source_path
      if (!destination_path.endsWith("/") && destination_path.indexOf('.') == -1) {
        // Извлекаем имя файла из source_path
        String file_name = source_path.substring(source_path.lastIndexOf("/") + 1);
        destination_path += "/" + file_name;
      }

      // Перемещаем файл
      sd.move_file(source_path.c_str(), destination_path.c_str());
    }
};
