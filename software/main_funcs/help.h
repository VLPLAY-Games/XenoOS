// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Help {
  public:
    void print_help(const char* commands[], size_t command_count) {
      // Обработка команды "help"
      for (size_t i = 0; i < command_count; ++i) {
        Serial.print(commands[i]);
        if (i < command_count - 1) {
          Serial.print(", ");
        }
      }
      Serial.println();
    }
};