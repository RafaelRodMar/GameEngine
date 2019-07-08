//game engine functions declarations
bool GameInitialize();  //calls Initialize
void GameStart();
void GameEnd();
void GameActivate();
void GameDeactivate();
void GamePaint(sf::RenderWindow &window);
void GameCycle(sf::Time delta);

//game engine class
class GameEngine
{
public:
    static GameEngine* pGameEngine;
    sf::RenderWindow window;
    std::string caption;
    int width,height;
    bool sleep;
    bool running;

    //time measurement
    sf::Clock clock;
    sf::Time timePerFrame;
    sf::Time elapsed = sf::Time::Zero;

    GameEngine(std::string pcaption, int pwidth, int pheight);  //calls GameInitialize
    //virtual ~GameEngine(){};

    //general methods
    static GameEngine* GetEngine() { return pGameEngine; };
    bool Initialize();  //initialize variables, create window, calls GameStart

    void HandleEvents(sf::RenderWindow &window);

    //Accessor methods
    int GetWidth() { return width; };
    int GetHeight() { return height; };
    std::string GetCaption() { return caption; };
    sf::Time GetTimePerFrame() { return timePerFrame; };
    void SetFrameRate(float FrameRate) { timePerFrame = sf::seconds(1.f / FrameRate); };
    bool GetSleep() { return sleep; };
    void SetSleep(bool bsleep) { sleep = bsleep; };
};

//pointer to the game engine
GameEngine *GameEngine::pGameEngine = nullptr;

GameEngine::GameEngine(std::string pcaption, int pwidth, int pheight)
{
    //set variables for the game engine
    pGameEngine = this;
    caption = pcaption;
    width = pwidth;
    height = pheight;
    sleep = false;
    running = true;
}

bool GameEngine::Initialize()
{
    window.create(sf::VideoMode(width,height),caption);

    //calculate position (center window)
    int x = ( sf::VideoMode::getDesktopMode().width - width ) / 2;
    int y = ( sf::VideoMode::getDesktopMode().height - height ) / 2;
    window.setPosition(sf::Vector2i( x, y));

    return true;
}

void GameEngine::HandleEvents(sf::RenderWindow &window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        //close window or press Escape key
        if((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed)
                                                && (event.key.code == sf::Keyboard::Escape)))
                                                    running = false;

        if (event.type == sf::Event::LostFocus)
        {
            GameDeactivate();
            sleep = true;
        }

        if (event.type == sf::Event::GainedFocus)
        {
            GameActivate();
            sleep = false;
        }

    }
}

int main()
{
    sf::Clock clock;
    sf::Time timePerFrame = sf::seconds(1.f / 60.f); //default
    sf::Time elapsed = sf::Time::Zero;

    if( GameInitialize() )
    {
        //initialize the game engine
        if( !GameEngine::GetEngine()->Initialize() )
            return false;

        //update timePerFrame after initialization
        timePerFrame = GameEngine::GetEngine()->GetTimePerFrame();

        //call to the game start
        GameStart();

        // enter the main loop
        while( GameEngine::GetEngine()->running )
        {
            elapsed += clock.restart();

            GameEngine::GetEngine()->HandleEvents(GameEngine::GetEngine()->window);

            //check if the game engine is sleeping
            if( !GameEngine::GetEngine()->GetSleep() )
            {
                while( elapsed >= timePerFrame )
                {
                    GameCycle(elapsed);
                    elapsed -= timePerFrame;
                }
            }

            GamePaint(GameEngine::GetEngine()->window);
        }
    }

    //end the game
    GameEnd();

    return EXIT_SUCCESS;
}

