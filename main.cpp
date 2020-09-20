
#include "MicroBit.h"
#include "spaceinvader.h"

MicroBit uBit;
  
MicroBitImage   friendlyFire(5,5);         //image that contains all frineldy fire
MicroBitImage   enemies(5,5);              //image that contains all enemies
MicroBitImage   enemyFire(5,5);            //image that contains all enemy fire
MicroBitImage   totalImage(5,5);           //image that is composed of the three images above + the player

uint8_t gameOver;                          //flag that determines if game is over
Player player;                                         

//the minimum brightness of the player and enemy, equals one life
int playerMinimumBrightness = 255/PLAYER_LIFES;        
int enemyMinimumBrightness = 255/ENEMY_LIFES;         


//-----------------------------------------------------------
// Player actions (eventdriven)
//-----------------------------------------------------------

void playerUpEvent(MicroBitEvent) {playerUp(&player);}
void playerDownEvent(MicroBitEvent){playerDown(&player);}
void playerShootEvent(MicroBitEvent){playerShoot(&player,&friendlyFire);}
           
//-----------------------------------------------------------
// methods to update enemy and friendly fire
//-----------------------------------------------------------

void updateFriendlyFire(){
        //check if an enemy has run into a bullet
        checkIfEnemyIsHit(&player,&enemies,&friendlyFire,enemyMinimumBrightness);
        //move all friendly bullets to right
        friendlyFire.shiftRight(1);
        //check if friendly bullets have hit an enemy bullet
        checkFireCollision(&friendlyFire,&enemyFire);
        //check if the friendly bullet has hit an enemy
        checkIfEnemyIsHit(&player,&enemies,&friendlyFire,enemyMinimumBrightness);
}
              
void updateEnemyFire(){
        //randomly fire enemy bullets
        enemyShoot(&uBit,&enemies,&enemyFire);
        //move all enemy bullets to left
        enemyFire.shiftLeft(1);
        //check if enemy bullets have hit a friendly bullet
        checkFireCollision(&friendlyFire,&enemyFire);
        //check if the enemy bullet has hit the player
        checkIfPlayerIsHit(&enemyFire, &player, &gameOver); 
}

//-----------------------------------------------------------
// event to save a game in flash memory
//-----------------------------------------------------------

void saveGameEvent(MicroBitEvent){
    saveGame(&uBit,player,&enemies,enemyMinimumBrightness);
}

//-----------------------------------------------------------
// method to load a game from flash memory
//-----------------------------------------------------------

void loadGameEvent(MicroBitEvent){
        enemies.clear();                 //empties the enemies image
        friendlyFire.clear();            //empties the friendly fire image
        enemyFire.clear();               //empties the enemy fire image 
        
        loadGame(&uBit,&player,&enemies,enemyMinimumBrightness); 
}

//-----------------------------------------------------------
// main thread that controls the game
//-----------------------------------------------------------
void game(){
    
    player.position = 2;            //player start in center
    player.lifes = PLAYER_LIFES;    //initialize amount of player lifes
    player.score = 0;               //set score to zero
    gameOver = 0;                   //set game over flag to zero
    
    int counter = 0;                //counter is used to determine which actions to trigger
    int enemyMoveDelay;             //number of process cycles between enemy moves
    int enemyCreateDelay;           //number of process cycles between enemy creation
    
    //initialize all eventhandlers
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, playerUpEvent);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, playerDownEvent);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, saveGameEvent);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, loadGameEvent);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, playerShootEvent);
    
    //start screen update loop
    while (!gameOver){    
        
        //calculate the number of process cycles between enemy move/creation
        enemyMoveDelay = calculateEnemySpeedDelay(player.score);
        enemyCreateDelay = calculateEnemyCreateDelay(player.score);
        
        //update friendly fire if update delay is reached
        if(!(counter%FIRE_UPDATE_DELAY)) updateFriendlyFire();

        //clear total image and add fire, enemies and player
        totalImage.clear();                                     
        addImageToTotalImage(&totalImage,friendlyFire);         
        addImageToTotalImage(&totalImage,enemyFire);            
        addImageToTotalImage(&totalImage,enemies);              
        totalImage.setPixelValue(0, player.position, playerMinimumBrightness*player.lifes);
        
        //put total image to screen
        uBit.display.image.paste(totalImage);
        
        uBit.sleep(TIME_BETWEEN_GAME_UPDATES/2);
        
        //update enemy movement if update delay is reached
        if(!(counter%enemyMoveDelay)) {
            moveEnemy(&enemies,&player, &gameOver);
        }
        
        //initialize enemy creation if update delay is reached
        if(!(counter%enemyCreateDelay)|| noEnemiesLeft(&enemies)){
            createEnemy(&uBit,&enemies,enemyMinimumBrightness);    
        } 
        
        //update enemy fire if update delay is reached
        if(!(counter%FIRE_UPDATE_DELAY)) updateEnemyFire();
        
        //clear total image and add fire enemies and player
        totalImage.clear();                                     
        addImageToTotalImage(&totalImage,friendlyFire);         
        addImageToTotalImage(&totalImage,enemyFire);            
        addImageToTotalImage(&totalImage,enemies);           
        totalImage.setPixelValue(0, player.position, playerMinimumBrightness*player.lifes);
        
        //put total image to screen
        uBit.display.image.paste(totalImage);
        
        //adjust counter and check if maximum score is reached
        counter++;
        if(counter > 1000) counter = 0;
        if(player.score>= MAXSCORE) gameOver = 1;
        
        //wait for next loop
        uBit.sleep(TIME_BETWEEN_GAME_UPDATES/2);
    }
}


//-----------------------------------------------------------
// main method to initialize and end game
//-----------------------------------------------------------


int main(){
    //initialize microbit
    uBit.init();                                            

    //allow varying LED brightness
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);    
    
    //play game loop
    while(!gameOver) game();                                
    
    //print score when game is lost or "you won" when won
    uBit.display.clear();
    if(player.score< MAXSCORE){
        uBit.display.scroll(player.score,200);
    }
    else uBit.display.scroll("You won",200);              
    
    //end all fibers
    release_fiber();                                        
}

