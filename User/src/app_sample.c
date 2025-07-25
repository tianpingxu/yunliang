#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_adc.h"
#include "bsp_ntc.h"
#include "bsp_timer.h"
#include "ADS1230.h"
#include "app_io.h"
#include "app_system.h"
#include "app_sample.h"
#include "Table_Refrigerant.h"

static void vAPI_Param_DataChatt(const st_state *wk_param,  UCHAR *wk_param_chatta_cnt, UCHAR *wk_param_pre_data, USHORT *wk_param_fix_data,  UCHAR wk_cnt_max );
extern const ST_REF_HL TAB_REF_HL[];

static SHORT sDelayTime3s = 60;
static SHORT sDelayTime1_5s = 30;
static SHORT sDelayTime5s = 16;

USHORT ucBatSocResult = 0;
UCHAR u8_gSocAdjustLimit = FALSE;


SHORT u16_soc0_th = 0;//3400;
SHORT u16_soc1_th = 3650;//3650;
SHORT u16_soc2_th = 3720;//3850;
SHORT u16_soc3_th = 3800;//4050;
SHORT u16_soc4_th = 3900;
const st_state PARAM_INFO_BAT_SOC[] =
{
    { &s16_gBatVoltage,   				&u16_soc0_th,        &sDelayTime5s,	API_COMP_OVER_EQUAL       },  
    { &s16_gBatVoltage,   				&u16_soc1_th,        &sDelayTime5s,	API_COMP_OVER_EQUAL       },  
    { &s16_gBatVoltage,   				&u16_soc2_th,        &sDelayTime5s,	API_COMP_OVER_EQUAL       },  
    { &s16_gBatVoltage,   				&u16_soc3_th,        &sDelayTime5s,	API_COMP_OVER_EQUAL       },  
    { &s16_gBatVoltage,   				&u16_soc4_th,        &sDelayTime5s,	API_COMP_OVER_EQUAL       },



//    { &u16_soc0_th,   				&s16_gBatVoltage,        &sDelayTime1_5s,	API_COMP_OVER_EQUAL       },  
//    { &u16_soc1_th,   				&s16_gBatVoltage,        &sDelayTime1_5s,	API_COMP_OVER_EQUAL       },  
//    { &u16_soc2_th,   				&s16_gBatVoltage,        &sDelayTime1_5s,	API_COMP_OVER_EQUAL       },  
//    { &u16_soc3_th,   				&s16_gBatVoltage,        &sDelayTime1_5s,	API_COMP_OVER_EQUAL       },  
//    { &u16_soc4_th,   				&s16_gBatVoltage,        &sDelayTime1_5s,	API_COMP_OVER_EQUAL       },
};
static UCHAR	dc_bat_soc_chatta_cnt[ELEMOF(PARAM_INFO_BAT_SOC)];
static UCHAR	dc_bat_soc_pre_data[ELEMOF(PARAM_INFO_BAT_SOC)];



SHORT s16_gBatTemp;
ST_PRESSURE_UNIT st_gHL_Pressure[2];
SHORT s16_gHL_Temp[2];
SHORT s16_gBatVoltage;
SHORT s16_gBatSocReport;
UCHAR u8_gBatSocLevel;
SHORT s16_gBatSocCal;
ST_PRESSURE_CALI st_gPressureCaliParam;


LONG g_adc_high=0,g_adc_low=0;
double pressure_high=0.0,pressure_low=0.0;

