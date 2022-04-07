
#include "main.h"


void PWM_Config(int period, int duty,int pwmIndex){
	PWM_Disable(XPAR_PWM_0_PWM_AXI_BASEADDR);
	PWM_Set_Period(XPAR_PWM_0_PWM_AXI_BASEADDR, period);
	PWM_Set_Duty(XPAR_PWM_0_PWM_AXI_BASEADDR, duty, pwmIndex);
	PWM_Enable(XPAR_PWM_0_PWM_AXI_BASEADDR);
}


void timerPWM_Config(XTmrCtr *InstancePtr, int period, int duty){
	/* Disable PWM for reconfiguration */
	XTmrCtr_PwmDisable(InstancePtr);
	XTmrCtr_PwmConfigure(InstancePtr, period, duty);
	/* Enable PWM */
	XTmrCtr_PwmEnable(InstancePtr);
}

void timerPWM_Init(XTmrCtr *InstancePtr){
	int Status;
	Status = XTmrCtr_Initialize(InstancePtr, TMRCTR_PWM_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		printf("PWM INIT FAIL");
	}
}
