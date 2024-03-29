#include "PObjBox.h"

PObjBox::PObjBox(BoxDesc boxDesc)
{
    PObjBox::boxDesc = boxDesc;

    PObjBox::bodyDef.type = b2_dynamicBody;
    PObjBox::bodyDef.angle = PObjBox::boxDesc.angle / PObjBox::RAD2DEG;
    PObjBox::bodyDef.position.Set(PObjBox::boxDesc.x, PObjBox::boxDesc.y);

    PObjBox::boxShape.SetAsBox(PObjBox::boxDesc.w / 2.0f, PObjBox::boxDesc.h / 2.0f);

    PObjBox::fixtureDef.shape = &(PObjBox::boxShape);
    PObjBox::fixtureDef.density = 1;
    PObjBox::fixtureDef.friction = 0.3f;
    PObjBox::fixtureDef.restitution = 0.5f;
}
PObjBox::~PObjBox()
{
    PObjBox::body->GetWorld()->DestroyBody(PObjBox::body);
}

void PObjBox::SetParam(std::string name, Json::Value value)
{
    if (name == "id")
        PObjBox::id = value.asInt();

    else if (name == "x")
    {
        b2Vec2 pos = PObjBox::body->GetPosition();
        pos.x = value.asFloat();
        PObjBox::body->SetTransform(pos, PObjBox::body->GetAngle());
    }
    else if (name == "y")
    {
        b2Vec2 pos = PObjBox::body->GetPosition();
        pos.y = value.asFloat();
        PObjBox::body->SetTransform(pos, PObjBox::body->GetAngle());
    }
    else if (name == "angle")
    {
        b2Vec2 pos = PObjBox::body->GetPosition();
        PObjBox::body->SetTransform(pos, value.asFloat() / PObjBox::RAD2DEG);
    }
    else if (name == "vel_x")
    {
        b2Vec2 vel = PObjBox::body->GetLinearVelocity();
        vel.x = value.asFloat();
        PObjBox::body->SetLinearVelocity(vel);
    }
    else if (name == "vel_y")
    {
        b2Vec2 vel = PObjBox::body->GetLinearVelocity();
        vel.y = value.asFloat();
        PObjBox::body->SetLinearVelocity(vel);
    }
    else if (name == "vel_ang")
    {
        PObjBox::body->SetAngularVelocity(value.asFloat());
    }
    else if (name == "texture_path")
    {
        PObjBox::texture_path = value.asString();
    }
    else if (name == "undeletable")
    {
        PObjBox::undeletable = value.asBool();
    }
}

Json::Value PObjBox::GetParam(std::string name)
{
    if (name == "id")
        return Json::Value(PObjBox::id);

    if (PObjBox::isRegistered)
    {
        if (name == "x")
            return Json::Value(PObjBox::body->GetPosition().x);
        else if (name == "y")
            return Json::Value(PObjBox::body->GetPosition().y);
        else if (name == "angle")
            return Json::Value(PObjBox::body->GetAngle() * PObjBox::RAD2DEG);
        else if (name == "vel_x")
            return Json::Value(PObjBox::body->GetLinearVelocity().x);
        else if (name == "vel_y")
            return Json::Value(PObjBox::body->GetLinearVelocity().y);
        else if (name == "vel_ang")
            return Json::Value(PObjBox::body->GetAngularVelocity());
    }
    else
    {
        if (name == "x")
            return Json::Value(PObjBox::boxDesc.x);
        else if (name == "y")
            return Json::Value(PObjBox::boxDesc.y);
        else if (name == "angle")
            return Json::Value(PObjBox::boxDesc.angle);
        else if (name == "vel_x")
            return Json::Value(PObjBox::boxDesc.vel_x);
        else if (name == "vel_y")
            return Json::Value(PObjBox::boxDesc.vel_y);
        else if (name == "vel_ang")
            return Json::Value(PObjBox::boxDesc.vel_ang);
    }

    if (name == "w")
        return Json::Value(PObjBox::boxDesc.w);
    else if (name == "h")
        return Json::Value(PObjBox::boxDesc.h);
    else if (name == "texture_path")
        return Json::Value(PObjBox::texture_path);
    else if (name == "undeletable")
        return Json::Value(PObjBox::undeletable);

    return 0;
}

void PObjBox::Register(b2World* world, Renderer* rr, std::map<std::string, SDL_Texture*>& textures)
{
    PObjBox::body = world->CreateBody(&(PObjBox::bodyDef));
    PObjBox::body->SetLinearVelocity(b2Vec2(PObjBox::boxDesc.vel_x, PObjBox::boxDesc.vel_y));
    PObjBox::body->SetAngularVelocity(PObjBox::boxDesc.vel_ang);
    PObjBox::body->CreateFixture(&(PObjBox::fixtureDef));

    PObjBox::texture = PObjBox::LoadTexture(textures, PObjBox::GetParam("texture_path").asString(), rr->GetRenderer());

    PObjBox::isRegistered = true;
}

bool PObjBox::Render(Renderer* rr, float x_offset, float y_offset, float zoom)
{
    b2Vec2 pos = PObjBox::body->GetPosition();
    SDL_Rect box;

    box.w = PObjBox::boxDesc.w * zoom;
    box.h = PObjBox::boxDesc.h * zoom;

    box.x = (pos.x * zoom) + x_offset - (box.w / 2.0f);
    box.y = (pos.y * zoom) + y_offset - (box.h / 2.0f);

    float maxima = std::max(box.w * 1.5, box.h * 1.5);

    if (box.x > -maxima && box.x < rr->GetWidth() + maxima
    &&  box.y > -maxima && box.y < rr->GetHeight() + maxima)
    {
        SDL_RenderCopyEx(rr->GetRenderer(), PObjBox::texture, NULL, &box, PObjBox::body->GetAngle() * PObjBox::RAD2DEG, NULL, SDL_FLIP_NONE);
        return true;
    }
    else
        return false;
}

float PObjBox::GetX()
{
    if (PObjBox::isRegistered)
        return PObjBox::body->GetPosition().x;
    else
        return PObjBox::boxDesc.x;
}
float PObjBox::GetY()
{
    if (PObjBox::isRegistered)
        return PObjBox::body->GetPosition().y;
    else
        return PObjBox::boxDesc.y;
}
