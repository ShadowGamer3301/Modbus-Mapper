#include "Exception.h"
#include "mb_client.h"
#include <ctime>
#include <map>
#include "mb_reading.h"

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
    void SetReadingClient(std::string name);
    std::string TimeToString(std::time_t t);

  private:
    bool mQuit = false;
    std::string mReadingClientName;
    std::map<std::string, modbus_t*> mClientMap;
    std::vector<Reading> vecReadings;
  };
} 
