#include "loguru.hpp"
#include "ini.h"
#include "Exception.h"
#include "mb_cmdline.h"
#include <iostream>
#include <string>

int main(void) try
{
	LOG_F(INFO, "Modbus Mapper ver 1.0");
  LOG_F(INFO, "By Zbigniew Przybyła (c), published under BSD-3 License");
  LOG_F(INFO, "Using Loguru by emilk, mINI by metayeti:");

  LOG_F(INFO, "Looking for config.ini...");

  mINI::INIFile fcfg("config.ini");
  mINI::INIStructure cfgStruct;

  if(!fcfg.read(cfgStruct))
  {
    LOG_F(ERROR, "Failed to load config file");
    return 1;
  }

  if(std::stoi(cfgStruct["NETWORK"]["MB_TYPE"]) == 0)
  {
    LOG_F(INFO, "Starting Modbus-Mapper in client mode");
    Mapper::MB_CmdLine cmdline;
    while(true)
    {
      std::string command = "";
      std::cout << "MB_cmd > ";
      std::getline(std::cin, command);
      cmdline.HandleCommand(command);
    }
  }

	return 0;
} catch(const Mapper::Exception& e)
{
  printf("%s", e.what());
  return 1;
}