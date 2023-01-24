#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "Renderer.h"
#include "Controls.h"
#include "Translations.h"
#include "Animations.h"
#include "Settings.h"

class LangSelector
{
private:
    std::map<std::string, std::string> langs;
    size_t hovered_lang = 0;

    std::string translations_base = "";

    bool fadeout = false;
public:
    LangSelector();
    ~LangSelector();

    /// @brief Init LangSelector.
    /// @param translations_base path to the directory where all the translations are stored. 
    void Init(std::string translations_base);

    /// @brief Make LangSelector logical step.
    /// @param settings link to settings manager. Used to store and read chosen language param.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl keys pressed in this frame.
    /// @param old_ctrl keys pressed in previous frame.
    /// @return true if LangSelector wants to render next frame.
    /// False if it's the end and we know what language user wants.
    bool Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl);

    /// @brief Render LangSelector.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    void Render(Renderer* rr);
};
