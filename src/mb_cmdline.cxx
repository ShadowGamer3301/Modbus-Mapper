#include "mb_cmdline.h"
#include "loguru.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <utility>

using namespace Mapper;

void MB_CmdLine::HandleCommand(std::string cmd)
{
  LOG_F(INFO, "Passed instruction is: %s", cmd.c_str());

  //Since every instructions has length of five characters
  //Compare first five characters to existing instructions
  //But first check if command is even longer than five characters
  if(cmd.length() < 5)
  {
    LOG_F(ERROR, "Invalid instruction");
    return;
  }

  std::string instruction;

  for(int i = 0; i < 5; i++)
  {
    instruction += cmd[i];
  }

  if(strcmp(instruction.c_str(), "mkclt") == 0)
  {
    std::string addr;
    std::string port;
    std::string name;
    int ComaCounter = 0;
    int lastComaIndex = 5;

  cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](unsigned char x) {return std::isspace(x);}), cmd.end());

    LOG_F(INFO, "Instruction is: %s", cmd.c_str());
    for(int i = 5; i <= cmd.length(); i++)
    {
      if(cmd[i] == ',' || i == cmd.length())
      {
        switch (ComaCounter) {
        case 0:
          for(int j = lastComaIndex; j < i; j++)
            addr += cmd[j];
          break;

        case 1:
          for(int j = lastComaIndex + 1; j < i; j++)
            port += cmd[j];
          break;

        case 2:
          for(int j = lastComaIndex + 1; j < i; j++)
            name += cmd[j];
          break;
        default:
          LOG_F(WARNING, "Unusable data in mkclt instruction (Too many information)");
        }

        lastComaIndex = i;
        ComaCounter++;
      }
    }
    LOG_F(INFO, "Creating %s client (IP: %s, PORT: %s)", name.c_str(), addr.c_str(), port.c_str());
    CreateClient(addr, std::stoi(port), name);
    return;
  }

  else if(strcmp(instruction.c_str(), "frclt") == 0)
  {
    cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](unsigned char x) {return std::isspace(x);}), cmd.end());
    std::string name;
    for(int i = 5; i < cmd.length(); i++)
    {
      name += cmd[i];
    }
    FreeClient(name);
  }

  else if(strcmp(instruction.c_str(), "rdmem") == 0)
  {}

  else {
    LOG_F(ERROR, "%s is not recognized as a valid instruction", instruction.c_str());
  }

  return;
}

MB_CmdLine::~MB_CmdLine()
{
  mClientMap.clear();
}

void MB_CmdLine::CreateClient(std::string ip, int port, std::string name)
{
  mClientMap.emplace(std::make_pair(name, modbus_new_tcp(ip.c_str(), port)));
}

void MB_CmdLine::FreeClient(std::string name)
{
  if(auto search = mClientMap.find(name); search != mClientMap.end())
  {
    LOG_F(INFO, "Removing client %s", name.c_str());
    modbus_close(search->second);
    modbus_free(search->second);
    LOG_F(INFO, "Client %s deleted", name.c_str());
  }
  else 
  {
    LOG_F(ERROR, "Cannot find %s client", name.c_str());
  }
  return;
}
