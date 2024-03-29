#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Json.h"

/// @brief Static class for loading translated strings.
class Translations
{
private:
    // Path to the directory where all the translations are stored.
    static std::string translations_base;
    // Code of the desired language. For example, "en" (English).
    static std::string translation_code;
    // Is loaded language Japanese? Used because we need to render Japanese text with another font.
    static bool isJp;
public:
    Translations();
    ~Translations();

    /// @brief Load translation to use it then.
    /// @param translations_base path to the directory where all the translations are stored.
    /// @param translation_code code of the desired language. For example, "en" (English).
    static void LoadTranslation(std::string translations_base, std::string translation_code);

    /// @brief Load localized string.
    /// Note that it will load string from file everytime this method called.
    /// So I recommend to load string once and store it somewhere and not to load it every frame from here.
    /// @param to_load specify path to the desired string.
    /// For example, "menu.json/item_play" to get "PLAY" when "en" language is loaded.
    /// @return localized string. Empty string if there was problem.
    static std::string Load(std::string to_load);

    /// @brief Is loaded language Japanese? Used because we need to render Japanese text with another font.
    static bool GetJp();
};
