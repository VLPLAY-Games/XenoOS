class Timer {
  private:
    uint32_t start_time;  // Время старта программы в миллисекундах

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
      Serial.print("[");
      Serial.print(get_sec(), 3);  // Печатаем время с точностью до 3 знаков после запятой
      Serial.print("] ");
    }

    // Утилита для вывода лога с таймером
    void log_with_timer(const String& message) {
      print_time();
      Serial.println(message);
    }
};