SHORT sPressureCalTemp(USHORT ref_type,float p_kpa,UCHAR hl)
{
	USHORT wk_idx;
	float wk_temp = 0;
	float wk_kpa_low,wk_kpa_high;
	float wk_temp_low, wk_temp_high;
	ST_REF_P_T *wk_ref_tab_select = (ST_REF_P_T *)TAB_REF_HL[ref_type].ref_high;
	USHORT wk_ref_tab_len = 0;

	if(hl == 0)
	{
		wk_ref_tab_select = (ST_REF_P_T *)TAB_REF_HL[ref_type].ref_high;
		wk_ref_tab_len = TAB_REF_HL[ref_type].high_total;
	}
	else
	{
		wk_ref_tab_select = (ST_REF_P_T *)TAB_REF_HL[ref_type].ref_low;
		wk_ref_tab_len = TAB_REF_HL[ref_type].low_total;
	}

	if(p_kpa <= wk_ref_tab_select[0].p_kpa)
	{
		wk_temp = wk_ref_tab_select[0].temp_01;
	}
	else if(p_kpa >= wk_ref_tab_select[wk_ref_tab_len - 1].p_kpa)
	{
		wk_temp = wk_ref_tab_select[wk_ref_tab_len - 1].temp_01;
	}
	else
	{
		for(wk_idx = 0; wk_idx < wk_ref_tab_len; wk_idx++)
		{
			wk_kpa_low = wk_ref_tab_select[wk_idx].p_kpa;
			wk_kpa_high = wk_ref_tab_select[wk_idx + 1].p_kpa;
			wk_temp_low = wk_ref_tab_select[wk_idx].temp_01;
			wk_temp_high = wk_ref_tab_select[wk_idx + 1].temp_01;
			if ((wk_kpa_high >= p_kpa) && (p_kpa >= wk_kpa_low))
			{
				wk_temp = (float)(wk_temp_high - wk_temp_low) * (p_kpa - wk_kpa_low) / (wk_kpa_high - wk_kpa_low) + wk_temp_low;
				break;
			}
		}
	}

	return (wk_temp);
}

static LONG adc1_buff[ADC_BUFF_MAX];
static LONG adc2_buff[ADC_BUFF_MAX];
static LONG adc3_buff[ADC_BUFF_MAX];
//void sort(LONG *a,int len)
//{
//    uint8_t i=0;uint8_t j;LONG t;
//    for(i=0;i<len-1;i++){
//        for(j=0;j<len-i-1;j++){
//            if(a[j]>a[j+1]){
//                t=a[j];
//                a[j]=a[j+1];
//                a[j+1]=t;
//            }
//        }
//    }
//}

LONG adc_buff_inc(uint8_t adc_idx,LONG value)
{
	 LONG adc_value_min;
	 LONG adc_value_max;
	 
	if(adc_idx==ADC_TYPE_ADS1230_1)
	{
		static uint8_t buff_cnt=0;
		static LONG new_value=0;
		adc1_buff[buff_cnt++] = value;
		if(buff_cnt == ADC_BUFF_MAX)
		{
			LONG total_value=(LONG)adc1_buff[0];
			adc_value_min = (LONG)adc1_buff[0];
			adc_value_max = (LONG)adc1_buff[0];
			for(uint8_t i=1;i<ADC_BUFF_MAX;i++)
			{
				total_value+=adc1_buff[i];
				adc_value_max = (adc1_buff[i] > adc_value_max)?adc1_buff[i]:adc_value_max;
				adc_value_min = (adc1_buff[i] < adc_value_min)?adc1_buff[i]:adc_value_min;
			}
			buff_cnt=0;
			new_value = (total_value -adc_value_max -adc_value_min) /(ADC_BUFF_MAX-2);
		}
		return new_value;
	}
	else if(adc_idx==ADC_TYPE_ADS1230_2)
	{
		static uint8_t buff_cnt=0;
		static LONG new_value=0;
		adc2_buff[buff_cnt++] = value;
		if(buff_cnt == ADC_BUFF_MAX)
		{
			LONG total_value=(LONG)adc2_buff[0];
			adc_value_min = (LONG)adc2_buff[0];
			adc_value_max = (LONG)adc2_buff[0];
			for(uint8_t i=1;i<ADC_BUFF_MAX;i++)
			{
				total_value+=adc2_buff[i];
				adc_value_max = (adc2_buff[i] > adc_value_max)?adc2_buff[i]:adc_value_max;
				adc_value_min = (adc2_buff[i] < adc_value_min)?adc2_buff[i]:adc_value_min;
			}
			buff_cnt=0;
			new_value = (total_value -adc_value_max -adc_value_min) /(ADC_BUFF_MAX-2);
		}
		return new_value;
	}
	else if(adc_idx==ADC_TYPE_VBAT)
	{
		static uint8_t buff_cnt=0;
		static LONG new_value=0;
		adc3_buff[buff_cnt++] = value;
		if(buff_cnt == ADC_BUFF_MAX)
		{
			LONG total_value=adc3_buff[0];
			adc_value_min = adc3_buff[0];
			adc_value_max = adc3_buff[0];
			for(uint8_t i=1;i<ADC_BUFF_MAX;i++)
			{
				total_value+=adc3_buff[i];
				adc_value_max = (adc3_buff[i] > adc_value_max)?adc3_buff[i]:adc_value_max;
				adc_value_min = (adc3_buff[i] < adc_value_min)?adc3_buff[i]:adc_value_min;
			}
			buff_cnt=0;
			new_value = (total_value -adc_value_max -adc_value_min) /(ADC_BUFF_MAX-2);
		}
		if(new_value==0)
			return value;
		return new_value;
	}

	return 0;
}


