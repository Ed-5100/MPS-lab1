
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#ifdef __cplusplus//making sure those files are compiled in c not cpp
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
    int task=12;//value = 12 for task1 and task2, 31 for task3 hal,32 for task3 register , 4 for task4
    int halt=0;// task1 and 2 halt function, indicate whether program is halted
    pair<int, int> posit(0, 1);// saving task4 current position in term of <y, x>
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
    gpio_hal_init();//GPIO INIT though hal for task 3/hal and task4
    switch(task){//select which task to init
    case 12: task12_init(); break;
    case 31: gpio_hal_init(); break;
    case 32: task3_register_pin_set();break;
    case 4: task4_init(maze,posit);
    		gpio_hal_init();
    }


    while(1)//start loop
    {

    	switch(task){//select which task to run
    	case 12: halt=task12_update_screen(); break;
    	case 31: task3_hal_update_led(); break;
    	case 32: task3_register_update_led();break;
    	case 4: task4_update(maze, posit,reset); break;
    	}
    	if(halt) break;//stop loop if halted
	}
    return 0;
}

void task12_init(){//print out halt info
    printf("\033[0;33;44m\033[?25l");//set colors and hide cursor
    fflush(stdout);
    printf("\033[2J\033[2;25H"); // Erase screen & move cursor to home position
    fflush(stdout);
    printf("PRESS <ESC> OR <CTL>+[ TO QUIT\n");//print halt info
    printf("\033[11;H\033[s");//use 11 instead of 12 to offset for the first \n in unprintable
    fflush(stdout);
    printf("\033[12;24r");//set scroll section
    fflush(stdout);
}

int task12_update_screen(){//update keyboard input
	char choice;
	choice = getchar();//get input

	if(choice=='\e' || choice=='^['){//halt
		printf("\033[2J\033[;H");//clear screen and set cursor to top
		fflush(stdout);
		printf("<ESC pressed, program halted>");
		fflush(stdout);
		return 1;
	}
	if(isprint(choice)){//printable char
		printf("\033[6;H\033[K");//reset cursor to line 6
		fflush(stdout);
		printf("The keyboard character is ");//print input info
		fflush(stdout);
		printf("\033[31m");//set text color to red
		fflush(stdout);
		putchar(choice);
		fflush(stdout);
		printf("\033[33m");//reset color
	} else {//unprintable char
		printf("\a");
		fflush(stdout);
		printf("\033[u\033[5m");//set attribute blink
		fflush(stdout);
		printf("\nThe keyboard character $%02x is ",choice);//print first so that the last line won't be empty
		fflush(stdout);
		printf("\033[4;5m");//set attribute underline
		fflush(stdout);
		printf("'not printable'");
		fflush(stdout);
		printf("\033[0;5;33;44m");//reset attribute
		fflush(stdout);
		printf(".\r");
		fflush(stdout);
		printf("\033[s");//save cursor
		fflush(stdout);
	}
	return 0;
}

void gpio_hal_init(){
	//led: pj13 pj5 pa12 pd4 for ld1-4
	__HAL_RCC_GPIOJ_CLK_ENABLE(); 	// enable clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef* config = new GPIO_InitTypeDef;//configuration pointer

	//set leds
	config->Mode=GPIO_MODE_OUTPUT_PP;
	config->Pin=GPIO_PIN_13|GPIO_PIN_5;
	config->Pull=GPIO_PULLUP ;
	config->Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOJ, config);
	config->Pin=GPIO_PIN_12;
	HAL_GPIO_Init(GPIOA, config);
	config->Pin=GPIO_PIN_4;
	HAL_GPIO_Init(GPIOD, config);

	//set inputs pc7 pc6 pj1 pf6 for d0-3
	config->Mode=GPIO_MODE_INPUT;
	config->Pin=GPIO_PIN_7|GPIO_PIN_6;
	HAL_GPIO_Init(GPIOC, config);
	config->Pin=GPIO_PIN_1;
	HAL_GPIO_Init(GPIOJ, config);
	config->Pin=GPIO_PIN_6;
	HAL_GPIO_Init(GPIOF, config);

	//init leds to off
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

	//set blue button as pe0
	config->Mode=GPIO_MODE_INPUT;
	config->Pin=GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, config);


}

