//Lab 09 Design and implement a video game
//EE215L
//May 12, 2016

#include <msp430F5438A.h>
#include "hal_MSP-EXP430F5438.h"
//#include <stdio.h>

/*
Design and Implement a Video Game
There are a number of classic computer games that are fun to play and possible
to emulate on the MSP 430 processor and experiment board.
Assignment: Write code in C on the MSP 430 processor to implement an Atari style
video game. The game should use the LCD display for output and one or more input
devices (push buttons, 5-way joystick button, or accelerometers) for controlling
the game.

Game Chosen: Space Invaders
*/

///////////////////////////////////////////////////////////////alein/enemy ships
// functions to draw the objects on the LCD screen
int enemy_ship(x){
	halLcdHLine(x+7,x+15,21, 3);//1st top
	halLcdHLine(x+4,x+18,22, 3);//2nd line
	halLcdHLine(x+1,x+20,23, 3);//3rd line
	halLcdHLine(x,x+21,24, 3);//4th line
	halLcdHLine(x,x+2,25, 3);//5
	halLcdHLine(x+4,x+6,25, 3);//5
	halLcdHLine(x+8,x+11,25, 3);//5
	halLcdHLine(x+14,x+17,25, 3);//5
	halLcdHLine(x+19,x+21,25, 3);//5
	halLcdHLine(x,x+21,26, 3);//6
	halLcdHLine(x+2,x+7,27, 3);//7 left
	halLcdHLine(x+9,x+11,27, 3);//7 mid
	halLcdHLine(x+13,x+18,27, 3);//7 right
	halLcdHLine(x+3,x+6,28, 3);//8 left
	halLcdHLine(x+14,x+17,28, 3);//8 right
} // end - enemy ship
int alein(x,y){
	halLcdHLine(x+4,x+9,29, 3);//1st top
	halLcdHLine(x+12,x+17,29, 3);//1

	halLcdHLine(x,x+2,28, 3);//
	halLcdHLine(x+4,x+6,28, 3);//
	halLcdHLine(x+15,x+17,28, 3);//
	halLcdHLine(x+19,x+21,28, 3);//

	halLcdHLine(x,x+2,27, 3);//body
	halLcdHLine(x+4,x+17,27, 3);//
	halLcdHLine(x+19,x+21,27, 3);//

	halLcdHLine(x,x+2,26, 3);//
	halLcdHLine(x+4,x+17,26, 3);//
	halLcdHLine(x+19,x+21,26, 3);//

	halLcdHLine(x,x+21,25, 3);//
	halLcdHLine(x+2,x+19,24, 3);//
	halLcdHLine(x+4,x+15,23, 3);//

	halLcdHLine(x+4,x+6,22, 3);//ears
	halLcdHLine(x+15,x+17,22, 3);//

	halLcdHLine(x+2,x+4,21, 3);//
	halLcdHLine(x+17,x+19,21, 3);//

	//halLcdHLine(x+3,x+6,28, 3);//8 left
	//halLcdHLine(x+14,x+17,28, 3);//8 right
} // end - enemy_alien1

int enemy2_ship(x,y){
	halLcdHLine(x,x+2,y, 3);//1
	halLcdHLine(x+4,x+6,y, 3);//1
	halLcdHLine(x+10,x+12,y, 3);//1
	halLcdHLine(x+14,x+16,y, 3);//1
	halLcdHLine(x+2,x+4,y-1, 3);//2
	halLcdHLine(x+6,x+10,y-1, 3);//2
	halLcdHLine(x+12,x+14,y-1, 3);//2
	halLcdHLine(x+4,x+6,y-2, 3);//3
	halLcdHLine(x+10,x+12,y-2, 3);//3
	halLcdHLine(x,x+16,y-3, 3);//4
	halLcdHLine(x,x+4,y-4, 3);//5
	halLcdHLine(x+6,x+10,y-4, 3);//5
	halLcdHLine(x+12,x+16,y-4, 3);//5
	halLcdHLine(x+2,x+14,y-5, 3);//6
	halLcdHLine(x+4,x+12,y-6, 3);//7
	halLcdHLine(x+6,x+10,y-7, 3);//8
} // end - enemy_ship2

