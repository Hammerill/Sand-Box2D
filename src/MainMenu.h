#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Renderer.h"
#include "Controls.h"
#include "Translations.h"
#include "Animations.h"

/// @brief Class that manages main menu and can download levels.
class MainMenu
{
private:
    std::vector<std::string> menu_items;
    size_t hovered_item = 0;

    /// Status that used after unloading MainMenu.
    /// 
    /// If it contains path to the level, this level has to be loaded (player pressed PLAY).
    /// If it's empty, it means that game needs to be closed (player pressed EXIT).
    std::string status = "";
    std::string level_name = "";
public:
    MainMenu();
    ~MainMenu();

    /// @brief Init menu.
    void Init();

    /// @brief Make MainMenu logical step.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl keys pressed in this frame.
    /// @param old_ctrl keys pressed in previous frame.
    /// @return true if MainMenu wants to render next frame. False if it's the end and we need to load something else
    /// (also it will look at "status" variable).
    bool Step(Renderer* rr, Controls ctrl, Controls old_ctrl);

    /// @brief Render MainMenu.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    void Render(Renderer* rr);

    /// @brief Get "status" variable. It contains what item player decided to select in menu.
    /// For more info refer to MainMenu::status.
    std::string GetStatus();
    
    std::string GetLevelName();
};

