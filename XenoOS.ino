#include "defines.h"
#include "config.h"
#include "timer.h"
#include "main_funcs/help.h"
#include "serialconnection.h"
#include "hardware/esp.h"
#include "hardware/sdcard.h"
#include "modules.h"
#include "main_funcs/main_funcs.h"
#include "software.h"
#include "bootloader.h"


Timer timer;
SdCard sd;
Wifi wifi;
Esp esp;
System sys;


void setup() {
  Serial.begin(115200);
  Bootloader bl(sd, wifi, esp, timer);
  bl.boot();
}

void loop() {
  sys.check_input(esp, wifi, sd);
}
