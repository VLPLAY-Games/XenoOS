// Определение ANSI кодов с помощью макросов
#define ANSI_RED   "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE  "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN  "\033[36m"
#define ANSI_RESET "\033[0m"

// Класс для работы с цветами
class ColorPrinter {
public:
    // Печать сообщения об ошибке (красный цвет)
    static void print_error(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_RED, need_ln);
    }

    // Печать предупреждения (желтый цвет)
    static void print_warning(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_YELLOW, need_ln);
    }

    // Печать успешной операции (зеленый цвет)
    static void print_success(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_GREEN, need_ln);
    }

    // Печать информационного сообщения (синий цвет)
    static void print_info(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_BLUE, need_ln);
    }

    // Печать отладочной информации (пурпурный цвет)
    static void print_debug(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_MAGENTA, need_ln);
    }

    // Печать логов (циановый цвет)
    static void print_log(const String& text, bool need_ln = false) {
        print_custom(text, ANSI_CYAN, need_ln);
    }

    // Универсальная функция для печати с цветом
    static void print_custom(const String& text, const char* color, bool need_ln = false) {
        Serial.print(color);
        if (need_ln) {
            Serial.println(text);
        } else {
            Serial.print(text);
        }
        Serial.print(ANSI_RESET);  // Сброс цвета
    }
};