int enemy3_ship(x,y){
	halLcdHLine(x+4,x+6,y, 3);//1
	halLcdHLine(x+16,x+18,y, 3);//1
	halLcdHLine(x+4,x+6,y-1, 3);//2
	halLcdHLine(x+16,x+18,y-1, 3);//2

	halLcdHLine(x+6,x+8,y-2, 3);//3
	halLcdHLine(x+14,x+16,y-2, 3);//3

	halLcdHLine(x+2,x+18,y-3, 3);//4

	halLcdHLine(x,x+2,y-4, 3);//5
	halLcdHLine(x+6,x+8,y-4, 3);//5
	halLcdHLine(x+10,x+12,y-4, 3);//5
	halLcdHLine(x+14,x+16,y-4, 3);//5
	halLcdHLine(x+18,x+20,y-4, 3);//5

	halLcdHLine(x,x+2,y-5, 3);//5
	halLcdHLine(x+6,x+16,y-5, 3);//5
	halLcdHLine(x+18,x+20,y-5, 3);//5

	halLcdHLine(x,x+2,y-6, 3);//6
	halLcdHLine(x+6,x+16,y-6, 3);//6
	halLcdHLine(x+18,x+20,y-6, 3);//6

	halLcdHLine(x,x+2,y-7, 3);//7
	halLcdHLine(x+8,x+14,y-7, 3);//7
	halLcdHLine(x+18,x+20,y-7, 3);//7
} // end - enemy_ship3
///////////////////////////////////////////////////////////////user ship choices
//rocket ship
int rocket_ship(x,y){
	halLcdHLine(x,x+2,y, 3);//Left rocket
	halLcdHLine(x,x+2,y-1, 3);//
	halLcdHLine(x,x+2,y-2, 3);//
	halLcdHLine(x,x+2,y-3, 3);//
	halLcdHLine(x,x+2,y-4, 3);//
	halLcdHLine(x,x+2,y-5, 3);//
	halLcdHLine(x+14,x+16,y, 3);//right rocket
	halLcdHLine(x+14,x+16,y-1, 3);//
	halLcdHLine(x+14,x+16,y-2, 3);//
	halLcdHLine(x+14,x+16,y-3, 3);//
	halLcdHLine(x+14,x+16,y-4, 3);//
	halLcdHLine(x+14,x+16,y-5, 3);//
	halLcdHLine(x+7,x+9,y, 3);//Lower Bar left side
	halLcdHLine(x+5,x+8,y-1, 3);//
	halLcdHLine(x+3,x+6,y-2, 3);//
	halLcdHLine(x+2,x+4,y-3, 3);//
	halLcdHLine(x+8,x+11,y-1, 3);//Lower Bar Right side
	halLcdHLine(x+10,x+13,y-2, 3);//
	halLcdHLine(x+12,x+14,y-3, 3);//
	halLcdHLine(x+7,x+9,y-3, 3);//space rocket main
	halLcdHLine(x+4,x+12,y-4, 3);//rectangle
	halLcdHLine(x+4,x+12,y-5, 3);//
	halLcdHLine(x+4,x+12,y-6, 3);//
	halLcdHLine(x+4,x+12,y-7, 3);//
	halLcdHLine(x+4,x+12,y-8, 3);//
	halLcdHLine(x+4,x+12,y-9, 3);//
	halLcdHLine(x+4,x+12,y-10, 3);//
	halLcdHLine(x+7,x+9,y-11, 3);//launcher
	halLcdHLine(x+7,x+9,y-12, 3);//
//	halLcdHLine(x+7,x+9,y-10, 2);//grey block
//	halLcdHLine(x+7,x+9,y-9, 2);//
//	halLcdHLine(x+7,x+9,y-8, 2);//
//	halLcdHLine(x+7,x+9,y-7, 2);//
//	halLcdHLine(x+7,x+9,y-6, 2);//
} // end - rocket ship

