#include "udp_server.hpp"
#include <iostream>

UdpServer::UdpServer(boost::asio::io_context& io_context,
                     uint16_t port,
                     const std::string& config_path,
                     size_t thread_pool_size)
    : io_context_(io_context),
      socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      validator_(config_path),
      thread_pool_(std::make_unique<ThreadPool>(thread_pool_size)) 
{
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Using thread pool with " << thread_pool_size << " workers\n";
}

void UdpServer::start() {
    start_receive();
}

void UdpServer::start_receive() {
    if (stopped_) return;
    
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](const boost::system::error_code& error, size_t bytes) {
            handle_receive(error, bytes);
        });
}

void UdpServer::handle_receive(const boost::system::error_code& error, size_t bytes) {
    if (error) {
        if (error != boost::asio::error::operation_aborted) {
            std::cerr << "Receive error: " << error.message() << std::endl;
        }
        return;
    }
    
    // Создаем контекст для обработки
    auto context = std::make_shared<PacketContext>();
    context->endpoint = remote_endpoint_;
    context->length = bytes;
    std::copy_n(recv_buffer_.begin(), bytes, context->buffer.begin());
    
    thread_pool_->enqueue([this, context]() {
        process_packet(context);
    });
    
    start_receive();
}

void UdpServer::process_packet(std::shared_ptr<PacketContext> context) {
    try {
        if (context->length != sizeof(uint64_t)) {
            context->response = {1, 0};
            return;
        }
        
        uint64_t packet_data;
        std::memcpy(&packet_data, context->buffer.data(), sizeof(packet_data));
        packet_data = network_to_host(std::move(packet_data));
        
        auto telemetry = unpack_data(packet_data);
        if (!telemetry) {
            context->response = {1, 0}; // Ошибка распаковки
            return;
        }
        
        auto valid = validator_.validate(*telemetry);
        context->response = {0, static_cast<uint8_t>(valid ? 1 : 0)};
        
        // Планируем отправку ответа через IO контекст
        boost::asio::post(io_context_, [this, context]() {
            send_response(context);
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Packet processing error: " << e.what() << std::endl;
        context->response = {2, 0}; // Ошибка сервера
        boost::asio::post(io_context_, [this, context]() {
            send_response(context);
        });
    }
}

void UdpServer::send_response(std::shared_ptr<PacketContext> context) {
    socket_.async_send_to(
        boost::asio::buffer(context->response),
        context->endpoint,
        [](const boost::system::error_code& error, size_t /*bytes*/) {
            if (error) {
                std::cerr << "Send error: " << error.message() << std::endl;
            }
        });
}

void UdpServer::stop() {
    stopped_ = true;
    socket_.close();
    thread_pool_->wait_completion();
}