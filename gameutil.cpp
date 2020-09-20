#include "MicroBit.h"
#include "spaceinvader.h"

//-----------------------------------------------------------------------
// saveGame:  stores the player's position, lifes, score and level
//            and the enemy's position and lifes in the flash memory.
//            For every LED position the enemy life is stored in 4 bits                    
//            thus 1 byte contains 2 enemy positions
//            The player's score (int) is divided into two uint8_t
//            and saved as such.
//-----------------------------------------------------------------------

void saveGame(MicroBit *uBit,Player player,MicroBitImage *enemies,int enemyMinimumBrightness){
    
    uint8_t savedData[19];          //temp array to store all data to be saved
    
    int     mask8LSB = 255;         //mask: 0000 0000 1111 1111
    int     mask8MSB = 65280;       //mask: 1111 1111 0000 0000
    uint8_t mask4LSB = 15;          //mask: 0000 1111
    uint8_t mask4MSB = 240;         //mask: 1111 0000
    
    uint8_t tempEnemy = 0;          //temp variable to store enemy's lifes
    uint8_t tempStorageValue;       //temp variable to compose 
    
    *savedData = player.position;
    *(savedData + 1) =  player.lifes;
    
    //the score is divided into two separate uint8_t and stored
    *(savedData + 2) =  (player.score & mask8MSB) >> 8;
    *(savedData + 3) =  player.score & mask8LSB;
    
    //counts the number of bytes tha will be stored in flash memory
    // first 5 bytes are used above
    uint8_t savedByteCounter = 4;   

    //for each enemy LED the brightness value (which is a measure for the enemy's life)
    //will be transformed to the life value and stored in 4 bits
    for(int x = 0; x <5; x++){
        for(int y =0; y < 5; y++){  
           
           //stores the LED brightness as a life value
           tempEnemy = enemies->getPixelValue(x,y)/enemyMinimumBrightness;
           
           //if y is even, value is stored in MSB
           if(!(y%2)){
                tempStorageValue = (tempEnemy << 4);
           }
           
           //the fifth LED is stored alone
           if(y == 4){
               *(savedData + savedByteCounter) = tempStorageValue;
               savedByteCounter++;
           }   
           
           //if y is uneven, value is stored in LSB
           if(y%2){
               tempStorageValue = tempStorageValue | (tempEnemy & mask4LSB);
               *(savedData + savedByteCounter) = tempStorageValue;
               savedByteCounter++;
           }
        }
    }
    //store all values in Flash memory
    uBit->storage.put("game", savedData,20);

}

void loadGame(MicroBit *uBit,Player *player,MicroBitImage *enemies,int enemyMinimumBrightness){
    KeyValuePair* firstTime;                        //pointer to loaded game
    firstTime = uBit->storage.get("game");          //get pointer to heap allocated KeyValuePair struct
    if(firstTime != NULL){                          //if key was found (no NULL pointer is returned)
        
        int     mask8LSB = 255;                     //mask: 0000 0000 1111 1111
        int     mask8MSB = 65280;                   //mask: 1111 1111 0000 0000
        uint8_t mask4LSB = 15;                      //mask: 0000 1111
        uint8_t mask4MSB = 240;                     //mask: 1111 0000
        
        uint8_t *savedData = firstTime->value;      //get starting pointer of byte array loaded from Flash memory
        uint8_t savedByteCounter = 4;               //count byte position from starting pointer
        uint8_t enemyLifeValue = 0;                 //temporarely stores the amount of lifes                 
        int tempscore = 0;                          //temp value used to compose the score
        
        player->position = *savedData;               //load player position from first byte
        player->lifes = *(savedData + 1);            //load player lifes from second byte
        
        //the score is divided into two separate uint8_t and therefore recomposed
        tempscore = (*(savedData + 2)& mask8LSB) <<8;
        tempscore = tempscore | (*(savedData + 3) & mask8LSB);
        player->score = tempscore;                   
        
        //for each enemy LED the life value which is stoerd in 4 bits
        //(and which is a measure for the LED brightness)
        //will be transformed to the LED brightness.  
        for(int x = 0; x <5; x++){
            for(int y = 0; y < 5; y++){
                
                //if y is even, the life value will be taken from the MSB
                if(!(y%2)){ 
                    enemyLifeValue = ((*(savedData + savedByteCounter)) & mask4MSB)>>4;
                    enemies->setPixelValue(x,y, enemyLifeValue * enemyMinimumBrightness);
                }
                
                //the fifth LED is stored alone
                if(y==4){
                    savedByteCounter++;
                } 
                
                //if y is uneven, the life value will be taken from the LSB
                if(y%2){
                    enemyLifeValue = (*(savedData + savedByteCounter) & mask4LSB);
                    enemies->setPixelValue(x,y,enemyLifeValue * enemyMinimumBrightness);
                    savedByteCounter++;
                }
            }
        }
    }
}

//-----------------------------------------------------------
// method to add image to totalImage
//-----------------------------------------------------------
void addImageToTotalImage(MicroBitImage *totalImage, MicroBitImage image)
{
    for(int x = 0; x < 5;x++){
        for(int y= 0;y < 5;y++){
            if(image.getPixelValue(x,y) > totalImage->getPixelValue(x,y)){
                totalImage->setPixelValue(x,y,image.getPixelValue(x,y));
            }
        }
    }
}