//square tank
int user_ship(x,y){
	halLcdHLine(x,x+18,y, 3);//BOT
	halLcdHLine(x,x+18,y-1, 3);//bot
	halLcdHLine(x+3,x+15,y-2, 3);//rectangle
	halLcdHLine(x+3,x+15,y-3, 3);//rectangle
	halLcdHLine(x+3,x+15,y-4, 3);//rectangle
	halLcdHLine(x+3,x+15,y-5, 3);//rectangle
	halLcdHLine(x+3,x+15,y-6, 3);//rectangle
	halLcdHLine(x+8,x+10,y-7, 3);//top
	halLcdHLine(x+8,x+10,y-8, 3);//top
} // end - user_ship

////////////////////////////////////////////////////////////////user/enemy shots
// displays the action of a shot from the enemy and user shots
int user_shot(x,y){
	halLcdHLine(x,x+2,y, 3);//
	halLcdHLine(x,x+2,y-1, 3);//
	halLcdHLine(x,x+2,y-2, 3);//
	halLcdHLine(x,x+2,y-3, 3);//
	halLcdHLine(x,x+2,y-4, 3);//
	halLcdHLine(x,x+2,y-5, 3);//
}//end - user_shot
int user_shot2(x,y){
	halLcdHLine(x,x+2,y, 3);//
	halLcdHLine(x,x+2,y-1, 3);//
	halLcdHLine(x,x+2,y-2, 3);//
	halLcdHLine(x,x+2,y-3, 3);//
	halLcdHLine(x,x+2,y-4, 3);//
	halLcdHLine(x,x+2,y-5, 3);//
}//user_shot 2nd shot
int user_shot3(x,y){
	halLcdHLine(x,x+2,y, 3);//
	halLcdHLine(x,x+2,y-1, 3);//
	halLcdHLine(x,x+2,y-2, 3);//
	halLcdHLine(x,x+2,y-3, 3);//
	halLcdHLine(x,x+2,y-4, 3);//
	halLcdHLine(x,x+2,y-5, 3);//
}//end - user_shot 3rd shot

int alien_shot(x,y){
	halLcdHLine(x,x+2,y, 3);//
	halLcdHLine(x,x+2,y-1, 3);//
	halLcdHLine(x,x+2,y-2, 3);//
	halLcdHLine(x,x+2,y-3, 3);//
	halLcdHLine(x,x+2,y-4, 3);//
	halLcdHLine(x,x+2,y-5, 3);//
}//end - user_shot

int enemy_shotf(x,y){
	halLcdHLine(x,x+2,y, 3);//
	halLcdHLine(x,x+2,y-1, 3);//
	halLcdHLine(x,x+2,y-2, 3);//
	halLcdHLine(x,x+2,y-3, 3);//
	halLcdHLine(x,x+2,y-4, 3);//
	halLcdHLine(x,x+2,y-5, 3);//
}//end - user_shot

