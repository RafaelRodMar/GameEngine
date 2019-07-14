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

//global common variables
int fireinputdelay;
int score, numlives, difficulty;
bool gameover = false;
bool canfire = true;

#include "Global.h"
#include "CSprite.h"
#include "AlienSprite.h"
#include "Background.h"
#include "GameEngine.h"

GameEngine *pGame;
StarryBackground* pBackground;
CSprite* pCarSprite;

//functions
void NewGame();
void AddAlien();

bool GameInitialize()
{
    pGame = new GameEngine("Space Out",600,450);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    pGame->loadAssets();
    pBackground = new StarryBackground(600,450);
    pGame->playMusic("music",true);
    NewGame();
}

void GameEnd()
{
    pGame->stopMusic("music");
    delete pBackground;

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
    pBackground->Draw(window);
    pGame->showTexture("desert",0,371,window);
    pGame->DrawSprites(window);
    pGame->Text(std::to_string(score), 460,0,sf::Color::White, 20, "font", window);
    for(int i=0;i<numlives;i++)
        pGame->showTexture("smcar",520 + pGame->getTexture("smcar").getSize().x * i, 10, window);

    if(gameover)
        pGame->showTexture("gameover",190,149,window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    if(!gameover)
    {
        //randomly add aliens
        if( rnd.getRndInt(0,difficulty) == 0 )
            AddAlien();

        //update the background
        pBackground->Update();

        //update the sprites
        pGame->UpdateSprites(delta);
    }
}

void HandleKeys()
{
    if( !gameover )
    {
        //move the car based upon left/right key presses
        sf::Vector2f ptVelocity = pCarSprite->GetVelocity();
        if( pGame->KeyPressed(sf::Keyboard::Left) || pGame->KeyHeld(sf::Keyboard::Left) )
        {
            //move left
            ptVelocity.x = std::max( (int) ptVelocity.x - 7, -28 );
            pCarSprite->SetVelocity(ptVelocity);
        }
        else if ( pGame->KeyPressed(sf::Keyboard::Right) || pGame->KeyHeld(sf::Keyboard::Right) )
        {
            //move right
            ptVelocity.x = std::min( (int) ptVelocity.x + 14, 42);
            pCarSprite->SetVelocity(ptVelocity);
        }

        // Fire missiles based upon spacebar presses
        if((++fireinputdelay > 6) && pGame->KeyPressed(sf::Keyboard::Space) )
        {
            //create a new missile sprite
            sf::FloatRect rcBounds(0,0,600,450);
            sf::FloatRect rcPos = pCarSprite->GetPosition();
            CSprite* pSprite = new CSprite("missile", rcBounds, BA_DIE);
            pSprite->SetName("missile");
            pSprite->SetPosition(rcPos.left + 15, 400);
            pSprite->SetVelocity(0,-50);
            pGame->AddSprite(pSprite);

            //play the missile (fire) sound
            pGame->playSound("missile");

            //reset the input delay
            fireinputdelay = 0;
        }
    }

    // start a new game based upon an enter(return) key press
    if( gameover && pGame->KeyPressed(sf::Keyboard::Return))
        NewGame(); //start a new game
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
    //see if a player missile and an alien have collided
    std::string Hitter = pSpriteHitter->GetName();
    std::string Hittee = pSpriteHittee->GetName();

    if(( Hitter == "missile" && (Hittee == "blobbo" || Hittee == "jelly" || Hittee == "timmy")) ||
       (Hittee == "missile" && (Hitter == "blobbo" || Hitter == "jelly" || Hitter == "timmy")))
    {
        //play the small explosion sound
        pGame->playSound("smexplode");

        //kill both sprites
        pSpriteHittee->Kill();
        pSpriteHitter->Kill();

        //create a large explosion sprite at the alien's position
        sf::FloatRect rcBounds(0,0,600,450);
        sf::FloatRect rcPos;
        if( Hitter == "missile" )
            rcPos = pSpriteHittee->GetPosition();
        else
            rcPos = pSpriteHitter->GetPosition();

        CSprite* pSprite = new CSprite("lgexplosion", rcBounds);
        pSprite->setNumFrames(8,true);
        pSprite->SetPosition(rcPos.left, rcPos.top);
        pGame->AddSprite(pSprite);

        //update the score
        score += 25;
        difficulty = std::max( 240 - (score/20), 20);
    }

    // See if an alien missile has collided with the car
  if ((Hitter == "car" && (Hittee == "bmissile" ||
    Hittee == "jmissile" || Hittee == "tmissile")) ||
    (Hittee == "car" && (Hitter == "bmissile" ||
    Hitter == "jmissile" || Hitter == "tmissile")))
  {
    // Play the large explosion sound
    pGame->playSound("lgexplode");

    // Kill the missile sprite
    if (Hitter == "car")
      pSpriteHittee->Kill();
    else
      pSpriteHitter->Kill();

    // Create a large explosion sprite at the car's position
    sf::FloatRect rcBounds(0, 0, 600, 480);
    sf::FloatRect rcPos;
    if (Hitter == "car")
      rcPos = pSpriteHitter->GetPosition();
    else
      rcPos = pSpriteHittee->GetPosition();
    CSprite* pSprite = new CSprite("lgexplosion", rcBounds);
    pSprite->setNumFrames(8, true);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    pGame->AddSprite(pSprite);

    // Move the car back to the start
    pCarSprite->SetPosition(300, 405);

    // See if the game is over
    if (--numlives == 0)
    {
      // Play the game over sound
      pGame->playSound("gameover");
      gameover = true;
    }
  }
    return false;
}

void SpriteDying(CSprite* pSprite)
{
    // See if an alien missile sprite is dying
  if (pSprite->GetName() == "bmissile" ||
    pSprite->GetName() == "jmissile" ||
    pSprite->GetName() == "tmissile")
  {
    // Play the small explosion sound
    pGame->playSound("smexplode");

    // Create a small explosion sprite at the missile's position
    sf::FloatRect rcBounds(0, 0, 600, 450);
    sf::FloatRect rcPos = pSprite->GetPosition();
    CSprite* pSprite = new CSprite("smexplosion", rcBounds);
    pSprite->setNumFrames(8, true);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    pGame->AddSprite(pSprite);
  }
}

void NewGame()
{
    //clear the sprites
    pGame->CleanupSprites();

    //create the car sprite
    sf::FloatRect rcBounds(0,0,600,450);
    pCarSprite = new CSprite("car", rcBounds, BA_WRAP);
    pCarSprite->SetPosition(300,405);
    pCarSprite->SetName("car");
    pGame->AddSprite(pCarSprite);

    //initialize the game variables
    fireinputdelay = 0;
    score = 0;
    numlives = 3;
    difficulty = 240;
    gameover = false;

    //play background music
    pGame->continueMusic("music");
}

void AddAlien()
{
    //create a new random alien sprite
    sf::FloatRect rcBounds(0,0,600,410);
    AlienSprite* pSprite = nullptr;
    switch( rnd.getRndInt(0,2) )
    {
    case 0:
        //Blobbo
        pSprite = new AlienSprite("blobbo", rcBounds, BA_BOUNCE);
        pSprite->SetName("blobbo");
        pSprite->setNumFrames(8);
        pSprite->SetPosition( rnd.getRndInt(0,1) == 0 ? 0: 600, rnd.getRndInt(0,369) );
        pSprite->SetVelocity( rnd.getRndInt(0,42) - 14, rnd.getRndInt(0,42) - 14 );
        break;
    case 1:
        //Jelly
        pSprite = new AlienSprite("jelly", rcBounds, BA_BOUNCE);
        pSprite->SetName("jelly");
        pSprite->setNumFrames(8);
        pSprite->SetPosition( rnd.getRndInt(0,1) == 0 ? 0: 600, rnd.getRndInt(0,369) );
        pSprite->SetVelocity( rnd.getRndInt(0,28) - 14, rnd.getRndInt(0,28) + 21 );
        break;
    case 2:
        //Timmy
        pSprite = new AlienSprite("timmy", rcBounds, BA_BOUNCE);
        pSprite->SetName("timmy");
        pSprite->setNumFrames(8);
        pSprite->SetPosition( rnd.getRndInt(0,1) == 0 ? 0: 600, rnd.getRndInt(0,369) );
        pSprite->SetVelocity( rnd.getRndInt(0,42) + 21, 0 );
        break;
    }

    //add the alien sprite
    pGame->AddSprite(pSprite);
}
