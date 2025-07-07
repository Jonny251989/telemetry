#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <sys/wait.h>
#include <boost/asio.hpp>

class FunctionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        wait_for_server("telemetry_server", 12345);
    }

    void wait_for_server(const std::string& host, uint16_t port) {
        using namespace boost::asio;
        using ip::udp;
        
        io_context io;
        udp::resolver resolver(io);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), host, std::to_string(port));
        
        for (int i = 0; i < 30; ++i) {
            try {
                udp::socket socket(io);
                socket.connect(*endpoints.begin());
                return;
            } catch (const boost::system::system_error& e) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        throw std::runtime_error("Server not available after 30 attempts");
    }

    std::string run_client_test(const std::string& test_name) {
        std::string command = "/test_data/run_client.sh " + test_name;
        int status = system(command.c_str());
        
        std::string output_file = "/test_data/" + test_name + "_output.txt";
        std::ifstream file(output_file);
        if (!file) return "ERROR: Cannot open output file";
        
        return std::string((std::istreambuf_iterator<char>(file)), 
                          std::istreambuf_iterator<char>());
    }
};

TEST_F(FunctionalTest, ValidData) {
    const std::string output = run_client_test("valid_data");
    EXPECT_NE(output.find("Data is valid"), std::string::npos)
        << "Client output:\n" << output;
}

TEST_F(FunctionalTest, InvalidData) {
    const std::string output = run_client_test("invalid_data");
    EXPECT_NE(output.find("Data is invalid"), std::string::npos)
        << "Client output:\n" << output;
}