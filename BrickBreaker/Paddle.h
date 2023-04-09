#ifndef PADDLE_H_INCLUDED
#define PADDLE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>

class Paddle
{
public:
    Paddle(SDL_Renderer* renderer);
    ~Paddle();

    float x, y, width, height;

    void Render();

private:
    SDL_Texture* PaddleTexture;
    SDL_Renderer* renderer;
};

#endif // PADDLE_H_INCLUDED
