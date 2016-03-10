
#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

struct LTexture
{
    char* Filepath;  
    SDL_Texture* Image;
};

struct Vector2
{
    int X;  
    int Y;
};

struct Sprite
{
    Vector2 SpriteLoc;
    int Width;
    int Height;
    LTexture* Texture;
    char* Filepath;
};

struct Game
{
    SDL_Window *Window;    
    SDL_Renderer* Renderer;
    SDL_Surface* Surface;
    bool bRunning;
    int ScreenWidth;
    int ScreenHeight;
    std::vector<Sprite*> Sprites;
};

void InitGame( char* ScreenName, int Width, int Height );
void MainLoop( );

void InitBackground( );

Sprite* CreateSprite( int X, int Y, int Width, int Height, char* Filepath );