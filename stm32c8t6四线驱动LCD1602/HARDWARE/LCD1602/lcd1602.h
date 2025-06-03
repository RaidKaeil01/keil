#ifndef _LCD1602_H
#define _LCD1602_H

//º¯ÊıÉùÃ÷
void lcd1602_write_cmd(unsigned char cmd);


void lcd1602_init(void);
void lcd1602_clear(void);
void lcd1602_user_char(void);
void lcd1602_show_char(unsigned char x,unsigned char y,unsigned char chr);
void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str);

#endif
