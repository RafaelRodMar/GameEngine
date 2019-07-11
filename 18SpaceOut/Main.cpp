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

#include "Global.h"
#include "CSprite.h"
#include "AlienSprite.h"
#include "Background.h"
#include "GameEngine.h"

GameEngine *pGame;
StarryBackground* pBackground;
CSprite* pCarSprite;

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

void NewGame()
{
    //clear the sprites
    pGame->CleanupSprites();

    //create the car sprite
    sf::FloatRect rcBounds(0,0,600,450);
    pCarSprite = new CSprite(pGame->getTexture("car"), rcBounds, BA_WRAP);
    pCarSprite->SetPosition(300,405);
    pCarSprite->SetName("car");
    pGame->AddSprite(pCarSprite);

    //initialize the game variables
    fireinputdelay = 0;
    score = 0;
    numlives = 3;
    difficulty = 80;
    gameover = false;

    //play background music
    pGame->continueMusic("music");
}

void AddAlien()
{
}
