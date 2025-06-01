#ifndef RRENDERER_ENGINE_EXCEPTIONS_FILE_IO_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTIONS_FILE_IO_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <string>

namespace rr
{

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
