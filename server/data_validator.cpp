#include "data_validator.hpp"

using json = nlohmann::json;

DataValidator::DataValidator(const std::string& config_path) {
    std::ifstream f(config_path);   
    if (!f) {
        throw std::runtime_error("Cannot open limits file: " + config_path);
    }
    
    try {
        json config;
        f >> config;
        
        limits = config["limits"][0];
        
        std::cout << "Loaded validation limits:\n";
        for (auto& [key, val] : limits.items()) {
            std::cout << "  " << key << ": [" << val[0] << ", " << val[1] << "]\n";
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing limits file: " + std::string(e.what()));
    }
}

bool DataValidator::validate(const TelemetryData& data) {
    bool valid = true;
    
    int original_y = static_cast<int>(data.y) - 32;
    float original_acceleration = static_cast<float>(data.acceleration) / 10.0f - 12.7f;
    
    auto check = [&](auto value, const std::string& field) {

        using ValueType = decltype(value);
        try {
            const ValueType min = limits[field][0].get<ValueType>();
            const ValueType max = limits[field][1].get<ValueType>();

            if (value < min || value > max) {
                std::cout << " Error " << field << " out of range: " 
                          << std::fixed << std::setprecision(2) << value
                          << " not in [" << min << ", " << max << "]\n";
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "  ! Error: type mismatch for field '" << field << "': " << e.what() << "\n";
            return false;
        }
        return true;
    };

    try {
        valid &= check(data.x, "X");
        valid &= check(original_y, "Y");
        valid &= check(data.velocity, "V");
        valid &= check(data.mode, "M");
        valid &= check(data.state, "S");
        valid &= check(original_acceleration, "A");
        valid &= check(data.power, "P");
    } catch (const std::exception& e) {
        std::cerr << "Validation error: " << e.what() << "\n";
        return false;
    }
    
    return valid;
}