#pragma once

#include <SDL2/SDL.h>

#ifdef Vita
#include <psp2/ctrl.h>
#endif

/// @brief Static class for accessing pressed keys and/or buttons
class Ctrl
{
private:
    static bool exit;
    static bool reset;

    static bool up;
    static bool right;
    static bool down;
    static bool left;

    static bool plus;
    static bool minus;

    static SDL_Event e;
public:
    Ctrl();
    ~Ctrl();

    /// @brief Checks currently pressed buttons and writes their status to private
    /// variables which can be accessed through getters functions of this class.
    static void Check();

    static bool getExit();
    static bool getReset();

    static bool getUp();
    static bool getRight();
    static bool getDown();
    static bool getLeft();
    
    static bool getPlus();
    static bool getMinus();
};
