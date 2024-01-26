#pragma once
#include <ctime>
#include <string>

namespace Mapper {
  struct Reading
  {
    float recValue;
    std::string recTime;
  };

  inline std::string TimeToString(time_t t)
  {
    std::tm* ptm = std::localtime(&t);
    char buffer[32];
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M.%S", ptm);

    return buffer;
  }
}
