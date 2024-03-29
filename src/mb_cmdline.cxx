#include "mb_cmdline.h"
#include "Reading.h"
#include "loguru.hpp"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <ios>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <utility>
#include "csv2/writer.hpp"
#include "csv2/parameters.hpp"
#include "csv2/mio.hpp"

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
  {
    std::string name;
    std::string addr;
    std::string regs;
    int ComaCounter = 0;
    int lastComaIndex = 5;

    cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](unsigned char x) {return std::isspace(x);}), cmd.end());
    for(int i = 5; i <= cmd.length(); i++)
    {
      if(cmd[i] == ',' || i == cmd.length())
      {
        switch (ComaCounter) {
        case 0:
          for(int j = lastComaIndex; j < i; j++)
            name += cmd[j];
          break;
        case 1:
          for(int j = lastComaIndex + 1; j < i; j++)
            addr += cmd[j];
          break;
        case 2:
          for(int j = lastComaIndex + 1; j < i; j++)
            regs += cmd[j];
          break;
        dedefault:
          LOG_F(WARNING, "Unusable data in rdmem instruction (Too many information)");
        }
        lastComaIndex = i;
        ComaCounter++;
      }
    }
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << addr;
    ss >> x;
    ReadMemory(name, x, std::stoi(regs));
    return;
  }

  else if (strcmp(instruction.c_str(), "endmm") == 0) {
    mQuit = true;
    return;
  }

  else if (strcmp(instruction.c_str(), "store") == 0)
  {
    vecReadings.clear();
    mStoreReadings = true;
    LOG_F(WARNING, "Storage option enabled");
  }

  else if (strcmp(instruction.c_str(), "tocsv") == 0)
  {
    std::string outName;
    cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](unsigned char x) {return std::isspace(x);}), cmd.end());
    for(int i = 5; i < cmd.length(); i++)
    {
      outName += cmd[i];
    }

    LOG_F(WARNING, "Outputing readings to %s", outName.c_str());
    std::vector<std::vector<std::string>> rows;
    std::vector<std::string> row;
    row.push_back("Value");
    row.push_back("Time");
    rows.push_back(row);

    for(int i = 0; i < vecReadings.size(); i++)
    {
      row.clear();
      row.push_back(std::to_string(vecReadings[i].recValue));
      row.push_back(vecReadings[i].recTime);
      rows.push_back(row);
    }
    std::ofstream stream(outName);
    csv2::Writer<csv2::delimiter<';'>> writer(stream);
    writer.write_rows(rows);
  }
  else if (strcmp(instruction.c_str(), "rdenl") == 0)
  {
    std::string name, addr, regs;
    int ComaCounter = 0, lastComaIndex = 5;
    cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](unsigned char x) {return std::isspace(x);}), cmd.end());
    for(int i = 5; i <= cmd.length(); i++)
    {
      if(cmd[i] == ',' || i == cmd.length())
      {
        switch (ComaCounter) {
          case 0:
            for(int j = lastComaIndex; j < i; j++)
              name += cmd[j];
            break;
          case 1:
            for(int j = lastComaIndex + 1; j < i; j++)
              addr += cmd[j];
            break;
          case 2:
            for(int j = lastComaIndex + 1; j < i; j++)
              regs += cmd[j];
            break;
        }
        ComaCounter++;
        lastComaIndex = i;
      }
    }
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << addr;
    ss >> x;
    HandleCommand("store");
    LOG_F(WARNING, "Regs is: %s", regs.c_str());
    while(true)
    {
      for(int i = 0; i < 60; i++)
      {
        ReadMemory(name, x, std::stoi(regs));
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(60));
      }
      HandleCommand("tocsv output.csv");
    }
  }
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
  for(auto client : mClientMap)
  {
    if(strcmp(name.c_str(), client.first.c_str()) == 0)
    {
      LOG_F(ERROR, "Cannot create client, name no uniqe!");
      return;
    }
  }

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

void MB_CmdLine::ConnectClient(std::string name)
{
  if(auto search = mClientMap.find(name); search != mClientMap.end())
  {
    LOG_F(INFO, "Connecting %s client to master server", name.c_str());
    if(modbus_connect(search->second) == -1)
    {
      LOG_F(ERROR, "Failed to connect to master server");
      return;
    }

    LOG_F(INFO, "Connection established");
  }
}

void MB_CmdLine::ReadMemory(std::string name, int addr, int regs)
{
  ConnectClient(name);
  if(auto search = mClientMap.find(name); search != mClientMap.end())
  {
    LOG_F(INFO, "Reading memory from %x addr", addr);
    std::vector<uint16_t> vecData;
    vecData.resize(sizeof(uint16_t) * regs);
    int rc = modbus_read_registers(search->second, addr, regs, vecData.data());
    if(rc == -1)
    {
      LOG_F(ERROR, "Cannot read desired memory address");
      return;
    }

    float result = 0.0f;
    for(int i = 0; i < rc; i++)
    {
      result += modbus_get_float_abcd(&vecData[i]);
    }

    if(mStoreReadings)
    {
      time_t now = std::time(0);
      Reading rd;
      rd.recTime = Mapper::TimeToString(now);
      rd.recValue = result;
      vecReadings.push_back(rd);
      LOG_F(INFO, "Value %f stored (TIME: %s)", rd.recValue, rd.recTime.c_str());
    }
    else {
      LOG_F(INFO, "Value is: %f", result);
    }
  }
}
