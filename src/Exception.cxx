#include "Exception.h"
#include <source_location>
#include <sstream>

using namespace Mapper;

Mapper::Exception::Exception(std::source_location loc)
//Gather all data that is necessary for debugging
  : line(loc.line()), file(loc.file_name()), func(loc.function_name())
{}

const char* Exception::what() const noexcept
{
//Display info about exception
  std::ostringstream oss;
  oss << "Exception caught!\n"
      << "File: " << file << "\n"
      << "Func: " << func << "\n"
      << "Line: " << line << "\n";

  wBuffer = oss.str();
  return wBuffer.c_str();
}

