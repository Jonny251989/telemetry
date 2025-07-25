#include "udp_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <csignal>

std::function<void()> shutdown_handler;
void signal_handler(int) {
    if (shutdown_handler) shutdown_handler();
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <port> <config_path>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        auto server = std::make_shared<UdpServer>(
            io_context, 
            static_cast<uint16_t>(std::stoi(argv[1])), 
            argv[2]
        );
        
        server->start();
        
        // Обработка сигналов
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](const auto&, int) {
            std::cout << "Stopping server..." << std::endl;
            server->stop();
            io_context.stop();
        });
        
        // Запускаем IO контекст в нескольких потоках
        const size_t num_threads = std::max(1u, std::thread::hardware_concurrency() / 2);
        std::vector<std::thread> threads;
        for(size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([&io_context]() {
                io_context.run();
            });
        }
        
        std::cout << "Server running. Press Ctrl+C to exit." << std::endl;
        
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Server exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}