
#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <assert.h>
#include <stdio.h>
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
    int CurrentXFrame;
    int CurrentYFrame;
    int X;
    int Y;
    int BaseGround;
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
    float Gravity;
    float GravityTime;
    const float MAX_GRAVITY = 12.25;
    const float ADD_GRAVITY_TIME = 0.005f;
    std::vector<Sprite*> Sprites;
};

struct Timing 
{
    const float m_MaxFPS = 60;
    const float m_TargetFPS = 60;
	float m_FPS;
	float m_FrameTime;
	float DeltaTime;
    Uint16 m_End;
	Uint16 m_StartTicks;  
};

void InitGame( char* ScreenName, int Width, int Height );
void MainLoop( );

void TimingBegin( );
void TimingCalculateFPS( );
float TimingEnd( );

void InitBackground( );
void InitPlayer( );

Sprite* CreateSprite( int X, int Y, int Width, int Height, char* Filepath, TextureType );