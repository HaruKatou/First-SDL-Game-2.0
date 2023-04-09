#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>

#include "Field.h"
#include "Paddle.h"
#include "Ball.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* texture;

    // Timing
    unsigned int curframe, lastframe;
    //unsigned int startframe, actualDelay;


    Field* field;
    Paddle* paddle;
    Ball* ball;
    bool Ball_on_Paddle;

    void Update(float delta);
    void Render();
    void CleanUp();
    void StartGame();

    void ResetPaddle();
    void InitBall();
    void SetPaddlePosition(float x);
    float Reflection(float hitx);

    void FieldCollision();
    void PaddleCollision();
    void BrickCollision();
    void SideCollision(int sidehit);

    int BrickCount();
};


#endif // GAME_H_INCLUDED