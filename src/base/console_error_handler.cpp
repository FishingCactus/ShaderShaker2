#include "console_error_handler.h"

#include <iostream>

namespace Base
{
	void ConsoleErrorHandler::ReportError(
		const std::string & message,
		const std::string & file
		)
	{
		std::cerr << file << ": " << message;
	}
}