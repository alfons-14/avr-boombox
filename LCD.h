/*
 * LCD.h
 *
 *  Created on: 4 черв. 2016
 *      Author: alfons-14
 */

#ifndef LCD_H_
#define LCD_H_

#define LCD_ADDRES 0b00111111
#define SLA_W (LCD_ADDRES << 1)

//pins PCF8574
/*
Po - RS
P1 - R/W
P2 - E
P3 - LED
P4 - D4
P5 - D5
P6 - D6
P7 - D7
 */

void LCD_init();
void LCD_print(char);
void LCD_print(char*);
void LCD_create_char(char, char*);
void LCD_clear_screen();
void LCD_go_to_xy(char, char);
void LCD_backlight(char);




#endif /* LCD_H_ */
