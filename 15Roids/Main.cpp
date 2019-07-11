#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Various.h"

//Textures
std::map<std::string, sf::Texture> mTextures;
//fonts
std::map<std::string, sf::Font> mFonts;
//Sound Buffers
std::map<std::string, sf::SoundBuffer> mSoundBuffers;
//Sounds
std::map<std::string, sf::Sound> mSounds;
//Music
std::map<std::string, sf::Music*> mMusic;


#include "CSprite.h"
#include "Background.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
Rnd rnd;
StarryBackground* pBackground;


bool GameInitialize()
{
    pGame = new GameEngine("Roids",500,400);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    //load the asteroid texture
    pGame->loadTexture("asteroid", "assets/img/asteroid.png");

    //create the starry background
    pBackground = new StarryBackground(500,400);

    //create the asteroid sprites
    sf::FloatRect rcBounds(0,0,500,400);

    CSprite* pSprite;
    pSprite = new CSprite(pGame->getTexture("asteroid"), rcBounds, BA_WRAP);
    pSprite->setNumFrames(14);
    pSprite->setFrameDelay(1);
    pSprite->SetPosition(250,200);
    pSprite->SetVelocity(-3,1);
    pGame->AddSprite(pSprite);
    pSprite = new CSprite(pGame->getTexture("asteroid"), rcBounds, BA_WRAP);
    pSprite->setNumFrames(14);
    pSprite->setFrameDelay(2);
    pSprite->SetPosition(250,200);
    pSprite->SetVelocity(3,-2);
    pGame->AddSprite(pSprite);
    pSprite = new CSprite(pGame->getTexture("asteroid"), rcBounds, BA_WRAP);
    pSprite->setNumFrames(14);
    pSprite->setFrameDelay(3);
    pSprite->SetPosition(250,200);
    pSprite->SetVelocity(-2,-4);
    pGame->AddSprite(pSprite);
}

void GameEnd()
{
    delete pBackground;
    pGame->CleanupAll();
    pGame->window.close();
    delete pGame;
}

void GameActivate()
{

}

void GameDeactivate()
{

}

void GamePaint(sf::RenderWindow &window)
{
    // draw the background
    pBackground->Draw(window);

    // draw the sprites
    pGame->DrawSprites(window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    //update the background
    pBackground->Update();

    //update the sprites
    pGame->UpdateSprites(delta);

  //check for mouse activity
  if(pGame->mouseClicked)
  {
    MouseButtonDown(pGame->mousePos.x, pGame->mousePos.y, pGame->mouseLeftButton);
  }
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, bool bLeft)
{
}

void MouseButtonUp(int x, int y, bool bLeft)
{

}

void MouseMove(int x, int y)
{

}

bool SpriteCollision(CSprite* pSpriteHitter, CSprite* pSpriteHittee)
{
    return false;
}
