#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define LCD_PRT PORTD
#define LCD_DDR DDRD
#define LCD_PIN PIND
#define LCD_RS 2
#define LCD_EN 3


unsigned char keypad[4][6] = {{'c','d','e','f','+','x'},
							  {'8','9','a','b','-','n'},
							  {'4','5','6','7','&','B'},
							  {'01','1','2','3','|','='}};  //x:xor,n:nor,&:and,|:or,b:basedec

unsigned char c[32];
unsigned int x = 1  ;
unsigned int y = 1;
uint32_t v1 = 0;
uint32_t v2 = 0;
unsigned int q = 50;
unsigned int d = 0;
uint32_t z = 0;
unsigned int L = 0;

void lcdData(unsigned char d);
void lcd_print(char *str);
void lcd_position();
void toBin(char character,int select);
void Delay_us(d)
{
	_delay_us(d);	
}

void Delay_ms(d)
{
	_delay_ms(d);
}


void num(unsigned char m) //print symbol 
{
	if(m=='+')
	{
		lcdData('+');
		lcd_position();
		q=1;
	}
	else if(m=='-')
	{
		lcdData('-');
		lcd_position();
		q=2;
	}
	else if(m=='&')
	{
		lcdData('&');
		lcd_position();
		q=3;
	}
	else if(m=='|')
	{
		lcdData('|');
		lcd_position();
		q=4;
	}
	else if(m=='x')
	{
		lcdData('x');
		lcd_position();
		q=5;
	}
	else if(m=='n')
	{
		lcdData('n');
		lcd_position();
		
		z=~v1;
		ltoa(z,c,2);
		lcd_print(c);
	}
	else if(m=='B')
	{
		lcdData('B');
		lcd_position();
		z= v1;
		if(L >= 3) L =0;
		  if (L == 0) ltoa(z,c,10);
		else if (L == 1) ltoa(z,c,16);
		else if (L == 2) ltoa(z,c,2);
		L=L+1;
		lcd_print(c);
	}
	else if(m=='=')   //if have = will calculate equation
	{  
		if(q==50)
		{
			z=v1;
			ltoa(z,c,2);
		}
		lcdData('=');
		lcd_position();
		if(q==1)
		{
			z=v1+v2;
		}
		if(q==2)
		{
			z=v1-v2;
		}
		if(q==3)
		{
			z=v1&v2;
		}
		if(q==4)
		{
			z=v1|v2;
		}
		if(q==5)
		{
			z=v1^v2;
		}
		ltoa(z,c,2);
		lcd_print(c);  //print

		q=50;
	}
	
	else if(q==50)		//sign number set one
	{
		toBin(m,1);
	}
	
	else if(q!=50)   //sign number set two
	{
		toBin(m,2);
	}
}
void toBin(char character,int select)
{
uint32_t temp=0;
char strG[32];

	if((character - 48 >= 0) && (character - 48 <= 9))
	{
		
		strG[0] = character;
		temp = atoi(strG);
		if(temp == 0) lcd_print("0000");
		else if(temp == 1) lcd_print("0001");
		else if(temp == 2) lcd_print("0010");
		else if(temp == 3) lcd_print("0011");
		else if(temp == 4) lcd_print("0100");
		else if(temp == 5) lcd_print("0101");
		else if(temp == 6) lcd_print("0110");
		else if(temp == 7) lcd_print("0111");
		else 
		{
		ltoa(temp,strG,2);
		lcd_print(strG);
		}
	}
	else
	{
		
		temp = character - 87;
		ltoa(temp,strG,2);
		lcd_print(strG);
	}

if(select==1) 
	{
	v1=(v1*16)+temp;
	}
else  
	{
	v2=(v2*16)+temp;
	}
}
void lcdCommand(unsigned char cmnd)
{
     LCD_PRT = (LCD_PRT & 0x0F) | (cmnd & 0xF0);
     LCD_PRT &= ~(1<<LCD_RS);
     Delay_us(1);
     LCD_PRT |= (1<<LCD_EN);
     Delay_us(1);
     LCD_PRT &= ~ (1<<LCD_EN);

     Delay_us(20);

     LCD_PRT =(LCD_PRT & 0x0F) | (cmnd <<4);
     LCD_PRT |= (1<<LCD_EN);
     Delay_us(1);
     LCD_PRT &=~ (1<<LCD_EN);
}
void lcdData(unsigned char d)
{
     LCD_PRT = (LCD_PRT & 0x0F) | (d&0xF0);
     LCD_PRT |= (1<<LCD_RS);
     Delay_us(1);
     LCD_PRT |= (1<<LCD_EN);
     Delay_us(1);
     LCD_PRT &= ~(1<<LCD_EN);
     
     LCD_PRT= (LCD_PRT & 0x0F) | (d << 4);
     LCD_PRT |= (1<<LCD_EN);
     Delay_us(1);
     LCD_PRT &= ~(1<<LCD_EN);

}
void lcd_init_my()
{
     LCD_DDR = 0xFF;

     LCD_PRT &=~(1<<LCD_EN);
     Delay_us(2000);
     lcdCommand(0x33);
     Delay_us(100);
     lcdCommand(0x32);
     Delay_us(100);
     lcdCommand(0x28);
     Delay_us(100);
     lcdCommand(0x0e);
     Delay_us(100);
     lcdCommand(0x01);
     Delay_us(2000);
     lcdCommand(0x06);

     Delay_us(100);
}

