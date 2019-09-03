#include "heimdall/engine.h"

int main(int argc, char* argv[])
{
    Heimdall::Engine& engine = Heimdall::Engine::GetInstance();
    engine.Start();
    return 0;
}
