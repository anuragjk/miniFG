#include<LPC214X.H>

#define PRESCALE 15

char option[20]="NILL";

unsigned int i;
unsigned int j;

void init(){
	PINSEL0 = 0x0;
	PINSEL1 = 0x0;
	PINSEL2 = 0x0;
}

void uart_init()		/*****************************************************
					This function initises uart module to 9600 buad rate. 
					******************************************************/
{
	PINSEL0 |= 0x00000005;
	U0LCR = 0X83;
	U0DLL = 97;
	U0LCR = 0X03;
}

void timer0_init()		/********************************
					This function initializes Timer0 
					*********************************/
{
	T0CTCR = 0x0;
	T0PR = PRESCALE-1;
	T0TCR = 0x02;
}

void delay_us(unsigned int tick)	/*******************************************
							This function is used to create microsecond 
							delays in the controller.
							Function takes argument number of 
							micro seconds
							*******************************************/
{
	T0TCR = 0x02;
	T0TCR = 0x01;
	while(T0TC < tick);
	T0TC = 0x0;
	T0TCR = 0x00;
}

void tx(char a)				/*************************************
							Function is used to transmit a single 
							character over UART.
							**************************************/
{
	U0THR = a;
	while(!(U0LSR & (1<<6)));
}

char rx()						/**************************************
							Function is used to recieve a single 
							character over UART
							***************************************/
{
	char a;
	while(!(U0LSR&(1<<0)));
	a = U0RBR;
	tx(a);
	return a;
}

void scans(char *a)				/******************************************
							Function scans a string From UART0.
							argument is the addres to which the scaned
							string will be saved.
							*******************************************/
{
	do
		          *a = rx();
	while(*(a++)!='\r');
		          *a = '\0';
}

void prints(char *a)			/***********************************
							Function prints a string to UART0.
							argument is location of string.
							************************************/
{
	while(*a)
		tx(*a++);
}

void uart0_int()__irq			/**************************
							uart interrupt irq.
							**************************/
{
	scans(option);
	VICVectAddr = 0x0;
}

void IRQ_init(){							//***********************
	VICIntEnable |= (1<<6); 					//Enable UART0 Interrupt
	VICIntSelect& = (~(1<<6)); 				//to configure as irq
	VICVectCntl1 = (1<<5)|6; 				//IRQ slot 1
	VICVectAddr1 = (unsigned)uart0_int;		//
	U0IER = 0x01; 							// UART Receive interrupt
}										//************************	

void dac_init()				/******************************************
							Initializes DAC module.
							P0.25 is set to DAC output.
							******************************************/
{
	PINSEL1 |= 0x00080000;
}

void dac_do(int data)			/*****************************************
							converts the data to an analog.
							output is produced at P0.25
							16th bit of DACR is BIAS bit, its made 
							1 to make DAC in BIAS 1 mode, in which 
							settling time is 2us and maximum current 
							is 350uA.
							******************************************/
{
	DACR = (data << 6) | 0x00010000;
}

int sine_wave(float theta)		/*****************************************
							Returns amplitude of sine at a specific 
							value of theta (in rad) 
							******************************************/
{
	float 	p1,
			p2[5]={1,-0.166667,0.008333,-0.000198,0.000002},
			s=0;
	for( i = 0 ; i <= 4 ; i++ ){
		p1=1;
		for( j = 0 ; j < 2*i+1 ; j++ )
			p1 *= theta;
		s += (p1*p2[i]);
	}
	return((int)(s*1024));
}


int main(){
	// char a[20];
	init();
	uart_init();
	IRQ_init();
	timer0_init();


	while(1){
		prints(option);
		prints("\r\n");
		delay_us(1000);
	}
	// scans(a);
}


