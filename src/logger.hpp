#pragma once
#include <iostream>
class Logger {
public:

	static void error_log(const std::string& description)
	{
		std::cerr << "Error: " << description << std::endl;
	}
};