void vSamplePressureTemp(void)
{
	static BOOL high_flag=FALSE;
	float wk_pressure_kpa;	//绝对压力 = 表压 x 100 + 101.35
	float wk_pressure_bar;
	UCHAR wk_adc_enable;
	static UCHAR wk_adc_pre_enable = TRUE;

	wk_adc_enable = ((SYS_STATE_CHARG_IN_STOP == u8_gSysState) || (SYS_STATE_CHARG_IN_LOWPOWER == u8_gSysState))?FALSE:TRUE;
	if(wk_adc_enable == FALSE)
	{
		ADS1230_SCLK1(LOW);
		ADS1230_SCLK2(LOW);
		POWER_DOWN_1(LOW);//AD OFF
		POWER_DOWN_2(LOW);
		wk_adc_pre_enable = wk_adc_enable;
		return;
	}
	if((wk_adc_pre_enable == FALSE) && (wk_adc_enable == TRUE))
	{
		vADS1230_Init();
		vBSP_DelayMs(200);
		vADS1230_Offset1();
		vADS1230_Offset2();
	}
	wk_adc_pre_enable = wk_adc_enable;


	if(high_flag)
	{
		g_adc_high = adc_buff_inc(ADC_TYPE_ADS1230_1,vADS1230_Read1());
		pressure_high = (double)st_gPressureCaliParam.pressure_h_factor * g_adc_high + st_gPressureCaliParam.pressure_h_offset1 + st_gPressureCaliParam.pressure_h_offset2;
		wk_pressure_bar = pressure_high / 1000;
		wk_pressure_kpa = (double)wk_pressure_bar * 100 + 101.35;
		st_gHL_Pressure[0].p_bar = wk_pressure_bar;
		st_gHL_Pressure[0].p_0001xbar = (double)wk_pressure_bar * 1000;
		st_gHL_Pressure[0].p_0001xbar_report =st_gHL_Pressure[0].p_0001xbar;			
		st_gHL_Pressure[0].p_kpa = (double)wk_pressure_bar * 100;
		st_gHL_Pressure[0].p_mpa = (double)wk_pressure_bar * 0.1;
		st_gHL_Pressure[0].p_psi = (double)wk_pressure_bar * 14.5;
		if((wk_pressure_bar > -0.3f) && (wk_pressure_bar < 0.4f))
		{
			wk_pressure_kpa = (double)0 * 100 + 101.35;
			st_gHL_Pressure[0].p_0001xbar_report = 0;			
		}
		s16_gHL_Temp[0] = sPressureCalTemp(st_gSysParam.refrigerant_type,wk_pressure_kpa,0);
	}
	else
	{
		g_adc_low = adc_buff_inc(ADC_TYPE_ADS1230_2,vADS1230_Read2());
		pressure_low = (double)st_gPressureCaliParam.pressure_l_factor * g_adc_low + st_gPressureCaliParam.pressure_l_offset1 + st_gPressureCaliParam.pressure_l_offset2;
		wk_pressure_bar = pressure_low / 1000;
		wk_pressure_kpa = (double)wk_pressure_bar * 100 + 101.35;
		st_gHL_Pressure[1].p_bar = wk_pressure_bar;
		st_gHL_Pressure[1].p_0001xbar = (double)wk_pressure_bar * 1000;
		st_gHL_Pressure[1].p_0001xbar_report =st_gHL_Pressure[1].p_0001xbar;			
		st_gHL_Pressure[1].p_kpa = (double)wk_pressure_bar * 100;
		st_gHL_Pressure[1].p_mpa = (double)wk_pressure_bar * 0.1;
		st_gHL_Pressure[1].p_psi = (double)wk_pressure_bar * 14.5;	
		if((wk_pressure_bar > -0.3f) && (wk_pressure_bar < 0.4f))
		{
			wk_pressure_kpa = (double)0 * 100 + 101.35;
			st_gHL_Pressure[1].p_0001xbar_report = 0;			
		}
		s16_gHL_Temp[1] = sPressureCalTemp(st_gSysParam.refrigerant_type,wk_pressure_kpa,1);
	}
	high_flag = !high_flag;
}


