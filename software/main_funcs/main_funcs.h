// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



#include "SD.h"
#include <HTTPClient.h>
#include "system/help.h"
#include "fs/cat.h"
#include "fs/cd.h"
#include "hardware/cpu.h"
#include "fs/ls.h"
#include "fs/mkdir.h"
#include "fs/rename.h"
#include "fs/rm.h"
#include "system/hash.h"
#include "net/wget.h"
#include "system/sys_update.h"
#include "hardware/mac.h"
#include "fs/touch.h"
#include "fs/mv.h"
#include "fs/cp.h"
#include "fs/history.h"
#include "net/ping.h"
#include "hardware/sdcard.h"
#include "net/wifi.h"
#include "fs/find.h"
#include "fs/df.h"
#include "archive/tar.h"
#include "system/checker.h"
#include "system/freset.h"
#include "system/installer.h"

class MainFuncs{
  private:
    Help help;
  public:
    void update(){}
    void restart(Esp esp){
      esp.restart();
    }
    void free(Esp esp){
      esp.print_ram_info();
    }
    void clear(){
      Serial.write(27);       // ESC command
      Serial.print("[2J");    // clear screen command
      Serial.write(27);
      Serial.print("[H");     // cursor to home command
    }
    void info(Esp esp){
      Serial.println(os_name);
      Serial.print("Core Version ");
      Serial.println(core_version);
      Serial.print("Core Date ");
      Serial.println(core_date);
      Serial.print("OS Version ");
      Serial.println(os_version);
      Serial.print("OS Date ");
      Serial.println(os_date);
      Serial.println(made_by);
      esp.print_all_info();
      Serial.println(os_github);
    }
    void help_commands(){
      Serial.println("Available commands:");
      help.print_help(commands, sizeof(commands) / sizeof(commands[0]));
    }
};
