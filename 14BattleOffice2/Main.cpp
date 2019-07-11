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
#include "GameEngine.h"

//global variables
GameEngine *pGame;
Rnd rnd;
CSprite* pTargetSprite;
CSprite* pPowSprite;
CSprite* pGuySprites[5];
int guydelay[5];
int guymasterdelay;
int hits, misses;
bool gameover;


bool GameInitialize()
{
    pGame = new GameEngine("Battle Office 2",500,400);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    pGame->loadAssets();

    //create the target, pow and guy sprites
    sf::FloatRect rcBounds(0,0,500,400);
    pTargetSprite = new CSprite(pGame->getTexture("target"), rcBounds, BA_STOP);
    pTargetSprite->SetZOrder(4);
    pGame->AddSprite(pTargetSprite);
    pPowSprite = new CSprite(pGame->getTexture("pow"), rcBounds, BA_STOP);
    pPowSprite->SetZOrder(3);
    pPowSprite->SetHidden(true);
    pGame->AddSprite(pPowSprite);
    pGuySprites[0] = new CSprite(pGame->getTexture("guy1"), rcBounds);
    pGuySprites[0]->SetPosition(92, 175);
    pGuySprites[0]->SetZOrder(2);
    pGuySprites[0]->SetHidden(true);
    pGame->AddSprite(pGuySprites[0]);
    pGuySprites[1] = new CSprite(pGame->getTexture("guy2"), rcBounds);
    pGuySprites[1]->SetPosition(301, 184);
    pGuySprites[1]->SetZOrder(2);
    pGuySprites[1]->SetHidden(true);
    pGame->AddSprite(pGuySprites[1]);
    pGuySprites[2] = new CSprite(pGame->getTexture("guy3"), rcBounds);
    pGuySprites[2]->SetPosition(394, 61);
    pGuySprites[2]->SetZOrder(2);
    pGuySprites[2]->SetHidden(true);
    pGame->AddSprite(pGuySprites[2]);
    rcBounds.left = 340;
    pGuySprites[3] = new CSprite(pGame->getTexture("guy4"), rcBounds, BA_WRAP);
    pGuySprites[3]->setNumFrames(4);
    pGuySprites[3]->SetPosition(420, 10);
    pGuySprites[3]->SetVelocity(-5, 0);
    pGuySprites[3]->SetZOrder(1);
    pGuySprites[3]->SetHidden(true);
    pGame->AddSprite(pGuySprites[3]);
    rcBounds.left = 385;
    pGuySprites[4] = new CSprite(pGame->getTexture("guy5"), rcBounds, BA_WRAP);
    pGuySprites[4]->setNumFrames(4);
    pGuySprites[4]->setFrameDelay(5);
    pGuySprites[4]->SetPosition(260, 60);
    pGuySprites[4]->SetVelocity(5, 0);
    pGuySprites[4]->SetZOrder(1);
    pGuySprites[4]->SetHidden(true);
    pGame->AddSprite(pGuySprites[4]);

    //initialize the remaining global variables
    guymasterdelay = 50;
    hits = 0;
    misses = 0;
    gameover = false;

    pGame->playMusic("music", true);
}

void GameEnd()
{
    pGame->stopMusic("music");

    pGame->CleanupAll();
    pGame->window.close();
    delete pGame;
}

void GameActivate()
{
    pGame->continueMusic("music");
}

void GameDeactivate()
{
    pGame->pauseMusic("music");
}

void GamePaint(sf::RenderWindow &window)
{
    //draw the background office
    pGame->showTexture("office",0,0,window);

    //draw the sprites
    pGame->DrawSprites(window);

    //draw the number of guys who were hit
    pGame->Text(std::to_string(hits), 237, 360, sf::Color::Black, 15, "font", window);

    //draw the number of guys who were missed
    for(int i=0;i<misses;i++)
        pGame->showTexture("smallguy", 389 + pGame->getTexture("smallguy").getSize().x * i, 359, window);

    //draw the game over message, if necessary
    if(gameover)
        pGame->showTexture("gameover",120,110,window);

    window.display();
}

void GameCycle(sf::Time delta)
{
  if (!gameover)
  {
    // Randomly show and hide the guys
    for (int i = 0; i < 5; i++)
      if (pGuySprites[i]->IsHidden())
      {
        if (rnd.getRndInt(0,60) == 0)
        {
          // Show the guy
          pGuySprites[i]->SetHidden(false);

          // Start the countdown delay
          if (i == 3)
          {
            // Start the guy running left
            guydelay[i] = 80;
            pGuySprites[i]->SetPosition(420, 10);
          }
          else if (i == 4)
          {
            // Start the guy running right
            guydelay[i] = 45;
            pGuySprites[i]->SetPosition(260, 60);
          }
          else
            // Start the stationary guys
            guydelay[i] = 20 + rnd.getRndInt(0, guymasterdelay);
        }
      }
      else
      {
        if (--guydelay[i] == 0)
        {
          // Play a sound for the guy getting away
          pGame->playSound("taunt");

          // Hide the guy
          pGuySprites[i]->SetHidden(true);

          // Increment the misses
          if (++misses == 5)
          {
            // Play a sound for the game ending
            pGame->playSound("boo");

            // End the game
            for (int i = 0; i < 5; i++)
              pGuySprites[i]->SetHidden(true);
            gameover = true;

            // Pause the background music
            pGame->pauseMusic("music");
          }
        }
      }

    // Update the sprites
    pGame->UpdateSprites(delta);
  }

  //check for mouse activity
  if(pGame->mouseClicked)
  {
    MouseButtonDown(pGame->mousePos.x, pGame->mousePos.y, pGame->mouseLeftButton);
  }

    //update the target position with the mouse position
    int mx = sf::Mouse::getPosition(pGame->window).x;
    int my = sf::Mouse::getPosition(pGame->window).y;
    int tw = pGame->getTexture("target").getSize().x;
    int th = pGame->getTexture("target").getSize().y;
    pTargetSprite->SetPosition( mx - tw / 2, my - th / 2);
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, bool bLeft)
{
  // Only check the left mouse button
  if (!gameover && bLeft)
  {
    // Temporarily hide the target and pow sprites
    pTargetSprite->SetHidden(true);
    pPowSprite->SetHidden(true);

    // See if a guy sprite was clicked
    CSprite* pSprite;
    if ((pSprite = pGame->IsPointInSprite(x, y)) != nullptr)
    {
      // Play a sound for hitting the guy
      pGame->playSound("whack");

      // Position and show the pow sprite
      pPowSprite->SetPosition(x - (pGame->getTexture("pow").getSize().x / 2),
        y - (pGame->getTexture("pow").getSize().y / 2));
      pPowSprite->SetHidden(false);

      // Hide the guy that was clicked
      pSprite->SetHidden(true);

      // Increment the hits and make the game harder, if necessary
      if ((++hits % 5) == 0)
        if (--guymasterdelay == 0)
          guymasterdelay = 1;
    }

    // Show the target sprite again
    pTargetSprite->SetHidden(false);
  }
  else if (gameover && !bLeft)
  {
    // Start a new game
    gameover = false;
    hits = 0;
    misses = 0;

    // Restart the background music
    pGame->continueMusic("music");
  }
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
