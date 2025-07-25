/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    radio.c
 * @brief   Generic radio handlers
 *
 * @version 1.1
 * @date    Feb 08 2017
 * @author  CMOSTEK R@D
 */
 
 #include "radio.h"
 #include "cmt2300_params.h"
 #include "sys_type.h"

 #include "motor.h"

 static EnumRFStatus g_nNextRFState = RF_STATE_IDLE;
 static u8* g_pRxBuffer = NULL;
 static u8* g_pTxBuffer = NULL;
 u16 g_nRxLength = 8;//cmt����Ϊ�̶�������ģʽ���շ���Ϊ16�ֽ�.����Ϊ�ɱ����ģʽ��ʹ��RFPDK����������������
 u16 g_nTxLength = 8;//cmt����Ϊ�̶�������ģʽ���շ���Ϊ16�ֽ�.����Ϊ�ɱ����ģʽ��ʹ��RFPDK����������������
 
 static u32 g_nRxTimeout = INFINITE;
 static u32 g_nTxTimeout = INFINITE;
 static u32 g_nRxTimeCount = 0;
 static u32 g_nTxTimeCount = 0;
 
 static u8 g_nInterrutFlags = 0;
 
 void RF_Init(void)
 {
     Cmt2300_InitGpio();

     Cmt2300_Init();
     
     /* Config registers */
     Cmt2300_ConfigRegBank(CMT2300_CMT_BANK_ADDR       , g_cmt2300CmtBank       , CMT2300_CMT_BANK_SIZE       );
     Cmt2300_ConfigRegBank(CMT2300_SYSTEM_BANK_ADDR    , g_cmt2300SystemBank    , CMT2300_SYSTEM_BANK_SIZE    );
     Cmt2300_ConfigRegBank(CMT2300_FREQUENCY_BANK_ADDR , g_cmt2300FrequencyBank , CMT2300_FREQUENCY_BANK_SIZE );
     Cmt2300_ConfigRegBank(CMT2300_DATA_RATE_BANK_ADDR , g_cmt2300DataRateBank  , CMT2300_DATA_RATE_BANK_SIZE );
     Cmt2300_ConfigRegBank(CMT2300_BASEBAND_BANK_ADDR  , g_cmt2300BasebandBank  , CMT2300_BASEBAND_BANK_SIZE  );
     Cmt2300_ConfigRegBank(CMT2300_TX_BANK_ADDR        , g_cmt2300TxBank        , CMT2300_TX_BANK_SIZE        );
 
     RF_Config();
 }
 
 void RF_Config(void)
 {
 #ifdef ENABLE_ANTENNA_SWITCH
     /* If you enable antenna switch, GPIO1/GPIO2 will output RX_ACTIVE/TX_ACTIVE,
        and it can't output INT1/INT2 via GPIO1/GPIO2 */
     //Cmt2300_EnableAntennaSwitch(0);
     Cmt2300_EnableAntennaSwitch(1);
 
 #else
     /* Config GPIOs */
     Cmt2300_ConfigGpio(
         CMT2300_GPIO1_SEL_INT1 | /* INT1 > GPIO1 */
         CMT2300_GPIO2_SEL_INT2 | /* INT2 > GPIO2 */
         CMT2300_GPIO3_SEL_DOUT
         );
     
     /* Config interrupt */
     Cmt2300_ConfigInterrupt(
         CMT2300_INT_SEL_TX_DONE, /* Config INT1 */
         CMT2300_INT_SEL_PKT_OK   /* Config INT2 */
         );
 #endif
 
     /* Enable interrupt */
     Cmt2300_EnableInterrupt(
         CMT2300_MASK_TX_DONE_EN  |
         CMT2300_MASK_PREAM_OK_EN |
         CMT2300_MASK_SYNC_OK_EN  |
         CMT2300_MASK_NODE_OK_EN  |
         CMT2300_MASK_CRC_OK_EN   |
         CMT2300_MASK_PKT_DONE_EN
         );
         
     Cmt2300_EnableLfosc(FALSE);
 
 
     Cmt2300_WriteReg(CMT2300_CUS_PKT14, 0x80); // CUS_PKT14 固定长度使能
     Cmt2300_WriteReg(CMT2300_CUS_PKT15, 8); // CUS_PKT15 设定包长度为8
     
     /* Use a single 64-byte FIFO for either Tx or Rx */
     // Cmt2300_EnableFifoMerge(TRUE);
     
     // Cmt2300_SetFifoThreshold(16);
     
     /* Go to sleep for configuration to take effect */
     Cmt2300_GoSleep();
         
 }
 
 void RF_SetStatus(EnumRFStatus nStatus)
 {
     g_nNextRFState = nStatus;
 }
 
 EnumRFStatus RF_GetStatus(void)
 {
     return g_nNextRFState;
 }
 
 u8 RF_GetInterruptFlags(void)
 {
     return g_nInterrutFlags;
 }
 
 
