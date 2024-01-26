#include "Window.h"
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

    if(std::stoi(cfgStruct["GENERAL"]["MB_GUI"]) == 0)
    {
      Mapper::MB_CmdLine cmdline;
      while(!cmdline.ShouldQuit())
      {
        std::string command = "";
        std::cout << "MB_cmd > ";
        std::getline(std::cin, command);
        cmdline.HandleCommand(command);
      }
    }
    else {
      LOG_F(INFO, "Creating modbus GUI");
      Mapper::Window wnd("Modbus-Mapper", 800, 600);
      while(!wnd.ProcessEvent())
      {

      }
    }
  }

	return 0;
} catch(const Mapper::Exception& e)
{
  printf("%s", e.what());
  return 1;
}