//////////////////////////////////////////////////////////////////////////health
// calculates user and enemy health
int health(x, y, lives){
	if (lives == 1){
		halLcdHLine(x+4,x+6,y, 3);//
		halLcdHLine(x+3,x+7,y-1, 3);//
		halLcdHLine(x+2,x+8,y-2, 3);//
		halLcdHLine(x+1,x+9,y-3, 3);//
		halLcdHLine(x,x+10,y-4, 3);//
		halLcdHLine(x+1,x+4,y-5, 3);//
		halLcdHLine(x+6,x+9,y-5, 3);//
		halLcdHLine(x+2,x+4,y-6, 3);//
		halLcdHLine(x+7,x+9,y-6, 3);//
	}
	else if (lives == 2){
		halLcdHLine(x+4,x+6,y, 3);//
		halLcdHLine(x+3,x+7,y-1, 3);//
		halLcdHLine(x+2,x+8,y-2, 3);//
		halLcdHLine(x+1,x+9,y-3, 3);//
		halLcdHLine(x,x+10,y-4, 3);//
		halLcdHLine(x+1,x+4,y-5, 3);//
		halLcdHLine(x+6,x+9,y-5, 3);//
		halLcdHLine(x+2,x+4,y-6, 3);//
		halLcdHLine(x+7,x+9,y-6, 3);//

		halLcdHLine(x+14,x+16,y, 3);//
		halLcdHLine(x+13,x+17,y-1, 3);//
		halLcdHLine(x+12,x+18,y-2, 3);//
		halLcdHLine(x+11,x+19,y-3, 3);//
		halLcdHLine(x+10,x+20,y-4, 3);//
		halLcdHLine(x+11,x+14,y-5, 3);//
		halLcdHLine(x+16,x+19,y-5, 3);//
		halLcdHLine(x+12,x+14,y-6, 3);//
		halLcdHLine(x+17,x+19,y-6, 3);//
	}
	else {
		halLcdHLine(x+4,x+6,y, 3);//
		halLcdHLine(x+3,x+7,y-1, 3);//
		halLcdHLine(x+2,x+8,y-2, 3);//
		halLcdHLine(x+1,x+9,y-3, 3);//
		halLcdHLine(x,x+10,y-4, 3);//
		halLcdHLine(x+1,x+4,y-5, 3);//
		halLcdHLine(x+6,x+9,y-5, 3);//
		halLcdHLine(x+2,x+4,y-6, 3);//
		halLcdHLine(x+7,x+9,y-6, 3);//

		halLcdHLine(x+14,x+16,y, 3);//
		halLcdHLine(x+13,x+17,y-1, 3);//
		halLcdHLine(x+12,x+18,y-2, 3);//
		halLcdHLine(x+11,x+19,y-3, 3);//
		halLcdHLine(x+10,x+20,y-4, 3);//
		halLcdHLine(x+11,x+14,y-5, 3);//
		halLcdHLine(x+16,x+19,y-5, 3);//
		halLcdHLine(x+12,x+14,y-6, 3);//
		halLcdHLine(x+17,x+19,y-6, 3);//

		halLcdHLine(x+24,x+26,y, 3);//
		halLcdHLine(x+23,x+27,y-1, 3);//
		halLcdHLine(x+22,x+28,y-2, 3);//
		halLcdHLine(x+21,x+29,y-3, 3);//
		halLcdHLine(x+20,x+30,y-4, 3);//
		halLcdHLine(x+21,x+24,y-5, 3);//
		halLcdHLine(x+26,x+29,y-5, 3);//
		halLcdHLine(x+22,x+24,y-6, 3);//
		halLcdHLine(x+27,x+29,y-6, 3);//
	}
} // end-health
///////////////////////////////////////////////////////////win/game over screen
// if the user won
int winner(){
	halLcdPrintXY("Winner", 35, 45, 4);
	halLcdReverse();
	__delay_cycles(100000);
}
//game over screen if user lost
int game_over(){
	halLcdPrintXY("Game Over", 35, 45, 4);
	halLcdReverse();
	__delay_cycles(100000);
}
// display user score
int score_display(score){
	halLcdPrintXY("score: ", 5, 10, 4);
	halLcdPrintXY(score, 35, 10, 4);
}
//want to play again
 int again(){
	 if ((P2IN & BIT7) == 0){	//left button
	 	return 1;
	 }//if
 }

