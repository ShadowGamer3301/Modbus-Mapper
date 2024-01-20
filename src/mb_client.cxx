#include "mb_client.h"
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>

using namespace Mapper;

MB_Client::MB_Client()
{
 pModbus = modbus_new_tcp("", 0);
}
  

