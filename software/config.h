// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.



const char* os_name = "XenoOS";

const char* os_version = "0.2.2";
const char* os_version_upd = "022";

const char* made_by = "Made by VL_PLAY Games";
const char* os_date = "25.06.2025";
const char* commands[] = {
  "wget", "mkdir", "ls", "cd", "rm", "wifi", "free", "sdcard", "clear", "system", "help", "cpu", "cat", "rename", "mac", "touch", "mv", "cp", "history", "hash", "ping", "find", "df", "tar"
};
const char* os_github = "github.com/VLPLAY-Games/XenoOS";
bool system_started = false;
const char* sys_upd_url = "https://raw.githubusercontent.com/VLPLAY-Games/Updates/main/XenoOS/firmware.bin";
const char* sys_upd_url_txt = "https://raw.githubusercontent.com/VLPLAY-Games/Updates/main/XenoOS/update.txt";
const char* historylog_path = "/sys/history.log";

const char* sys_main_dirs[] = {
  "/cfg",
  "/downloads",
  "/sys",
  "/upd"
};

const size_t sys_dir_count = sizeof(sys_main_dirs) / sizeof(sys_main_dirs[0]);

const char* sys_main_files[] = {
  "/cfg/date.cfg",
  "/cfg/sys.cfg",
  "/sys/history.log",
};

const size_t sys_files_count = sizeof(sys_main_files) / sizeof(sys_main_files[0]);