////////////////////////////////////////////////////////////////////////////////
/////////////////Game Functionality/////////////////////////////////////////////

 int game() {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P2DIR = 1;
    P1DIR = 0x0003;
   //initial screen
	halLcdInit();
	halLcdClearScreen( );
	halLcdBackLightInit( );		//turn backlight on
	halLcdSetBackLight(5); 		 //bright level
	//halLcdCursor();
	//halLcdDrawBlock(50, 30);
	//halLcdHLine(50,120,75, 3);

///////////////////////////////////////////////////////////initializes variables
//health
	int x_life = 105; 	//LIFE right corner
	int y_life = 8;		//LIFE bot: 104 top:8
///user initial variables
	//initalize player setup
	int score = 0;			//starting score
	int lives = 3;			//starting lives
//initial starting position of user
//	int x_user_ship = 2;	//user ship
//	int y_user_ship = 104;	//user ship
	int x2 = 50;		//new user ship
	int y2 = 104;		//new user ship
	//initialize player shots
	int shot_valid = 0; 	//user shot on screen  0 = no shot 1 = shot
//	int shot_valid2 = 0;	//user shot2 on screen
//	int shot_valid3 = 0; 	//user shot3 on screen
	int rocket_valid = 1;	//user rocket shot valid
	int y_shot = 90;		//y-cord user bullet
//	int y_shot2 = 90;
//	int y_shot3 = 90;
//	int user_hit = 0;		//inital if hit or not
//	int enemy_hit = 0;
	//initialize if user is hit
	int user_hit = 0;		//user hit - if hit or not
	int shot_count = 0;
	int shot_count2 = 0;
	int shot_count3 = 0;

////enemy initial variables
	int enemy_dir = 1;		//enemy moving direction 0 = left 1 = right
	int alein_dir = 1;		//alein direction  0 = left 1 = right
	int enemy2_dir = 1;		//enemy moving direction 0 = left 1 = right
	int enemy3_dir = 0;
	//checks if valid
	int alien_valid = 1;	//enemy/alein/user valid
	int enemy_valid = 1;	//0=dead 1=valid
	int enemy2_valid = 1;
	int enemy3_valid = 1;
	//initialize enemy shot variables
	int enemy_shot = 31;	//enemy y shot coordinance
	int enemy_count = 0;
	int alien_count = 0;
	int enemySV=1;			//enemy shot valid
	int alienSV=1;			//alien shot vaild
	int ec=0;				//enemy count
	int ac=0;				//alien count
	int y_shot_init = 94;	//y_bullet init
	//initialize if user is hit
	int enemy_hit = 0;		//enemy hit
//initial starting position of enemy/alein
	int Ax = 50;		//alein x
	int Ay = 21;		//alein y(21-28)
	int x_enemy_ship = 2;//enemy ship
	int A2x = 50;		//enemy2
	int A2y = 45;
	int A3x = 50;		//enemy2
	int A3y = 60;
//	int x_enemy_bot = 2;
//	int y_enemy_bot = 55;

////////////////////////////////////////////////////////////////////INTRO SCREEN
	//halLcdPrintXY("Space Invaders", 20, 40, 4);
	//__delay_cycles(1000000);
	//halLcdClearScreen( );
	int again1 = 1;
	// starts the game
	while(again1){
		halLcdClearScreen( );
		int x_shot = x2+7;//x_bullet X2+7
		int y_shot = 104 - 14;//y_bullet
//		int enemy_array = [1 2 3 4];
		int x_alein_shot = Ax+10;
		int x_enemy_shot = x_enemy_ship + 10;
		//score_display(score);
		health(x_life, y_life, lives);

		//user_shot(x_shot,y_shot);
		//y_shot -= 2;	//user-shot moving at -y axis
//////////////////////////////////////////////////////////////////objects valid
		if (alien_valid ==1){
			alein(Ax,Ay);
		}
		if (enemy_valid ==1){
			enemy_ship(x_enemy_ship);
		}
		if (rocket_valid ==1){
			rocket_ship(x2,y2);
		}
		if (enemy2_valid ==1){
			enemy2_ship(A2x,A2y);
		}
		if (enemy3_valid ==1){
			enemy3_ship(A3x,A3y);
		}
		if ((alien_valid == 0) && (enemy_valid==0) && (enemy2_valid == 0) && (enemy3_valid ==0)){
			return 1;
			//winner();
			//again1 = again();
		}
		if (user_hit ==1){
			lives -= 1;
			health(x_life, y_life, lives);
			user_hit = 0;
		}
		if (lives == 0){
			return 0;
			//game_over();
			//again1 = again();
		}
////////////////////////////////////////////////enemy ship moving left and right
		if(x_enemy_ship > 75){
			enemy_dir = 0;
		}else if (x_enemy_ship < 5){
			enemy_dir = 1;
		}
		if(enemy_dir){
			x_enemy_ship += 5;
			enemy_count+=1;
		}else{
			x_enemy_ship -= 5;
			enemy_count+=1;
		}
		//alien ship moving left and right

		if(Ax > 110){
			alein_dir = 0;
		}else if (Ax < 40){
			alein_dir = 1;
		}
		if(alein_dir){
			Ax += 5;
			alien_count += 1;
		}else{
			Ax -= 5;
			alien_count = 0;
		}
		//enemy2 ship moving left and right
		if(A2x > 110){
			enemy2_dir = 0;
		}else if (Ax < 40){
			enemy2_dir = 1;
		}
		if(enemy2_dir){
			A2x += 7;
		}else{
			A2x -= 7;
		}
		//enemy3 ship
		if(A3x > 110){
			enemy3_dir = 0;
		}else if (A3x < 30){
			enemy3_dir = 1;
		}
		if(enemy3_dir){
			A3x += 8;
		}else{
			A3x -= 8;
		}
////////////////////////////////////////////////////////////////enemy ship fire
		if (enemy_valid == 1){
			if (enemy_count == 5){
				enemySV = 1;
				enemy_count = 0;
				//alien_shot(x_alein_shot, enemy_shot);
				enemy_shotf(x_enemy_shot, enemy_shot);
			}
			if (enemySV){
				ec += 10;
				enemy_shot += ec;
				//alien_shot(x_alein_shot, enemy_shot);
				enemy_shotf(x_enemy_shot, enemy_shot);
			}
			if (enemy_shot >110){
			//stops bullet after it leaves screen
				ec = 0;
				enemy_shot = 0;
				enemySV = 0;
				enemy_shot = 31;
			}
		}
		if (alien_valid == 1){
			if (enemy_count == 5){
				enemySV = 1;
				enemy_count = 0;
				alien_shot(x_alein_shot, enemy_shot);
				//enemy_shotf(x_enemy_shot, enemy_shot);
			}
			if (enemySV){
				ec += 10;
				enemy_shot += ec;
				alien_shot(x_alein_shot, enemy_shot);
				//enemy_shotf(x_enemy_shot, enemy_shot);
			}
			if (enemy_shot >110){
			//stops bullet after it leaves screen
				ec = 0;
				enemy_shot = 0;
				enemySV = 0;
				enemy_shot = 31;
			}
		}
////////////////////////////////////////////////////////user push button to fire
		if ((P2IN & BIT7) == 0){	//BUTToN  to shoot
			shot_valid = 1;
			user_shot(x_shot,y_shot);
		}//if
		if (alien_valid == 0){
			shot_valid = 0;
		}
		if (shot_valid){
			shot_count +=15;
			y_shot -= shot_count;
			user_shot(x_shot,y_shot);
		}
		if (y_shot <1){
			//stops bullet after it leaves screen
			shot_count = 0;
			shot_valid = 0;
			y_shot = y_shot_init;
		}

//////////////////////////////////////////////////////////////user ship movement
		if ((P2IN & BIT1) == 0){	//analog stick SHIP MOVEMENT
			x2 -= 10;
			//enemy_ship(x_enemy_top);
		}//if
		if ((P2IN & BIT2) == 0){
			x2 += 10;
			//enemy_ship(x_enemy_top);
		}//if
//////////////////////////////////////////////////////////////////enemy ship hit
//checks if enemy ship hit
		int i = x_enemy_ship;
		int j = x_enemy_ship+22;
		if (y_shot <= 21){
				for (i; i<j;++i){
					if (x_shot == i){
						enemy_valid = 0;
						score += 50;
						shot_count = 0;
						shot_valid = 0;
						y_shot = y_shot_init;
					}
				}
		}
//checks if alein ship hit
		int m = Ax;
		int n = Ax+28;
		if (y_shot <= 21){
				for (m; m<n;++m){
					if (x_shot == m){
						alien_valid = 0;
						score += 100;
						shot_count = 0;
						shot_valid = 0;
						y_shot = y_shot_init;
					}
				}
		}
//checks if enemy2 ship hit
		int p = A2x;
		int q = A2x+16;
		if (y_shot <= 45){
				for (p; p<q;++p){
					if (x_shot == p){
						enemy2_valid = 0;
						score += 500;
						shot_count = 0;
						shot_valid = 0;
						y_shot = y_shot_init;
					}
				}
		}
//checks if enemy3 ship hit
		int r = A3x;
		int s = A3x+20;
		if (y_shot <= 60){
				for (r; r<s;++r){
					if (x_shot == r){
						enemy3_valid = 0;
						score += 400;
						shot_count = 0;
						shot_valid = 0;
						y_shot = y_shot_init;
					}
				}
		}
/////////////////////////////////////////////////////////////////////user is hit
		int u = x2;
		int v = x2+16;
		if (enemy_shot >= 80){
			for (u; u<v;++u){
				if (x_alein_shot == u){
					enemy_shot = 0;
					enemySV = 0;
					enemy_shot = 31;
					user_hit = 1;
				}
				if (x_enemy_shot == u){
					enemy_shot = 0;
					alienSV = 0;
					enemy_shot = 31;
					user_hit = 1;
				}
			}
		}
//alien ship shots-- checks if user gets hit
		if (enemy_shot >= 80){
			for (u; u>v;++u){
				if (x_enemy_shot == u){
					enemy_shot = 0;
					alienSV = 0;
					enemy_shot = 31;
					user_hit = 1;
				}
			}
		}
		//game_over();									//game over
		__delay_cycles(100000);
	}//while
}//main
//////////////////////////////////////////////////////////////////MAIN FUNCTION
// initalizes the LCD screen and game setup
int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
 	int again1 = 5;
 	int start = 0;
 	//INTRO SCREEN
 	halLcdInit();
 	halLcdClearScreen( );
 	halLcdBackLightInit( );		//turn backlight on
 	halLcdSetBackLight(5); 		 //bright level
 	halLcdPrintXY("Space Invaders", 20, 40, 4);
 	halLcdPrintXY("   Lab 09", 20, 55, 4);
 	while (1){
 		if ((P2IN & BIT7) == 0){	//BUTToN  to shoot
 			again1 = game(again1);
 			halLcdClearScreen( );
 			start = 1;
 			break;
 		}//if
 	}
 	//determines if user wants to play again
 	while(start){
 		if (again1 == 1){
 			winner();
 		}
 		if (again1 == 0){
 		 	game_over();
 		}
 		if ((P2IN & BIT7) == 0){	//BUTToN  to shoot
 			again1=5;
 		}
		// if user wants to play again
 		if (again1 == 5){
 			halLcdClearScreen( );
 			halLcdInit();
 			halLcdClearScreen( );      //clear screen
 			halLcdBackLightInit( );		 //turn backlight on
 			halLcdSetBackLight(5); 		 //bright level
 			again1 = game();
 		}//if
 	__delay_cycles(100000);
 	}
}// END -- MAIN Functionality
