class SystemUpdate {
  private:
    Wget &wget;                 // Ссылка на объект Wget для загрузки обновлений
    Esp &esp;                   // Ссылка на объект Esp для обновлений
    Help help;                  // Объект для справки
    const char* update_commands[3] = {"download", "install", "help"}; // Список доступных команд для обновлений
    

  public:
    // Конструктор
    SystemUpdate(Wget &wget_instance, Esp &esp_instance) : wget(wget_instance), esp(esp_instance) {}


    // Обработка команд update
    void handle_update_commands(const std::vector<String> &command) {
      if (command.size() < 2) {
        Serial.println("Usage: update download | update install | update help");
        return;
      }

      // Команда "help" для вывода справки
      if (command[2] == "help") {
        Serial.print("Available update commands: ");
        help.print_help(update_commands, sizeof(update_commands) / sizeof(update_commands[0]));
        return;
      }

      // Команда загрузки обновления
      if (command[2] == "download") {
        wget.handle_wget_commands({"wget", sys_upd_url, "/upd"});
      } 
      // Команда установки обновления
      else if (command[2] == "install") {
        esp.updateFromFS(SD);
      } else {
        Serial.println("Invalid update command");
      }
    }
};