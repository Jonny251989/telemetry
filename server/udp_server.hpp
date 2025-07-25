#pragma once
#include "data_validator.hpp"
#include "network_utils.hpp"
#include "protocol.hpp"
#include "thread_pool.hpp"
#include <boost/asio.hpp>
#include <array>
#include <memory>
#include <atomic>
#include <iostream>

class UdpServer {
public:
    UdpServer(boost::asio::io_context& io_context, 
              uint16_t port, 
              const std::string& config_path,
              size_t thread_pool_size = std::thread::hardware_concurrency());
              
    void start();
    void stop();
    
private:
    struct PacketContext {
        std::array<uint8_t, 1024> buffer;
        size_t length;
        boost::asio::ip::udp::endpoint endpoint;
        std::array<uint8_t, 2> response;
    };
    
    void start_receive();
    void handle_receive(const boost::system::error_code& error, size_t bytes);
    void process_packet(std::shared_ptr<PacketContext> context);
    void send_response(std::shared_ptr<PacketContext> context);
    
    boost::asio::io_context& io_context_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<uint8_t, 1024> recv_buffer_;
    
    DataValidator validator_;
    std::unique_ptr<ThreadPool> thread_pool_;
    std::atomic<bool> stopped_{false};
};