#include "init.h"
#include "Mcu.h"
#include "Mcl.h"
#include "Pwm.h"
#include "Siul2_Port_Ip.h"
#include "Platform.h"
#include "Icu.h"
#include "CDD_Uart.h"
#include "Wdg_43_Instance0.h"
#include "Gpt.h"

//#define _DEBUG_CLOCK_
#ifdef _DEBUG_CLOCK_
#include "Clock_Ip.h"
#include "Clock_Ip_Types.h"
#include "Clock_Ip_Cfg.h"
#endif

void ALL_COMPONENT_INIT(void)
{
	/* MCU */
	Mcu_Init(NULL_PTR);
	uint8_t u8Ret = Mcu_InitClock(0);
	if (u8Ret != E_OK)
	{
		for (;;)
		{
			/* CLOCK Configure Failed */
		}
	}

	/* Wait for PLL lock. */
	while (MCU_PLL_LOCKED != Mcu_GetPllStatus())
	{
	  /* DUMMY */
	}

	/* Switch PLL-fed clock muxes to their configured sources. */
	if (E_OK != Mcu_DistributePllClock())
	{
	  for (;;)
	  {
		  /* PLL distribution failed */
	  }
	}

	Mcu_SetMode(0);

#ifdef _DEBUG_CLOCK_
	volatile uint64 u32CurCoreClkHz = Clock_Ip_GetClockFrequency(CORE_CLK);
	volatile uint64 u32CurEmios1ClkHz = Clock_Ip_GetClockFrequency(EMIOS1_CLK);
#endif
	/* WATCHDOG */
	Wdg_43_Instance0_Init(NULL_PTR);

	/* STM0 in GPT Timer */
	Gpt_Init(NULL_PTR);
	Gpt_EnableNotification(0U);

	/* PORT */
	Siul2_Port_Ip_Init(	NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
						g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals
						);

	/* NVIC ... */
	Platform_Init(NULL_PTR);

	/* Create Clock BUS */
	Mcl_Init(NULL_PTR);

	/* PWM */
	Pwm_Init(NULL_PTR);

	/* ICU IRQ ENABLE */
	Icu_Init(NULL_PTR);
	Icu_EnableNotification(IcuChannel_0);
	Icu_EnableEdgeDetection(IcuChannel_0);

	/* UART */
	Uart_Init(NULL_PTR);

	/* Clock Source : 48MHz, 4800000tick is 100ms */
	Gpt_StartTimer(0U, 4800000U);
}