void vSampleTemp(void)
{
	USHORT wk_vol = 0;
	ULONG wk_res;

	wk_vol = usGetMCUADCVolt(ADC_CH_10_PB1);
	wk_res = (ULONG)10000 * wk_vol / (ADC_REF_VOLT - wk_vol);
	
	s16_gBatTemp = nADC_Calc_TH(wk_res);
}

void vBatteryCalSoc(SHORT *soc,UCHAR *soc_level)
{
	USHORT wk_bat_vol = 0;
	//USHORT wk_uv_th = 3350;
	//USHORT wk_ov_th = 4150;
	//static UCHAR wk_pre_soc_level = 4;
	//static UCHAR wk_soc_level = 4;
	static USHORT wk_time = 0;

	wk_bat_vol = usGetMCUADCVolt(ADC_CH_1_PA0);
	wk_bat_vol = (ULONG)wk_bat_vol * 3 / 2;
	s16_gBatVoltage = adc_buff_inc(ADC_TYPE_VBAT,wk_bat_vol);

	if((u8_gSysState == SYS_STATE_NORMAL))// && (ucPlugInsertStatus == STATUS_PLUG_REMOVE))
	{
		s16_gBatVoltage+=50;
	}
	if(u8_gSysState == SYS_STATE_CHARG_IN_LOWPOWER)
	{
		s16_gBatVoltage-=10;
	}

//	if(wk_bat_vol > wk_ov_th)
//	{
//		*soc = 100;
//	}
//	else if(wk_bat_vol <= wk_uv_th)
//	{
//		*soc = 0;
//	}
//	else
//	{
//		*soc = ((ULONG)wk_bat_vol - wk_uv_th) * 100 /(wk_ov_th - wk_uv_th);
//	}	

	vAPI_Param_DataChatt(PARAM_INFO_BAT_SOC,&dc_bat_soc_chatta_cnt[0],&dc_bat_soc_pre_data[0],&ucBatSocResult,ELEMOF(PARAM_INFO_BAT_SOC));//ELEMOF(PARAM_INFO_CELL_MIN));

	if(ucBatSocResult & MASK_BIT4)
	{
		s16_gBatSocCal = 1000;
	}
	else if(ucBatSocResult & MASK_BIT3)
	{
		s16_gBatSocCal = 750;
	}
	else if(ucBatSocResult & MASK_BIT2)
	{
		s16_gBatSocCal = 500;
	}
	else if(ucBatSocResult & MASK_BIT1)
	{
		s16_gBatSocCal = 250;
	}
	else if(ucBatSocResult & MASK_BIT0)
	{
		s16_gBatSocCal = 0;
	}

	if(u8_gSocAdjustLimit == TRUE)
	{
		/*充电时候电压突增，SOC缓慢逼近目标soc*/
		if((s16_gBatSocCal > s16_gBatSocReport))
		{
			if(IN_CHARGING())
			{
				wk_time+=50;
				if(wk_time>= 5000)
				{
					wk_time = 0;
					s16_gBatSocReport = (s16_gBatSocReport >= 1000)?s16_gBatSocReport:s16_gBatSocReport + 1;
				}
			}
			else	//非充电时，不允许soc变大
			{
				return;
			}
		}
		/*非充电时电压突降，，soc缓慢逼近目标soc*/
	//	else if((s16_gBatSocCal < s16_gBatSocReport) && (STATUS_PLUG_REMOVE == ucPlugInsertStatus))
	//	{
	//		wk_time+=50;
	//		if(wk_time>= 5000)
	//		{
	//			wk_time = 0;
	//			s16_gBatSocReport = (s16_gBatSocReport <= 0)?0:s16_gBatSocReport - 1;
	//		}
	//	}
		else
		{
			s16_gBatSocReport = s16_gBatSocCal;
		}	
	}
	else
	{
		s16_gBatSocReport = s16_gBatSocCal;
	}
	
	if(IN_CHARGING_COMPLETE())
	{
		s16_gBatSocReport = 1000;
	}


	if(s16_gBatSocReport <= 50)
	{
		u8_gBatSocLevel = 0;
	}
	else if(s16_gBatSocReport <= 250)
	{
		u8_gBatSocLevel = 1;
	}
	else if(s16_gBatSocReport <= 500)
	{
		u8_gBatSocLevel = 2;
	}
	else if(s16_gBatSocReport <= 750)
	{
		u8_gBatSocLevel = 3;
	}
	else if(s16_gBatSocReport <= 1000)
	{
		u8_gBatSocLevel = 4;
	}



	ucChargeStateCheck();
	

//	if((wk_soc_level > wk_pre_soc_level) && (!IN_CHARGING()))
//	{
//		return; //不在充电中，soc不允许增加
//	}
//	*soc_level = wk_soc_level;
//	wk_pre_soc_level = wk_soc_level;
}

