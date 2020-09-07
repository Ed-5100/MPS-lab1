//------------------------------------------------------------------------------------
// Hello.c
//------------------------------------------------------------------------------------
//
// Test program to demonstrate serial port I/O.  This program writes a message on
// the console using the printf() function, and reads characters using the getchar()
// function.  An ANSI escape sequence is used to clear the screen if a '2' is typed.
// A '1' repeats the message and the program responds to other input characters with
// an appropriate message.
//
// Any valid keystroke turns on the green LED on the board; invalid entries turn it off
//


//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "stm32f769xx.h"
#include<stdint.h>
#include <ctype.h>
#include <main.h>


//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
    Sys_Init(); // This always goes at the top of main (defined in init.c)
    int task=2;
    char choice;
	char in[3];
    int size = 3;
    printf("\033[0;33;44m\033[?25l");
    fflush(stdout);
    printf("\033[2J\033[2;15H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n
    printf("PRESS <ESC> OR <CTL>+[ TO QUIT\n");
    printf("\033[12;H\033[s");
    fflush(stdout);
    printf("\033[12;24r");
    fflush(stdout);
    gpio_hal_init();
    //printf("Test of the printf() function.\n\n");

    // Need to enable clock for peripheral bus on GPIO Port J

    //RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN; // or through registers
    //// Below two lines are example on how to access a register by knowing it's memory address
    //volatile uint32_t * RCC_AHB1_CLOCK_ENABLE = (uint32_t*) 0x40023830U; // Enable clock for peripheral bus on GPIO Port J
    //*RCC_AHB1_CLOCK_ENABLE |= 512U; // Bitmask for RCC AHB1 initialization: 0x00000200U or 512U in decimal
/*
    GPIOJ->MODER |= 1024U; //Bitmask for GPIO J Pin 5 initialization (set it to Output mode): 0x00000400U or 1024U in decimal
    GPIOJ->BSRR = (uint16_t)GPIO_PIN_5; // Turn on Green LED (LED2)
    GPIOJ->BSRR = (uint32_t)GPIO_PIN_5 << 16; // Turn off Green LED (LED2)
    GPIOJ->ODR ^= (uint16_t)GPIO_PIN_5; // Toggle LED2

// It doesn't get lower level than this!
//    volatile uint32_t * GREENLEDMODER = (uint32_t*) 0x40022400U; // Init GPIO J Pin 5 (LED2 with no Alt. func.) to Output
//    *GREENLEDMODER |= 1024U; // Bitmask for GPIO J Pin 5 initialization: 0x00000400U or 1024U in decimal

    volatile uint32_t * GREENLEDBSRR = (uint32_t*) 0x40022418U; // Address of GPIO J Bit Set/Reset Register
    *GREENLEDBSRR = (uint16_t)0x0020U; // Turn on Green LED (LED2)
*/
    //HAL_Delay(1000); // Pause for a second

//    volatile uint32_t * GREENLEDODR = (uint32_t*) 0x40022414U; // Address of GPIO J Output Data Register
//    *GREENLEDODR ^= (uint16_t)0x0020U; // Toggle Green LED (LED2)

    while(1)
    {
    	if(task==2){
        	update_led();
        	continue;
    	}
    	update_led();
    	continue;
//        printf("Hello World!\r\n\n");
//        printf("( Welcome to Microprocessor Systems )\r\n\n\n");
//        printf("1=repeat, 2=clear, 0=quit.\r\n\n"); // Menu of choices
				// Don't forget to end printf with newline or run fflush(stdout) after it!

//        choice = uart_getchar(&USB_UART, 1);
		choice = getchar();
//    	putchar(choice);
		if(choice=='\e'){
			printf("\033[2J\033[;H");
			fflush(stdout);
			printf("<ESC pressed, program halted>");
			fflush(stdout);
			break;
		}
		if(isprint(choice)){
			printf("\033[6;H\033[K");
			fflush(stdout);
			printf("The keyboard character is ");
			fflush(stdout);
			printf("\033[31m");
			fflush(stdout);
			putchar(choice);
			fflush(stdout);
			printf("\033[33m");
		} else {
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


// Messing around with stuff:
//        putchar('9'); // Putchar is weird, man.
//				choice = uart_getchar(&USB_UART, 0);
//				uart_putchar(&USB_UART, &choice);
//				puts("stuff\r\n");

        // select which option to run
//        HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_SET);         // Turn green LED on (GPIO_PIN_SET == 1)
//        *GREENLEDBSRR = (uint16_t)0x0020U; // Turn on Green LED (LED2)
/*
        if (choice == '0')
            return 1;
        else if(choice == '1')
            printf("\r\n\nHere we go again.\r\n\n");
        else if(choice == '2')          // clear the screen with <ESC>[2J
        {
            printf("\033[2J\033[;H");
            fflush(stdout);
        }
        else
        {
            // inform the user how bright he is
            *GREENLEDBSRR = (uint32_t)0x0020U << 16; // Turn off Green LED (LED2)
            printf("\r\nA \"");
						putchar(choice);
//						uart_print(&USB_UART, choice);
            printf("\" is not a valid choice.\r\n\n");
        }

// Messing around with more stuff
				printf("Enter an alphanumeric sequence (press <enter> if you want to send less than %d characters): \r\n", size + 1);
				int a = uart_getline(&USB_UART, in, size);
				printf("\r\nuart_getline result: %d\r\n", a);*/
	}
}

void gpio_hal_init(){
	//led pj13 pj5 pa12 pd4
	__HAL_RCC_GPIOJ_CLK_ENABLE(); 	// Through HAL
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitTypeDef* config = (GPIO_InitTypeDef*)malloc(sizeof(GPIO_InitTypeDef));
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
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}

void update_led(){
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7));
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)==GPIO_PIN_SET?GPIO_PIN_RESET:GPIO_PIN_SET);
	//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
}


//------------------------------------------------------------------------------------
//Extra thing to consider...
//------------------------------------------------------------------------------------
void serial_print_things(void) {
	//Input Buffer
	char input[2];
	input[0]=0;
	input[1]=0;

	//Initialize the system
	Sys_Init();
	initUart(&Second_UART, 9600, USART6); // Allow printing over USART6 (Arduino pins D0 - Rx and D1 - TX)
	uart_print(&USB_UART, "\033[2J\033[;H");
	uart_print(&Second_UART, "\033[2J\033[;H");
	uart_print(&USB_UART, "Hello World: This is the USB Serial Connection\r\n");
	uart_print(&Second_UART, "Hello World: This is the other UART Connection\r\n");
	uart_print(&USB_UART, "In order to send newlines to this terminal,\r\n");
	uart_print(&USB_UART, "Press <ESC> and type \"[20h\" (without quotes)\r\n");
	uart_print(&USB_UART, "To get out of newline mode and back to line feed mode,\r\n");
	uart_print(&USB_UART, "Press <ESC> and type \"[20l\" (without quotes)\r\n");
	uart_print(&USB_UART, "\"Thanks for Playing!\"\r\n");

	printf("THIS SENTENCE USES PRINTF!!!\r\n");
  // Don't forget to end printf with newline or run fflush(stdout) after it!

	while(1) {
		input[0]=uart_getchar(&USB_UART, 0);
		uart_print(&USB_UART, input);
		uart_print(&Second_UART, input);
	}

	while(1);// HALT AND CATCH FIRE
}
