#pragma once

#include <base/error_handler_interface.h>
#include <sstream>

namespace Base
{
    class TextErrorHandler : public ErrorHandlerInterface
    {
    public:

        void ReportError(
            const std::string & message,
            const std::string & file
            ) override;

        std::string GetErrorMessage();

    private:

        std::stringstream
            m_ErrorStream;
    };
}