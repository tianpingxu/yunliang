#ifndef APP_SAMPLE_H
#define APP_SAMPLE_H

#define  ADC_BUFF_MAX	(6)

//#define BAT_VOL_MAX 	(4180)
#define ADC_TYPE_ADS1230_1 1
#define ADC_TYPE_ADS1230_2 2
#define ADC_TYPE_VBAT 3

typedef struct __attribute__((packed, aligned(1)))
{
	float pressure_h_factor;
	float pressure_h_offset1;
	float pressure_h_offset2;
	float pressure_l_factor;
	float pressure_l_offset1;
	float pressure_l_offset2;
	UCHAR cali_flag;
}ST_PRESSURE_CALI;

typedef struct
{
	float p_bar;
	LONG p_0001xbar;	//0.001
	LONG p_0001xbar_report;	//0.001
	float p_kpa;
	float p_mpa;
	float p_psi;
}ST_PRESSURE_UNIT;



extern ST_PRESSURE_UNIT st_gHL_Pressure[2];
extern SHORT s16_gHL_Temp[2];
extern UCHAR u8_gBatSocLevel;
extern SHORT s16_gBatSocReport;
extern SHORT s16_gBatSocCal;
extern SHORT s16_gBatVoltage;
extern SHORT s16_gBatTemp;
extern ST_PRESSURE_CALI st_gPressureCaliParam;
extern LONG g_adc_high,g_adc_low;
extern UCHAR u8_gSocAdjustLimit;

void vSamplePressureTemp(void);
void vBatteryCalSoc(SHORT *soc,UCHAR *soc_level);
void vSampleTemp(void);
LONG adc_buff_inc(uint8_t adc_idx,LONG value);


#endif
