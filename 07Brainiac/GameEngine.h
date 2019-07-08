//game engine functions declarations
bool GameInitialize();  //calls Initialize
void GameStart();
void GameEnd();
void GameActivate();
void GameDeactivate();
void GamePaint(sf::RenderWindow &window);
void GameCycle(sf::Time delta);
void HandleKeys();
void MouseButtonDown(int x,int y, bool bLeft);
void MouseButtonUp(int x, int y, bool bLeft);
void MouseMove(int x, int y);

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

    //keyboard handling
    // the keyboard's state in the current and the previous frame
    bool CurrentKeyState[sf::Keyboard::KeyCount];
    bool PreviousKeyState[sf::Keyboard::KeyCount];

    //mouse
    bool mouseClicked = false;
    bool mouseLeftButton = false;
    bool mouseRightButton = false;
    sf::Vector2i mousePos;

    //time measurement
    sf::Clock clock;
    sf::Time timePerFrame;
    sf::Time elapsed = sf::Time::Zero;

    GameEngine(std::string pcaption, int pwidth = 640, int pheight = 480);  //calls GameInitialize
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

    //keyboard functions
    bool KeyPressed(sf::Keyboard::Key Key)
        { return (CurrentKeyState[Key] && !PreviousKeyState[Key]); }

    bool KeyReleased(sf::Keyboard::Key Key)
        { return (!CurrentKeyState[Key] && PreviousKeyState[Key]); }

    bool KeyHeld(sf::Keyboard::Key Key)
        { return CurrentKeyState[Key]; }
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

        //left mouse button press
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                mouseClicked = true;
                mouseLeftButton = true;
                mousePos = sf::Mouse::getPosition(window);
            }
        }

        //left mouse button release
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                mouseClicked = false;
                mouseLeftButton = false;
            }
        }

        //right mouse button press
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Right)
            {
                mouseClicked = true;
                mouseRightButton = true;
                mousePos = sf::Mouse::getPosition(window);
            }
        }

        //right mouse button release
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Right)
            {
                mouseClicked = false;
                mouseRightButton = false;
            }
        }
    }

    // Save the state of each keyboard key (must be done before any Key* function is executed)
    for(unsigned int i = 0; i < sf::Keyboard::KeyCount; ++i)
    {
        // Save the keyboard's state from the previous frame
        PreviousKeyState[i] = CurrentKeyState[i];

        // And save the keyboard's state in the current frame
        CurrentKeyState[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
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
            HandleKeys();

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

