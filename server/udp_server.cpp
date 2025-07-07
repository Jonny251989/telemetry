#include "udp_server.hpp"

UdpServer::UdpServer(boost::asio::io_context& io_context, uint16_t port, const std::string& config_path)
    : socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      validator_(config_path) {
    start_receive();
}

void UdpServer::start_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(&recv_buffer_, sizeof(recv_buffer_)),
        remote_endpoint_,
        [this](const boost::system::error_code& error, size_t bytes) {
            handle_receive(error, bytes);
        }
    );
}

void UdpServer::send_response(const std::array<uint8_t, 2>& response) {
    socket_.async_send_to(
        boost::asio::buffer(response),
        remote_endpoint_,
        [this, response](const boost::system::error_code& error, size_t) {
            start_receive();
        }
    );
}

void UdpServer::handle_receive(const boost::system::error_code& error, size_t bytes) {
    if (error) {
        std::cerr << "Receive error: " << error.message() << "\n";
        send_response({2,0});
        return;
    }
    uint64_t host_data = network_to_host(std::move(recv_buffer_));
    std::optional<TelemetryData> data_opt = unpack_data(host_data);

    if (!data_opt) {
        std::cerr << "Packet unpacking failed\n";
        send_response({1,0});
        return;
    }

    if (bool is_valid = validator_.validate(*data_opt); is_valid){
        send_response({0,1});
    } else {
        send_response({0,0});
    }
}