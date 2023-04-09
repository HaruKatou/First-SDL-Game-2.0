#include "Ball.h"

Ball::Ball(SDL_Renderer* ball_renderer)
{
    renderer = ball_renderer;
    SDL_Surface* surface = IMG_Load("pokeball.png");
    BallTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    x = 0;
    y = 0;
    width = 24;
    height = 24;

}

Ball::~Ball()
{
    // Clean resources
    SDL_DestroyTexture(BallTexture);
}

void Ball::Update(float delta)
{
    x += dirx * delta;          //using delta in movements to avoid fps spike
    y += diry * delta;
}

void Ball::Render()
{
    SDL_Rect realrect;          //source rect
    realrect.x = 0;
    realrect.y = 0;
    realrect.w = 512;
    realrect.h = 512;

    SDL_Rect ballrect;          //destination rect
    ballrect.x = x;
    ballrect.y = y;
    ballrect.w = width;
    ballrect.h = height;
    SDL_RenderCopy(renderer, BallTexture, &realrect, &ballrect);
}

void Ball::SetDirection(float _dirx, float _diry)
{
    // Normalize the direction vector then multiply speed
    float length = sqrt(_dirx * _dirx + _diry * _diry);
    dirx = _dirx / length * BALL_SPEED;
    diry = _diry / length * BALL_SPEED;
}

