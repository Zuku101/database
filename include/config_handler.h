#pragma once
#include <string>
#include <map>

class ConfigHandler {
public:
    static ConfigHandler& getInstance();
    void loadConfig(const std::string& filepath);
    const std::string& getComponentIdentifier(const std::string& component) const;

private:
    ConfigHandler() = default;
    std::map<std::string, std::string> components;
    void validateConfig();
};