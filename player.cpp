#include "MicroBit.h"
#include "spaceinvader.h"

//-----------------------------------------------------------------------
// playerUp: all actions to be taken when player is moved up
//-----------------------------------------------------------------------

void playerUp(Player *player){
        if(player->position > 0) player->position--; 
}
    
//-----------------------------------------------------------------------
// playerDown: all actions to be taken when player is moved down
//-----------------------------------------------------------------------    

void playerDown(Player *player){
    if(player->position <4) player->position++;
}  

//-----------------------------------------------------------------------
// playerShoot: all actions to be taken when the player shoots
//-----------------------------------------------------------------------   

void playerShoot(Player *player,MicroBitImage *friendlyFire){
    friendlyFire->setPixelValue(0,player->position,FIRE_BRIGHTNESS);
}

//-----------------------------------------------------------------------
// checkIfPlayerIsHit: checks if player is hit and reduces life
//-----------------------------------------------------------------------   

void checkIfPlayerIsHit(MicroBitImage *enemyFire, Player *player, uint8_t *gameOver){
    //check if player is hit and if so decrease lifes by one
    if(enemyFire->getPixelValue(0,player->position)) player->lifes--;
    
    //check if player has no more lifes left and stop game if so
    if(player->lifes <= 0) *gameOver = 1;
}

//-----------------------------------------------------------------------
// checkFireCollision: checks if two bullets collide and delets both if
//                     applicable
//-----------------------------------------------------------------------   

void checkFireCollision(MicroBitImage *friendlyFire,MicroBitImage *enemyFire){
    for(int x = 0; x < 5;x++){
        for(int y= 0;y < 5;y++){
            if(friendlyFire->getPixelValue(x,y) && enemyFire->getPixelValue(x,y)){
                friendlyFire->setPixelValue(x,y,0);
                enemyFire->setPixelValue(x,y,0);
            }
        }
    }
}
