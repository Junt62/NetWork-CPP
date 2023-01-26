#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

int main() {

    boost::system::error_code ec;

    boost::asio::io_context context;

    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::make_address("101.37.161.19", ec), 80);

    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec) {
        std::cout << "Connected!" << std::endl;
    }
    else {
        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    }

    if (socket.is_open()) {
        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()), ec);

        socket.wait(socket.wait_read);

        size_t bytes = socket.available();
        std::cout << "Bytes Available: " << bytes << std::endl;

        if (bytes > 0) {
            std::vector<char> vBuffer(bytes);
            socket.read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()), ec);

            for (auto c : vBuffer)
                std::cout << c;
        }
    }

    return 0;
}