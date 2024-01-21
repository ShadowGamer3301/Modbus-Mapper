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
    inline bool ShouldQuit() const noexcept { return mQuit; }

  private:
    void CreateClient(std::string ip, int port, std::string name);
    void FreeClient(std::string name);
    void ReadMemory(std::string name, int addr, int regs);
    void ConnectClient(std::string name);

  private:
    bool mQuit = false;
    std::map<std::string, modbus_t*> mClientMap;
  };
} 
