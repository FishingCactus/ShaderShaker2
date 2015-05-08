#include "text_error_handler.h"


namespace Base
{
    void TextErrorHandler::ReportError(
        const std::string & message,
        const std::string & file
        )
    {
        m_ErrorStream << file << ": " << message << std::endl;
    }

    std::string TextErrorHandler::GetErrorMessage()
    {
        return m_ErrorStream.str();
    }
}