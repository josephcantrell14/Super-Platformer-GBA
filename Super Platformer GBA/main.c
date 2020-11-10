#include "lib.h"
#include "endScreen.h"
#include "gameScreen.h"
#include "startScreen.h"


struct PlayerStruct{
    int x;
    int y;
    int height;
    int width;
    int score;
    int jumpMaxIncrements;
    int dy;
    int state;
    int collision;
};
struct PlatformStruct {
    int x;
    int y;
    int width;
    int height;
};
struct PrincessStruct {
    int x;
    int y;
    int width;
    int height;
};

int jumpCalc(struct PlayerStruct player);
int collision(struct PlayerStruct player, struct PlatformStruct o);
void endGame();
int jumpCounter = 0;

int main(void) {
    //setup player struct
    struct PlayerStruct player;
    player.x = 10;
    player.y = 136;
    player.height = PLAYERRIGHT_HEIGHT;
    player.width = PLAYERRIGHT_WIDTH;
    player.score = 0;
    player.jumpMaxIncrements = 20;
    player.dy = 1;
    player.state = 0;
    player.collision = 0;
    jumpCounter = 0;
    //setup princess struct
    struct PlatformStruct aprincess;
    aprincess.x = 190;
    aprincess.y = 10;
    aprincess.width = PRINCESS_WIDTH;
    aprincess.height = PRINCESS_HEIGHT;
    //setup food
    struct PlatformStruct food[4];
    food[0].x = 131;
    food[0].y = 135;
    food[0].width = 5;
    food[0].height = 5;
	food[1].x = 83;
    food[1].y = 38;
    food[1].width = 5;
    food[1].height = 5;
	food[2].x = 155;
    food[2].y = 94;
    food[2].width = 5;
    food[2].height = 5;
	food[3].x = 8;
    food[3].y = 112;
    food[3].width = 5;
    food[3].height = 5;    
    //four map edges setup
    struct PlatformStruct boundaries[4];
    //top
    boundaries[0].x = 0;
    boundaries[0].y = 0;
    boundaries[0].width = 240;
    boundaries[0].height = 5;
    //left
    boundaries[1].x = 0;
    boundaries[1].y = 0;
    boundaries[1].width = 5;
    boundaries[1].height = 160;
    //bottom
    boundaries[2].x = 0;
    boundaries[2].y = 150;
    boundaries[2].width = 240;
    boundaries[2].height = 10;
    //right
    boundaries[3].x = 236;
    boundaries[3].y = 0;
    boundaries[3].width = 5;
    boundaries[3].height = 160;

    //platforms setup
    struct PlatformStruct platforms[18];
    //left side of the map
    platforms[0].x = 47;
    platforms[0].y = 30;
    platforms[0].width = 100;
    platforms[0].height = 8;
    platforms[1].x = 35;
    platforms[1].y = 94;
    platforms[1].width = 11;
    platforms[1].height = 23;
    platforms[2].x = 27;
    platforms[2].y = 68;
    platforms[2].width = 34;
    platforms[2].height = 10;
    platforms[3].x = 70;
    platforms[3].y = 55;
    platforms[3].width = 35;
    platforms[3].height = 7;
    platforms[4].x = 123;
    platforms[4].y = 70;
    platforms[4].width = 14;
    platforms[4].height = 10;
    platforms[5].x = 93;
    platforms[5].y = 80;
    platforms[5].width = 44;
    platforms[5].height = 12;
    platforms[6].x = 4;
    platforms[6].y = 117;
    platforms[6].width = 73;
    platforms[6].height = 10;
    platforms[7].x = 103;
    platforms[7].y = 117;
    platforms[7].width = 34;
    platforms[7].height = 10;
    //center column of the map
    platforms[8].x = 137;
    platforms[8].y = 38;
    platforms[8].width = 10;
    platforms[8].height = 112;
    //right side of the map
    platforms[9].x = 147;
    platforms[9].y = 49;
    platforms[9].width = 15;
    platforms[9].height = 6;
    platforms[10].x = 207;
    platforms[10].y = 138;
    platforms[10].width = 31;
    platforms[10].height = 12;
    platforms[11].x = 188;
    platforms[11].y = 110;
    platforms[11].width = 29;
    platforms[11].height = 9;
    platforms[12].x = 188;
    platforms[12].y = 5;
    platforms[12].width = 11;
    platforms[12].height = 105;
    platforms[13].x = 220;
    platforms[13].y = 85;
    platforms[13].width = 16;
    platforms[13].height = 7;
    platforms[14].x = 232;
    platforms[14].y = 71;
    platforms[14].width = 4;
    platforms[14].height = 14;
    platforms[15].x = 199;
    platforms[15].y = 49;
    platforms[15].width = 20;
    platforms[15].height = 7;
    //second falling obstacle
    platforms[16].x = 167;
    platforms[16].y = 84;
    platforms[16].width = 23;
    platforms[16].height = 6;
    //helper platform
    platforms[17].x = 4;
    platforms[17].y = 48;
    platforms[17].width = 23;
    platforms[17].height = 13;


    REG_DISPCNT = MODE4 | BG2_ENABLE;
    //start screen
    loadPalette(0);
    while(1) {
    	drawImage4(0, 0, STARTSCREEN_WIDTH, STARTSCREEN_HEIGHT, startScreen);
    	waitForVBlank();
    	flipPage();
        if (KEY_DOWN_NOW(BUTTON_START)) {
            break;
        }
    }
    loadPalette(1);
    while(1) {
    	//draw background color
	    drawRect4(0, 0, 240, 160, bgColor);
	    drawRect4(0, 0, 5, 160, black);
	    //draw boundary elements
	    for (int i = 0; i < 4; i++) {
	        drawRect4(boundaries[i].x, boundaries[i].y, boundaries[i].width,
	            boundaries[i].height, black);
	    }
	    //draw platforms
	    for (int i = 0; i < 18; i++) {
	        drawRect4(platforms[i].x, platforms[i].y, platforms[i].width, platforms[i].height, red);
	    }    	
	    //draw food
	    for (int i = 0; i < 4; i++) {
	    	drawRect4(food[i].x, food[i].y, food[i].width, food[i].height, gold);
	    }
	    //draw player in left or right depiction depending on last player input
        if (player.state == 0) {
        	drawImage4(player.x, player.y, player.width, player.height, playerRight);
        } else {
        	drawImage4(player.x, player.y, player.width, player.height, playerLeft);        	
        }
        //draw princess	
        drawImage4(aprincess.x, aprincess.y, aprincess.width, aprincess.height, princess);
        //enable return to main menu with select
        if (KEY_DOWN_NOW(BUTTON_SELECT)) {
        	main();
        }

        //check for collision
        if (player.collision == 0) {
	        for (int i = 0; i < 18; i++) {
	        	player.collision = collision(player, platforms[i]);
	        	if (player.collision > 0) {     		     		
	        		endGame();
	        	}
	        }
    	}
    	//check for food collision
    	//if it collides, move the coins location and add to the player's score
    	for (int i = 0; i < 4; i++) {
    		if (collision(player, food[i]) != 0) {
    			player.score++;
    			food[i].x = (5 + 7*i);
    			food[i].y = 153;
    		}
    	}
    	//check for princess collision
        if (collision(player, aprincess) != 0) {
        	endGame(player.score);
        }    	
        //player input
        if (KEY_DOWN_NOW(BUTTON_RIGHT) && (player.x < boundaries[3].x)) {
        	player.x++;
        	player.state = 0;       	
        }
        if (KEY_DOWN_NOW(BUTTON_LEFT) && (player.x > 5)) {       	
        	player.x--;
        	player.state = 1;       	
        }     
        if ((KEY_DOWN_NOW(BUTTON_UP)) && (jumpCounter == 0) && (player.y > 5)) {
        	player.y--;
        }
        if (KEY_DOWN_NOW(BUTTON_DOWN) && ((player.y + player.width) < boundaries[2].y)) {
        	player.y++;
        }
        if (jumpCounter != 0) {
        	player.y = jumpCalc(player);
        }
        waitForVBlank();
        flipPage();           
    }
    return 0;
    
}

