/*
 * init.c
 *
 *  Created on: 2026. 9. 17.
 *      Author: David.Kang
 */

#include "init.h"
#include "Platform.h"
#include "Mcu.h"
#include "Mcl.h"
#include "Pwm.h"
#include "Icu.h"
#include "Gpt.h"
#include "Adc.h"
#include "Wdg_43_Instance0.h"
#include "CDD_Uart.h"
#include "CDD_Rm.h"
#include "Siul2_Port_Ip.h"

#include "../ADC/App_Adc.h"

//#define _DEBUG_CLOCK_
#ifdef _DEBUG_CLOCK_
#include "Clock_Ip.h"
#include "Clock_Ip_Types.h"
#include "Clock_Ip_Cfg.h"
#endif

void ALL_COMPONENT_INIT(void)
{
	Adc_ValueGroupType * ptrDmaBuf = App_Adc_Get_Dma_Result_Buf();

	/* MCU */
	Mcu_Init(NULL_PTR);
	uint8_t u8Ret = Mcu_InitClock(0);
	if (u8Ret != E_OK) { for (;;) { /* CLOCK Configure Failed */ } }

	/* Wait for PLL lock. */
	while (MCU_PLL_LOCKED != Mcu_GetPllStatus()) { /* DUMMY */ }

	/* Switch PLL-fed clock muxes to their configured sources. */
	if (E_OK != Mcu_DistributePllClock()) { for (;;) { /* PLL distribution failed */ } }

	/* MCU Set Mode 0 */
	Mcu_SetMode(0);

#ifdef _DEBUG_CLOCK_
	volatile uint64 u64CurCoreClkHz = Clock_Ip_GetClockFrequency(CORE_CLK);
	volatile uint64 u64CurEmios1ClkHz = Clock_Ip_GetClockFrequency(EMIOS1_CLK);
#endif

#if WATCHDOG_ENABLE
	/* WATCHDOG */
	Wdg_43_Instance0_Init(NULL_PTR);
#endif

	/* STM0 in GPT Timer */
	Gpt_Init(NULL_PTR);
	Gpt_EnableNotification(0U);

	/* PORT */
	Siul2_Port_Ip_Init(	NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
						g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals );

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

	MCAL_DATA_SYNC_BARRIER();
	/* Resource Manager */
	Rm_Init(NULL_PTR);
	MCAL_DATA_SYNC_BARRIER();

	/* ADC */
	Adc_Init(NULL_PTR);
	MCAL_DATA_SYNC_BARRIER();

	ptrDmaBuf[0] = 0xFFFFU; /* Sentinel: no ADC result received yet. */
	App_Adc_SetupResult = Adc_SetupResultBuffer(AdcGroup_0, ptrDmaBuf);
	if (App_Adc_SetupResult == E_OK)
	{
		Adc_EnableHardwareTrigger(0U);
	}

	/* Clock Source : 48MHz, 4800000tick is 100ms */
	Gpt_StartTimer(0U, 4800000U);
}
