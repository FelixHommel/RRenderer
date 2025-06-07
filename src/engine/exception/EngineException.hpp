#ifndef RRENDERER_ENGINE_EXCEPTION_ENGINE_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTION_ENGINE_EXCEPTION_HPP

#include "spdlog/spdlog.h"

#include <exception>
#include <source_location>
#include <string>
#include <utility>

namespace rr
{

class EngineException : public std::exception
{
public:
    explicit EngineException(std::string msg) : message(std::move(msg))
    {}

    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }

private:
    std::string message;
};

template<typename ExceptionType, typename... Args>
[[noreturn]] void throwWithLog(std::source_location loc, Args&&... args)
{
    spdlog::error("[{}:{}:{}] throwing {}\n", loc.file_name(), loc.function_name(), loc.line(), typeid(ExceptionType).name());
    throw ExceptionType(std::forward<Args>(args)...);
}

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTION_ENGINE_EXCEPTION_HPP
