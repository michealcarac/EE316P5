#include "main.h"

/* GPIO Init */
int GPIO_Init(XGpio *InstancePtr, u32 DeviceId){
	/* Initialize Status */
	int Status;

	/* Initialize GPIO */
	Status = XGpio_Initialize(InstancePtr, DeviceId);

	/* print fail if Timer does not initialize */
	if (Status != XST_SUCCESS) {
		printf("GPIO INIT FAIL");
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

/* Interrupt Init */
int Int_Init(XScuGic *IntInstancePtr, XGpio *GpioInstancePtr, u32 IntDeviceId, u32 GpioDeviceId){
	/* Initialize Status */
	XScuGic_Config *IntcConfig;
	int Status;

	/* Initialize Interrupt Controller */
	IntcConfig = XScuGic_LookupConfig(IntDeviceId);
	Status = XScuGic_CfgInitialize(IntInstancePtr,IntcConfig,IntcConfig->CpuBaseAddress);

	/* print fail if Timer does not initialize */
	if (Status != XST_SUCCESS) {
		printf("INT INIT FAIL");
		return XST_FAILURE;
	}

	Status = InterruptSystemSetup(IntInstancePtr,GpioInstancePtr, GpioDeviceId);

	/* print fail if Timer does not initialize */
	if (Status != XST_SUCCESS) {
		printf("INT INIT FAIL");
		return XST_FAILURE;
	}

	// Connect interrupt to GPIO
	Int_Connect_to_GPIO(IntInstancePtr,GpioInstancePtr,IntDeviceId, GpioDeviceId);

	return XST_SUCCESS;
}


int Int_Connect_to_GPIO(XScuGic *IntInstancePtr, XGpio *GpioInstancePtr, u32 IntDeviceId, u32 GpioDeviceId){
	int Status;

	// Connect GPIO interrupt to handler
	Status = XScuGic_Connect(IntInstancePtr,INTC_GPIO_INTERRUPT_ID,(Xil_ExceptionHandler)Gpio_Intr_Handler,(void*)GpioInstancePtr);

	/* print fail if Timer does not initialize */
	if (Status != XST_SUCCESS) {
		printf("INT INIT FAIL");
		return XST_FAILURE;
	}

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr,XGPIO_IR_CH1_MASK);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO interrupts in the controller
	XScuGic_Enable(IntInstancePtr,INTC_GPIO_INTERRUPT_ID);

	return XST_SUCCESS;
}

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr, XGpio *GpioInstancePtr, u32 GpioDeviceId){
	// Enable Interrupt
	XGpio_InterruptEnable(GpioInstancePtr,GpioDeviceId);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler) XScuGic_InterruptHandler,XScuGicInstancePtr);
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}



