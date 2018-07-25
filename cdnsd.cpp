#include <iostream>
#include <string>
#include "AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "ServerCore/Controller.hpp"
#include "AsyncEvent/AEvBase/AEventTypes.hpp"
#include "Config/GlobalConf.hpp"
#include "TmpForTests.hpp"


auto cb = [](aev::AEvPtrBase tptr, aev::AEvExitSignal sin_num) -> int
{
    log_debug("Finising main async event.");
    return 0;
};


int main (int argc, char* argv[]) {


    std::cout << "Helo World!!" << std::endl;
#if 0
    try
    {
      if (argc != 2)
      {
        std::cerr << "Usage: async_udp_echo_server <port>\n";
        return 1;
      }

      asio::io_service io_service;

      server s(io_service, std::atoi(argv[1]));

      io_service.run();
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
#endif

    if (!Config::glob().read_config("main.conf")) {
        log_main("Can't load config file. %", Config::glob().error_text());
        exit(0);
    }

    aev::AEvRootConf main_conf(cb);
    std::shared_ptr<Controller> conn = std::make_shared<Controller>(main_conf);
    conn->begin();
    conn->run();

}
