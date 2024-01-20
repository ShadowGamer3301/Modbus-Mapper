#include "mb_client.h"
#include "Exception.h"
#include "ini.h"
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <string>
#include "loguru.hpp"

using namespace Mapper;

MB_Client::MB_Client()
{
  //Resize vector
  vecData.resize(sizeof(uint16_t) * 100);

  //Read config file
  mINI::INIFile cfile("config.ini");
  mINI::INIStructure cfgStruct;

  if(!cfile.read(cfgStruct))
  {
    LOG_F(ERROR, "Failed to load configuration file");
    throw Mapper::Exception();
  }

  std::string ip_addr = cfgStruct["NETWORK"]["MB_IP"];
  unsigned int port = std::stol(cfgStruct["NETWORK"]["MB_PORT"]);

  LOG_F(INFO, "Master server set to %s on port %d", ip_addr.c_str(), port);
  pModbus = modbus_new_tcp(ip_addr.c_str(), port);

  if(pModbus == nullptr)
  {
    LOG_F(ERROR, "Failed to create modbus-tcp context");
    throw Mapper::Exception();
  }
}
  
MB_Client::~MB_Client()
{
  vecData.clear();
  modbus_close(pModbus);
  modbus_free(pModbus);
}

bool MB_Client::EstablishConnection()
{
  if(modbus_connect(pModbus) == -1) {
    LOG_F(ERROR, "Connection to master server failed");
    return false;
  }

  LOG_F(INFO, "Connection to master server established");
  return true;
}

void MB_Client::ReadMemory(int addr, int regs)
{
  int rc = modbus_read_registers(pModbus, addr, regs, vecData.data());
  if(rc == -1)
  {
    LOG_F(ERROR, "Failed to read data");
  }

  float result = 0.0f;

  for(int i = 0; i < rc; i++)
  {
    //printf("reg[%d]=%d (0x%X)\n", i, vecData[i], vecData[i]);
    result += modbus_get_float_abcd(&vecData[i]);
  }

  printf("Result is: %f\n", result);
}