void lcd_gotoxy(unsigned char x,unsigned char y)
{
     unsigned char firstCharAdr[] = {0x80,0xC0,0x94,0xD4};
     lcdCommand(firstCharAdr[y-1]+x-1);
     Delay_us(100);
}
void lcd_print(char *str)
{
     unsigned char i =0;

     while(str[i]!=0)
     {
      lcdData(str[i]);
	  lcd_position();
      i++;
     }
}
void lcd_position()
{
	x++;
	if (x>40)
	{
		x = 1;
		y++	;
		if(y>4)
		{
			y = 1;
			lcdCommand(0x01);
			Delay_us(2000);			
		}	
	}
	lcd_gotoxy(x,y);
	
}	
							
int main(void)
{	
unsigned char colloc,rowloc;  //declar input B Output C
	DDRB = 0x00;
	PORTB= 0xFF;
	DDRC = 0x0F;
	PORTC= 0x00;
	lcd_init_my();
	lcd_gotoxy(1,1);
	
    while(1)
    {
		do
		{
			PORTC &= 0xF0;
			colloc = (PINB & 0x3F);
			
		} while(colloc != 0x3F);

		do 
		{
			do 
			{
				Delay_ms(20);
				colloc = (PINB &0x3F);
			} while (colloc == 0x3F);
			
			Delay_ms(20);
			colloc = (PINB & 0x3F);
		} while (colloc == 0x3F);
		
		while(1)    //chack vertical
		{
			PORTC = 0x0E;
			colloc = (PINB & 0x3F);
			if(colloc != 0x3F)
			{
				rowloc =0;
				break;	
			}			
			PORTC = 0x0D;
			colloc = (PINB & 0x3F);
			
			if (colloc != 0x3F)
			{
				rowloc = 1;
				break;
			}
			
			PORTC = 0x0B;
			colloc = (PINB & 0x3F);
			
			if(colloc != 0x3F)
			{
				rowloc =2;

				break;
			}
			
			PORTC = 0x07;
			colloc = (PINB & 0x3F);
			
			if(colloc != 0x3F)
			{
				rowloc =3;
				break;
			}		
		}	
				
		if(colloc ==0x3E)    //chack horizon
		{
			number(keypad[rowloc][0]);
		}			
		else if(colloc == 0x3D)
		{
			number(keypad[rowloc][1]);
		}		
		else if(colloc == 0x3B)
		{
			number(keypad[rowloc][2]);
		}			
		else if(colloc == 0x37)
		{
			number(keypad[rowloc][3]);
		}
		else if(colloc == 0x2F)
		{
			number(keypad[rowloc][4]);
		}
		else if(colloc == 0x1F)
		{
			number(keypad[rowloc][5]);
		}

		
		}
    }


