#include "udp_client.hpp"

UdpClient::UdpClient(const char* host, const char* port)
    : resolver_(io_context_),
      socket_(io_context_)
{
    endpoints_ = resolver_.resolve(host, port);
    socket_.open(boost::asio::ip::udp::v4());
}

void UdpClient::run() {
    while (true) {
        TelemetryData data = input_telemetry_data();
        uint64_t packet = pack_data(data);
        
        send_udp_packet(packet);
        std::array<uint8_t, 2> response = receive_udp_response();
        analyse_server_response(response);

        std::cout << "Continue? (y/n): ";
        std::string answer;
        std::getline(std::cin, answer);
        
        if (answer == "n" || answer == "N") {
            std::cout << "Exiting client\n";
            break;
        }
    }
}
template <typename T>
void UdpClient::validate_input(T& value_ref, T min, T max, const std::string& field){
    using ValueType = std::decay_t<decltype(value_ref)>;
        
    while (true) {
        std::cout << "Enter " << field << " [" << min << "-" << max << "]: ";
        std::string input_line;
        if (!std::getline(std::cin, input_line)) {
            if (std::cin.eof()) {
                std::cout << "Error: End of input reached. Exiting.\n";
                exit(1);
            }
            std::cout << "Error reading input. Please try again.\n";
            continue;
        }
        try {
            size_t pos;
            long value = std::stol(input_line, &pos);
            if (pos != input_line.size()) {
                std::cout << "Invalid input: extra characters after number. Please try again.\n";
                continue;
            }
            value_ref = static_cast<T>(value);
        }
        catch (...) {
            std::cout << "Invalid input: not a valid number. Please try again.\n";
            continue;
        }

        if (value_ref < min || value_ref > max) {
            std::cout << "Error: Value must be between " << min << " and " << max << ".\n";
            continue;
        }
        break;
    }
}

TelemetryData UdpClient::input_telemetry_data() {
    TelemetryData data{};
    int tmp_int; 
    float tmp_float;
    
    validate_input(tmp_int, 0, 63, "X");
    data.x = tmp_int;
    
    validate_input(tmp_int, -32, 31, "Y");
    data.y = tmp_int + 32;
    
    validate_input(tmp_int, 0, 255, "Velocity");
    data.velocity = tmp_int;
    
    validate_input(tmp_int, 0, 3, "Mode");
    data.mode = tmp_int;
    
    validate_input(tmp_int, 0, 3, "State");
    data.state = tmp_int;
    
    validate_input(tmp_float, -12.7f, 12.8f, "Acceleration");
    data.acceleration = static_cast<uint8_t>((tmp_float + 12.7f) * 10);
    
    validate_input(tmp_int, 0, 130, "Power");
    data.power = tmp_int;
    
    return data;
}

void UdpClient::send_udp_packet(uint64_t data) {
    if (endpoints_.empty()) {
        throw std::runtime_error("No endpoints available for sending");
    }
    uint64_t net_data = host_to_network(std::move(data));
    
    const auto endpoint = endpoints_.begin()->endpoint();
    
    const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(&net_data);
    
    socket_.send_to(boost::asio::buffer(&net_data, sizeof(net_data)), endpoint);
}

void UdpClient::analyse_server_response(std::array<uint8_t, 2> response) {
    switch (response[0]) {
        case 0:
            if (response[1])  std::cout << "Data is valid\n";
            else std::cout << "Data is invalid\n";  
            break;
            
        case 1:
            std::cout << "Unpack_failed or invalid packet size\n";
            break;
            
        case 2:
            std::cout << "Server error\n";
            break;

        default: 
            std::cout << "Error\n";
            break;
    }
}

std::array<uint8_t, 2> UdpClient::receive_udp_response() {
    std::array<uint8_t,2> response = {0};
    boost::system::error_code ec;
    
    size_t length = socket_.receive_from(
        boost::asio::buffer(response, sizeof(response)), 
        server_endpoint_,
        0,
        ec
    );

    if (ec) {
        std::cout << "  Receive error: " << ec.message() << "\n";
    }
    return response;
}