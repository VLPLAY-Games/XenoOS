// MIT License
// Copyright (c) 2025 VL_PLAY (Vlad)
// See https://github.com/VLPLAY-Games/XenoOS/blob/main/LICENSE for details.


// Определение ANSI кодов с помощью макросов
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_LIGHT_BLUE "\033[94m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_RESET   "\033[0m"

// Класс для работы с цветами
class ColorPrinter {
public:
    // Печать сообщения об ошибке (красный цвет)
    template <typename T>
    static void print_error(const T& text, bool need_ln = false) {
        print_custom(text, ANSI_RED, need_ln);
    }

    // Печать предупреждения (желтый цвет)
    template <typename T>
    static void print_warning(const T& text, bool need_ln = false) {
        print_custom(text, ANSI_YELLOW, need_ln);
    }

    // Печать успешной операции (зеленый цвет)
    template <typename T>
    static void print_success(const T& text, bool need_ln = false) {
        print_custom(text, ANSI_GREEN, need_ln);
    }

    // Печать информационного сообщения (светло-синий цвет)
    static void print_info(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_LIGHT_BLUE, need_ln);
    }

    // Печать отладочной информации (пурпурный цвет)
    template <typename T>
    static void print_debug(const T& text, bool need_ln = false) {
        print_custom(text, ANSI_MAGENTA, need_ln);
    }

    // Печать логов (циановый цвет)
    template <typename T>
    static void print_log(const T& text, bool need_ln = false) {
        print_custom(text, ANSI_CYAN, need_ln);
    }

    // Печать результата
    template <typename T>
    static void print_result(const T& text, bool need_ln = false) {
        if (text) print_success("OK", need_ln);
        else if (!text) print_error("ERROR", need_ln);
    }

    // Универсальная функция для печати с цветом
    template <typename T>
    static void print_custom(const T& text, const char* color, bool need_ln = false) {
        Serial.print(color);
        print_value(text, need_ln);
        Serial.print(ANSI_RESET);  // Сброс цвета
    }

private:
    // Обработка различных типов данных
    static void print_value(const String& text, bool need_ln) {
        need_ln ? Serial.println(text) : Serial.print(text);
    }

    static void print_value(const char* text, bool need_ln) {
        need_ln ? Serial.println(text) : Serial.print(text);
    }

    static void print_value(int value, bool need_ln) {
        need_ln ? Serial.println(value) : Serial.print(value);
    }

    static void print_value(float value, bool need_ln) {
        need_ln ? Serial.println(value, 3) : Serial.print(value, 3);
    }
};
