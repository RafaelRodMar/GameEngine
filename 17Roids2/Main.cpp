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
CSprite* pAsteroids[3];
CSprite* pSaucer;

void UpdateSaucer();

bool GameInitialize()
{
    pGame = new GameEngine("Roids 2",500,400);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    pGame->loadAssets();
    pBackground = new StarryBackground(500,400);

    sf::FloatRect rcBounds(0,0,500,400);
    pAsteroids[0] = new CSprite(pGame->getTexture("asteroid"), rcBounds, BA_WRAP);
    pAsteroids[0]->setNumFrames(14);
    pAsteroids[0]->setFrameDelay(1);
    pAsteroids[0]->SetPosition(250,200);
    pAsteroids[0]->SetVelocity(-24,8);
    pAsteroids[0]->SetName("asteroid");
    pGame->AddSprite(pAsteroids[0]);
    pAsteroids[1] = new CSprite(pGame->getTexture("asteroid"), rcBounds, BA_WRAP);
    pAsteroids[1]->setNumFrames(14);
    pAsteroids[1]->setFrameDelay(2);
    pAsteroids[1]->SetPosition(250,200);
    pAsteroids[1]->SetVelocity(24,-16);
    pAsteroids[1]->SetName("asteroid");
    pGame->AddSprite(pAsteroids[1]);
    pAsteroids[2] = new CSprite(pGame->getTexture("asteroid"), rcBounds, BA_WRAP);
    pAsteroids[2]->setNumFrames(14);
    pAsteroids[2]->setFrameDelay(3);
    pAsteroids[2]->SetPosition(250,200);
    pAsteroids[2]->SetVelocity(-16,-32);
    pAsteroids[2]->SetName("asteroid");
    pGame->AddSprite(pAsteroids[2]);

    pSaucer = new CSprite(pGame->getTexture("saucer"), rcBounds, BA_WRAP);
    pSaucer->SetPosition(0,0);
    pSaucer->SetVelocity(24,8);
    pSaucer->SetName("saucer");
    pGame->AddSprite(pSaucer);
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
    pBackground->Draw(window);
    pGame->DrawSprites(window);
    window.display();
}

void GameCycle(sf::Time delta)
{
    pBackground->Update();
    pGame->UpdateSprites(delta);
    UpdateSaucer();

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

void SpriteDying(CSprite* pSprite)
{

}

void UpdateSaucer()
{
  // Obtain the saucer's position
  sf::FloatRect rcSaucer, rcRoid;
  rcSaucer = pSaucer->GetCollision();

  // Find out which asteroid is closest to the saucer
  int iXCollision = 500, iYCollision = 400, iXYCollision = 900;
  for (int i = 0; i < 3; i++)
  {
    // Get the asteroid position
    rcRoid = pAsteroids[i]->GetCollision();

    // Calculate the minimum XY collision distance
    int iXCollisionDist = (rcSaucer.left +
      (rcSaucer.width) / 2) -
      (rcRoid.left +
      (rcRoid.width) / 2);
    int iYCollisionDist = (rcSaucer.top +
      (rcSaucer.height) / 2) -
      (rcRoid.top +
      (rcRoid.height) / 2);
    if ((abs(iXCollisionDist) < abs(iXCollision)) ||
      (abs(iYCollisionDist) < abs(iYCollision)))
      if ((abs(iXCollisionDist) + abs(iYCollisionDist)) < iXYCollision)
      {
        iXYCollision = abs(iXCollision) + abs(iYCollision);
        iXCollision = iXCollisionDist;
        iYCollision = iYCollisionDist;
      }
  }

  // Move to dodge the asteroids, if necessary
  sf::Vector2f ptVelocity;
  ptVelocity = pSaucer->GetVelocity();
  if (abs(iXCollision) < 60)
  {
    // Adjust the X velocity
    if (iXCollision < 0)
      ptVelocity.x = std::max((int)ptVelocity.x - 4, -32);
    else
      ptVelocity.x = std::min((int)ptVelocity.x + 4, 32);
  }
  if (abs(iYCollision) < 60)
  {
    // Adjust the Y velocity
    if (iYCollision < 0)
      ptVelocity.y = std::max((int)ptVelocity.y - 4, -32);
    else
      ptVelocity.y = std::min((int)ptVelocity.y + 4, 32);
  }

  // Update the saucer to the new position
  pSaucer->SetVelocity(ptVelocity);
}
