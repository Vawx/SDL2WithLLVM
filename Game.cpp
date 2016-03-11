#include "Game.h"

std::vector<LTexture*> TextureList;

float DeltaTime;

Game LGame;
Player LPlayer;
Timing LTiming;

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
                    InitPlayer( );
                    LGame.Gravity = 4.25f;
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
        TimingBegin( );
        
        // Gravity 
        LGame.GravityTime += DeltaTime;
        if( LGame.GravityTime >= LGame.ADD_GRAVITY_TIME )
        {
            LGame.Gravity += 0.35f;
            if( LGame.Gravity > LGame.MAX_GRAVITY )
            {
                LGame.Gravity = LGame.MAX_GRAVITY;
            }
            LGame.GravityTime = 0.f;

            LPlayer.Y += LGame.Gravity;
            if( LPlayer.Y > LPlayer.BaseGround )
            {
                LPlayer.Y = LPlayer.BaseGround;
            }            
        }
        else
        {
            LPlayer.Y = LPlayer.BaseGround;
        }
        
        // Render
        SDL_RenderClear( LGame.Renderer );
        for( size_t i = 0; i < LGame.Sprites.size( ); i++ )
        {
            SDL_Rect renderSrcQuad = { 0,0,0,0 };
            SDL_Rect renderQuad = { 0,0,0,0 };
            
            // Game
            if( LGame.Sprites[ i ]->Texture->Type == TT_GROUND )
            {
                LGame.Sprites[ i ]->SpriteLoc.X -= 2.25f;
                if( LGame.Sprites[ i ]->SpriteLoc.X + LGame.ScreenWidth <= 0 )
                {
                    LGame.Sprites[ i ]->SpriteLoc.X = float( LGame.ScreenWidth - 2 );
                }
                
                renderQuad.x = LGame.Sprites[ i ]->SpriteLoc.X;
                renderQuad.y = LGame.Sprites[ i ]->SpriteLoc.Y;
                renderQuad.w = LGame.Sprites[ i ]->Width;
                renderQuad.h = LGame.Sprites[ i ]->Height;
                
                 SDL_RenderCopy( LGame.Renderer, LGame.Sprites[ i ]->Texture->Image,  NULL, &renderQuad );
            }
            else if(  LGame.Sprites[ i ]->Texture->Type == TT_BACKGROUND )
            {                
                renderQuad.x = LGame.Sprites[ i ]->SpriteLoc.X;
                renderQuad.y = LGame.Sprites[ i ]->SpriteLoc.Y;
                renderQuad.w = LGame.Sprites[ i ]->Width;
                renderQuad.h = LGame.Sprites[ i ]->Height;
                
                 SDL_RenderCopy( LGame.Renderer, LGame.Sprites[ i ]->Texture->Image,  NULL, &renderQuad );
            }
            else if( LGame.Sprites[ i ]->Texture->Type == TT_PLAYER )
            {
                static int playerFrameCount;
                playerFrameCount++;
              
                if( playerFrameCount > 8 )
                {
                    playerFrameCount = 0;
                    LPlayer.CurrentXFrame += 32;
                    if( LPlayer.CurrentXFrame >= LPlayer.MaxX )
                    {
                        LPlayer.CurrentXFrame = 0;
                        LPlayer.CurrentYFrame += 32;
                        if( LPlayer.CurrentYFrame >= LPlayer.MaxY )
                        {
                            LPlayer.CurrentYFrame = 0;
                        }
                    }
                }
                
                renderSrcQuad.x = LPlayer.CurrentXFrame;
                renderSrcQuad.y = LPlayer.CurrentYFrame;
                renderSrcQuad.w = 32;
                renderSrcQuad.h = 32;
                
                renderQuad.x = LPlayer.X;
                renderQuad.y = LPlayer.Y;
                renderQuad.w = LGame.Sprites[ i ]->Width;
                renderQuad.h = LGame.Sprites[ i ]->Height;
                
                 SDL_RenderCopyEx( LGame.Renderer, LGame.Sprites[ i ]->Texture->Image, &renderSrcQuad, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL );
            }
       }
        SDL_RenderPresent( LGame.Renderer );

        // Input
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
                    case SDLK_SPACE:
                    LGame.Gravity = -7.5f;
                    break;
                }
            }
        }
        
        TimingEnd( );
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

void TimingBegin( )
{
    LTiming.m_StartTicks = SDL_GetTicks( );
}

void TimingCalculateFPS( )
{
    static const int NUM_SAMPLES = 10;
	static float frameTimes[ NUM_SAMPLES ];
	static int currentFrame = 0;
	static Uint16 previousTicks = SDL_GetTicks( );
	Uint16 currentTicks = SDL_GetTicks( );

    LTiming.m_FrameTime = float( currentTicks - previousTicks );
	frameTimes[ currentFrame % NUM_SAMPLES ] = LTiming.m_FrameTime;
	previousTicks = currentTicks;

	int count; 
	currentFrame++;
	count = ( currentFrame < NUM_SAMPLES ) ? currentFrame : NUM_SAMPLES;

	float frameTimeAverage = 0;
	for ( int i = 0; i < count; i++ )
	{
		frameTimeAverage += frameTimes[ i ];
	}

	frameTimeAverage /= count;
	LTiming.m_FPS = ( frameTimeAverage > 0 ) ? 1000.f / frameTimeAverage : 60.f;
}

float TimingEnd( )
{
    TimingCalculateFPS( );
    Uint32 frameTicks = SDL_GetTicks( ) - LTiming.m_StartTicks;
	if( 1000.f / LTiming.m_MaxFPS >= frameTicks )
	{
		SDL_Delay( int( 1000.f / LTiming.m_MaxFPS - frameTicks ) );
	}
	LTiming.m_End = SDL_GetTicks( );
	DeltaTime = ( LTiming.m_End - LTiming.m_StartTicks ) * 0.001f;
	return LTiming.m_FPS;
}

Sprite* CreateSprite( int X, int Y, int Width, int Height, char* Filepath, TextureType Type )
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
            resultTexture->Type = Type;
            
            Vector2 resultLoc;
            resultLoc.X = float( X );
            resultLoc.Y = float( Y );
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
    char* bigtree = "./content/img/background/a3.png";
    
    LGame.Sprites.push_back( CreateSprite( 0, 0, LGame.ScreenWidth, LGame.ScreenHeight, background, TT_BACKGROUND ) );

    Vector2 startingLocation;
    startingLocation.X = 0;
    startingLocation.Y = 482;
    for( int i = 0; i < 7; i++ )
    {
        LGame.Sprites.push_back( CreateSprite( int( startingLocation.X ), int( startingLocation.Y ), 160, 224, trees, TT_BACKGROUND  ) );
        startingLocation.X += 160;
    }

    LGame.Sprites.push_back( CreateSprite( 0, 396, LGame.ScreenWidth, LGame.ScreenHeight / 2, bigtree, TT_GROUND ) );
    LGame.Sprites.push_back( CreateSprite( LGame.ScreenWidth, 396, LGame.ScreenWidth, LGame.ScreenHeight / 2, bigtree, TT_GROUND ) );
}

void InitPlayer( )
{
    char* sheet = "./content/img/player/a1.png";    
    
    LPlayer.BaseGround = 672;
    LPlayer.X = 256;
    LPlayer.Y = 672;
    LGame.Sprites.push_back( CreateSprite( 256, 672, 64, 64, sheet, TT_PLAYER ) );
}