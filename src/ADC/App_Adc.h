/*
 * App_Adc.h
 *
 *  Created on: 2026. 9. 30.
 *      Author: David.Kang
 */

#ifndef ADC_APP_ADC_H_
#define ADC_APP_ADC_H_

#include "Adc_Types.h"

#define ADC_DMA_RESULT_BUF_SIZE			(1U)

Adc_ValueGroupType * App_Adc_Get_Dma_Result_Buf(void);
void ISR_DMA_CALLBACK(void);

/* Live debugger watch values; completion count is updated last by the ISR. */
extern volatile uint32 App_Adc_DmaCompleteCount;
extern volatile Adc_ValueGroupType App_Adc_LastResult;
extern volatile Adc_StatusType App_Adc_LastStatus;
extern volatile Std_ReturnType App_Adc_SetupResult;



#endif /* ADC_APP_ADC_H_ */
