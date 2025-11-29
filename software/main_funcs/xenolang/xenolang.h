// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.

#ifndef XENOLANG_H
#define XENOLANG_H

#include <XenoLanguage.h>

class XenoLang {
  private:
    const char* xenolang_commands[5] = {
      "help", "run", "compile", "execute", "security_info"
    };
    
    const char* xenolang_dependencies[2] = {"XenoLanguage", "SdCard"};
    Help help;
    XenoLanguage xeno;

    // Flags for additional information
    bool show_state = false;
    bool show_disasm = false;
    bool show_compiled = false;

    // Helper function for setting security parameters
    bool set_security_parameter(const String& param, const String& value) {
      if (param == "max_instr") {
        return xeno.setMaxInstructions(value.toInt());
      } else if (param == "max_string_len") {
        return xeno.setStringLimit(value.toInt());
      } else if (param == "max_var_name_len") {
        return xeno.setVariableNameLimit(value.toInt());
      } else if (param == "max_expr_depth") {
        return xeno.setExpressionDepth(value.toInt());
      } else if (param == "max_loop_depth") {
        return xeno.setLoopDepth(value.toInt());
      } else if (param == "max_if_depth") {
        return xeno.setIfDepth(value.toInt());
      } else if (param == "max_stack_size") {
        return xeno.setStackSize(value.toInt());
      } else if (param == "allowed_pins") {
        // Processing a comma-separated list of pins: "1,2,3,4"
        std::vector<uint8_t> pins;
        int start = 0;
        int end = value.indexOf(',');
        while (end != -1) {
          pins.push_back(value.substring(start, end).toInt());
          start = end + 1;
          end = value.indexOf(',', start);
        }
        pins.push_back(value.substring(start).toInt());
        return xeno.setAllowedPins(pins);
      }
      return false;
    }

    bool set_info_flag(const String& flag, const String& value) {
      if (flag == "state") {
        show_state = (value == "true");
        return true;
      } else if (flag == "disasm") {
        show_disasm = (value == "true");
        return true;
      } else if (flag == "print_compiled") {
        show_compiled = (value == "true");
        return true;
      }
      return false;
    }

    String read_file_from_sd(const String& file_path, SdCard& sd) {
      String normalized_path = sd.normalize_path(file_path);
      String resolved_path = sd.resolve_path(normalized_path);
      
      if (!sd.is_path_exists(resolved_path.c_str())) {
        Serial.println("File not found: " + resolved_path);
        return "";
      }

      File file = SD.open(resolved_path);
      if (!file) {
        Serial.println("Failed to open file: " + resolved_path);
        return "";
      }

      String content = "";
      while (file.available()) {
        content += (char)file.read();
      }
      file.close();
      
      return content;
    }

    void show_additional_info() {
      if (show_state) {
        Serial.println("=== XenoLanguage VM State ===");
        xeno.dumpState();
      }
      if (show_disasm) {
        Serial.println("=== XenoLanguage Disassembly ===");
        xeno.disassemble();
      }
      if (show_compiled) {
        Serial.println("=== XenoLanguage Compiled Code ===");
        xeno.printCompiledCode();
      }

      show_state = false;
      show_disasm = false;
      show_compiled = false;
    }

