#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "GameEngine.h"

//global variables
GameEngine* pGame;
sf::Texture skel;
sf::Sprite Skel;


bool GameInitialize()
{
    //create the game engine
    pGame = new GameEngine("Game Skeleton", 500, 500);

    if( pGame == nullptr )
    {
        return false;
    }

    //set the frame rate
    pGame->SetFrameRate(15);

    return true;
}

void GameStart()
{
    srand(time(NULL));
    skel.loadFromFile("skeleton.png");
    Skel.setTexture(skel);
}

void GameEnd()
{
    GameEngine::GetEngine()->window.close();
    delete pGame;
}

void GameActivate()
{
    pGame->window.setTitle("Game Skeleton (act)");
}

void GameDeactivate()
{
    pGame->window.setTitle("Game Skeleton (deact)");
}

void GamePaint(sf::RenderWindow &window)
{
    window.draw(Skel);
    window.display();
}

void GameCycle(sf::Time delta)
{
    Skel.setPosition((float) (rand() % pGame->GetWidth()), (float) (rand() % pGame->GetHeight()));
}
