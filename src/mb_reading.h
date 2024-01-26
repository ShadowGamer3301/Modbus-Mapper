#pragma once
#include <chrono>
#include <ctime>

namespace Mapper
{
  inline time_t GetCurrentTime()
  {
    auto start = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(start);
  }

  struct Reading
  {
    time_t readTime = GetCurrentTime();
    float readVal = 0.0f;
  };
}