  public:
    void handle_xenolanguage_commands(const std::vector<String>& command, SdCard& sd) {
      if (command.size() < 2) {
        Serial.println("Incomplete xenolanguage command. Usage: xenolanguage [options] <command> [file_path]");
        Serial.println("Use 'xenolanguage help' for more information");
        return;
      }

      if (command[1] == "help") {
        Serial.println("XenoLanguage Commands:");
        Serial.println("======================");
        help.print_help(xenolang_commands, sizeof(xenolang_commands) / sizeof(xenolang_commands[0]));
        Serial.println("Dependencies: ");
        help.print_help(xenolang_dependencies, sizeof(xenolang_dependencies) / sizeof(xenolang_dependencies[0]));
        Serial.println("\nSecurity Options (prefix with -):");
        Serial.println("  max_instr=N          - Maximum instructions");
        Serial.println("  max_string_len=N     - Maximum string length");
        Serial.println("  max_var_name_len=N   - Maximum variable name length");
        Serial.println("  max_expr_depth=N     - Maximum expression depth");
        Serial.println("  max_loop_depth=N     - Maximum loop depth");
        Serial.println("  max_if_depth=N       - Maximum if depth");
        Serial.println("  max_stack_size=N     - Maximum stack size");
        Serial.println("  allowed_pins=X,Y,Z   - Allowed pin numbers");
        Serial.println("\nInfo Flags (prefix with -):");
        Serial.println("  state=true/false     - Show VM state after execution");
        Serial.println("  disasm=true/false    - Show disassembly after execution");
        Serial.println("  print_compiled=true/false - Show compiled code");
        Serial.println("\nExamples:");
        Serial.println("  xenolanguage -max_instr=1000 -state=true -print_compiled=true run /test.xeno");
        Serial.println("  xenolanguage -disasm=true compile /program.xeno");
        Serial.println("  xenolanguage -state=true execute");
        Serial.println("  xenolanguage security_info");
        return;
      }

      // Processing security parameters and information flags (start with -)
      int command_index = 1;
      while (command_index < command.size() && command[command_index].startsWith("-")) {
        String option = command[command_index].substring(1); // Убираем -
        int equals_pos = option.indexOf('=');
        
        if (equals_pos != -1) {
          String param = option.substring(0, equals_pos);
          String value = option.substring(equals_pos + 1);
          
          // Try to set it as a security parameter
          if (set_security_parameter(param, value)) {
            Serial.println("Set security parameter: " + param + " = " + value);
          } 
          // If it's not a security parameter, we'll try it as an information flag.
          else if (set_info_flag(param, value)) {
            Serial.println("Set info flag: " + param + " = " + value);
          }
          else {
            Serial.println("Unknown parameter: " + param);
          }
        } else {
          // Handling flags without a value (default true)
          if (set_info_flag(option, "true")) {
            Serial.println("Set info flag: " + option + " = true");
          } else {
            Serial.println("Unknown flag: " + option);
          }
        }
        command_index++;
      }

      // Check if the command remains after processing the parameters
      if (command_index >= command.size()) {
        Serial.println("No command specified after parameters");
        return;
      }

      String action = command[command_index];
      
      if (action == "run") {
        if (command_index + 1 >= command.size()) {
          Serial.println("Missing file path for run command");
          return;
        }
        
        String file_path = command[command_index + 1];
        String program = read_file_from_sd(file_path, sd);
        
        if (program.length() > 0) {
          Serial.println("Running Xeno program from: " + file_path);
          xeno.compile_and_run(program);
          show_additional_info();
        }
        
      } else if (action == "compile") {
        if (command_index + 1 >= command.size()) {
          Serial.println("Missing file path for compile command");
          return;
        }
        
        String file_path = command[command_index + 1];
        String program = read_file_from_sd(file_path, sd);
        
        if (program.length() > 0) {
          Serial.println("Compiling Xeno program from: " + file_path);
          xeno.compile(program);
          Serial.println("Compilation completed");
          show_additional_info();
        }
        
      } else if (action == "execute") {
        Serial.println("Executing compiled Xeno program");
        xeno.run();
        show_additional_info();
        
      } else if (action == "security_info") {
        Serial.println("=== XenoLanguage Security Information ===");
        Serial.println(xeno.getSecurityLimitsInfo());
        Serial.print("Allowed Pins: ");
        auto pins = xeno.getAllowedPins();
        for (auto pin : pins) {
          Serial.print(pin);
          Serial.print(" ");
        }
        Serial.println();
        
      } else {
        Serial.println("Unknown xenolanguage command: " + action);
        Serial.println("Use 'xenolanguage help' for available commands");
      }
    }
};

#endif