#pragma once
#include <cstdint>
#include <modbus/modbus-rtu.h>
#include <vector>

namespace Mapper
{
  class MB_Client
  {
  public:
    MB_Client();
    ~MB_Client();

    bool EstablishConnection();
    void ReadMemory(int addr, int regs);

  private:
    modbus_t* pModbus = nullptr;
    std::vector<uint16_t> vecData; //Vector for holding data
  };
}
