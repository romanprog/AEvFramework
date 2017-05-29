#include <iostream>
#include <string>
#include "AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "ServerCore/Listeners.hpp"
#include "AsyncEvent/AEvBase/AEventTypes.hpp"
#include "Config/GlobalConf.hpp"

auto cb = [](aev::AEvPtrBase tptr, aev::AEvExitSignal sin_num) -> int
{
    log_debug("Finising main async event.");
    return 0;
};

int main () {

    std::cout << "Helo World!!" << std::endl;
    // aev::AEventUtilBase ut;
    if (!Config::glob().read_config("main.conf")) {
        log_main("Can't load config file. %", Config::glob().error_text());
        exit(0);
    }

    aev::AEvRootConf main_conf(cb);
    std::shared_ptr<ListenerBase<asio::ip::tcp>> conn = std::make_shared<ListenerBase<asio::ip::tcp>>(main_conf, "127.0.0.1", 8888);
    conn->begin();
    conn->run();

}
