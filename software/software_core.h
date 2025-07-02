// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



class System {
  private:
    MainFuncs funcs;
    SerialConnection sc;
    Help help;
    History history;
    Esp esp;
    Wifi wifi;
    SdCard sd;
    Spiffs spiffs;
    Eeprom eeprom;

    Installer* installer;
    ModuleManager* modMgr;

    const char* system_commands[9] = {
      "restart","info","update","diagnostic",
      "freset","check","installer","mod","help"
    };
    bool confirmation_pending = false;
    String pending_command;

    void handle_system_commands(const std::vector<String>& command) {
      if (command.size() < 2) {
        Serial.println("Incomplete system command");
        return;
      }
      const String& cmd = command[1];
      if (cmd == "restart") {
        funcs.restart(esp);
      }
      else if (cmd == "info") {
        funcs.info(esp);
      }
      else if (cmd == "diagnostic") {
        Diagnostics diagnostic(esp, spiffs, sd, eeprom, *installer);
        diagnostic.diagnostics();
      }
      else if (cmd == "update") {
        Wget wget(wifi, sd);
        SystemUpdate upd(wget, esp, wifi, sd);
        upd.handle_update_commands(command);
      }
      else if (cmd == "freset") {
        Serial.println("You want to make a system factory reset. Continue? (yes/no)");
        confirmation_pending = true;
        pending_command = "system freset";
      }
      else if (cmd == "check") {
        Checker checker;
        checker.sys_checker(sd);
      }
      else if (cmd == "installer") {
        installer->install_sys_files();
      }
      else if (cmd == "mod") {
        // Module management: install, flash, run, remove, ls
        if (command.size() < 3) {
          Serial.println("Usage: system mod <install|flash|run|remove|ls> [args]");
          return;
        }
        const String& sub = command[2];
        if (sub == "install") {
          if (command.size() < 4) Serial.println("Usage: system mod install <sd_path>");
          else installer->install_from_sd(command[3].c_str());
        }
        else if (sub == "flash") {
          if (command.size() < 4) Serial.println("Usage: system mod flash <sd_path>");
          else installer->install_module_to_flash(command[3].c_str());
          modMgr->scanModules();
        }
        else if (sub == "run") {
          // args after name are passed to module
          std::vector<String> args(command.begin()+3, command.end());
          modMgr->scanModules();
          if (!modMgr->handleCommand(std::vector<String>(args.begin()-1, args.end()))) {
            Serial.println("Module not found or failed to run");
          }
        }
        else if (sub == "remove") {
          if (command.size() < 4) Serial.println("Usage: system mod remove <name>");
          else {
            installer->remove_module(command[3].c_str());
            modMgr->scanModules();
          }
        }
        else if (sub == "ls") {
          sd.listDir(SD, MODULES_DIR, 0);
        }
        else {
          Serial.println("Unknown subcommand for system mod");
        }
      }
      else if (cmd == "help") {
        Serial.print("Available system commands: ");
        help.print_help(system_commands, 9);
      }
      else {
        Serial.println("Unknown system command");
      }
    }

    void handle_confirmation(const String& response) {
      if (pending_command == "system freset") {
        if (response == "yes") {
          Serial.println("Performing factory reset...");
          Freset freset;
          bool reset_success = freset.factory_reset(sd);
          if (reset_success) {
            Serial.println("Factory reset success, reinstalling system files...");
            installer->install_sys_files();
          } else {
            Serial.println("Factory reset ERROR");
          }
          funcs.restart(esp);
        } else {
          Serial.println("Factory reset canceled.");
        }
      }
      confirmation_pending = false;
      pending_command = "";
    }

  public:
    System(Esp esp_instance, Wifi wifi_instance, SdCard sd_instance, Spiffs spiffs_instance, Eeprom eeprom_instance)
      : esp(esp_instance), wifi(wifi_instance), sd(sd_instance), spiffs(spiffs_instance), eeprom(eeprom_instance) {
        installer = new Installer(&sd);
        modMgr = new ModuleManager(&sd, installer);
        modMgr->scanModules();
        Serial.print("Initialization complete. Ready. ");
        Serial.print(current_directory + " $ ");
    }
    void check_input(Esp esp, Wifi wifi, SdCard sd, Spiffs spiffs, Eeprom eeprom) {
      sc.read_serial(sd);
      auto command = sc.get_command();

      if (!command.empty()) {
        history.write_history((sc.get_input() + "\n").c_str(), sd);
        Serial.println();

        if (confirmation_pending) {
          handle_confirmation(command[0]);
        }
        else if (command[0] == "system") {
          handle_system_commands(command);
        }
        else if (command[0] == "cd") {
          Cd cd;
          cd.handle_cd_commands(command, sd);
        }
        else if (command[0] == "ls") {
          Ls ls;
          ls.handle_ls_commands(command, sd);
        }
        else if (command[0] == "mkdir") {
          Mkdir mkdir;
          mkdir.handle_mkdir_commands(command, sd);
        }
        else if (command[0] == "rm") {
          Rm rm;
          rm.handle_rm_commands(command, sd);
        }
        else if (command[0] == "cpu") {
          Cpu cpu;
          cpu.handle_cpu_commands(command, esp);
        }
        else if (command[0] == "sdcard") {
          Sd sd_task;
          sd_task.handle_sdcard_commands(command, sd);
        }
        else if (command[0] == "wifi") {
          Wifi_T wf;
          wf.handle_wifi_commands(command, wifi);
        }
        else if (command[0] == "wget") {
          Wget wget(wifi, sd);
          wget.handle_wget_commands(command);
        }
        else if (command[0] == "cat") {
          Cat cat(sd);
          cat.handle_cat_commands(command);
        }
        else if (command[0] == "rename") {
          Rename rename(sd);
          rename.handle_rename_commands(command);
        }
        else if (command[0] == "mac") {
          Mac mac;
          mac.handle_mac_commands(command, esp);
        }
        else if (command[0] == "touch") {
          Touch touch;
          touch.handle_touch_commands(command, sd);
        }
        else if (command[0] == "mv") {
          Mv mv;
          mv.handle_mv_commands(command, sd);
        }
        else if (command[0] == "cp") {
          Cp cp;
          cp.handle_cp_commands(command, sd);
        }
        else if (command[0] == "history") {
          history.handle_history_commands(command, sd);
        }
        else if (command[0] == "hash") {
          Hash hash;
          hash.handle_hash_commands(command, sd);
        }
        else if (command[0] == "ping") {
          Wifi_ping wifi_ping;
          wifi_ping.handle_ping_commands(command);
        }
        else if (command[0] == "find") {
          Find find;
          find.handle_find_commands(command, sd);
        }
        else if (command[0] == "df") {
          DF df(esp, spiffs, sd);
          df.handle_df_commands(command);
        }
        else if (command[0] == "tar") {
           Tar tar;
           tar.handle_tar_commands(command);
        }
        else if (sc.get_input() == "free") {
          funcs.free(esp);
        }
        else if (sc.get_input() == "update") {
          funcs.update();
        }
        else if (sc.get_input() == "clear") {
          funcs.clear();
        }
        else if (sc.get_input() == "help") {
          funcs.help_commands();
        }
        else {
          // Попытка запустить модуль из /modules
          if (!modMgr->handleCommand(command)) {
            Serial.println(sc.get_input() + ": Unknown command");
          }
        }

        sc.empty_command();
        sc.empty_input();
        Serial.println();
        Serial.print(current_directory + " $ ");
      }
    }
};