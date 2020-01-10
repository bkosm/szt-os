#include <stdexcept>

class SztosException : public std::exception {
public:
	SztosException(char const* const msg);
	SztosException(const std::string& msg);
	virtual ~SztosException() = default;

};