extern uint8_t Speed_up_flag;
     
 void RF_StartRx(u8 buf[], u16 len, u32 timeout)
 {
     g_pRxBuffer = buf;
     //g_nRxLength = len;
     g_nRxLength = g_nRxLength;
     g_nRxTimeout = timeout;
     
     
     g_nNextRFState = RF_STATE_RX_START;
 }
 
 void RF_StartTx(u8 buf[], u16 len, u32 timeout)
 {
     g_pTxBuffer = buf;
     //g_nTxLength = len;
     g_nTxLength = g_nTxLength;
     g_nTxTimeout = timeout;
 
     g_nNextRFState = RF_STATE_TX_START;
 }
 
 EnumRFResult RF_Process(void)
 {
    EnumRFResult nRes = RF_BUSY;

    switch(g_nNextRFState) 
    {
    case RF_STATE_IDLE:
    {
        nRes = RF_IDLE;
        break;
    }
    
    case RF_STATE_RX_START:
    {
        Cmt2300_GoStby();
        Cmt2300_ClearInterruptFlags();
        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        Cmt2300_EnableReadFifo();
        Cmt2300_ClearFifo();
                
        if(FALSE==Cmt2300_GoRx()) {
            g_nNextRFState = RF_STATE_ERROR;
            printf("RF_STATE_ERROR\r\n");
        }else
            g_nNextRFState = RF_STATE_RX_WAIT;
        
        g_nRxTimeCount = Cmt2300_GetTickCount();
        
        break;
    }
         
    case RF_STATE_RX_WAIT:
    {
 #ifdef ENABLE_ANTENNA_SWITCH
        if(CMT2300_MASK_PKT_OK_FLG & Cmt2300_ReadReg(CMT2300_CUS_INT_FLAG))  /* Read PKT_OK flag */
 #else
        if(Cmt2300_ReadGpio2())  /* Read INT2, PKT_OK */
 #endif
        {
            g_nNextRFState = RF_STATE_RX_DONE;
        }
         
        // if( (INFINITE != g_nRxTimeout) && ((g_nSysTickCount-g_nRxTimeCount) > g_nRxTimeout) )
        //     g_nNextRFState = RF_STATE_RX_TIMEOUT;
         
        break;
    }
        
    case RF_STATE_RX_DONE:
    {
        Cmt2300_GoStby();

        /* The length need be smaller than 32 */
        Cmt2300_ReadFifo(g_pRxBuffer, 9);

        // printf("rf_data = [");
        // for(uint8_t i = 0; i < 8; i++) {
        //     printf("%02X, ", g_pRxBuffer[i]);
        // } printf("%02X]\r\n", g_pRxBuffer[8]);

        remote_chat_timeout_flag = 0;
        CLEAR_BIT(DEV_ERR_STATE, 7);

        if(g_pRxBuffer[0] == 8) {
            if((g_pRxBuffer[1] == 0xAA) && (g_pRxBuffer[2] == 0x55)) {
                if(g_pRxBuffer[3] == 0x26) {
                    if((g_pRxBuffer[4] == 0x09) &&(g_pRxBuffer[5] == 0x02) && (g_pRxBuffer[6] == 0x00) && (g_pRxBuffer[7] == 0x30) & (g_pRxBuffer[8] == 0xD2)) {
                        //缠网开启
                        motor_set_dir(MOTOR_LOCATION_LEFT);
                        // printf("MOTOR_LOCATION_LEFT\r\n");
                    }
                }else if (g_pRxBuffer[3] == 0x27) {
                    if((g_pRxBuffer[4] == 0x09) &&(g_pRxBuffer[5] == 0x02) && (g_pRxBuffer[6] == 0x00) && (g_pRxBuffer[7] == 0x31) & (g_pRxBuffer[8] == 0xD3)) {
                        //缠网关闭
                        motor_set_dir(MOTOR_LOCATION_RIGHT);
                        // printf("MOTOR_LOCATION_RIGHT\r\n");
                    }
                }else if (g_pRxBuffer[3] == 0x28) {
                    if((g_pRxBuffer[4] == 0x09) &&(g_pRxBuffer[5] == 0x03) && (g_pRxBuffer[6] == 0x00) && (g_pRxBuffer[7] == 0x33) & (g_pRxBuffer[8] == 0xDD)) {
                        //拉网开启
                        // Speed_up_flag = 1;
						// if(get_dir_state() == MOTOR_DIR_0) {
						// 	motor_set_dir(MOTOR_DIR_0);
						// }else if(get_dir_state() == MOTOR_DIR_1) {
						// 	motor_dir_diff();
						// 	motor_set_dir(MOTOR_DIR_0);
						// }else {
						// }
                    }
                }else{
                }
            }
        }

        g_nInterrutFlags = Cmt2300_ClearInterruptFlags();
            
        Cmt2300_GoSleep();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_RX_DONE;
        break;
    }
     
    case RF_STATE_RX_TIMEOUT:
    {
        Cmt2300_GoSleep();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_RX_TIMEOUT;
        break;
    }
     
    case RF_STATE_TX_START:
    {
        Cmt2300_GoStby();
        Cmt2300_ClearInterruptFlags();
        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        Cmt2300_EnableWriteFifo();
        Cmt2300_ClearFifo();
        
        /* The length need be smaller than 32 */
        Cmt2300_WriteFifo(g_pTxBuffer, g_nTxLength);
        
        if( 0==(CMT2300_MASK_TX_FIFO_NMTY_FLG & Cmt2300_ReadReg(CMT2300_CUS_FIFO_FLAG)) )
            g_nNextRFState = RF_STATE_ERROR;

        if(FALSE==Cmt2300_GoTx()) {
        g_nNextRFState = RF_STATE_ERROR;
        printf("RF_STATE_ERROR\r\n");
        }else
        g_nNextRFState = RF_STATE_TX_WAIT;
        
        g_nTxTimeCount = Cmt2300_GetTickCount();
        
        break;
    }
         
    case RF_STATE_TX_WAIT:
    {
#ifdef ENABLE_ANTENNA_SWITCH
        if(CMT2300_MASK_TX_DONE_FLG & Cmt2300_ReadReg(CMT2300_CUS_INT_CLR1))  /* Read TX_DONE flag */
#else
        if(Cmt2300_ReadGpio1())  /* Read INT1, TX_DONE */
#endif
        {
            g_nNextRFState = RF_STATE_TX_DONE;
        }
        
        //if( (INFINITE != g_nTxTimeout) && ((g_nSysTickCount-g_nTxTimeCount) > g_nTxTimeout) )
        //    g_nNextRFState = RF_STATE_TX_TIMEOUT;
            
        break;
    }
             
    case RF_STATE_TX_DONE:
    {
        Cmt2300_ClearInterruptFlags();
        Cmt2300_GoSleep();

        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_TX_DONE;
        break;
    }
    
    case RF_STATE_TX_TIMEOUT:
    {
        Cmt2300_GoSleep();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_TX_TIMEOUT;
        break;
    }
     
    case RF_STATE_ERROR:
    {
        Cmt2300_SoftReset();
        Cmt2300_DelayMs(20);
        
        Cmt2300_GoStby();
        RF_Config();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_ERROR;
        break;
    }
    
    default:
        break;
    }

    return nRes;
 }
 