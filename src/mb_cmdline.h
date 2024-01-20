#include "Exception.h"
#include "mb_client.h"
#include <map>

namespace Mapper
{
  class MB_CmdLine
  {
  public:
    MB_CmdLine() = default;
    ~MB_CmdLine();

    void HandleCommand(std::string cmd);

  private:
    void CreateClient(std::string ip, int port, std::string name);
    void FreeClient(std::string name);
    void ReadMemory(int addr, int regs);

  private:
    std::map<std::string, modbus_t*> mClientMap;
  };
} 
