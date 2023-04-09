#include "main.h"

int main(int argc, char* argv[])
{

    Game* game = new Game();

    // Initialize and run the game
    game->Init();
    game->Run();

    delete game;
    return 0;
}