static void MATH_IncUint8( UCHAR *wk_cnt )
{
    if( *wk_cnt < 0xff )
    {
        ( *wk_cnt )++;
    }
}

static void vAPI_Param_DataChatt(const st_state *wk_param,  UCHAR *wk_param_chatta_cnt, UCHAR *wk_param_pre_data, USHORT *wk_param_fix_data,  UCHAR wk_cnt_max )
{
     UCHAR	wk_cnt;
     UCHAR	wk_sig;
     SHORT DATA1,DATA2;

    *wk_param_fix_data = CLR;
    for( wk_cnt = 0; wk_cnt < wk_cnt_max; wk_cnt++ ) 	/* ?????y????h?E?????E?P???? */
    {
    	DATA1 = *wk_param[wk_cnt].DATA1;
    	DATA2 = *wk_param[wk_cnt].DATA2;

        /* ????????????h?????????NG?????????????? */
        switch ( wk_param[wk_cnt].COMPAREMODE )
        {
	        case API_COMP_EQUAL:
	            wk_sig	= ( DATA1 == DATA2 ) ? SET : CLR;	/* DATA1??DATA2??? */
	            break;
	        case API_COMP_OVER_EQUAL:
	            wk_sig	= ( DATA1 >= DATA2 ) ? SET : CLR;	/* DATA1?DATA2?? */
	            break;
	        case API_COMP_OVER:
	            wk_sig	= ( DATA1 > DATA2 ) ? SET : CLR;	/* DATA1?DATA2???? */
	            break;
	        default:
	            continue;
        }

        /* ???E?P????C??E??? */
        if( wk_sig ) 											/* ???????? */
        {
            if( wk_param_pre_data[wk_cnt] == SET ) 				/* ????????(??????? */
            {
                MATH_IncUint8( &wk_param_chatta_cnt[wk_cnt] );	/* ???E?P????C??E?????P??? */
            }
            else 												/* ????????h??????(???Y???? */
            {
                wk_param_chatta_cnt[wk_cnt] = CLR;				/* ???E?P????C??E???P?X */
            }
        }
        else 								/* ???? */
        {
            if( wk_param_pre_data[wk_cnt] == CLR ) 				/* ????????(??????? */
            {
                MATH_IncUint8( &wk_param_chatta_cnt[wk_cnt] );	/* ???E?P????C??E?????P??? */
            }
            else 												/* ????????(???Y???? */
            {
                wk_param_chatta_cnt[wk_cnt] = CLR;				/* ???E?P????C??E???P?X */
            }
        }

        /* ?????? */
        wk_param_pre_data[wk_cnt] 	= wk_sig;					/* ????????? */

        /* ?????? */
        if( wk_param_chatta_cnt[wk_cnt] >= *wk_param[wk_cnt].count ) 	/* ?????? */
        {
            *wk_param_fix_data |= ( wk_sig << wk_cnt );
        }
        else
        {
            /* do nothing */
        }
    }
}



void vAPI_ParamCheck( void )
{	
}


