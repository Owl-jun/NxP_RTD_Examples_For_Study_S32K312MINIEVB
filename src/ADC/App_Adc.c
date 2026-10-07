/*
 * App_Adc.c
 *
 *  Created on: 2026. 9. 30.
 *      Author: David.Kang
 */

#include "App_Adc.h"
#include "Adc.h"

/* RTD DMA completion entry point, normally selected as the MCL callback. */
extern void Adc_Ipw_Adc0DmaTransferCompleteNotification(void);

static Adc_ValueGroupType aDmaBuf[ADC_DMA_RESULT_BUF_SIZE] __attribute__((section(".mcal_bss_no_cacheable"), aligned(32)));
volatile Std_ReturnType App_Adc_SetupResult = E_NOT_OK;

#ifdef _DEBUG_ADC_
volatile uint32 App_Adc_DmaCompleteCount = 0U;
volatile Adc_ValueGroupType App_Adc_LastResult = 0xFFFFU;
volatile Adc_StatusType App_Adc_LastStatus = ADC_IDLE;
#endif

Adc_ValueGroupType * App_Adc_Get_Dma_Result_Buf(void)
{
	return aDmaBuf;
}

void ISR_DMA_CALLBACK(void)
{
    /* Required: update ADC state and prepare DMA for the next HW trigger. */
    Adc_Ipw_Adc0DmaTransferCompleteNotification();

#ifdef _DEBUG_ADC_
    App_Adc_LastResult = aDmaBuf[0];
    App_Adc_LastStatus = Adc_GetGroupStatus(AdcGroup_0);
    App_Adc_DmaCompleteCount++;
#endif
}
