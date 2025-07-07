#pragma once

#include "network_utils.hpp"
#include <boost/asio.hpp>
#include "protocol.hpp"
#include <iomanip>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <array>

class UdpClient {
public:
    UdpClient(const char* host, const char* port);
    void run();
    
private:
    void send_udp_packet(uint64_t data);

    template <typename T>
    void validate_input(T& value_ref, T min, T max, const std::string& field);

    TelemetryData input_telemetry_data();
    void analyse_server_response(std::array<uint8_t, 2> response);
    std::array<uint8_t, 2> receive_udp_response();
    
    boost::asio::io_context io_context_;
    boost::asio::ip::udp::resolver resolver_;
    boost::asio::ip::udp::resolver::results_type endpoints_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
};