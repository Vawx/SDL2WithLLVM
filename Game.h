
#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

typedef enum __TEXTURETYPE__
{
    TT_BACKGROUND,
    TT_GROUND,
    TT_PLAYER,
} TextureType;

struct LTexture
{
    TextureType Type;
    char* Filepath;  
    SDL_Texture* Image;
};

struct Vector2
{
    float X;  
    float Y;
};

struct Sprite
{
    Vector2 SpriteLoc;
    int Width;
    int Height;
    LTexture* Texture;
    char* Filepath;
};

struct Ground
{
    int X;  
    int Y;
};

struct Player
{
    const int MaxX = 128;
    const int MaxY = 128;
    int CurrentX = 0;
    int CurrentY = 0;
    int X;
    int Y;
    Vector2 Position;
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

struct Timing 
{
    const float m_MaxFPS = 60;
    const float m_TargetFPS = 60;
	float m_FPS;
	float m_FrameTime;
	float DeltaTime;
    float m_End;
	unsigned int m_StartTicks;  
};

void InitGame( char* ScreenName, int Width, int Height );
void MainLoop( );

void TimingBegin( );
void TimingCalculateFPS( );
float TimingEnd( );

void InitBackground( );
void InitPlayer( );

Sprite* CreateSprite( int X, int Y, int Width, int Height, char* Filepath, TextureType );