#pragma once

#include "Json.h"

#include <iostream>
#include <fstream>

/// @brief Class that manages settings in the game and operates with theirs file.
class Settings
{
private:
    // Game settings loaded as Json Value.
    Json::Value settings;
    // Predefined game settings loaded as Json Value.
    Json::Value def_settings;
    // Where game settings are located on the drive?
    const char* path_to_settings = nullptr;
public:
    /// @brief Load settings file and create Settings instance.
    /// At every Set() call will write updated parameter to file if it's successfully loaded.
    /// Will also call Set() when setting default value to
    /// parameter which was requested through Get() and doesn't have value.
    /// @param path_to_settings path to the JSON where user settings are stored.
    /// If doesn't exist - it'll create it. If leave nullptr, settings affect only current session.
    /// @param path_to_def_settings path to the JSON where default settings are stored.
    /// If leave nullptr, all default settings will be considered as 0.
    Settings(const char* path_to_settings = nullptr, const char* path_to_def_settings = nullptr);

    /// @brief Set some value at the settings.
    /// Will affect current session and write it to the file if it's loaded.
    /// @param key Name of the parameter to be setted.
    /// @param value Value of the parameter to be setted.
    void Set(std::string key, Json::Value value);

    /// @brief Get some value from the settings.
    /// @param key Name of the parameter to be returned.
    /// @return Value of the parameter. If it's not set - set to default value and return it.
    Json::Value Get(std::string key);

    /// @brief Clear all the settings (reset them to default).
    void Clear();
};