void task3_hal_update_led(){//update leds based on inputs d0-3
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7));
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)==GPIO_PIN_SET?GPIO_PIN_RESET:GPIO_PIN_SET);//invert set and reset
}

void task3_register_pin_set(){
		//led pj13(LD1) pj5(LD2) pa12(LD3) pd4(LD4)    input pc7(D0) pc6(D1) pj1(D2) pf6(D3)

		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
		GPIOJ->MODER |= 0x04000400; //Set PJ13&PJ5 as GPI/O, set PJ1 as input

		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		GPIOA->MODER |= 0x01000000;//Set PA12 as GPI/O

		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
		GPIOD->MODER |= 0x00000100;//Set PD4 as GPI/O


		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
		GPIOC->MODER |= 0x00000000;//Set PC6 as Input

		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
		GPIOF->MODER |= 0x00000000;//Set PF6 as input

}

void task3_register_update_led(){
	if(GPIOC->IDR & (1<<7) ){
	    GPIOJ->BSRR |=0x00002000;
	}
	else{
	    GPIOJ->BSRR |=0x20000000;
	}
	if(GPIOC -> IDR & (1<<6)){
	    GPIOJ->BSRR |=0x00000020;
	}
	else{
	    GPIOJ->BSRR |=0x00200000;
	}
	if(GPIOJ -> IDR & (1<<1)){
	    GPIOA->BSRR |=0x00001000;
	}
	else{
	    GPIOA->BSRR |=0x10000000;
	}
	if(GPIOF -> IDR & (1<<6)){
	   GPIOD->BSRR |=0x00100000;
	 }
	else{
	    GPIOD->BSRR |=0x00000010;
	 }

}

void task4_init(vector<string>& maze, pair<int,int>& posit){//print out the maze
	cout<<"\033[2J\033[;H"<<"\033[0;33;44m\033[?25l";//clear screen and set attribute
	cout.flush();
	for(int i=0;i<maze.size();i++){//output maze
		cout<<maze[i]<<"\r"<<endl;
	}
	cout<<"\033["<<posit.first+1<<";"<<posit.second+1<<"H\033[31;1m";//change color and highlight current position
	cout.flush();
	cout<<"*"<<endl;

}

void task4_update(vector<string>& maze, pair<int,int>& posit, int reset[2]){//update location and deal with reset
	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_13)==GPIO_PIN_SET){//if finished
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET){//if button pressed, reset every thing
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);//reset led
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H\033[0;33;44m";//reset end position
			cout.flush();
			cout<<"!"<<endl;

			cout<<"\033[31;1m";//reset color and print start position
			cout.flush();
			posit.first=reset[0];
			posit.second=reset[1];
			cout<<"\033["<< posit.first+1<<";"<<posit.second+1<<"H";
			cout.flush();
			cout<<"*"<<endl;
		}
		return;
	}
	char direction= getchar();//input wasd
	switch(direction){
	case 'w':
		//delete current position and get to new position
		if(posit.first-1>=0 && maze[posit.first-1][posit.second]!='o'){//check wall
			if(maze[posit.first-1][posit.second]=='!')//check if end
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
		//delete current position and get to new position
		if(posit.first+1<maze.size() && maze[posit.first+1][posit.second]!='o'){//check wall
			if(maze[posit.first+1][posit.second]=='!')//check if end
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);//set led
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
		//delete current position and get to new position
		if(posit.second-1>=0 && maze[posit.first][posit.second-1]!='o'){//check wall
			if(maze[posit.first][posit.second-1]=='!')//check if end
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);//set led
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
		//delete current position and get to new position
		if(posit.second+1<maze[0].length() && maze[posit.first][posit.second+1]!='o'){//check wall
			if(maze[posit.first][posit.second+1]=='!')//check if end
				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);//set led
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


