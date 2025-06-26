// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class System {
  private:
    MainFuncs funcs;
    SerialConnection sc;
    Help help;
    History history;
    Eeprom eeprom;
    const char* system_commands[8] = {"restart", "info", "update", "diagnostic", "freset", "check", "installer", "help"}; // Массив строк с командами
    bool confirmation_pending = false; // Флаг ожидания подтверждения
    String pending_command;            // Сохраненная команда для выполнения после подтверждения


    void handle_system_commands(const std::vector<String>& command, Esp esp, SdCard sd, Wifi wifi, Spiffs spiffs, Eeprom eeprom) {
      if (command.size() < 2) {
        Serial.println("Incomplete system command");
        return;
      }

      if (command[1] == "restart") {
        funcs.restart(esp);
      } else if (command[1] == "info") {
        funcs.info(esp);
      } else if (command[1] == "diagnostic") {
        Diagnostics diagnostic(esp, spiffs, sd, eeprom);
        diagnostic.diagnostics();
      } else if (command[1] == "update") {
        Wget wget(wifi, sd);
        SystemUpdate upd(wget, esp, wifi, sd);
        upd.handle_update_commands(command);
      } else if (command[1] == "freset") {
        Serial.println("You want to make a system factory reset. Do you want to continue? (yes/no)");
        confirmation_pending = true; // Устанавливаем флаг ожидания
        pending_command = "system freset"; // Сохраняем полную команду
      } else if (command[1] == "check") {
        Checker checker;
        checker.sys_checker(sd);  
      } else if (command[1] == "installer") {
         Installer installer;
         installer.install_sys_files(sd);
      } 
      // Обработка команды "help"
      else if (command[1] == "help") {
        Serial.print("Available system commands: ");
        help.print_help(system_commands, sizeof(system_commands) / sizeof(system_commands[0]));
      } 
      else {
        Serial.println("Unknown system command");
      }
    }

    // Функция обработки подтверждения
    void handle_confirmation(const String& response, SdCard& sd, Esp esp) {
        if (pending_command == "system freset") {
          if (response == "yes") {
            Serial.println("Performing factory reset...");
            // Выполняем действия по сбросу
            
            Freset freset;
            Installer installer;

            bool reset_success = freset.factory_reset(sd);
            if (reset_success) {
              Serial.println("\r\nFactory reset success \r\nPreparing to reinstall system files");
              bool install_success = installer.install_sys_files(sd);
              if (install_success) Serial.println("\r\nReinstalling system files successfully \r\nRebooting");
              else Serial.println("\r\nReinstalling system files ERROR \nRebooting");
            } else Serial.println("\r\nFactory reset ERROR \r\nRebooting");
            funcs.restart(esp);
          } else {
            Serial.println("Factory reset canceled.");
          }
        }
        
        confirmation_pending = false;
        pending_command = "";
    }

  public:
    void check_input(Esp esp, Wifi wifi, SdCard sd, Spiffs spiffs, Eeprom eeprom) {
      sc.read_serial(sd);
      auto command = sc.get_command();

      if (!command.empty()) {
        Serial.println();

        // Если ожидается подтверждение
        if (confirmation_pending) handle_confirmation(command[0], sd, esp);
  
        else if (!command.empty() && command[0] == "system") {
          handle_system_commands(command, esp, sd, wifi, spiffs, eeprom);
        } else if (!command.empty() && command[0] == "cd") {
          Cd cd;
          cd.handle_cd_commands(command, sd);
        } else if (!command.empty() && command[0] == "ls") {
          Ls ls;
          ls.handle_ls_commands(command, sd);
        } else if (!command.empty() && command[0] == "mkdir") {
          Mkdir mkdir;
          mkdir.handle_mkdir_commands(command, sd);
        } else if (!command.empty() && command[0] == "rm") {
          Rm rm;
          rm.handle_rm_commands(command, sd);
        } else if (!command.empty() && command[0] == "cpu") {
          Cpu cpu;
          cpu.handle_cpu_commands(command, esp);
        } else if (!command.empty() && command[0] == "sdcard") {
          Sd sd_task;
          sd_task.handle_sdcard_commands(command, sd);
        } else if (!command.empty() && command[0] == "wifi") {
          Wifi_T wf;
          wf.handle_wifi_commands(command, wifi);
        } else if (!command.empty() && command[0] == "wget") {
          Wget wget(wifi, sd);
          wget.handle_wget_commands(command);
        } else if (!command.empty() && command[0] == "cat") {
          Cat cat(sd);
          cat.handle_cat_commands(command);
        } else if (!command.empty() && command[0] == "rename") {
          Rename rename(sd);
          rename.handle_rename_commands(command);
        } else if (!command.empty() && command[0] == "mac") {
          Mac mac;
          mac.handle_mac_commands(command, esp);
        } else if (!command.empty() && command[0] == "touch") {
          Touch touch;
          touch.handle_touch_commands(command, sd);
        } else if (!command.empty() && command[0] == "mv") {
          Mv mv;
          mv.handle_mv_commands(command, sd);
        } else if (!command.empty() && command[0] == "cp") {
          Cp cp;
          cp.handle_cp_commands(command, sd);
        } else if (!command.empty() && command[0] == "history") {
          history.handle_history_commands(command, sd);
        } else if (!command.empty() && command[0] == "hash") {
          Hash hash;
          hash.handle_hash_commands(command, sd);
        } else if (!command.empty() && command[0] == "ping") {
          Wifi_ping wifi_ping;
          wifi_ping.handle_ping_commands(command);
        } else if (!command.empty() && command[0] == "find") {
          Find find;
          find.handle_find_commands(command, sd);
        } else if (!command.empty() && command[0] == "df") {
          DF df(esp, spiffs, sd);
          df.handle_df_commands(command);
        } else if (!command.empty() && command[0] == "tar") {
           Tar tar;
           tar.handle_tar_commands(command);
        }  else if (sc.get_input() == "free") funcs.free(esp);
        else if (sc.get_input() == "update") funcs.update();
        else if (sc.get_input() == "clear") funcs.clear();
        else if (sc.get_input() == "help") funcs.help_commands();
        else Serial.println(sc.get_input() + ": Unknown command");
        
        history.write_history((sc.get_input() + "\n").c_str(), sd);

        sc.empty_command();
        sc.empty_input();
        Serial.println();
        Serial.print(current_directory + " $ ");
      }
    }
};