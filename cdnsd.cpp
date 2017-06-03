#include <iostream>
#include <string>
#include "AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "ServerCore/Listeners.hpp"
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
    // aev::AEventUtilBase ut;
    DnsReadBufferT buff;
    buff.release(5000);
    for (int i = 0; i < 400; ++i ) {
        log_main("% %",buff.size_avail(), i);
        memcpy(buff.data_top(),"1234567890", 10);
        buff.accept(10);

    }

    buff.release(7000);

    exit(0);

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

    if (!Config::glob().read_config("main.conf")) {
        log_main("Can't load config file. %", Config::glob().error_text());
        exit(0);
    }

    aev::AEvRootConf main_conf(cb);
    std::shared_ptr<ListenerTCP> conn = std::make_shared<ListenerTCP>(main_conf, "0.0.0.0", 5338);
    conn->begin();
    conn->run();

}
