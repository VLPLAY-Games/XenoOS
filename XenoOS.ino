
#include "software/config.h"
#include "core/modules/timer.h"
#include "software/main_funcs/help.h"
#include "software/serialconnection.h"
#include "core/config.h"
#include "core/crypto/md5.h"
#include "core/hardware/esp.h"
#include "core/hardware/sdcard.h"
#include "core/modules/wifi.h"
#include "software/main_funcs/main_funcs.h"
#include "software/software.h"
#include "boot/recovery.h"
#include "boot/bootloader.h"


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
