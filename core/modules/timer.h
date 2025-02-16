// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See LICENSE.md for details.



class Timer {
  private:
    uint32_t start_time;  // Время старта программы в миллисекундах
    ColorPrinter color;

  public:
    // Конструктор, который инициализирует время старта
    Timer() {
      start_time = millis();
    }

    // Метод для получения времени в секундах с момента старта программы
    float get_sec() {
      return (millis() - start_time) / 1000.0f;  // Время в секундах
    }

    // Метод для получения времени в формате [0.000] (секунды и миллисекунды)
    const char* get_sec_str() {
      float seconds = (millis() - start_time) / 1000.0f;
      String time_str = "[" + String(seconds, 3) + "]";  // Форматируем с 3 знаками после запятой
      return time_str.c_str();  // Возвращаем строку в формате [0.000]
    }

    // Метод для получения времени в миллисекундах с момента старта программы
    uint32_t get_millis() {
      return millis() - start_time;  // Время в миллисекундах
    }

    // Метод для печати времени на экран в формате [0.000]
    void print_time() {
      color.print_log("[");
      color.print_log(get_sec());  // Печатаем время с точностью до 3 знаков после запятой
      color.print_log("] ");
    }

    // Утилита для вывода лога с таймером и цветной обработкой
    void println_with_timer(const String& message, const String& type = "text") {
      print_time();
      print_colored(message, type, true);
    }

    void print_with_timer(const String& message, const String& type = "text") {
      print_time();
      print_colored(message, type, false);
    }

    // Функция для цветного вывода через ColorPrinter
    void print_colored(const String& message, const String& type, bool newLine) {
      if (type == "error") {
          newLine ? color.print_error(message, true) : color.print_error(message, false);
      } else if (type == "warning") {
          newLine ? color.print_warning(message, true) : color.print_warning(message, false);
      } else if (type == "success") {
          newLine ? color.print_success(message, true) : color.print_success(message, false);
      } else if (type == "info") {
          newLine ? color.print_info(message, true) : color.print_info(message, false);
      } else if (type == "log") {
          newLine ? color.print_log(message, true) : color.print_log(message, false);
      } else {
        newLine ? Serial.println(message) : Serial.print(message);
      }
    }
};
