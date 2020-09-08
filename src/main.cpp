
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#ifdef __cplusplus
	extern "C"{
#endif
#include "stm32f769xx.h"
#include<stdint.h>
#include <ctype.h>
#include <init.h>
#ifdef __cplusplus
	}
#endif
#include <main.h>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;



//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
    Sys_Init(); // This always goes at the top of main (defined in init.c)
    int task=4;//value = 12, 3, 4
    int halt=0;// task1 and 2 halt function
    pair<int, int> posit(0, 1);// task4 current position
    vector<string> maze={//maze for task4, ! is goal, * is current position
    					"o*oooooooooooooooooooooo!oooo",
					   	"o          ooo           oooo",
						"oooooooo ooooo oooooooooooooo",
						"o    o     o   oooooooooooooo",
						"o o ooo oooo ooo           oo",
						"o o        o ooooooooooooo oo",
						"o oooooooooo           o o  o",
						"o o   oo   ooooooooooo o oo o",
						"o o o o  o ooooooooooo oooo o",
						"o   o   oo                  o",
						"ooooooooooooooooooooooooooooo",
    };
    int reset[2]={posit.first, posit.second};//original position for task 4
    gpio_hal_init();//GPIO INIT though hal
    switch(task){//task change
    case 12: task12_init(); break;
    case 4: task4_init(maze,posit);
    }


    while(1)
    {

    	switch(task){
    	case 12: halt=task12_update_screen(); break;
    	case 3: task3_hal_update_led(); break;
    	case 4: task4_update(maze, posit,reset); break;
    	}
    	if(halt) break;
	}
    return 0;
}

void task12_init(){//print out halt info
    printf("\033[0;33;44m\033[?25l");
    fflush(stdout);
    printf("\033[2J\033[2;25H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n
    printf("PRESS <ESC> OR <CTL>+[ TO QUIT\n");
    printf("\033[12;H\033[s");
    fflush(stdout);
    printf("\033[12;24r");
    fflush(stdout);
}

int task12_update_screen(){//update keyboard input
	char choice;
	choice = getchar();
//    	putchar(choice);
	if(choice=='\e' || choice=='^['){//halt
		printf("\033[2J\033[;H");
		fflush(stdout);
		printf("<ESC pressed, program halted>");
		fflush(stdout);
		return 1;
	}
	if(isprint(choice)){//printable char
		printf("\033[6;H\033[K");
		fflush(stdout);
		printf("The keyboard character is ");
		fflush(stdout);
		printf("\033[31m");
		fflush(stdout);
		putchar(choice);
		fflush(stdout);
		printf("\033[33m");
	} else {//unprintable char
		printf("\033[u\033[5m");
		fflush(stdout);
		printf("The keyboard character $%02x is ",choice);
		fflush(stdout);
		printf("\033[4;5m");
		fflush(stdout);
		printf("'not printable'");
		fflush(stdout);
		printf("\033[0;5;33;44m");
		fflush(stdout);
		printf(".\r\n");
		fflush(stdout);
		printf("\033[s\033[0;33;44m");
		fflush(stdout);
	}
	return 0;
}

void gpio_hal_init(){
	//led pj13 pj5 pa12 pd4
	__HAL_RCC_GPIOJ_CLK_ENABLE(); 	// Through HAL
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef* config = new GPIO_InitTypeDef;
	config->Mode=GPIO_MODE_OUTPUT_PP;
	config->Pin=GPIO_PIN_13|GPIO_PIN_5;
	config->Pull=GPIO_PULLUP ;
	config->Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOJ, config);
	config->Pin=GPIO_PIN_12;
	HAL_GPIO_Init(GPIOA, config);
	config->Pin=GPIO_PIN_4;
	HAL_GPIO_Init(GPIOD, config);

	//input pc7 pc6 pj1 pf6
	config->Mode=GPIO_MODE_INPUT;
	config->Pin=GPIO_PIN_7|GPIO_PIN_6;
	HAL_GPIO_Init(GPIOC, config);
	config->Pin=GPIO_PIN_1;
	HAL_GPIO_Init(GPIOJ, config);
	config->Pin=GPIO_PIN_6;
	HAL_GPIO_Init(GPIOF, config);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

	//button pe0
	config->Mode=GPIO_MODE_INPUT;
	config->Pin=GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, config);


}

void task3_hal_update_led(){//update leds based on inputs
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7));
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)==GPIO_PIN_SET?GPIO_PIN_RESET:GPIO_PIN_SET);
	//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
}

void task4_init(vector<string>& maze, pair<int,int>& posit){//print out the maze
	cout<<"\033[2J\033[;H"<<"\033[0;33;44m\033[?25l";
	cout.flush();
	for(int i=0;i<maze.size();i++){
		cout<<maze[i]<<"\r"<<endl;
	}
	cout<<"\033["<<posit.first+1<<";"<<posit.second+1<<"H\033[31;1m";
	cout.flush();
	cout<<"*"<<endl;

}

void task4_update(vector<string>& maze, pair<int,int>& posit, int reset[2]){//update location and deal with reset
	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_13)==GPIO_PIN_SET){//if finished
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET){//if button pressed, reset every thing
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H\033[0;33;44m";
			cout.flush();
			cout<<"!"<<endl;
			cout<<"\033[31;1m";
			cout.flush();
			posit.first=reset[0];
			posit.second=reset[1];
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<"*"<<endl;
		}
		return;
	}
	char direction= getchar();//wasd
	switch(direction){
	case 'w':
		if(posit.first-1>=0 && maze[posit.first-1][posit.second]!='o'){
			if(maze[posit.first-1][posit.second]=='!')
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<" "<<endl;
			posit.first-=1;
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<"*"<<endl;
		}
		break;
	case 's':
		if(posit.first+1<maze.size() && maze[posit.first+1][posit.second]!='o'){
			if(maze[posit.first+1][posit.second]=='!')
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<" "<<endl;
			posit.first+=1;
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<"*"<<endl;
		}
		break;
	case 'a':
		if(posit.second-1>=0 && maze[posit.first][posit.second-1]!='o'){
			if(maze[posit.first][posit.second-1]=='!')
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<" "<<endl;
			posit.second-=1;
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<"*"<<endl;
		}
		break;
	case 'd':
		if(posit.second+1<maze[0].length() && maze[posit.first][posit.second+1]!='o'){
			if(maze[posit.first][posit.second+1]=='!')
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<" "<<endl;
			posit.second+=1;
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<"*"<<endl;
		}
		break;
	}
}


