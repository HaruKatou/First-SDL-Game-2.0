#include "Game.h"
#include <math.h>

Game::Game()
{}

Game::~Game()
{}


bool Game::Init()   //Initialize SDL
{
    bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
	else
	{

		//Create window
		window = SDL_CreateWindow("Brick Breakout",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window==NULL)
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}

        //Create renderer for window
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL)
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }

        //create music
        music = Mix_LoadMUS( "Will.wav" );
        if( music == NULL)
        {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
        }

        //create sound effect
        sound = Mix_LoadWAV("collide.wav");
        if( sound == NULL )
        {
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
        }

        //Set font
        font = TTF_OpenFont("Cramps.ttf",20);
    }
    lastframe = SDL_GetTicks();
    return success;
}


void Game::Run() //How the game works
{

    field = new Field(renderer);
    paddle = new Paddle(renderer);
    ball = new Ball(renderer);

    StartGame();


    // Game loop
    while (1)
    {
        //startframe = SDL_GetTicks();
        // Handler events
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                {
                break;          //break from while
                }
        }

        // delta timing
        curframe = SDL_GetTicks();
        float delta = (curframe - lastframe) / 1000.0f;
        lastframe = curframe;

        // Update and render the game
        Update(delta);
        Render();

        //Delay frame rates
        /*actualDelay = SDL_GetTicks() - startframe;
        if(FRAME_DELAY > actualDelay)
            SDL_Delay(FRAME_DELAY - actualDelay);*/

        if(isRunning == false)  //Game stops by a condition somewhere
            break;
    }

    delete paddle;
    delete ball;
    delete field;

    CleanUp();

    SDL_Quit();
    Mix_Quit();
}

void Game::CleanUp()
{
    Mix_FreeMusic(music);

    Mix_FreeChunk(sound);

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
}

void Game::StartGame()
{
    field->CreateBricks();
    ResetPaddle();
    PlayMusic();
}

void Game::ResetPaddle()
{
    Ball_on_Paddle = true;
    InitBall();
}

void Game::InitBall()
{
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    ball->y = paddle->y - ball->height;
}

void Game::Update(float delta)
{
    // Game logic

    // mouse handling
    int mouse_x, mouse_y;
    Uint32 mousestate = SDL_GetMouseState(&mouse_x, &mouse_y);
    SetPaddlePosition(mouse_x - paddle->width/2.0f);

    if ( mousestate == SDL_BUTTON(1) )   //click to start game
    {
        if (Ball_on_Paddle)
        {
            Ball_on_Paddle = false;
            ball->SetDirection(1, -1);
        }
    }

    if (Ball_on_Paddle)
    {
        InitBall();     //hold the ball on the paddle when move paddle
    }

    FieldCollision();
    PaddleCollision();
    BrickCollision();
    BrickCollision();

    if (BrickCount() == BRICK_NUM_WIDTH * BRICK_NUM_HEIGHT)
    {
        StopMusic();
        GameWin();
        if(level < 3)
        {
            level++;
            ball->BALL_SPEED += 150;        //Ball go faster
            StartGame();
        }
    }

    if (!Ball_on_Paddle)
    {
        ball->Update(delta);
    }
    Score();
}

void Game::Render()
{
    SDL_RenderClear(renderer);

    field->Render();
    paddle->Render();
    ball->Render();

    SDL_RenderPresent(renderer);
}

void Game::SetPaddlePosition(float x)
{
    float out_x;        // x for the output wanted
    //keep the paddle inside of the field
    if (x < field->x)
    {
        // Left
        out_x = field->x;
    }
    else if (x + paddle->width > field->x + field->width)
    {
        // Right
        out_x = field->x + field->width - paddle->width;
    }
    else
    {
        out_x = x;
    }
    paddle->x = out_x;
}

void Game::FieldCollision()
{
    if (ball->y < field->y)
    {
        // Top
        ball->y = field->y;
        ball->diry *= -1;

    }
    else if (ball->y + ball->height > field->y + field->height)
    {
        // Reach the void

        // Bottom
        if(life > 0)
        {
            ResetPaddle(); //GameOver();
            life--;
        }
        else
            GameLost();
    }

    if (ball->x <= field->x)
    {
        // Left
        ball->x = field->x;
        ball->dirx *= -1;
    }
    else if (ball->x + ball->width >= field->x + field->width)
    {
        // Right
        ball->x = field->x + field->width - ball->width;
        ball->dirx *= -1;
    }
}

