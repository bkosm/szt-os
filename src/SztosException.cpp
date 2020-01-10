#include "SztosException.hpp"

SztosException::SztosException(char const* const msg) : std::exception(msg) {}

SztosException::SztosException(const std::string& msg) : SztosException(msg.c_str()) {}
