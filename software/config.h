// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



const char* os_name = "XenoOS";

const char* os_version = "0.2.1";
const char* os_version_upd = "021";

const char* made_by = "Made by VL_PLAY Games";
const char* os_date = "10.03.2025";
const char* commands[] = {
  "wget", "mkdir", "ls", "cd", "rm", "wifi", "free", "sdcard", "clear", "system", "help", "cpu", "cat", "rename", "mac", "touch", "mv", "cp", "history", "hash", "ping", "find", "df", "tar"
};
const char* os_github = "github.com/VLPLAY-Games/XenoOS";
bool system_started = false;
const char* sys_upd_url = "https://raw.githubusercontent.com/VLPLAY-Games/Updates/main/XenoOS/firmware.bin";
const char* sys_upd_url_txt = "https://raw.githubusercontent.com/VLPLAY-Games/Updates/main/XenoOS/update.txt";
const char* historylog_path = "/sys/history.log";