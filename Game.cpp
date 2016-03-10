#include "Game.h"

std::vector<LTexture*> TextureList;

Game LGame;

void InitGame( char* ScreenName, int Width, int Height )
{
    LGame.Window = SDL_CreateWindow( ScreenName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_SHOWN );
    if( LGame.Window )
    {
        LGame.Renderer =  SDL_CreateRenderer( LGame.Window, -1, SDL_RENDERER_ACCELERATED );
        if( LGame.Renderer )
        {
            LGame.Surface = SDL_GetWindowSurface( LGame.Window );
            if( LGame.Surface )
            {
                if( IMG_Init( IMG_INIT_PNG ) )
                {
                    LGame.ScreenWidth = Width;
                    LGame.ScreenHeight = Height;
                    InitBackground( );
                    LGame.bRunning = true;
                    MainLoop( );   
                }
                else
                {
                    printf( "Failed to init IMG %s\n", IMG_GetError( ) );
                }
            }
            else
            {
                printf( "Failed to create surface %s\n", SDL_GetError( ) );
            }
        }
        else
        {
            printf( "Failed to create renderer %s\n", SDL_GetError( ) );
        }
    }
    else
    {
        printf( "Failed to Create window %s\n", SDL_GetError( ) );
    }
}

void MainLoop( )
{
    while( LGame.bRunning )
    {
        SDL_RenderClear( LGame.Renderer );
        
        for( size_t i = 0; i < LGame.Sprites.size( ); i++ )
        {
           SDL_Rect  renderQuad = 
            {   LGame.Sprites[ i ] ->SpriteLoc.X,
                LGame.Sprites[ i ] ->SpriteLoc.Y,
                LGame.Sprites[ i ] ->Width, 
                LGame.Sprites[ i ] ->Height
            };
            
            SDL_RenderCopy( LGame.Renderer, LGame.Sprites[ i ]->Texture->Image, NULL, &renderQuad );
       }
        SDL_RenderPresent( LGame.Renderer );

        SDL_Event e;
        while( SDL_PollEvent( &e ) )
        {
            if( e.type == SDL_QUIT )
            {
                LGame.bRunning = false;
            }
            else if( e.type == SDL_KEYUP )
            {
                switch( e.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                    LGame.bRunning = false;
                    break;
                }
            }

        }
    }

    for( size_t i = 0; i < LGame.Sprites.size( ); i++ )
    {
        SDL_DestroyTexture( LGame.Sprites[ i ]->Texture->Image );
        free( LGame.Sprites[ i ] );
    }

    SDL_DestroyWindow( LGame.Window );
    SDL_DestroyRenderer( LGame.Renderer );
    SDL_FreeSurface( LGame.Surface );
    SDL_Quit( );
}

Sprite* CreateSprite( int X, int Y, int Width, int Height, char* Filepath )
{
    SDL_Texture *asImage  = NULL;
    SDL_Surface *fromFile = IMG_Load( Filepath );
    if( fromFile != NULL )
    {
        asImage = SDL_CreateTextureFromSurface( LGame.Renderer, fromFile );
        if( asImage != NULL )
        {
            Sprite *result = (Sprite*)malloc( sizeof( Sprite ) );
            assert( result != NULL );
            
            LTexture *resultTexture = (LTexture*)malloc( sizeof( LTexture ) );
            assert( resultTexture != NULL );
            
            resultTexture->Image = asImage;
            resultTexture->Filepath = Filepath;
            
            Vector2 resultLoc;
            resultLoc.X = X;
            resultLoc.Y = Y;
            result->SpriteLoc = resultLoc;
            result->Width = Width;
            result->Height = Height;
            result->Texture = resultTexture;
            
            return result;
        }
        else
        {
            printf( "Failed to create texture from surface: %s\n", IMG_GetError( ) );
        }
    }
    else
    {
        printf( "Failed to load file: %s\n", IMG_GetError( ) );
    }
    return NULL;
}

void InitBackground( )
{
    char* background = "./content/img/background/a1.png";
    char* trees = "./content/img/background/a2.png";
    
    LGame.Sprites.push_back( CreateSprite( 0, 0, LGame.ScreenWidth, LGame.ScreenHeight, background ) );
            
    Vector2 startingLocation;
    startingLocation.X = 0;
    startingLocation.Y = 545;
    for( int i = 0; i < 7; i++ )
    {
         LGame.Sprites.push_back( CreateSprite( startingLocation.X, startingLocation.Y, 160, 224, trees  ) );
        startingLocation.X += 160;
    }
    
}