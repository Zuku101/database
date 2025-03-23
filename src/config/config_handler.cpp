#include "config_handler.h"
#include <fstream>
#include <vector>
#include <stdexcept>

ConfigHandler& ConfigHandler::getInstance() {
    static ConfigHandler instance;
    return instance;
}

void ConfigHandler::loadConfig(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filepath);
    }

    components.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            components[key] = value;
        }
    }

    validateConfig();
}

const std::string& ConfigHandler::getComponentIdentifier(const std::string& component) const {
    auto it = components.find(component);
    if (it == components.end()) {
        throw std::runtime_error("Component not found in config: " + component);
    }
    return it->second;
}

void ConfigHandler::validateConfig() {
    std::vector<std::string> required = {"CPU", "GPU", "MOTHERBOARD", "CHIP"};
    for (const auto& comp : required) {
        if (components.find(comp) == components.end() || components[comp].empty()) {
            throw std::runtime_error("Missing or empty configuration for: " + comp);
        }
    }
}