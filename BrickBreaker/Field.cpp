#include "Field.h"

Field::Field(SDL_Renderer* field_renderer)
{
    renderer = field_renderer;      //Field::renderer = Game::renderer
    SDL_Surface* surface = IMG_Load("bricks.png");
    BrickTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("AncientHellPillars.png");
    SideTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    x = 8;
    y = 0;
    width = 800 - 2*8;
    height = 600;

}

Field::~Field()
{};


void Field::Render() {
    // Render bricks
    for (int i=0; i < BRICK_NUM_WIDTH; i++)
    {
        for (int j=0; j < BRICK_NUM_HEIGHT; j++)
        {

            if (bricks[i][j].condition)
            {

            SDL_Rect srcrect;
            srcrect.x = bricks[i][j].color * 112 + bricks[i][j].color * 28;        //source rect
            srcrect.y = 0;
            srcrect.w = 112;
            srcrect.h = 56;

            SDL_Rect dstrect;
            dstrect.x = x + i * BRICK_WIDTH ;                    //destination rect
            dstrect.y = j * BRICK_HEIGHT ;
            dstrect.w = BRICK_WIDTH;
            dstrect.h = BRICK_HEIGHT;

            SDL_RenderCopy(renderer, BrickTexture, &srcrect, &dstrect);

            }
        }
    }

    // Render sides
    SDL_Rect realrect;
    realrect.x = 0;
    realrect.y = 0;
    realrect.w = 33;
    realrect.h = 128;

    SDL_Rect siderect;
    siderect.x = 0;                     //Left
    siderect.y = 0;
    siderect.w = 8;
    siderect.h = 600;
    SDL_RenderCopy(renderer, SideTexture, &realrect , &siderect);

    siderect.x = 800 - 8;              //Right
    siderect.y = 0;
    siderect.w = 8;
    siderect.h = 600;
    SDL_RenderCopy(renderer, SideTexture, &realrect, &siderect);
}

void Field::CreateBricks()
{
    srand(time(0));         //Randomizer

    for (int i=0; i < BRICK_NUM_WIDTH; i++)
    {
        for (int j=0; j < BRICK_NUM_HEIGHT; j++)
        {
            bricks[i][j].color = rand() % 8;    // Random color
            bricks[i][j].condition = true;       // Brick is born
        }
    }
}
