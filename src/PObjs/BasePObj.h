#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <box2d/box2d.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

/// @brief Base class for physics objects in the WorldManager environment.
class BasePObj
{
protected:
    /// @brief By multiplying this with value in radians
    /// you will get a value in degrees.
    /// @example RAD * RAD2DEG == DEG
    /// @example DEG / RAD2DEG == RAD
    const float RAD2DEG = 180 / M_PI;

    /// @brief Texture of the PObj.
    SDL_Texture* texture;

    /// @brief Link to the main body of the PObj.
    b2Body* body;
    /// @brief Body definition of the PObj.
    b2BodyDef bodyDef;
    /// @brief Starting velocity of the PObj.
    b2Vec2 vel;
    /// @brief Fixture definition of the PObj.
    b2FixtureDef fixtureDef;

    /// @brief Path to the image file of texture.
    const char* pathToTexture = nullptr;

public:
    virtual ~BasePObj() {};
    
    /// Register this PObj in the world and set its texture. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where PObj should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer = nullptr);

    /// @brief Load a texture for PObj.
    /// @param renderer the rendering context.
    void LoadTexture(SDL_Renderer* renderer);

    b2Body* GetBody();

    /// @brief Set default position and angle of PObj.
    virtual void Reset() = 0;
    
    /// @brief Render this PObj.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if object rendered. False if didn't render because it's out of screen bounds. 
    virtual bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0) = 0;

    /// @brief If this is true, object can be deleted when user does want it.
    bool isMarkedToDelete = false;
};
