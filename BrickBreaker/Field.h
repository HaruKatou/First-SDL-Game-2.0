#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

// dimension of the brick
const int BRICK_WIDTH = 56;
const int BRICK_HEIGHT = 28;

// the number of bricks 14x10
const int BRICK_NUM_WIDTH = 14;
const int BRICK_NUM_HEIGHT = 10;

class Brick
{
public:
    int color;      //color
    bool condition; //alive or dead
};

class Field
{
public:
    Field(SDL_Renderer* renderer);
    ~Field();

    float x, y, width, height;

    void Render();

    void CreateBricks();

    Brick bricks[BRICK_NUM_WIDTH][BRICK_NUM_HEIGHT];

private:
    SDL_Texture* BrickTexture;
    SDL_Texture* SideTexture;
    SDL_Renderer* renderer;
};

#endif // FIELD_H_INCLUDED
