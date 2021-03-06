#pragma once
#include "../HardwareDefine/Version_Boards.h"
#include <stm32f4xx_usart.h>
#include <string>

/*
* Uart类负责写入
*	一个C风格的字符串
*	一个指定长度的数组
*	一个string类对象
*	一个字符
*	格式化整形、浮点型
*
* 具体的写入方法write为抽象函数，需子类自己定义
*
* 若要使用串口，必须先执行enable()，打开串口
*/

extern "C" void USART1_IRQHandler(void);
extern "C" void USART2_IRQHandler(void);
extern "C" void USART3_IRQHandler(void);
extern "C" void UART4_IRQHandler(void);
extern "C" void UART5_IRQHandler(void);

class Uart_Base_Class
{
public:
	Uart_Base_Class(USART_TypeDef *UartX) : Uart(UartX) {};
	virtual ~Uart_Base_Class() = default;

	inline void enable(void) { Uart->CR1 |= USART_CR1_UE; } //开启串口
	inline void disable(void) { Uart->CR1 &= ~USART_CR1_UE; }//关闭串口
	inline void Clear_IDLE_Flag(void) {	//先读入SR，再读入DR清除IDLE中断标志
		uint16_t temp = Uart->SR;
		temp = Uart->DR;
	}


	inline void print(const char *str) {
		while (*str)
			write(*str++);
	}
	inline void print(const char  *buffer, size_t size)
	{
		while (size--)
			write(*buffer++);
	}
	inline void print(const uint8_t *buffer, size_t size) { print((char *)buffer, size); }
	inline void print(const std::string &s)
	{
		for (auto i : s)
		{
			write((char)i);
		}
	}

	inline void print(char c, int base = 0) { print((long)c, base); }
	inline void print(unsigned char b, int base = 0) { print((unsigned long)b, base); }
	inline void print(int n, int base = 10) { print((long)n, base); }
	inline void print(unsigned int n, int base = 10) { print((unsigned long)n, base); }
	void print(long n, int base = 10);
	void print(unsigned long n, int base = 10);
	inline void print(float n, int digits = 2) { printFloat(n, digits); }

	Uart_Base_Class& operator<<(const char *str) { print(str); return *this; }
	Uart_Base_Class& operator<<(const std::string &s) { print(s); return *this; }
	Uart_Base_Class& operator<<(char c) { print(c); return *this; }
	Uart_Base_Class& operator<<(unsigned char b) { print(b); return *this; }
	Uart_Base_Class& operator<<(int n) { print(n); return *this; }
	Uart_Base_Class& operator<<(unsigned int n) { print(n); return *this; }
	Uart_Base_Class& operator<<(long n) { print(n); return *this; }
	Uart_Base_Class& operator<<(unsigned long n) { print(n); return *this; }
	Uart_Base_Class& operator<<(float n) { print(n); return *this; }

protected:
	void Init(USART_InitTypeDef *Usart_InitStructure) { USART_Init(Uart, Usart_InitStructure); }; //初始化串口

	USART_TypeDef *Uart;

private:
	virtual void write(const char c) = 0; //写一个字符
	void printNumber(unsigned long n, const uint8_t base);
	void printFloat(float number, uint8_t digits);
};
