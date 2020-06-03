#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/spawn.hpp>
int main() {

    boost::asio::io_context io(BOOST_ASIO_CONCURRENCY_HINT_UNSAFE);

    boost::asio::ip::tcp::acceptor acceptor(io);

    boost::asio::spawn(io, [&](boost::asio::yield_context yield) {
        auto bind_ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1111);
        acceptor.open(bind_ep.protocol());
        acceptor.bind(bind_ep);
        acceptor.listen();
        while(1) {
            boost::system::error_code ec;
            auto peer = std::make_unique<boost::asio::ip::tcp::socket>(io);
            acceptor.async_accept(*peer, yield[ec]);
            boost::asio::spawn(io, [peer = std::move(peer)](boost::asio::yield_context yield) {
                boost::system::error_code ec;
                char buff[1024];
                while(1) {
                    boost::asio::async_read(*peer, boost::asio::buffer(buff, 8), yield[ec]);
                    boost::asio::detail::throw_error(ec, "async_read 1");
                    boost::asio::async_read(*peer, boost::asio::buffer(buff, 128), yield[ec]);
                    boost::asio::detail::throw_error(ec, "async_read 2");
                    boost::asio::async_write(*peer, boost::asio::buffer(buff, 128 + 8), yield[ec]);
                    boost::asio::detail::throw_error(ec, "async_write 3");
                }
            });
        }
    });

    io.run();

    return 0;
}
