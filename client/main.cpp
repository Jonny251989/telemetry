#include "udp_client.hpp"
#include "protocol.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <clocale>
#include <stdexcept>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "C");
    try {
        if (argc != 3) {
            std::cout << "Usage: " << argv[0] << " <host> <port>\n";
            return 1;
        }
        UdpClient client(argv[1], argv[2]);
        client.run();
        
    } catch (const std::exception& e) {
        std::cout << "Client exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}