void Game::BrickCollision() {
    for (int i=0; i<BRICK_NUM_WIDTH; i++) {
        for (int j=0; j<BRICK_NUM_HEIGHT; j++) {

            // Check if brick is present
            if (field->bricks[i][j].condition) {
                // Brick x and y coordinates
                float brickx = field->x + i*BRICK_WIDTH ;
                float bricky = field->y + j*BRICK_HEIGHT ;

                float w = 0.5f * (ball->width + BRICK_WIDTH); // Ball width + Brick width
                float h = 0.5f * (ball->height + BRICK_HEIGHT);
                float dx = (ball->x + 0.5f*ball->width) - (brickx + 0.5f*BRICK_WIDTH);    // Ball center - Brick center
                float dy = (ball->y + 0.5f*ball->height) - (bricky + 0.5f*BRICK_HEIGHT);

                if (fabs(dx) <= w && fabs(dy) <= h) {
                    // Collision detected
                    PlaySoundEffect();
                    field->bricks[i][j].condition = false;

                    float wy = w * fabs(dy);
                    float hx = h * fabs(dx);

                    if (wy > hx)
                    {
                        if (dy < 0)
                        {
                            // Top (axis reversed)
                            SideCollision(1);
                        }
                        else
                        {
                            // Bottom
                            SideCollision(3);
                        }
                    }
                    else
                    {
                        if (dx < 0)
                        {
                            // Left
                            SideCollision(0);
                        }
                        else
                        {
                            // Right
                            SideCollision(2);
                        }
                    }
                    return;
                }
            }
        }
    }
}

/*void Game::SideCollision(int sidehit)     //Basic reflecting logic
{
    int cx = 1;
    int cy = 1;
    if(sidehit == 0 || sidehit == 2)
        cx = -1;
    else if(sidehit == 1 || sidehit == 3)
        cy = -1;
    ball->SetDirection(cx*ball->dirx, cy*ball->diry);
}*/

void Game::SideCollision(int sidehit)       //Solving bugs involving hitting the intersection of 2 bricks
 {
    // sidehit 0: Left, 1: Top, 2: Right, 3: Bottom

    // coeficient factor
    int cx = 1;
    int cy = 1;

    if (ball->dirx > 0)
    {
        if (ball->diry > 0)
        {
            // +1 +1
            if (sidehit == 0 || sidehit == 3)
            {
                cx = -1;
            }
            else
            {
                cy = -1;
            }
        }
        else if (ball->diry <= 0)
        {
            // +1 -1
            if (sidehit == 0 || sidehit == 1)
            {
                cx = -1;
            }
            else
            {
                cy = -1;
            }
        }
    }
    else if (ball->dirx <= 0)
    {
        if (ball->diry > 0)
            {
            // -1 +1
            if (sidehit == 2 || sidehit == 3)
            {
                cx = -1;
            }
            else
            {
                cy = -1;
            }
        }
        else if (ball->diry <= 0)
        {
            // -1 -1
            if (sidehit == 1 || sidehit == 2)
            {
                cx = -1;
            }
            else
            {
                cy = -1;
            }
        }
    }
    // Set the new direction by multiplying the coefficient
    ball->SetDirection(cx*ball->dirx, cy*ball->diry);
}

void Game::PaddleCollision()
{
    // center of the ball
    float ballcenterx = ball->x + ball->width / 2.0f;

    // Check paddle collision
    if (ball->x + ball->width > paddle->x &&
        ball->x < paddle->x + paddle->width &&
        ball->y + ball->height > paddle->y &&
        ball->y < paddle->y + paddle->height)
    {
        ball->y = paddle->y - ball->height;
        ball->SetDirection(Reflection(ballcenterx - paddle->x), -1);
    }
}

float Game::Reflection(float x)
{

    // Everything to the left of the center of the paddle is reflected to the left and vice versa
    x -= paddle->width / 2.0f;

    // range of -1 to 1
    return  x / (paddle->width / 2.0f);
}


int Game::BrickCount()  //count the destroyed bricks
{
    int brickcount = 0;
    for (int i=0; i<BRICK_NUM_WIDTH; i++)
    {
        for (int j=0; j<BRICK_NUM_HEIGHT; j++)
        {
            if (field->bricks[i][j].condition == false)
                brickcount++;
        }
    }

    return brickcount;
}

void Game::PlayMusic()
{
    if( Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(music, -1);
    }
}

void Game::PlaySoundEffect()
{
    Mix_PlayChannel(-1, sound, 0);
}

void Game::StopMusic()
{
    Mix_HaltMusic();
}

void Game::Score()
{
    int score = BrickCount() * 100;
    std::stringstream textscore;
    textscore << "BrickBreaker                                                                            SCORE: " << score <<"       ||        LIFE: "<<life;

    SDL_SetWindowTitle( window, textscore.str().c_str());
}

void Game::GameLost()
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game over", "You lose! Restart the game to try again!", window);
    isRunning = false;
}

void Game::GameWin()
{
    if(level == 1)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "LEVEL 1 PASSED!", "You finished level 1! Hit OK to try the next level", window);
    else if(level == 2)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "LEVEL 2 PASSED!", "You finished level 2! Hit OK to try the next level", window);
    else
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You win", "You win! Restart the game to try again!", window);
        isRunning = false;
    }
}

/*void Game::ShowHitSpot()
{
    std::cout<<HitSpot<<" ";
}*/


