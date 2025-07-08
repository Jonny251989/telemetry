#include "udp_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: server <port> <config_path>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        UdpServer server(io_context, 
                         static_cast<uint16_t>(std::stoi(argv[1])), 
                         argv[2]);
        
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait(
            [&](const boost::system::error_code&, int signo) {
                std::cout << "\nReceived signal " << signo 
                          << ", stopping server..." << std::endl;
                          
                server.stop();
                
                if (!io_context.stopped()) {
                    io_context.stop();
                }
            }
        );

        std::cout << "Server started on port " << argv[1] << std::endl;
        std::cout << "Press Ctrl+C to exit..." << std::endl;
        
        io_context.run();
        
        std::cout << "Server stopped gracefully" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
    