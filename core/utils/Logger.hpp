#pragma once

#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string_view>

template<typename T>
inline void format_helper(std::ostringstream& oss,
    std::string_view& str, const T& value)
{
    //find the {}
    std::size_t openBracket = str.find('{'); 
    if (openBracket == std::string::npos) { return; }
    std::size_t closeBracket = str.find('}', openBracket + 1);
    if (closeBracket == std::string::npos) { return; }

    //After this line, the output stream will be:
    //The output stream + the string view up to (but not including) {} + the value argument.
    oss << str.substr(0, openBracket) << value;

    //Change the string view reference so that subsequent recursive calls start after the {} that this function just took care of. 
    str = str.substr(closeBracket + 1);  
}

//Given a string with any number of {} and any number of arguments, return a string that has each subsequent {} replaced with one of the subsequent arguments.
// e.g. format("Hello {} {}", world, 123) returns "Hello world 123"
template<typename... Targs>
inline std::string format(std::string_view str, Targs...args) //Define a function that takes a string_view and any number of arguments of type Targs.
{
    std::ostringstream oss; //define an output string stream that allows us to insert characters into a string buffer. 
    (format_helper(oss, str, args), ...); //recursively call format_helper on each argument. This takes the string stream, a reference of the string view, and one of the arguments. 
    oss << str;
    return oss.str();
}

class Logger
{
public:
    enum c_color { BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35, CYAN = 36, WHITE = 37 };
    enum c_decoration { NORMAL = 0, BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4, RIVERCED = 26, FRAMED = 51 };

    template<typename T>
    static void INFO(T str)
    {
        std::cout << "\033[" << NORMAL << ";" << GREEN << "m" << "[Info] " << str << "\033[0m" << std::endl;
    }

    template<typename T>
    static void WARN(T str)
    {
        std::cout << "\033[" << ITALIC << ";" << YELLOW << "m" << "[Warning] " << str << "\033[0m" << std::endl;
    }

    template<typename T>
    static void ERROR(T str)
    {
        std::cout << "\033[" << BOLD << ";" << RED << "m" << "[Error] " << str << "\033[0m" << std::endl;
        abort();
    }

    static void DEBUG(const std::string str, c_color color, c_decoration decoration = c_decoration::NORMAL) 
    {
        std::cout << "\033[" << decoration << ";" << color << "m" << "[Debug] " << str << "\033[0m" << std::endl;
    }

    template <class... _Types>
    static void NULLIFY(_Types...Args) { }
};

#define LOG_DEBUG(...)   Logger::DEBUG(__VA_ARGS__)

#define LOG_INFO(...)    Logger::INFO(__VA_ARGS__)
#define LOG_WARN(...)    Logger::WARN(__VA_ARGS__)
#define LOG_ERROR(...)   Logger::ERROR(__VA_ARGS__)

#define LOG_INFO_F(...)    Logger::INFO(format(__VA_ARGS__))
#define LOG_WARN_F(...)    Logger::WARN(format(__VA_ARGS__))
#define LOG_ERROR_F(...)   Logger::ERROR(format(__VA_ARGS__))
