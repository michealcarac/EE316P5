/*
 * main.c
 *
 *  Created on: Apr 18, 2018
 *      Author: arthur
 */

#include "main.h"

const u8 Channel_List[NUMBER_OF_CHANNELS] = {
	3, // Start with VP/VN
	28, 16, 24, // Diff. Channels in ascending order
	17, 25, 22, 31, 21, 29 // Single-Ended Channels in ascending order
}; // 00008
const char *Channel_Names[32] = {
	"", "", "", "VP-VN",
	"", "", "", "",
	"", "", "", "",
	"", "", "", "",
	"A8-A9", "A0", "", "",
	"", "A4", "A2", "",
	"A10-A11", "A1", "", "",
	"A6-A7", "A5", "", "A3"
};

/* this needs to be worked on */
void Xadc_Init(XSysMon *InstancePtr, u32 DeviceId) {
	XSysMon_Config *ConfigPtr;
	ConfigPtr = XSysMon_LookupConfig(DeviceId);
	XSysMon_CfgInitialize(InstancePtr, ConfigPtr, ConfigPtr->BaseAddress);

	// Disable the Channel Sequencer before configuring the Sequence registers.
	XSysMon_SetSequencerMode(InstancePtr, XSM_SEQ_MODE_SAFE);
	// Disable all alarms
	XSysMon_SetAlarmEnables(InstancePtr, 0x0);
	// Set averaging for all channels to 16 samples
	XSysMon_SetAvg(InstancePtr, XSM_AVG_16_SAMPLES);   // Can change this in future.
	// Set differential input mode for all channels
	//XSysMon_SetSeqInputMode(InstancePtr, XADC_SEQ_CHANNELS);
	// Set 6ADCCLK acquisition time in all channels
	XSysMon_SetSeqAcqTime(InstancePtr, XADC_SEQ_CHANNELS);
	// Disable averaging in all channels
	XSysMon_SetSeqAvgEnables(InstancePtr, XADC_SEQ_CHANNELS);
	// Enable all channels
	XSysMon_SetSeqChEnables(InstancePtr, XADC_SEQ_CHANNELS);
	// Set the ADCCLK frequency equal to 1/32 of System clock
	XSysMon_SetAdcClkDivisor(InstancePtr, 32);
	// Enable Calibration
	XSysMon_SetCalibEnables(InstancePtr, XSM_CFR1_CAL_PS_GAIN_OFFSET_MASK | XSM_CFR1_CAL_ADC_GAIN_OFFSET_MASK);
	// Enable the Channel Sequencer in continuous sequencer cycling mode
	XSysMon_SetSequencerMode(InstancePtr, XSM_SEQ_MODE_CONTINPASS);
	// Clear the old status
//	XSysMon_GetStatus(InstancePtr);
}

#define READDATA_DBG 1
u32 Xadc_ReadData (XSysMon *InstancePtr, u16 RawData[32])
{
	u8 Channel;

	if (READDATA_DBG != 0)
		xil_printf("Waiting for EOS...\r\n");

	// Clear the Status
	XSysMon_GetStatus(InstancePtr);
	// Wait until the End of Sequence occurs
	while ((XSysMon_GetStatus(InstancePtr) & XSM_SR_EOS_MASK) != XSM_SR_EOS_MASK);

	if (READDATA_DBG != 0)
		xil_printf("Capturing XADC Data...\r\n");
	// Cycle through Channels
	for (Channel=0; Channel<32; Channel++) {
		if (((1 << Channel) & XADC_CHANNELS) != 0) {
			if (READDATA_DBG != 0)
				xil_printf("Capturing Data for Channel %d\r\n", Channel);
			RawData[Channel] = XSysMon_GetAdcData(InstancePtr, Channel);
		}
	}
	return XADC_CHANNELS; // return a high bit for each channel successfully read
}

float Xadc_RawToVoltage(u16 Data) {
	return ADC_MAXIMUM_VOLTAGE * ((float)Data / ((float)(2^10) - 1)); //2^resolution (10 bit resolution)
}

/* This needs to be looked at */
//void Btn_Init(XGpio *InstancePtr, u32 DeviceId) {
//	XGpio_Config *ConfigPtr;
//	printf("Btn_Init 1");
//	ConfigPtr = XGpio_LookupConfig(DeviceId);
//	printf("Btn_Init 2");
//	XGpio_CfgInitialize(InstancePtr, ConfigPtr, ConfigPtr->BaseAddress);
//	printf("Btn_Init 3");
//	XGpio_SetDataDirection(InstancePtr, 1, 0b11);
//	printf("Btn_Init 4");
//}

u16 Xadc_Read(XSysMon *InstancePtr, u32 ChannelSelect) {
	u16 Xadc_RawData[32];
	u32 ChannelValidVector;
	float Xadc_VoltageData;
	ChannelValidVector = Xadc_ReadData(InstancePtr, Xadc_RawData);
	if (Test_Bit(ChannelValidVector, ChannelSelect)) {
		Xadc_VoltageData = Xadc_RawToVoltage(Xadc_RawData[ChannelSelect]);
		printf("Analog Input %s: %.3fV\r\n", Channel_Names[ChannelSelect], Xadc_VoltageData);
	}
	return Xadc_VoltageData;
}
