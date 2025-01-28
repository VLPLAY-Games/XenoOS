class System {
  private:
    MainFuncs funcs;
    SerialConnection sc;
    Help help;
    History history;
    const char* system_commands[5] = {"restart", "info", "update", "diagnostic", "help"}; // Массив строк с командами


    void handle_system_commands(const std::vector<String>& command, Esp esp, SdCard sd, Wifi wifi, Spiffs spiffs) {
      if (command.size() < 2) {
        Serial.println("Incomplete system command");
        return;
      }

      if (command[1] == "restart") {
        funcs.restart(esp);
      } else if (command[1] == "info") {
        funcs.info(esp);
      } else if (command[1] == "diagnostic") {
        Diagnostics diagnostic(esp, spiffs, sd);
        diagnostic.diagnostics();
      } else if (command[1] == "update") {
        Wget wget(wifi, sd);
        SystemUpdate upd(wget, esp, wifi, sd);
        upd.handle_update_commands(command);
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

  public:
    void check_input(Esp esp, Wifi wifi, SdCard sd, Spiffs spiffs) {
      sc.read_serial();
      if (sc.get_input().length() > 0) {
        Serial.println(sc.get_input());
        sc.split_input_to_command();
        auto command = sc.get_command();
        Serial.println();

        if (!command.empty() && command[0] == "system") {
          handle_system_commands(command, esp, sd, wifi, spiffs);
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
        else Serial.println("Unknown command");
        
        history.write_history((sc.get_input() + "\n").c_str(), sd);

        sc.empty_command();
        sc.empty_input();
        Serial.println();
        Serial.print(current_directory + " $ ");
      }
    }
};