#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/spawn.hpp>
#include <chrono>
int main() {

    boost::asio::io_context io(BOOST_ASIO_CONCURRENCY_HINT_UNSAFE);

    boost::asio::spawn(io, [&](boost::asio::yield_context yield) {
        boost::asio::ip::tcp::socket peer(io);
        boost::system::error_code ec;
        char buff[1024];
        auto connect_ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1111);
        peer.open(connect_ep.protocol());
        peer.async_connect(connect_ep, yield[ec]);
        auto t1 = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 1000000; ++i) {
            auto res = 0;
            res = boost::asio::async_write(peer, boost::asio::buffer(buff, 128 + 8), yield[ec]);
            res = boost::asio::async_read(peer, boost::asio::buffer(buff, 8), yield[ec]);
            res = boost::asio::async_read(peer, boost::asio::buffer(buff, 128), yield[ec]);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        printf("requests: %zu  time used: %lf\n", 1000000 * 1, duration / double(1000));
        std::cout << "QPS:" << (1000000 * 1) / double(duration / double(1000))  << "\n";
        fflush(stdout);
    });

    io.run();

    return 0;
}
