#include "WorldManager.h"

WorldManager::WorldManager(int WINDOW_WIDTH, int WINDOW_HEIGHT, const char* path_to_font, bool fpsCorrection, float move_speed, float zoom_speed)
{
    WorldManager::WINDOW_WIDTH = WINDOW_WIDTH;
    WorldManager::WINDOW_HEIGHT = WINDOW_HEIGHT;

    WorldManager::speedCorrection = fpsCorrection;

    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<PhysicsObj*>();

    if (path_to_font != nullptr)
        Font::LoadFont(path_to_font);
    
    WorldManager::initVideo();
}
WorldManager::~WorldManager()
{
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::deleteObject(i);
    }

    SDL_DestroyRenderer(WorldManager::renderer);
	SDL_DestroyWindow(WorldManager::window);
    delete[] WorldManager::world;
}

void WorldManager::initVideo()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    if (WorldManager::WINDOW_WIDTH == 0 || WorldManager::WINDOW_HEIGHT == 0)
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);

        WorldManager::WINDOW_WIDTH = dm.w / 1.5;
        WorldManager::WINDOW_HEIGHT = dm.h / 1.5;
    }

    WorldManager::window = SDL_CreateWindow("Box2D", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    WorldManager::renderer = SDL_CreateRenderer(WorldManager::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(WorldManager::renderer, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT);
}

void WorldManager::addObject(PhysicsObj* obj) { WorldManager::order.push_back(obj); }
void WorldManager::deleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

bool WorldManager::Step()
{
    if (WorldManager::speedCorrection)
        WorldManager::b = WorldManager::a;

    Ctrl::Check();

    if (Ctrl::getExit())
    {
        return false;
    }

    if (Ctrl::getFullscreen())
    {
        if (WorldManager::holdingFullscreenButton == false)
        {
            WorldManager::isFullscreen = !WorldManager::isFullscreen;

            WorldManager::goFullscreen(WorldManager::isFullscreen);
            
            WorldManager::holdingFullscreenButton = true;
        }        
    }
    else    
        WorldManager::holdingFullscreenButton = false;

    if (Ctrl::getDebug())
    {
        if (WorldManager::holdingDebugButton == false)
        {
            WorldManager::isDebug = !WorldManager::isDebug;            
            WorldManager::holdingDebugButton = true;
        }        
    }
    else    
        WorldManager::holdingDebugButton = false;

    if (Ctrl::getDeleteObjs())
    {
        for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
        {
            if (WorldManager::objects[i]->isMarkedToDelete)
                WorldManager::deleteObject(i);        
        }
    }

    WorldManager::y_offset += Ctrl::getMoveUp() * WorldManager::move_speed;
    WorldManager::x_offset -= Ctrl::getMoveRight() * WorldManager::move_speed;
    WorldManager::y_offset -= Ctrl::getMoveDown() * WorldManager::move_speed;
    WorldManager::x_offset += Ctrl::getMoveLeft() * WorldManager::move_speed;

    if (Ctrl::getMoving())
    {
        WorldManager::x_offset += Ctrl::getDeltaX();
        WorldManager::y_offset += Ctrl::getDeltaY();
    }

    WorldManager::zoom += Ctrl::getZoomIn() * WorldManager::zoom_speed;
    if (WorldManager::zoom > 1)    
        WorldManager::zoom -= Ctrl::getZoomOut() * WorldManager::zoom_speed;

    if (!(WorldManager::zoom <= 1 && Ctrl::getWheel() > 0))
        WorldManager::zoom -= Ctrl::getWheel();

    if (WorldManager::zoom <= 1)
        WorldManager::zoom = 1;

    WorldManager::world->Step(1.0f / 60.0f, 6, 2);

    if (Ctrl::getReset())
    {
        for (size_t i = 0; i < WorldManager::objects.size(); i++)
        {
            WorldManager::objects[i]->Reset();
        }
    }

    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    { // Load order.
        WorldManager::objects.push_back(WorldManager::order[i]);
        WorldManager::objects[WorldManager::objects.size() - 1]->Register(WorldManager::world, WorldManager::renderer);
        WorldManager::order.pop_back();        
    }

    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    { // Out of bounds check.
        if (WorldManager::objects[i]->getBody()->GetPosition().x > 100 ||
            WorldManager::objects[i]->getBody()->GetPosition().y > 100 ||
            WorldManager::objects[i]->getBody()->GetPosition().x < -100 ||
            WorldManager::objects[i]->getBody()->GetPosition().y < -100 )
        {
            WorldManager::deleteObject(i);
        }
    }

    return true;
}

void WorldManager::Render()
{
    SDL_SetRenderDrawBlendMode(WorldManager::renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(WorldManager::renderer, 0x32, 0x32, 0x32, 0);
    SDL_RenderClear(WorldManager::renderer);

    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        WorldManager::objects[i]->Render(   WorldManager::renderer, 
                                            WorldManager::x_offset, 
                                            WorldManager::y_offset, 
                                            WorldManager::zoom);
    }

    if (WorldManager::isDebug && Font::getLoaded())
    {
        SDL_Rect debugBg {0, 0, 200, 400};

        SDL_SetRenderDrawBlendMode(WorldManager::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(WorldManager::renderer, 4, 4, 4, 0x80);

        SDL_RenderFillRect(WorldManager::renderer, &debugBg);

        Font::Render(WorldManager::renderer, "DEBUG", 8, 8);
    }

    SDL_RenderPresent(WorldManager::renderer);
}

void WorldManager::Cycle()
{
    bool isRunning = true;

    if (WorldManager::speedCorrection)
    {
        while (isRunning)
        {
            WorldManager::a = SDL_GetTicks();
            WorldManager::delta = WorldManager::a - WorldManager::b;

            if (delta > 1000/60.0)
            {
                isRunning = WorldManager::Step();
                WorldManager::Render();
            }
        }
    }
    else
    {
        while (isRunning)
        {
            isRunning = WorldManager::Step();
            WorldManager::Render();
        }
    }
}

SDL_Renderer* WorldManager::getRenderer() { return WorldManager::renderer; }

void WorldManager::goFullscreen(bool isToFullscreen)
{
    if (isToFullscreen)
    {
        SDL_SetWindowFullscreen(WorldManager::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(WorldManager::window, 0);
    }
}
