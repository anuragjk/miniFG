#include<LPC214X.H>

#define PRESCALE 15

char option[20]="NILL";

void init(){
	PINSEL0 = 0x0;
	PINSEL1 = 0x0;
	PINSEL2 = 0x0;
}

void uart_init(){
	PINSEL0=0x00000005;
	U0LCR=0X83;
	U0DLL=97;
	U0LCR=0X03;
}

void timer0_init(){
	T0CTCR = 0x0;
	T0PR = PRESCALE-1;
	T0TCR = 0x02;
}

void delay_us(unsigned int tick){
	T0TCR = 0x02;
	T0TCR = 0x01;
	while(T0TC < tick);
	T0TC = 0x0;
	T0TCR = 0x00;
}

void tx(char a){
	U0THR=a;
	while(!(U0LSR & (1<<6)));
}

char rx(){
	char a;
	while(!(U0LSR&(1<<0)));
	a = U0RBR;
	tx(a);
	return a;
}

void scans(char *a){
	do
		          *a=rx();
	while(*(a++)!='\r');
		          *a = '\0';
}

void prints(char *a){
	while(*a)
		tx(*a++);
}

void uart0_int()__irq
{
	scans(option);
	VICVectAddr=0x0;
}

void IRQ_init(){
	VICIntEnable|=(1<<6); 				//Enable UART0 Interrupt
	VICIntSelect&=(~(1<<6)); 			//to configure as irq
	VICVectCntl1=(1<<5)|6; 				//IRQ slot 1
	VICVectAddr1=(unsigned)uart0_int;
	U0IER=0x01; 						// UART Receive interrupt
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


