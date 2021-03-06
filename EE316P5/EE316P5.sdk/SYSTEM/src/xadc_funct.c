/*
 * main.c
 *
 *  Created on: Apr 18, 2018
 *      Author: arthur
 */

#include "main.h"

/* List of available Channels */
const u8 Channel_List[NUMBER_OF_CHANNELS] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
}; // 00008
/* Channel Names */
const char *Channel_Names[32] = {
	"", "", "", "VP-VN",
	"", "", "", "",
	"", "", "", "",
	"", "", "", "",
	"A0", "A1", "A2", "A3",
	"A4", "A5", "A6", "A7",
	"A8", "A9", "A10", "A11",
	"A12", "A13", "A14", "A15"
};

/* Initialize xADC in Single channel Mode */
void Xadc_Init(XSysMon *InstancePtr, u32 DeviceId) {
	XSysMon_Config *ConfigPtr;
	ConfigPtr = XSysMon_LookupConfig(DeviceId);
	XSysMon_CfgInitialize(InstancePtr, ConfigPtr, ConfigPtr->BaseAddress);

	// Set Sequencer mode to single channel
	XSysMon_SetSequencerMode(InstancePtr,XSM_SEQ_MODE_SINGCHAN);
	// Set single channel default to AUX min
	XSysMon_SetSingleChParams(InstancePtr , XADCPS_CH_AUX_MIN, FALSE, FALSE, FALSE);
	// Set all Unipolar
	XSysMon_SetSeqInputMode(InstancePtr,0);
	// Set the ADCCLK frequency equal to 1/32 of System clock
	XSysMon_SetAdcClkDivisor(InstancePtr, 32);
	// Enable Calibration
	XSysMon_SetCalibEnables(InstancePtr, XSM_CFR1_CAL_PS_GAIN_OFFSET_MASK | XSM_CFR1_CAL_ADC_GAIN_OFFSET_MASK);
}

/* For Debug Statements */
#define READDATA_DBG 0
/* Read data at selected channel */
u32 Xadc_ReadRaw (XSysMon *InstancePtr,u16 channelSel)
{
	u16 adc_data;
	// Change Channel Selected
	XSysMon_SetSingleChParams(InstancePtr , XADCPS_CH_AUX_MIN+channelSel, FALSE, FALSE, FALSE);
	// Read Data
	adc_data = (u16)XSysMon_GetAdcData(InstancePtr,XADCPS_CH_AUX_MIN+channelSel);

	if (READDATA_DBG != 0)
		printf("%d \n\r",adc_data);

	return adc_data; // return a high bit for each channel successfully read
}

/* Convert ADC data to Voltage */
float Xadc_RawToVoltage(u16 Data) {
	return ADC_MAXIMUM_VOLTAGE * (float)Data/(float)0xFFFF;
}

/* Read data and convert it to Voltage */
float Xadc_ReadConverted(XSysMon *InstancePtr, u32 ChannelSelect) {
	float Xadc_VoltageData;
	Xadc_VoltageData = Xadc_RawToVoltage(Xadc_ReadRaw(InstancePtr,ChannelSelect)); //Grab Raw Voltage data
	if (READDATA_DBG != 0)
		printf("Analog Input %s: %.3fV\r\n", Channel_Names[XADCPS_CH_AUX_MIN + ChannelSelect], Xadc_VoltageData);
	return Xadc_VoltageData;
}
