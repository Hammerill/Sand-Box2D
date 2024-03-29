#pragma once

// TODO: It's just a copy of LangSelector now.

#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "../System/Renderer.h"
#include "../System/Controls.h"
#include "../System/Translations.h"
#include "../System/Animations.h"
#include "../System/Settings.h"

class OptionsMenu
{
private:
    std::map<std::string, std::string> langs;
    size_t hovered_lang = 0;

    std::string choose_title = "", settings_reminder = "";

    std::string translations_base = "";

    bool fadeout = false;
public:
    OptionsMenu();
    ~OptionsMenu();

    /// @brief Init OptionsMenu.
    /// @param translations_base path to the directory where all the translations are stored. 
    void Init(std::string translations_base);

    /// @brief Make OptionsMenu logical step.
    /// @param settings link to settings manager. Used to store and read chosen language param.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl keys pressed in this frame.
    /// @param old_ctrl keys pressed in previous frame.
    /// @return true if OptionsMenu wants to render next frame.
    /// False if it's the end and we know what language user wants.
    bool Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl);

    /// @brief Render OptionsMenu.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    void Render(Renderer* rr);
};
