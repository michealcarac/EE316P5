
/***************************** Include Files *********************************/
#include "main.h"


/* Global States */
int FSM_State = 0;
int sys_en    = 1;

int main(void)
{
	/* Initialize Variables */

	/* ADC Value */
	float ADC_Val = 0;

	/* PWM Variables */
	u32 PWM_Servo = 0;
	u32 PWM_Timer = 0;

	/* Incrementors */
	u32 time_count = 0;  /* Counter for ADC, will be switched to a delay from timer interrupts */

	/* ADC Init */
	Xadc_Init(&Xadc, XADC_DEVICE_ID);

	/* Timer/PWM Init */
	timerPWM_Init(&TimerPWMInst);   //Initialize Timer PWM
	timerPWM_Config(&TimerPWMInst,TIMER_PWM_PERIOD,0); //Initialized to 0% duty
	PWM_Config(PWM_PERIOD,0,0); //Initialized to 0% duty

	/* GPIO Btns Initialize */
	GPIO_Init(&GpioBtn,BTN_DEVICE_ID);
	XGpio_SetDataDirection(&GpioBtn, 1,0xFF); // Set all buttons to inputs

	/* GPIO LCD Initialize */
	GPIO_Init(&GpioLCD,LCD_DEVICE_ID);
	LCD_init();

	/* Button Interrupt Enabled */
	Int_Init(&Intc,&GpioBtn,INTC_DEVICE_ID,BTN_DEVICE_ID);

	/* While loop */
	while(1){
		switch(sys_en) {
			case 0:
				/* System Disabled State */
				printf("SYSTEM DISABLED \n\r");
				/* Reset variables */
				FSM_State = 0;
				/* Reset Peripherals */
				timerPWM_Config(&TimerPWMInst,TIMER_PWM_PERIOD,100); //Initialized to near 0% duty
				PWM_Config(PWM_PERIOD,(u32)(((.1/ADC_MAXIMUM_VOLTAGE)*PWM_DUTY_MULTIPLIER)+PWM_DUTY_OFFSET),0); //Initialized to 2.5% duty

				LCD_command(1);
				LCD_data_ln("Reset/Disabled");
				usleep(100000);
				break;
			case 1:
				/* System Enabled State */
				if (time_count == 100000) { // print channel reading approx. 10x per second
					time_count = 0;
					switch(FSM_State) {
						case 0:
							ADC_Val = Xadc_ReadConverted(&Xadc,9); //Grab ADC Value of channel 9
							printf("ADC Value (A1): %.3f \n\r",ADC_Val);
							LCD_command(1);
							LCD_data_ln("    Enabled");
							LCD_command(0xC0);
							LCD_data_ln(" Potentiometer");

							break;
						case 1:
							ADC_Val = Xadc_ReadConverted(&Xadc,1); //Grab ADC Value of channel 1
							printf("ADC Value (A0): %.3f \n\r",ADC_Val);
							LCD_command(1);
							LCD_data_ln("    Enabled");
							LCD_command(0xC0);
							LCD_data_ln(" Photoresistor");
							break;
					}

					/* Custom PWM Core */
					/* This will be running a servo from 2.5% to 12.5% duty */
					PWM_Servo = (u32)(((ADC_Val/ADC_MAXIMUM_VOLTAGE)*PWM_DUTY_MULTIPLIER)+PWM_DUTY_OFFSET); // 2.5% to 12.5% period
					PWM_Config(PWM_PERIOD,PWM_Servo,0); //Set Duty Cycle
					printf("PWM Servo Duty: %lu      %.2f%%\n\r", PWM_Servo,100*((float)PWM_Servo/PWM_PERIOD));

					/* Timer PWM Core */
					/* This will be running the LEDs, DC Motor, Passive Buzzer */
					PWM_Timer = (u32)((ADC_Val/ADC_MAXIMUM_VOLTAGE)*TIMER_PWM_PERIOD); // 0 to max period
					timerPWM_Config(&TimerPWMInst,TIMER_PWM_PERIOD, PWM_Timer);
					printf("Timer PWM Duty: %lu     %.2f%%\n\r", PWM_Timer,100*((float)PWM_Timer/TIMER_PWM_PERIOD));

				}
				time_count++;
				usleep(1);
				break;
		}

	}

}

void Gpio_Intr_Handler(void *InstancePtr)
{
	int btn_value = 0;
	int prev_button;

	// Disable GPIO interrupts
	XGpio_InterruptDisable(&GpioBtn, XGPIO_IR_CH1_MASK);
	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&GpioBtn) & XGPIO_IR_CH1_MASK) !=XGPIO_IR_CH1_MASK) {
		return;
	}
	btn_value = XGpio_DiscreteRead(&GpioBtn, 1); // Channel 1
	if (btn_value == 1) {   // Reset System
		sys_en = 0;
	}
	else if (btn_value == 2) { // Switch Analog Source
		FSM_State = !FSM_State;
	}
	else if (btn_value == 4) { // Switch System Enabled
		sys_en = !sys_en;
	}
	else if ((prev_button == 1) & (btn_value == 0)){
		sys_en = 1;
	}
	prev_button = btn_value;
	(void) XGpio_InterruptClear(&GpioBtn, XGPIO_IR_CH1_MASK);
	// Enable GPIO interrupts
	XGpio_InterruptEnable(&GpioBtn, XGPIO_IR_CH1_MASK);
}





