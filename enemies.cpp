#include "MicroBit.h"
#include "spaceinvader.h"

//-----------------------------------------------------------------------
// noEnemiesLeft: checks if there are any enemies left on the screen
//-----------------------------------------------------------------------

int noEnemiesLeft(MicroBitImage *enemies){
    int flag = 1;
    for (int x=0; x<5; x++){
        for (int y=0; y<5; y++){
            if (enemies->getPixelValue(x,y)) flag = 0;
        }
    }
    return flag;
}   

//-----------------------------------------------------------------------
// hasEnemyReachedEnd: checks if any enemy has reached left end of screen
//-----------------------------------------------------------------------

int hasEnemyReachedEnd(MicroBitImage *enemies){
    int pixelcount = 0;
    for (int y = 0; y < 5; y++)
        if (enemies->getPixelValue(0,y)) pixelcount++;
    return pixelcount/2;
}

//-----------------------------------------------------------------------
// createEnemy: places a new enemy on a random position if position is
//              not taken
//-----------------------------------------------------------------------

void createEnemy(MicroBit *uBit,MicroBitImage *enemies, int enemyMinimumBrightness ){

    //randomly choice where to place new enemy 
    int r = uBit->random(4);     
            
    //check if there is't already an enemy on the selected position
    if(!enemies->getPixelValue(4,r) && !enemies->getPixelValue(4,r+1)){
          enemies->setPixelValue(4,r,enemyMinimumBrightness*ENEMY_LIFES);
          enemies->setPixelValue(4,r+1,enemyMinimumBrightness*ENEMY_LIFES);
    }
    
}

//-----------------------------------------------------------------------
// moveEnemy: moves enemies and checks if they reached the end
//-----------------------------------------------------------------------

void moveEnemy(MicroBitImage *enemies,Player *player, uint8_t *gameOver){      
        player->lifes -= hasEnemyReachedEnd(enemies);
        if(player->lifes == 0) *gameOver = 1;
        enemies->shiftLeft(1);
}

//-----------------------------------------------------------------------
// enemyShoot: randomly generates enemy fire           
//-----------------------------------------------------------------------

void enemyShoot(MicroBit *uBit,MicroBitImage *enemies,MicroBitImage *enemyFire){    
    for(int x = 0; x < 5;x++){
        for(int y= 0;y < 4;y++){
            //check if the cell contains an enemy
            if(enemies->getPixelValue(x,y)){
                //create random value
                int r = uBit->random(ENEMY_FIRE_RANDOMNESS);
                //if random value is zero, fire
                if(!r){
                    enemyFire->setPixelValue(x,y,FIRE_BRIGHTNESS);                    
                }
            }
        }  
    }
}

//-----------------------------------------------------------------------
// checkIfEnemyIsHit:  checks if enemy is hit.
//                     if so it deletes the bullet reduces the enemy's
//                     life (and LED brightness)
//                     if the enemy is dead, the score is adjusted         
//-----------------------------------------------------------------------

void checkIfEnemyIsHit(Player *player,MicroBitImage *enemies, MicroBitImage *friendlyFire, int enemyMinimumBrightness){
    uint8_t enemyPixValUp = 0;      //the brightness value of the enemy's upper LED
    uint8_t enemyPixValDown = 0;    //the brightness value of the enemy's lower LED
    uint8_t friendlyFireUp = 0;     //the brightness value of the plausible bullet in the upper LED
    uint8_t friendlyFireDown = 0;   //the brightness value of the plausible bullet in the lower LED
    
    for(int x = 0; x < 5;x++){
        int t = 1;
        for(int y = 0 ; y < 4;y+=t){
            //from the moment a enemy is found proceed 2 LEDS at a time
            t = (enemies->getPixelValue(x,y)) ? 2 : 1;
            //put all the LED values in the temp variables
            enemyPixValUp = enemies->getPixelValue(x,y);
            enemyPixValDown = enemies->getPixelValue(x,y+1);
            friendlyFireUp = friendlyFire->getPixelValue(x,y);
            friendlyFireDown = friendlyFire->getPixelValue(x,y+1);
            
            //check if the enemy is hit
            if(enemyPixValUp && enemyPixValDown && (friendlyFireUp || friendlyFireDown)){ 
                
                //if the bullet is in the enemy's upper LED, delete the bullet
                if(friendlyFireUp) friendlyFire->setPixelValue(x,y,0);       
                
                //if the bullet is in the enemy's upper LED, delete the bullet             
                if(friendlyFireDown)friendlyFire->setPixelValue(x,y+1,0);
                
                //calculate the new enemy LED brightness
                enemyPixValUp -= enemyMinimumBrightness;
                
                //adjust the enemy's LED brightness 
                enemies->setPixelValue(x,y,enemyPixValUp);
                enemies->setPixelValue(x,y+1,enemyPixValUp);
                
                //adjust score if enemy is dead
                if(!enemyPixValUp) player->score++;                           
                y++;        //TODO: check if this is necessary              
                   
            }
        }
    }
}

//-----------------------------------------------------------
// method to calculate impact of score on timer variable
//-----------------------------------------------------------

int calculateEnemyCreateDelay(int score){
    int tempValue = ENEMYCREATE_INITIAL - ((score/ENEMYCREATE_TRIGGER)*ENEMYCREATE_MULTIPLIER);
    if(tempValue < ENEMYCREATE_BOTTOM) return ENEMYCREATE_BOTTOM;
    else return tempValue;
}

//-----------------------------------------------------------
// method to calculate impact of score on timer variable
//-----------------------------------------------------------

int calculateEnemySpeedDelay(int score){
    int tempValue = ENEMYSPEED_INITIAL - ((score/ENEMYSPEED_TRIGGER)*ENEMYSPEED_MULTIPLIER);
    if(tempValue < ENEMYSPEED_BOTTOM) return ENEMYSPEED_BOTTOM;
    else return tempValue;
}
