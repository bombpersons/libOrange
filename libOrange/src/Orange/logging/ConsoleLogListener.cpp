#include <Orange/logging/ConsoleLogListener.hpp>
using namespace orange;

#include <iostream>

ConsoleLogListener::ConsoleLogListener() {
}

ConsoleLogListener::~ConsoleLogListener() {
}

void ConsoleLogListener::Log(const OChar* _line) {
	std::cout << _line;
}
