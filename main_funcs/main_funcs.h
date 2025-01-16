#include "SD.h"
#include <HTTPClient.h>
#include "cat.h"
#include "cd.h"
#include "cpu.h"
#include "ls.h"
#include "mkdir.h"
#include "rename.h"
#include "rm.h"
#include "wget.h"
#include "sys_update.h"
#include "mac.h"
#include "touch.h"
#include "mv.h"
#include "cp.h"


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
      Serial.print("Version ");
      Serial.println(os_version);
      Serial.println(made_by);
      Serial.print("OS Date ");
      Serial.println(os_date);
      esp.print_all_info();
      Serial.println(os_github);
    }
    void help_commands(){
      Serial.println("Available commands:");
      help.print_help(commands, sizeof(commands) / sizeof(commands[0]));
    }
};
