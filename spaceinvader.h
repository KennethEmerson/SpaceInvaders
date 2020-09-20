#ifndef SPACEINVADER_H_
    #define SPACEINVADER_H_

    #define TIME_BETWEEN_GAME_UPDATES 100         //time between process cycles
    
    #define FIRE_UPDATE_DELAY         4           //how many process cycles between fire movement (higher is slower speed of bullet)
    #define ENEMY_FIRE_RANDOMNESS     20          //chance of enemy firing (higher number is a lower chance)
    #define FIRE_BRIGHTNESS           125         //the brightness of the LED for fire 
    
    #define ENEMYSPEED_TRIGGER        2           //after how many points will the speed be increased (minimal 1)
    #define ENEMYSPEED_MULTIPLIER     2           //how much is the speed increased (0 is no speed increasement)
    #define ENEMYSPEED_BOTTOM         8           //minimal amount of process cycles between enemy movement (minimal 1)
    #define ENEMYSPEED_INITIAL        68          //initial amount of process cycles between enemy movement (minimal 1)
    
    #define ENEMYCREATE_TRIGGER       4           //after how many points will the creation speed be increased (minimal 1) 
    #define ENEMYCREATE_MULTIPLIER    5           //how much is the creation speed increased (0 is no speed increasement)
    #define ENEMYCREATE_BOTTOM        8           //minimal amount of process cycles between enemy creation (minimal 1)
    #define ENEMYCREATE_INITIAL       68          //initial amount of process cycles between enemy creation (minimal 1)
    
    #define ENEMY_LIFES               2           //determines amount of times a enemy needs to be shot
    #define PLAYER_LIFES              5           //determines the amount of lifes of player 
    #define MAXSCORE                  60          //score to reach to win game

    
    //-----------------------------------------------------------
    // player struct
    //-----------------------------------------------------------
    struct Player{
        uint8_t position;            // 0 to 4 depending on vertical position of player                       
        uint8_t lifes;               // the actual amount of lifes left
        int score;                   // the actual score (int to allow > 255)
    };

    //-----------------------------------------------------------
    // player related methods
    //-----------------------------------------------------------
    
    void playerUp(Player *player);
    void playerDown(Player *player);
    void playerShoot(Player *player,MicroBitImage *fiendlyFire);
    void checkIfPlayerIsHit(MicroBitImage *enemyFire, Player *player, uint8_t *gameOver);
    void checkFireCollision(MicroBitImage *friendlyFire,MicroBitImage *enemyFire);
    
    //-----------------------------------------------------------
    // enemy related methods
    //-----------------------------------------------------------
    
    int noEnemiesLeft(MicroBitImage *enemies);
    int hasEnemyReachedEnd(MicroBitImage *enemies);
    void createEnemy(MicroBit *uBit,MicroBitImage *enemies, int enemyMinimumBrightness);
    void moveEnemy(MicroBitImage *enemies,Player *player, uint8_t *gameOver);
    void enemyShoot(MicroBit *uBit,MicroBitImage *enemies,MicroBitImage *enemyFire);
    void checkIfEnemyIsHit(Player *player,MicroBitImage *enemies, MicroBitImage *friendlyFire, int enemyMinimumBrightness);
    int calculateEnemyCreateDelay(int score);
    int calculateEnemySpeedDelay(int score);
    
    //-----------------------------------------------------------
    //general game related methods
    //-----------------------------------------------------------
    
    void saveGame(MicroBit *uBit,Player player,MicroBitImage *enemies,int enemyMinimumBrightness);
    void loadGame(MicroBit *uBit,Player *player,MicroBitImage *enemies,int enemyMinimumBrightness);
    void addImageToTotalImage(MicroBitImage *totalImage, MicroBitImage image);
    
#endif /* SPACEINVADER_H_ */