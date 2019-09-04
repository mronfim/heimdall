#include "heimdall/heimdall.h"

using namespace Heimdall;

struct Position : Component<Position>
{
    Position(float x, float y): x(x), y(y) {};
    float x, y;
};

class Graphics : public System {
public:
    Graphics() {
        signature.addComponent<Position>();
    }

    void render(float alpha) {
        SDL_Renderer *renderer = Engine::GetInstance().getRenderer();
        for (auto &entity : registeredEntities)
        {
            ComponentHandle<Position> position;
            parentWorld->unpack(entity, position);

            SDL_Rect rect = { (int)position->x, (int)position->y, 10, 10 };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
};

int main(int argc, char* argv[])
{
    Engine& engine = Engine::GetInstance();
    engine.Initialize();

    std::unique_ptr<System> gfx = std::make_unique<Graphics>();
    engine.getWorld()->addSystem(std::move(gfx));

    auto player1 = engine.getWorld()->createEntity();
    player1.addComponent(Position(100, 100));

    engine.Start();
    return 0;
}
