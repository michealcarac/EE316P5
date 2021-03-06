/* Includes */
#include "xtmrctr.h"   /* xTimer Control */
#include "xadcps.h"    /* xADC Control */
#include "xscugic.h"   /* GIC Control */
#include "xgpio.h"     /* GPIO COntrol */
#include "xparameters.h"
#include "xil_exception.h"
#include "PWM.h"       /* Custom PWM Control */
#include "xil_io.h"
#include "xsysmon.h"
#include "sleep.h"
#include "stdio.h"
#include "math.h"
#include "xgpio.h"
#include "xil_types.h"

/************************** Constant Definitions *****************************/
/* PWM/Timer Usage */
#define TMRCTR_PWM_DEVICE_ID    XPAR_TMRCTR_0_DEVICE_ID // Timer device 1
#define TMRCTR_DEVICE_ID        XPAR_TMRCTR_1_DEVICE_ID // Timer device 2
#define TIMER_PWM_PERIOD        3125000  // 25ms
#define PWM_PERIOD              2500000  // 20ms
#define PWM_index               0        // 0 index
#define SERVO_MAX_PERCENT 		0.125    // For Servo 45
#define SERVO_MIN_PERCENT 		0.025    // For Servo -45
#define PWM_DUTY_MULTIPLIER 	PWM_PERIOD*(SERVO_MAX_PERCENT-SERVO_MIN_PERCENT)  // For Servo
#define PWM_DUTY_OFFSET     	PWM_PERIOD*SERVO_MIN_PERCENT                      // For Servo
/* ADC Usage */
#define XADC_DEVICE_ID 			XPAR_XADC_WIZ_0_DEVICE_ID  // ADC 0
#define ADC_MAXIMUM_VOLTAGE 	3.3
#define NUMBER_OF_CHANNELS 		16
/* GPIO Usage */
#define BTN_DEVICE_ID 			XPAR_AXI_GPIO_0_DEVICE_ID   // GPIO 0
#define INTC_DEVICE_ID          XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID  XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
/* LCD */
#define LCD_DEVICE_ID 			XPAR_AXI_GPIO_1_DEVICE_ID   // GPIO 1
#define LCD_GPIO 	XPAR_GPIO_1_BASEADDR

/************************** Variable Definitions *****************************/
XTmrCtr TimerCounterInst;  /* The instance of the Timer Counter */
XTmrCtr TimerPWMInst;      /* The instance of the Timer PWM */
XSysMon Xadc;              /* The instance of the xADC */
XGpio GpioBtn;             /* The instance of the Button GPIO */
XGpio GpioLCD;                 /* The instance of the LCD GPIO */
XScuGic Intc;              /* The instance of the Interrupt */
XScuGic_Config IntcConfig; /* The instance of the Int Config */



/* From Custom IP Core */
void PWM_Config(int period, int duty, int pwmIndex);
/* From Timer */
void timerPWM_Init(XTmrCtr *InstancePtr);
void timerPWM_Config(XTmrCtr *InstancePtr, int period, int duty);
/* From ADC */
void Xadc_Init(XSysMon *InstancePtr, u32 DeviceId);
u32 Xadc_ReadRaw (XSysMon *InstancePtr,u16 channelSel);
float Xadc_RawToVoltage(u16 Data);
float Xadc_ReadConverted(XSysMon *InstancePtr, u32 ChannelSelect);

/* From GPIO */
void Btn_Init(XGpio *InstancePtr, u32 DeviceId);
int GPIO_Init(XGpio *InstancePtr, u32 DeviceId);
/* From LCD */
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void LCD_data_ln(char* string);
/* Interrupts */
int Int_Init(XScuGic *IntInstancePtr, XGpio *GpioInstancePtr, u32 IntDeviceId, u32 GpioDeviceId);
int Int_Connect_to_GPIO(XScuGic *IntInstancePtr, XGpio *GpioInstancePtr, u32 IntDeviceId, u32 GpioDeviceId);
int InterruptSystemSetup(XScuGic *XScuGicInstancePtr, XGpio *GpioInstancePtr, u32 GpioDeviceId);
void Gpio_Intr_Handler(void *InstancePtr);

