/**
 * @file Secure_Diagnositc.c
 * @author Mohamed Sami Elmelegy
 * @brief
 * @date 2023-11-11
 *
 *
 */
#define F_CPU 8000000
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "UART_Interface.h"
#include <avr/io.h>
#include <avr/delay.h>
#define clear 0b00000001

/****************************Global varibles********************************/
u32 u32_send_command = 0x2701;
u8 random_hex_number[4];
u8 rece_data_UART[4];
u32 rece_data_UART_num = 0;
u8 sym_key[4] = {0x55, 0x25, 0x11, 0x13};
u32 u32_sym_key = 0x55251113;
u8 encr_data[4];
u8 encr_data_received[4];
u32 u32_num = 0;
u32 u32_random_num = 0;
/*****************************************************************/

/****************************Local API porotypes********************************/
void random_hex_generator(void);
void encrytion_fun(void)
/****************************************************************************/

/****************************Local API implementation********************************/

	void random_hex_generator(void)
{
	u8 j = 1;
	u32 u32_temp = 0;
	for (int i = 0; i < 4; i++)
	{
		random_hex_number[i] = (char)(rand());
	}
	// Convert the random number to hexadecimal
	for (int i = 0; i < 4; i++)
	{
		random_hex_number[i] += '0';
		if (random_hex_number[i] > '9')
		{
			random_hex_number[i] -= ('A' - '9');
		}
	}

	for (int i = 3; i >= 0; i--)
	{
		u32_temp = random_hex_number[i];
		u32_temp = u32_temp << (32 - (8 * j));
		u32_random_num = u32_random_num | u32_temp;
		j++;
	}
}

void encrytion_fun(void)
{

	u8 j = 1;
	u32 u32_temp = 0;
	u32_num = u32_random_num ^ u32_sym_key;
}

/*************************************************************************/

int main(void)
{

	LCD_VidInit();
	UART_Init();
	DIO_VidSetPinDirection(PORTC, PIN7, OUTPUT);
	u8 u8_command_flag = 0;
	u8 access_flag = 0;
	u32 service = 0x3101;
	u32 routine = 0xAA00;
	u32 u32_receive_hex = 0;
	LCD_VidSendCommand(clear);
	_delay_ms(2);
	LCD_VidSetPosition(0, 0);
	LCD_VidWriteString("Access Sys");
	_delay_ms(2000);
	while (1)
	{
		LCD_VidSendCommand(clear);
		_delay_ms(2);
		LCD_VidSetPosition(0, 0);
		LCD_VidWriteString("Enter a command");
		u32_receive_hex = receive_hex();
		if (u32_receive_hex == service)
		{
			u32_receive_hex = receive_hex();
			if (u32_receive_hex == routine)
			{
				if (access_flag == 1)
				{
					LCD_VidSendCommand(clear);
					_delay_ms(2);
					LCD_VidSetPosition(0, 0);
					LCD_VidWriteString("Access Granted");
					UART_SendString("7101AA00");
					DIO_VidSetPinValue(PORTC, PIN0, HIGH);
					_delay_ms(2000);
					DIO_VidSetPinValue(PORTC, PIN0, LOW);
					access_flag = 0;
				}
				else
				{
					LCD_VidSendCommand(clear);
					_delay_ms(2);
					LCD_VidSetPosition(0, 0);
					LCD_VidWriteString("Access denied");
					UART_SendString("NRC 7F 31 35 ");
					_delay_ms(2000);
				}
			}
		}

		u32_receive_hex = receive_hex();

		_delay_ms(2000);

		if (u32_send_command == u32_receive_hex)
		{
			u8_command_flag = 1;
		}
		else
		{
		}

		if (u8_command_flag == 1)
		{
			random_hex_generator();
			encrytion_fun();
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidSetPosition(0, 0);
			LCD_VidWriteString("6701");
			LCD_VidSetPosition(0, 5);
			LCD_VidWrite_Hex_Number(u32_random_num);
			_delay_ms(2000);
			rece_data_UART_num = receive_hex();
			LCD_VidSetPosition(1, 0);
			LCD_VidWrite_Hex_Number(rece_data_UART_num);
			_delay_ms(5000);

			if (rece_data_UART_num == u32_num)
			{

				access_flag = 1;
				LCD_VidSendCommand(clear);
				_delay_ms(2);
				LCD_VidSetPosition(0, 0);
				LCD_VidWriteString("6702");
				_delay_ms(1000);
			}
			else
			{
				LCD_VidSendCommand(clear);
				_delay_ms(2);
				LCD_VidSetPosition(0, 0);
				LCD_VidWriteString("7F2753");
				_delay_ms(1000);
			}
			u8_command_flag = 0;
		}
	}
}
