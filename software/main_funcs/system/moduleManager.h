// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.


#pragma once
#include <vector>
#include <functional>

// Менеджер модулей: динамическая регистрация и исполнение
class ModuleManager {
  public:
    // Тип обработчика: аргументы shell
    using Handler = std::function<bool(const std::vector<String>&)>;

    ModuleManager(SdCard* sd, Installer* installer)
      : sd(sd), installer(installer) {}

    // Сканирует /modules на SD и регистрирует все имена
    void scanModules() {
        modules.clear();
        // используем SdCard::listDir для чтения файлов в каталоге
        sd->listDir(SD, "/modules", 0);
        // предполагаем, что listDir выводит имена; здесь нужно получать список внутри API
        // Вместо стандартного listDir, можно реализовать readDir:
        std::vector<String> files = sd->listFiles("/modules"); // нужно добавить такой метод
        for (const auto& f : files) {
            if (f.endsWith(".xmod") || f.endsWith(".bin")) {
                String name = f;
                name.replace(".xmod", "");
                name.replace(".bin", "");
                modules.push_back(name);
            }
        }
    }

    // Попытка обработки команды, возвращает true, если команда — модуль
    bool handleCommand(const std::vector<String>& cmd) {
        if (cmd.empty()) return false;
        const String& name = cmd[0];
        for (const auto& mod : modules) {
            if (name == mod) {
                // сбор аргументов в C-style
                int argc = cmd.size();
                char** argv = new char*[argc];
                for (int i = 0; i < argc; ++i) {
                    argv[i] = (char*)cmd[i].c_str();
                }
                installer->run_module_from_flash(argc, argv);
                delete[] argv;
                return true;
            }
        }
        return false;
    }

  private:
    SdCard* sd;
    Installer* installer;
    std::vector<String> modules;
};