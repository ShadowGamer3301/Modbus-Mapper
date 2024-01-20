#pragma once
#include <source_location>
#include <exception>
#include <string>
#include <cstdint>

namespace Mapper
{
  class Exception : public std::exception
  {
  public:
    Exception(std::source_location loc = std::source_location::current());
    const char* what() const noexcept override;

  protected:
    uint32_t line;
    std::string func, file;
    mutable std::string wBuffer;
  };
}
