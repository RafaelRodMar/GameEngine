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
bool SpriteCollision(CSprite* pSpriteHitter, CSprite* pSpriteHittee);

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

    //Sprites
    std::vector<CSprite*> vSprites;

    //Helper methods
    bool CheckSpriteCollision(CSprite* pTestSprite);

    GameEngine(std::string pcaption, int pwidth = 640, int pheight = 480);  //calls GameInitialize
    //virtual ~GameEngine(){};

    //general methods
    static GameEngine* GetEngine() { return pGameEngine; };
    bool Initialize();  //initialize variables, create window, calls GameStart

    void HandleEvents(sf::RenderWindow &window);
    void AddSprite(CSprite* pSprite);
    void DrawSprites(sf::RenderWindow &window);
    void UpdateSprites(sf::Time delta);
    void CleanupSprites();
    CSprite* IsPointInSprite(float x, float y);

    bool loadTexture(std::string name, std::string filename);
    sf::Texture &getTexture(std::string name) { return mTextures[name]; };
    void showTexture(std::string name, float x, float y, sf::RenderWindow &window);
    void CleanupTextures();

    bool loadSoundBuffer(std::string name, std::string filename);
    void playSound(std::string name);
    void CleanupSounds();

    bool loadFont(std::string name, std::string filename);
    void Text(const std::string &pstr, float px, float py, sf::Color pcolor, int psize, std::string fontname, sf::RenderWindow &window);
    void CleanupFonts();

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
    vSprites.reserve(50);
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

bool GameEngine::loadTexture(std::string name, std::string filename)
{
    sf::Texture t;
    if( !t.loadFromFile(filename)) return false;

    mTextures[name] = t;
    return true;
}

void GameEngine::showTexture(std::string name, float x, float y, sf::RenderWindow &window)
{
    sf::Sprite sp;
    sp.setTexture(mTextures[name]);
    sp.setPosition(x,y);
    window.draw(sp);
}

void GameEngine::CleanupTextures()
{
    mTextures.clear();
}

//-----------------------------------------------------------------
// Sounds
//-----------------------------------------------------------------
bool GameEngine::loadSoundBuffer(std::string name, std::string filename)
{
    //soundbuffer
    sf::SoundBuffer sb;
    if( !sb.loadFromFile(filename) ) return false;

    mSoundBuffers[name] = sb;

    //sound
    sf::Sound s;
    s.setBuffer(mSoundBuffers[name]);
    mSounds[name] = s;
    return true;
}

void GameEngine::playSound(std::string name)
{
    mSounds[name].play();
}

void GameEngine::CleanupSounds()
{
    mSoundBuffers.clear();
    mSounds.clear();
}

//-----------------------------
//fonts
//-----------------------------
bool GameEngine::loadFont(std::string name, std::string filename)
{
    sf::Font f;
    if( !f.loadFromFile(filename) ) return false;
    mFonts[name] = f;
    return true;
}

void GameEngine::Text(const std::string &pstr, float px, float py, sf::Color pcolor, int psize, std::string fontname, sf::RenderWindow &window)
{
    sf::Text str;
    str.setString(pstr);
    str.setFont(mFonts[fontname]);
    str.setCharacterSize(psize);
    str.setPosition(px, py);
    str.setFillColor(pcolor);
    window.draw(str);
}

void GameEngine::CleanupFonts()
{
    mFonts.clear();
}

//-----------------------------------------------------------------
// Game Engine Helper Methods
//-----------------------------------------------------------------
bool GameEngine::CheckSpriteCollision(CSprite* pTestSprite)
{
  // See if the sprite has collided with any other sprites
  std::vector<CSprite*>::iterator siSprite;
  for (siSprite = vSprites.begin(); siSprite != vSprites.end(); siSprite++)
  {
    // Make sure not to check for collision with itself
    if (pTestSprite == (*siSprite))
      continue;

    // Test the collision
    if (pTestSprite->TestCollision(*siSprite))
      // Collision detected (SpriteCollision should be provided by every game)
      return SpriteCollision((*siSprite), pTestSprite);
  }

  // No collision
  return false;
}

void GameEngine::AddSprite(CSprite* pSprite)
{
    //add a sprite to the sprite vector
    if(pSprite != nullptr)
    {
        //see if there are sprites already in the sprite vector
        if(vSprites.size() > 0)
        {
            //find a spot in the sprite vector to insert the sprite by its z-order
            std::vector<CSprite*>::iterator siSprite;
            for(siSprite = vSprites.begin(); siSprite != vSprites.end(); siSprite++)
                if(pSprite->GetZOrder() < (*siSprite)->GetZOrder())
                {
                    //insert the sprite into the sprite vector
                    vSprites.insert(siSprite, pSprite);
                    return;
                }
        }

        // the sprite's z-order is highest, so add it to the end of the vector
        vSprites.push_back(pSprite);
    }
}

void GameEngine::DrawSprites(sf::RenderWindow &window)
{
    //draw the sprites in the sprite vector
    std::vector<CSprite*>::iterator siSprite;
    for(siSprite = vSprites.begin(); siSprite != vSprites.end(); siSprite++)
        (*siSprite)->Draw(window);
}

void GameEngine::UpdateSprites(sf::Time delta)
{
  // Update the sprites in the sprite vector
  sf::FloatRect rcOldSpritePos;
  SPRITEACTION  saSpriteAction;
  std::vector<CSprite*>::iterator siSprite;
  for (siSprite = vSprites.begin(); siSprite != vSprites.end(); siSprite++)
  {
    // Save the old sprite position in case we need to restore it
    rcOldSpritePos = (*siSprite)->GetPosition();

    // Update the sprite
    saSpriteAction = (*siSprite)->Update(delta);

    // Handle the SA_KILL sprite action
    if (saSpriteAction & SA_KILL)
    {
      delete (*siSprite);
      vSprites.erase(siSprite);
      siSprite--;
      continue;
    }

    // See if the sprite collided with any others
    if (CheckSpriteCollision(*siSprite))
      // Restore the old sprite position
      (*siSprite)->SetPosition(rcOldSpritePos);
  }
}

void GameEngine::CleanupSprites()
{
  // Delete and remove the sprites in the sprite vector
  std::vector<CSprite*>::iterator siSprite;
  for (siSprite = vSprites.begin(); siSprite != vSprites.end(); siSprite++)
  {
    delete (*siSprite);
    vSprites.erase(siSprite);
    siSprite--;
  }
}

CSprite* GameEngine::IsPointInSprite(float x, float y)
{
  // See if the point is in a sprite in the sprite vector
  std::vector<CSprite*>::reverse_iterator siSprite;
  for (siSprite = vSprites.rbegin(); siSprite != vSprites.rend(); siSprite++)
    if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
      return (*siSprite);

  // The point is not in a sprite
  return NULL;
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

