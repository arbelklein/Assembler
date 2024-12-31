#define BLACK "\033[22;30m"
#define RED "\033[22;31m"
#define GREEN "\033[22;32m"
#define BROWN "\033[22;33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define GRAY "\033[22;37m"
#define DARK_GRAY "\033[01;30m"
#define LIGHT_RED "\033[01;31m"
#define LIGHT_GREEN "\033[01;32m"
#define YELLOW "\033[01;33m"
#define LIGHT_BLUE "\033[01;34m"
#define LIGHT_MAGENTA "\033[01;35m"
#define LIGHT_CYAN "\033[01;36m"
#define WHITE "\033[01;37m"
#define COLOR_RESET "\x1b[0m"

#define PRINT_ERROR (WHITE "assembler: " LIGHT_RED "error: " COLOR_RESET)
#define PRINT_FATAL (WHITE "assembler: " LIGHT_RED "fatal error: " COLOR_RESET)
#define PRINT_WARN (WHITE "assembler: " LIGHT_MAGENTA "warning: " COLOR_RESET)
