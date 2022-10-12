#include "BoxEntity.h"

BoxEntity::BoxEntity(const char* path_to_texture, float x_box, float y_box, float w_box, float h_box, float angle_box, float vel_x, float vel_y)
{
    BoxEntity::x = x_box;
    BoxEntity::y = y_box;
    BoxEntity::w = w_box;
    BoxEntity::h = h_box;
    BoxEntity::angle = angle_box;

    BoxEntity::pathToTexture = path_to_texture;
    
    BoxEntity::bodyDef.type = b2_dynamicBody;
    BoxEntity::bodyDef.angle = BoxEntity::angle; 
    BoxEntity::bodyDef.position.Set(BoxEntity::x, BoxEntity::y);

    BoxEntity::vel.Set(vel_x, vel_y);

    BoxEntity::boxShape.SetAsBox(BoxEntity::w / 2.0f, BoxEntity::h / 2.0f);
    
    BoxEntity::fixtureDef.shape = &(BoxEntity::boxShape);
    BoxEntity::fixtureDef.density = 1;
    BoxEntity::fixtureDef.friction = 0.3f;
    BoxEntity::fixtureDef.restitution = 0.5f;
}

BoxEntity::BoxEntity(SDL_Texture* texture, float x_box, float y_box, float w_box, float h_box, float angle_box, float vel_x, float vel_y)
{
    BoxEntity::x = x_box;
    BoxEntity::y = y_box;
    BoxEntity::w = w_box;
    BoxEntity::h = h_box;
    BoxEntity::angle = angle_box;

    BoxEntity::pathToTexture = nullptr;
    BoxEntity::texture = texture;
    
    BoxEntity::bodyDef.type = b2_dynamicBody;
    BoxEntity::bodyDef.angle = BoxEntity::angle; 
    BoxEntity::bodyDef.position.Set(BoxEntity::x, BoxEntity::y);

    BoxEntity::vel.Set(vel_x, vel_y);

    BoxEntity::boxShape.SetAsBox(BoxEntity::w / 2.0f, BoxEntity::h / 2.0f);
    
    BoxEntity::fixtureDef.shape = &(BoxEntity::boxShape);
    BoxEntity::fixtureDef.density = 1;
    BoxEntity::fixtureDef.friction = 0.3f;
    BoxEntity::fixtureDef.restitution = 0.5f;
}
BoxEntity::~BoxEntity()
{   
    if (BoxEntity::pathToTexture != nullptr) 
        SDL_DestroyTexture(BoxEntity::texture);

    delete[] BoxEntity::pathToTexture;

    BoxEntity::body->GetWorld()->DestroyBody(BoxEntity::body);
}

void BoxEntity::Reset()
{
    BoxEntity::body->SetTransform(b2Vec2(BoxEntity::x, BoxEntity::y), BoxEntity::angle);
    BoxEntity::body->SetLinearVelocity(BoxEntity::vel);
}
void BoxEntity::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom)
{
    b2Vec2 pos = BoxEntity::body->GetPosition();
    SDL_Rect box;
    
    box.w = BoxEntity::w * zoom;
    box.h = BoxEntity::h * zoom;

    box.x = (pos.x * zoom) + x_offset - (box.w / 2.0f);
    box.y = (pos.y * zoom) + y_offset - (box.h / 2.0f);

    SDL_RenderCopyEx(renderer, BoxEntity::texture, NULL, &box, BoxEntity::body->GetAngle() * BoxEntity::RAD2DEG, NULL, SDL_FLIP_NONE);
}
