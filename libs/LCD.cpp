/*
 * LCD.cpp
 *
 *  Created on: 4 черв. 2016
 *      Author: alfons-14
 */

#include <util/delay.h>
#include "../drivers/TWI.h"
#include "LCD.h"

volatile unsigned char Led_port=0;

#define TWI_send_lcd(x) TWI_send_byte(LCD_ADDRES, x)

#define LCD_RS_SET  TWI_send_lcd (Led_port |= 0x01)
#define LCD_RS_CLR  TWI_send_lcd (Led_port &= ~0x01)
#define LCD_RW_SET  TWI_send_lcd (Led_port |= 0x02)
#define LCD_RW_CLR  TWI_send_lcd (Led_port &= ~0x02)
#define LCD_E_SET   TWI_send_lcd (Led_port |= 0x04)
#define LCD_E_CLR   TWI_send_lcd (Led_port &= ~0x04)
#define LCD_BL_SET  TWI_send_lcd (Led_port |= 0x08)
#define LCD_BL_CLR  TWI_send_lcd (Led_port &= ~0x08)

#define LCD_COMMAND 0
#define LCD_DATA 1

void LCD_put_nibble(char t){
	t<<=4;
	LCD_E_SET;
	_delay_us(50);
	TWI_send_lcd(Led_port|t);
	LCD_E_CLR;
	_delay_us(50);
}

void LCD_put_byte(char t, char rs){
	if (rs==LCD_COMMAND) LCD_RS_CLR;
	else                 LCD_RS_SET;
	LCD_put_nibble(t>>4);
	LCD_put_nibble(t);
}

void LCD_put_char(char t){
	LCD_put_byte(t, 1);
}

void LCD_init(){
	_delay_ms(15);
	LCD_put_nibble(0b00000011);
	_delay_ms(4);
	LCD_put_nibble(0b00000011);
	_delay_us(100);
	LCD_put_nibble(0b00000011);
	_delay_ms(1);
    LCD_put_nibble(0b00000010);
	_delay_ms(1);
    LCD_put_byte(0x28, LCD_COMMAND); // ����� (0x28) 5x8 0b00101000 ������ (0x2C) 5x10 0b00101100
    _delay_ms(1);
    LCD_put_byte(0x0C, LCD_COMMAND);
    _delay_ms(1);
    LCD_put_byte(0x06, LCD_COMMAND);
    _delay_ms(1);
	LCD_BL_SET;
	LCD_RW_CLR;
	LCD_clear_screen();
}

void LCD_go_to_xy(char x, char y){
	  char adr;
	  adr=0x40*y+x;
	  adr|=0x80;
	  LCD_put_byte(adr, LCD_COMMAND);
}

void LCD_create_char(char id, char *chars){
	char adr;
	adr=id*8;
	adr|=0b01000000;
	LCD_put_byte(adr, LCD_COMMAND);
	for(adr=0; adr<8; adr++) LCD_put_byte(chars[adr], LCD_DATA);
}

void LCD_backlight(char state){
	if (state) LCD_BL_SET;
	else LCD_BL_CLR;
}

void LCD_clear_screen(){
	LCD_put_byte(1, LCD_COMMAND);
    _delay_us(1500);
}

void LCD_print (char *stroka)
{  char i;
   for(i=0;stroka[i]!='\0';i++)
   LCD_put_char(stroka[i]);
}

void LCD_print(char s){
	LCD_put_char(s);
}
