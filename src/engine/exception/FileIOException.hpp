#ifndef RRENDERER_ENGINE_EXCEPTIONS_FILE_IO_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTIONS_FILE_IO_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <string>

namespace rr
{

/// \brief Exception that can be used when a file operation failed or didn't work in the expected way
///
/// \author Felix Hommel
/// \date 7/19/2025
class FileIOException : public EngineException
{
public:
    explicit FileIOException(const std::string& path)
        : EngineException("An error occured while trying to open a file: " + std::string(path))
        , m_path(path)
    {}

    [[nodiscard]] std::string getPath() const { return m_path; }

private:
    std::string m_path;
};

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTIONS_FILE_IO_EXCEPTION_HPP
