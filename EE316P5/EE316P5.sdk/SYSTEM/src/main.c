
/***************************** Include Files *********************************/
#include "main.h"

/************************** Variable Definitions *****************************/
XTmrCtr TimerCounterInst;  /* The instance of the Timer Counter */
XTmrCtr TimerPWMInst;      /* The instance of the Timer PWM */
XSysMon Xadc;              /* The instance of the xADC */
XGpio Btn;                 /* The instance of the Button GPIO */
XGpio LCD;                 /* The instance of the LCD GPIO */

int main(void)
{
	/* Initialize Variables */
	u8 ChannelIndex = 0;  /* Select ADC Channel */
	u16 duty_ADC = 0;
	u32 Btn_Data;         /* Initialize Button Data */
	u32 time_count = 0;  /* Counter for ADC, will be switched to a delay from timer interrupts */
	Debounce Btn0_Db, Btn1_Db, Btn2_Db, Btn3_Db;  /* Type of Debounce */

	/* ADC and Button Init */
	Xadc_Init(&Xadc, XADC_DEVICE_ID);
//	Btn_Init(&Btn, BTN_DEVICE_ID);
//	Debounce_Init(&Btn0_Db, 10000);
//	Debounce_Init(&Btn1_Db, 10000);
//	Debounce_Init(&Btn2_Db, 10000);
//	Debounce_Init(&Btn3_Db, 10000);
	/* Timer/PWM Init */
//	timerPWM_Init(&TimerPWMInst);   //Initialize Timer PWM
//	timerPWM_Config(&TimerPWMInst,TIMER_PWM_PERIOD,TIMER_PWM_PERIOD/2); //Initialized to 50% duty
	PWM_Config(PWM_PERIOD,PWM_PERIOD/2,0); //Initialized to 50% duty
	printf("test\n\r");
	/* While loop */
	while(1){
		if (time_count == 100000) { // print channel reading approx. 10x per second
			time_count = 0;
			duty_ADC = Xadc_Read(&Xadc,17); //Grab ADC Value of channel 0
			duty_ADC = PWM_PERIOD * duty_ADC/ADC_MAXIMUM_VOLTAGE;
		}
		PWM_Config(PWM_PERIOD,ceil(duty_ADC),0); //Set Duty Cycle
		//printf("duty_adc: %d \n\r",ceil(duty_ADC));
		//printf("PWM Duty: %d, PWM Period: %d \n\r",PWM_Get_Duty(XPAR_PWM_0_PWM_AXI_BASEADDR,PWM_index),PWM_PERIOD);

		time_count ++;
		usleep(1);
	}

}
//printf("Timer PWM Duty: %d, PWM Period: %d \n\r",duty_ADC,TIMER_PWM_PERIOD);




