#pragma once
#include <string>
#include <map>

/**
 * @brief Handles loading and managing component configuration data.
 */
class ConfigHandler {
public:
    /**
     * @brief Gets singleton instance of ConfigHandler.
     *
     * @return ConfigHandler&
     *   Reference to the singleton instance
     */
    static ConfigHandler& getInstance();

    /**
     * @brief Loads component identifiers from config file.
     *
     * @param filepath
     *   Path to the configuration file
     *
     * @throws runtime_error
     *   If config file cannot be opened or is invalid
     */
    void loadConfig(const std::string& filepath);

    /**
     * @brief Gets identifier string for specified component.
     *
     * @param component
     *   Component name (CPU/GPU/MOTHERBOARD/CHIP)
     *
     * @return string
     *   Component identifier string from config
     *
     * @throws runtime_error
     *   If component is not found in config
     */
    const std::string& getComponentIdentifier(const std::string& component) const;

private:
    /**
     * @brief Private constructor for singleton pattern.
     */
    ConfigHandler() = default;

    /**
     * @brief Map storing component identifiers from config file.
     */
    std::map<std::string, std::string> components;

    /**
     * @brief Validates that all required components are present in config.
     *
     * @throws runtime_error
     *   If any required component is missing or empty
     */
    void validateConfig();
};