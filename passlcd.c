#define	_XTAL_FREQ	4000000
#define	RS	RD2
#define	EN	RD3
#define	D4	RD4
#define	D5	RD5
#define	D6	RD6
#define	D7	RD7

#include <htc.h>
#include "lcd.h"

void port_init(), keycheck(), endp(), setPass(), pwmatch(), entpw(), clrkey();;
char keyscan();
char pass[5], key[5];
unsigned int count = 0;

int main()
{
	unsigned int a;
	port_init();
	Lcd_Init();
	while(1)
	{
		Lcd_Clear();
	// Intro
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String("     B23 CE");
	Lcd_Set_Cursor(2,1);
	Lcd_Write_String("  Password lock");
	__delay_ms(1000);
	setPass();
	Lcd_Clear();
hr:	entpw();
	Lcd_Clear();
	pwmatch();
	if(nm == '0')
	{	
		Lcd_Write_String(" Access Granted!");
		__delay_ms(1500);
		Lcd_Clear();
	}
	else if(nm == '1')
	{	
		Lcd_Write_String(" Access Denied!");
		Lcd_Set_Cursor(2,1);
		Lcd_Write_String("Please Try Again");
		__delay_ms(1500);
		Lcd_Clear();				
		goto hr;
	}
	endp();
	}
    return 0;
}

// Initialize all ports
void port_init() {
	TRISB = 0x0F;
	TRISD = 0x00;
	PORTB = 0;
}


char keyscan() {
	for (;;) {
		PORTB = 0b00010000;
		if (RB0 == 1)
			return '1';
		else if (RB1 == 1)
			return '4';
		else if (RB2 == 1)
			return '7';
		else if (RB3 == 1)
			return '*';

		PORTB = 0b00100000;
		if	(RB0 == 1)
			return '2';
		else if (RB1 == 1)
			return '5';
		else if (RB2 == 1)
			return '8';
		else if (RB3 == 1)
			return '0';
		
		PORTB = 0b01000000;
		if	(RB0 == 1)
			return '3';
		else if (RB1 == 1)
			return '6';
		else if (RB2 == 1)
			return '9';
		else if (RB3 == 1)
			return '#';
	}
}

// This function will only exit when all keys are released
void keycheck() {
	while (RB0 == 1 || RB1 == 1 || RB2 == 1 || RB3 == 1) {}
}

void endp() {
	Lcd_Clear();
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String("Program stopped");
	Lcd_Set_Cursor(2,1);
	for (int i = 0; i < 4; i++)
		Lcd_Write_Char(pass[i]);
	while(1) {}
}

void setPass() {
	unsigned int i = 0;
	// Set password
ts:	Lcd_Clear();
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String("Set password:");
	Lcd_Set_Cursor(2,1);

	while (i < 5) {
		pass[i] = keyscan();
		if (pass[i] == '#') {	// Checks if #(OK) is pressed
			if (i < 4) { 		// If key count is less than 4, it will
				Lcd_Clear();	// display to user that the password is too short
				Lcd_Write_String("Too short");
				__delay_ms(1500);
				i = 0;			// Resets function
				goto ts;
			}
			else {
				i++;
			}
		}
		else if (i > 0 && pass[i] == '*'){ // Erase the most recent key
			Lcd_Set_Cursor(2,i);
			Lcd_Write_Char(' ');
			Lcd_Set_Cursor(2,i);
			i--;
		}
		else if (i < 4) { 		// Stores a key value and add an asterisk in display
			Lcd_Write_Char('*');
			i++;
		}
		keycheck();
	}
	
	Lcd_Clear();
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String("Password set");
	__delay_ms(2000);
}

void pwmatch() // To Match Password Key In
{ 
	for (x = 0; x < 4; x++) 
	{
		if (key[x] != pass[x])
		{	
			nm = '1';		
		}
		else if(key[x] = pass[x])
		{
			nm = '0';	
		}
	}
}

void entpw() // To Enter Password
{
	Lcd_Clear();
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String("Enter Password:");
	Lcd_Set_Cursor(2,1);
	while(z < 4)
	{
		key[z] = keyscan();
		if (key[z] != '-' && z < 4)
		{	
			Lcd_Write_Char(key[z]);
			z++;
		}
		else if (z > 0 && key[z] == '-')
		{
			Lcd_Set_Cursor(2,z);
			Lcd_Write_Char(' ');
			Lcd_Set_Cursor(2,z);
			z--;
		}		
		keycheck();
	}	
}
