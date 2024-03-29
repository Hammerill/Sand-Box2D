#pragma once

#include <box2d/box2d.h>

#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "../System/Renderer.h"
#include "../System/Controls.h"
#include "../System/Translations.h"
#include "../System/Animations.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

// Special class for MainMenu to manage physics (box logo and paddle gravity and collision).
class MainMenuPhysics
{
private:
    // Link to the Box2D world of the easter egg.
    b2World* world = nullptr;

    // Link to the Box2D object of paddle for the easter egg.
    b2Body* paddle = nullptr;
    // Link to the Box2D object of box logo for the easter egg.
    b2Body* box_logo = nullptr;

    // Y pos of where paddle aims to move.
    b2Vec2 paddle_desired_pos;
    // Is paddle object already inited? Used for safety, we can't work with paddle when it's not inited.
    bool paddle_inited;
    // Paddle size in Box2D meters.
    float paddle_width, paddle_height;

    // Is box easter egg activated right now? True when it falls off.
    bool box_active;

    // Used to make game title fadeout when easter egg is activated. 
    uint8_t title_factual_alpha, title_desired_alpha;

    // TODO: I should move it to some tools module.
    float RAD2DEG;
    // TODO: I should move it to some tools module.
    float GetRandomFloat(float min, float max);

public:
    MainMenuPhysics();
    ~MainMenuPhysics();

    void Init();
    void InitPaddle(float paddle_width);

    void Step();
    void RenderTitle(Renderer* rr, SDL_Rect& title_rect);
    void RenderBox(Renderer* rr, int x_offset, int y_offset);
    void RenderPaddle(Renderer* rr, int x_offset, int y_offset, float menu_scale);

    SDL_Rect GetPaddleRect(Renderer* rr, int x_offset, int y_offset, float menu_scale);

    void ActivateBox(Renderer* rr);
    SDL_Rect GetBoxRect(Renderer* rr, int x_offset, int y_offset);

    void SetPaddleDesiredPosition(b2Vec2 pos);
    void SetPaddlePositionPermanently(b2Vec2 pos);

    void SetTitleDesiredAlpha(uint8_t alpha);
    void SetTitleAlphaPermanently(uint8_t alpha);

    void FreeMemory();
};


/// @brief Class that manages main menu.
class MainMenu
{
private:
    std::vector<std::string> menu_items;
    std::vector<SDL_Texture*> menu_items_textures;
    std::vector<SDL_Texture*> menu_black_items_textures;
    size_t hovered_item = 0;

    MainMenuPhysics physics;

    WindowParams old_wparams, now_wparams;

    /// Status that used after unloading MainMenu.
    ///
    /// If it contains path to the level, this level has to be loaded (player pressed PLAY).
    /// If it's empty, it means that game needs to be closed (player pressed EXIT).
    std::string status = "";
    std::string level_name = "";

    /// Idk how to explain it. Let's just consider it a badcode which does its job smoothly.
    int Precise(int input, float precise_factor);
    void RenderWhiteText(Renderer* rr, size_t index, int x, int y, float scale);
    void RenderBlackText(Renderer* rr, size_t index, int x, int y, float scale, SDL_Rect paddleRect);
public:
    MainMenu();
    ~MainMenu();

    /// @brief Init menu.
    void Init(Renderer* rr);
    /// @brief Destroy menu.
    void FreeMemory();

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

