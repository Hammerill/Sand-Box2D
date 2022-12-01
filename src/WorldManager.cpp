#include "WorldManager.h"

WorldManager::WorldManager(int physics_quality, int moving_inertia_frames, float move_speed, float zoom_speed)
{
    WorldManager::physics_quality = physics_quality;
    WorldManager::moving_inertia_frames = moving_inertia_frames;
    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<BasePObj*>();

    Font::LoadFont("./assets/fonts/PressStart2P-vaV7.ttf");
}
WorldManager::~WorldManager()
{
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::DeleteObject(i);
    }
    
    delete WorldManager::world;
}

void WorldManager::LoadLevel(Level level, Renderer* renderer)
{
    WorldManager::level = level;

    // CAMERA
    auto camera = WorldManager::level.GetCamera();

    if (camera.type == "static")
    {
        WorldManager::zoom = renderer->GetHeight() / camera.height;

        WorldManager::x_offset =    -(camera.x * WorldManager::zoom)
                                    +(renderer->GetWidth() / 2);

        WorldManager::y_offset =    -(camera.y * WorldManager::zoom)
                                    +(renderer->GetHeight() / 2);
    }
    /////////
    
    // OBJECTS
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    { // Remove current loaded objects
        WorldManager::DeleteObject(i);
    }
    
    auto objects = WorldManager::level.GetPObjects();
    for (size_t i = 0; i < objects.size(); i++)
    {
        WorldManager::AddObject(objects[i]);
    }
    //////////

    // CYCLES (everything other at the end of the Step())
    WorldManager::cyclesDelays = std::vector<int>();

    for (size_t i = 0; i < WorldManager::level.GetCycles().size(); i++)
    {
        WorldManager::cyclesDelays.push_back(1);
    }    
    /////////

    // ACTIONS (everything other at control handling sector)
    WorldManager::actions = WorldManager::level.GetActions();
    //////////
}

void WorldManager::AddObject(BasePObj* obj)
{
    WorldManager::order.push_back(obj);
}
void WorldManager::DeleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

template<typename T>
double getAverage(std::vector<T> const& v)
{
    if (v.empty())
        return 0;
 
    return std::reduce(v.begin(), v.end(), 0.0) / v.size();
}

std::vector<int> last_frames_speed_x = std::vector<int>();
std::vector<int> last_frames_speed_y = std::vector<int>();

