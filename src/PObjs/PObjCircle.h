#pragma once

#include "BasePObj.h"

#include <SDL2/SDL2_gfxPrimitives.h>

struct CircleDesc
{
    float x = 0, y = 0;
    float radius = 0;
    float angle = 0;
    float vel_x = 0, vel_y = 0;
    float vel_ang = 0;
};

/// Child class of the BasePObj which represents Circle
/// in the WorldManager environment.
class PObjCircle: public BasePObj
{
private:
    /// @brief What values you should declare for create Circle?
    /// Used only while creating Circle.
    CircleDesc circleDesc;

    /// @brief Color of the Circle.
    uint8_t r, g, b;

    /// @brief Color of the Circle angle renderer.
    uint8_t r_angle, g_angle, b_angle;

    /// @brief Is texture for the Circle is loaded? If true, render Circle as texture.
    /// Otherwise render it as colored shape.
    /// Can be accessed through SetParam() and GetParam() as "is_texture".
    bool is_texture = false;

    /// @brief Shape of the Circle.
    b2CircleShape circleShape;

public:
    PObjCircle(CircleDesc circleDesc);
    ~PObjCircle();

    /// @brief Set some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted.
    void SetParam(std::string name, Json::Value value);

    /// @brief Get some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return Json::Value. Call its functions like AsFloat(), etc.
    Json::Value GetParam(std::string name);

    /// Register this Circle in the world. Should be
    /// called only when no world calculations are performing.
    /// @param world link to the world where Circle should be registered.
    /// @param renderer the rendering context.
    /// @param textures textures vector to work with.
    void Register(b2World* world, Renderer* rr, std::map<std::string, SDL_Texture*>& textures);

    /// @brief Render this Circle.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @return true if Circle rendered. False if didn't render because it's out of screen bounds.
    bool Render(Renderer* rr, float x_offset, float y_offset, float zoom);

    // Quickly get X position of this Circle.
    float GetX();
    // Quickly get Y position of this Circle.
    float GetY();
};
