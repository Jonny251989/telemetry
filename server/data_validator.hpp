#pragma once

#include <json.hpp>
//#include <nlohmann/json.hpp>
#include "protocol.hpp"
#include <string>
#include <utility>
#include <fstream>
#include <stdexcept>
#include <iostream>

class DataValidator {
public:
    explicit DataValidator(const std::string& config_path);
    bool validate(const TelemetryData& data);

private:
    nlohmann::json limits;
};