void WorldManager::Step(Renderer* renderer, Controls ctrl, Controls old_ctrl)
{
    if (ctrl.GetDebug() && !old_ctrl.GetDebug())
    {
        WorldManager::isDebug = !WorldManager::isDebug;
    }        

    WorldManager::y_offset += ctrl.GetMoveUp() * WorldManager::move_speed;
    WorldManager::x_offset -= ctrl.GetMoveRight() * WorldManager::move_speed;
    WorldManager::y_offset -= ctrl.GetMoveDown() * WorldManager::move_speed;
    WorldManager::x_offset += ctrl.GetMoveLeft() * WorldManager::move_speed;

    if (last_frames_speed_x.size() > (size_t)(WorldManager::moving_inertia_frames))
    {
        last_frames_speed_x.erase(last_frames_speed_x.begin());
        last_frames_speed_y.erase(last_frames_speed_y.begin());
    }

    if (ctrl.GetIsMoving())
    {
        WorldManager::x_offset += (ctrl.GetMouse().x - old_ctrl.GetMouse().x);
        WorldManager::y_offset += (ctrl.GetMouse().y - old_ctrl.GetMouse().y);
        last_frames_speed_x.push_back(ctrl.GetMouse().x - old_ctrl.GetMouse().x);
        last_frames_speed_y.push_back(ctrl.GetMouse().y - old_ctrl.GetMouse().y);
    }
    else
    {
        WorldManager::x_offset += getAverage(last_frames_speed_x);
        WorldManager::y_offset += getAverage(last_frames_speed_y);
        last_frames_speed_x.push_back(0);
        last_frames_speed_y.push_back(0);
    }

    if (ctrl.GetIsPinching() && ((WorldManager::zoom + (ctrl.GetPinch() - old_ctrl.GetPinch())) > 10))
    {
        CorrectOffset(  ctrl.GetMouse(),
                        (ctrl.GetPinch() - old_ctrl.GetPinch()));
        WorldManager::zoom += (ctrl.GetPinch() - old_ctrl.GetPinch());
    }

    // ACTIONS (OOH, that's HUGE, I mean. What I'm gonna do with it to make it less DAT BEEG?)
    if (ctrl.GetActionUp() && !old_ctrl.GetActionUp())
        WorldManager::level.PerformAction(WorldManager::actions["up"]["keydown_once"], WorldManager::objects);
    else if (ctrl.GetActionUp())
        WorldManager::level.PerformAction(WorldManager::actions["up"]["keydown_hold"], WorldManager::objects);
    else if (!ctrl.GetActionUp() && old_ctrl.GetActionUp())
        WorldManager::level.PerformAction(WorldManager::actions["up"]["keyup"], WorldManager::objects);
    
    if (ctrl.GetActionRight() && !old_ctrl.GetActionRight())
        WorldManager::level.PerformAction(WorldManager::actions["right"]["keydown_once"], WorldManager::objects);
    else if (ctrl.GetActionRight())
        WorldManager::level.PerformAction(WorldManager::actions["right"]["keydown_hold"], WorldManager::objects);
    else if (!ctrl.GetActionRight() && old_ctrl.GetActionRight())
        WorldManager::level.PerformAction(WorldManager::actions["right"]["keyup"], WorldManager::objects);
    
    if (ctrl.GetActionDown() && !old_ctrl.GetActionDown())
        WorldManager::level.PerformAction(WorldManager::actions["down"]["keydown_once"], WorldManager::objects);
    else if (ctrl.GetActionDown())
        WorldManager::level.PerformAction(WorldManager::actions["down"]["keydown_hold"], WorldManager::objects);
    else if (!ctrl.GetActionDown() && old_ctrl.GetActionDown())
        WorldManager::level.PerformAction(WorldManager::actions["down"]["keyup"], WorldManager::objects);
    
    if (ctrl.GetActionLeft() && !old_ctrl.GetActionLeft())
        WorldManager::level.PerformAction(WorldManager::actions["left"]["keydown_once"], WorldManager::objects);
    else if (ctrl.GetActionLeft())
        WorldManager::level.PerformAction(WorldManager::actions["left"]["keydown_hold"], WorldManager::objects);
    else if (!ctrl.GetActionLeft() && old_ctrl.GetActionLeft())
        WorldManager::level.PerformAction(WorldManager::actions["left"]["keyup"], WorldManager::objects);
    
    if (ctrl.GetActionEnter() && !old_ctrl.GetActionEnter())
        WorldManager::level.PerformAction(WorldManager::actions["enter"]["keydown_once"], WorldManager::objects);
    else if (ctrl.GetActionEnter())
        WorldManager::level.PerformAction(WorldManager::actions["enter"]["keydown_hold"], WorldManager::objects);
    else if (!ctrl.GetActionEnter() && old_ctrl.GetActionEnter())
        WorldManager::level.PerformAction(WorldManager::actions["enter"]["keyup"], WorldManager::objects);
    //////////

    SDL_Point scr_center = {renderer->GetWidth() / 2, renderer->GetHeight() / 2};

    if (WorldManager::zoom <= 1)
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        WorldManager::zoom - 1);
        WorldManager::zoom = 1;
    }
    else
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        ctrl.GetZoomOut() * WorldManager::zoom_speed * -1 * WorldManager::zoom);
        WorldManager::zoom -= ctrl.GetZoomOut() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    if (WorldManager::zoom >= 1000)
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        1000 - WorldManager::zoom);
        WorldManager::zoom = 1000;
    }
    else
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        ctrl.GetZoomIn() * WorldManager::zoom_speed * WorldManager::zoom);
        WorldManager::zoom += ctrl.GetZoomIn() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    WorldManager::world->Step(1.0f / 60.0f, WorldManager::physics_quality * 3, WorldManager::physics_quality);

    // CYCLES
    auto cycles = WorldManager::level.GetCycles();
    for (size_t i = 0; i < WorldManager::cyclesDelays.size(); i++)
    {
        if (--(WorldManager::cyclesDelays[i]) <= 0)
        {
            auto cycle = cycles[i];

            WorldManager::cyclesDelays[i] = cycle.delay;

            for (size_t j = 0; j < cycle.objects.size(); j++)
            {
                WorldManager::AddObject(cycle.objects[j]);
            }
        }
    }    
    /////////

    
    // LATER IT WILL BE CONSIDERED DEPRECATED AND DESTROYED
    // Or no?
    if (ctrl.GetReloadLevel() && !old_ctrl.GetReloadLevel()){
        WorldManager::LoadLevel(WorldManager::level, renderer);
        
//         Network::SetRepo("https://raw.githubusercontent.com/Hammerill/Sand-Box2D-levels/main/levels");
// #ifdef Vita
//         Network::DownloadFile("ux0:Data/Sand-Box2D/levels", "index.json");
//         Network::DownloadFile("ux0:Data/Sand-Box2D/levels", "default_level/default_level.json");
// #else
//         Network::DownloadFile("./levels", "index.json");
//         Network::DownloadFile("./levels", "default_level/default_level.json");
// #endif
    }
    ///////////////////////////////////////////////////////

    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    { // Load order.
        WorldManager::objects.push_back(WorldManager::order[i]);
        WorldManager::objects[WorldManager::objects.size() - 1]->Register(WorldManager::world, renderer->GetRenderer());
        WorldManager::order.pop_back();        
    }

    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    { // Out of bounds check.
        if (WorldManager::objects[i]->GetBody()->GetPosition().x > 100 ||
            WorldManager::objects[i]->GetBody()->GetPosition().y > 100 ||
            WorldManager::objects[i]->GetBody()->GetPosition().x < -100 ||
            WorldManager::objects[i]->GetBody()->GetPosition().y < -100 )
        {
            WorldManager::DeleteObject(i);
        }
    }
}

