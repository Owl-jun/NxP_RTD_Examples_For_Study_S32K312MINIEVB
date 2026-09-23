
#include "App_Gpt.h"

#include "Gpt.h"
#include "stdio.h"
#include "../Uart/App_Uart.h"

static volatile uint32_t u32TimeSec = 0U;

uint32_t App_Gpt_Get_Elapsed_Sec(void) { return u32TimeSec; }

void App_Gpt_Send_Time_Uart(void)
{
	char cText[40];
    static uint32_t u32PrevElapsedSec = 0U;
    if (u32PrevElapsedSec != u32TimeSec)
    {
    	snprintf(cText, sizeof(cText), "Boot Time : %lu sec", (unsigned long)u32TimeSec);
		(void)App_Uart_Send(cText);
		u32PrevElapsedSec = u32TimeSec;
    }
}

void Gpt_Stm0_Noti(void)
{
	static uint8_t u8ElapseTime = 0U;

	u8ElapseTime++;
	if (u8ElapseTime >= 10U)
	{
		u32TimeSec++;
		u8ElapseTime = 0U;
	}
}