//calculate the player's changes in position after jumping
int jumpCalc(struct PlayerStruct player) {
	//scenario where player is rising
    if (jumpCounter < player.jumpMaxIncrements) {
	    jumpCounter++;
	    player.y -= player.dy;  
	    if (player.y < 5) {
	    	player.y = 5;
	    }	    	     
	    return player.y;  		    
	} 
	//scenario where player is at max height
	if (jumpCounter == player.jumpMaxIncrements) {
		jumpCounter *= -1;
		jumpCounter++;
		return player.y;
	}
	//scenario where player is falling
	if (jumpCounter < 0) {
		jumpCounter++;
		player.y += player.dy; 
		return player.y;
	}
	return player.y;
}


//checks whether each corner of the player is within an object
//returns which corner is within the object, if any
int collision(struct PlayerStruct player, struct PlatformStruct o) {
	//check bottom right corner
	if ( 
		((player.x + player.width) > o.x) &&
		 ((player.x + player.width) < (o.x + o.width)) &&
		 ((player.y + player.height) > o.y) &&
		 ((player.y + player.height) < (o.y + o.height))  ) {
		 	return 1;
		 }		 
	//check bottom left corner
	if ( 
		(player.x > o.x) &&
		 (player.x < (o.x + o.width)) &&
		 ((player.y + player.height) > o.y) &&
		 ((player.y + player.height) < (o.y + o.height))  ) {
		 	return 1;
		 }	
	//check top left corner
	if ( 
		(player.x > o.x) &&
		 (player.x < (o.x + o.width)) &&	
		 (player.y > o.y) &&
		 (player.y < (o.y + o.height))  ) {
		 	return 1;
		 }			 
	//check top right corner
	if ( 
		((player.x + player.width) > o.x) &&
		 ((player.x + player.width) < (o.x + o.width)) &&	
		 (player.y > o.y) &&
		 (player.y < (o.y + o.height))  ) {
		 	return 1;
		 }	
	return 0;
}

void endGame(int finalScore) {
	loadPalette(2);
	while (1) {
		drawImage4(0, 0, ENDSCREEN_WIDTH, ENDSCREEN_HEIGHT, endScreen);	
		for (int i = 0; i < finalScore; i++)	{
			drawRect4(80 + 15*i, 80, 10, 10, gold);
		}
		if (finalScore == 0) {
			drawRect4(110, 80, 25, 10, white);
			drawRect4(110, 80, 10, 40, white);
			drawRect4(125, 80, 10, 40, white);
			drawRect4(110, 110, 25, 10, white);
		}
		if ((KEY_DOWN_NOW(BUTTON_START)) || (KEY_DOWN_NOW(BUTTON_SELECT))) {
			main();
		}
		waitForVBlank();
		flipPage();
	}
}