int renderedItemsCount;
void WorldManager::Render(Renderer* renderer, Controls ctrl)
{
    SDL_SetRenderDrawBlendMode(renderer->GetRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer->GetRenderer(), 0x32, 0x32, 0x32, 0); //BG OPTION TO WORK ON
    SDL_RenderClear(renderer->GetRenderer());

    renderedItemsCount = 0;
    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        if (WorldManager::objects[i]->Render(   renderer->GetRenderer(), 
                                                WorldManager::x_offset, 
                                                WorldManager::y_offset, 
                                                WorldManager::zoom,
                                                renderer->GetWidth(),
                                                renderer->GetHeight()))
        {
            renderedItemsCount++;
        }        
    }

    if (WorldManager::isDebug && Font::GetLoaded())
    {
        SDL_Point mouse = ctrl.GetMouse();

        std::vector<std::string> debugStrings;

        debugStrings.push_back("DEBUG");
        debugStrings.push_back("");
        debugStrings.push_back("Camera offset X = " + std::to_string(WorldManager::x_offset));
        debugStrings.push_back("Camera offset Y = " + std::to_string(WorldManager::y_offset));
        debugStrings.push_back("Zoom = " + std::to_string(WorldManager::zoom));
        debugStrings.push_back("Mouse X = " + std::to_string(mouse.x));
        debugStrings.push_back("Mouse Y = " + std::to_string(mouse.y));
        debugStrings.push_back("IsMoving? = " + std::to_string(ctrl.GetIsMoving()));
        debugStrings.push_back("Pinch = " + std::to_string(ctrl.GetPinch()));
        debugStrings.push_back("IsPinching? = " + std::to_string(ctrl.GetIsPinching()));
        debugStrings.push_back("Zoom In = " + std::to_string(ctrl.GetZoomIn()));
        debugStrings.push_back("Zoom Out = " + std::to_string(ctrl.GetZoomOut()));
        debugStrings.push_back("Objects count = " + std::to_string(WorldManager::world->GetBodyCount()));
        debugStrings.push_back("Objects rendered = " + std::to_string(renderedItemsCount));
        debugStrings.push_back("");
        debugStrings.push_back("ACTIONS");
        debugStrings.push_back("Up = " + std::to_string(ctrl.GetActionUp()));
        debugStrings.push_back("Right = " + std::to_string(ctrl.GetActionRight()));
        debugStrings.push_back("Down = " + std::to_string(ctrl.GetActionDown()));
        debugStrings.push_back("Left = " + std::to_string(ctrl.GetActionLeft()));
        debugStrings.push_back("Enter = " + std::to_string(ctrl.GetActionEnter()));

        WorldManager::RenderDebugScreen(debugStrings, renderer);        
    }
}

void WorldManager::RenderDebugScreen(std::vector<std::string> debugStrings, Renderer* renderer)
{
    float debugScale = 2;

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back((debugStrings[i].size() + 2) * Font::FontWidth * debugScale);

    int debug_w = *std::max_element(debugWidths.begin(), debugWidths.end());
    int debug_h = (debugStrings.size() + 2) * Font::FontWidth * debugScale;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(renderer->GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer->GetRenderer(), 4, 4, 4, 0xA0);

    SDL_RenderFillRect(renderer->GetRenderer(), &debugBg);

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        Font::Render(renderer->GetRenderer(), debugStrings[i].c_str(), Font::FontWidth * debugScale, Font::FontWidth * debugScale * (i+1), debugScale);
    }
}

void WorldManager::CorrectOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}
