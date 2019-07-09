#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
Texture txtforest;
Texture txtgolfball;
CSprite *spgolfballsprite[3];
CSprite *spforest;
bool dragging;
int  dragball;

bool GameInitialize()
{
    //create the game engine
    pGame = new GameEngine("Fore", 600, 400);
    if(pGame == nullptr) return false;

    //set the frame rate
    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    srand(time(NULL));

    //load the textures
    txtforest.load("forest.png");
    txtgolfball.load("golfball.png");

    //create the golf ball sprites
    spforest = new CSprite(txtforest.texture);

    sf::IntRect rcBounds(0,0,600,400);
    spgolfballsprite[0] = new CSprite(txtgolfball.texture, rcBounds);
    spgolfballsprite[1] = new CSprite(txtgolfball.texture, rcBounds, BA_WRAP);
    spgolfballsprite[2] = new CSprite(txtgolfball.texture, rcBounds, BA_BOUNCE);

    spgolfballsprite[0]->SetVelocity(2,1);
    spgolfballsprite[1]->SetVelocity(3,-2);
    spgolfballsprite[2]->SetVelocity(7,4);

    //set the initial drag info
    dragging = false;
    dragball = -1;
}

void GameEnd()
{
    //cleanup the sprites
    delete spforest;
    for(int i=0;i<3;i++)
        delete spgolfballsprite[i];

    //cleanup the game engine
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
    //draw the background forest
    spforest->Draw(window);

    //draw the golf ball sprites
    for(int i=0;i<3;i++)
        spgolfballsprite[i]->Draw(window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    //update the golf ball sprites
    for(int i=0;i<3;i++)
        spgolfballsprite[i]->Update();

    //check if a ball was clicked with the left mouse button
    if( pGame->mouseClicked && pGame->mouseLeftButton && !dragging)
    {
        int x = pGame->mousePos.x;
        int y = pGame->mousePos.y;

        for(int i=0;i<3;i++)
        {
            if(spgolfballsprite[i]->IsPointInside(x,y))
            {
                //set the drag state and the drag ball
                dragging = true;
                dragball = i;
                //don't check for more balls
                break;
            }
        }
    }

    if( !pGame->mouseLeftButton && dragging ) dragging = false;

    if( pGame->mouseLeftButton && dragging )
    {
        //move the sprite to the mouse cursor position
        int offx = spgolfballsprite[dragball]->GetWidth() / 2;
        int offy = spgolfballsprite[dragball]->GetHeight() / 2;
        int moux = sf::Mouse::getPosition(pGame->window).x;
        int mouy = sf::Mouse::getPosition(pGame->window).y;
        spgolfballsprite[dragball]->SetPosition(moux - offx, mouy - offy);
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
