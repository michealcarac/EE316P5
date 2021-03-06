#include "main.h"

/* Custom IP Core */
void PWM_Config(int period, int duty,int pwmIndex){
	/* Disable Peripheral */
	PWM_Disable(XPAR_PWM_0_PWM_AXI_BASEADDR);
	/*     Set Period     */
	PWM_Set_Period(XPAR_PWM_0_PWM_AXI_BASEADDR, period);
	/*      Set Duty      */
	PWM_Set_Duty(XPAR_PWM_0_PWM_AXI_BASEADDR, duty, pwmIndex);
	/*  Enable Peripheral */
	PWM_Enable(XPAR_PWM_0_PWM_AXI_BASEADDR);
}

/* Timer PWM */
void timerPWM_Config(XTmrCtr *InstancePtr, int period, int duty){
	/* Disable PWM for reconfiguration */
	XTmrCtr_PwmDisable(InstancePtr);
	/* Configure PWM with duty/period  */
	XTmrCtr_PwmConfigure(InstancePtr, period, duty);
	/*           Enable PWM            */
	XTmrCtr_PwmEnable(InstancePtr);
}

/* Timer PWM */
void timerPWM_Init(XTmrCtr *InstancePtr){
	/* Initialize Status */
	int Status;
	/* Initialize Timer */
	Status = XTmrCtr_Initialize(InstancePtr, TMRCTR_PWM_DEVICE_ID);
	/* print fail if Timer does not initialize */
	if (Status != XST_SUCCESS) {
		printf("PWM INIT FAIL");
	}
}
