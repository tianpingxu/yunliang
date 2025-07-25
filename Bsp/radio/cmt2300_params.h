//;---------------------------------------
//;  CMT2300A Configuration File
//;  Generated by CMOSTEK RFPDK 1.45
//;  2017.11.02 14:25
//;---------------------------------------
//; Mode                      = Advanced
//; Part Number               = CMT2300A
//; Frequency                 = 433.920 MHz
//; Xtal Frequency            = 26.0000 MHz
//; Demodulation              = FSK
//; AGC                       = On
//; Data Rate                 = 2.4 kbps
//; Deviation                 = 35.0 kHz
//; Tx Xtal Tol.              = 20 ppm
//; Rx Xtal Tol.              = 20 ppm
//; TRx Matching Network Type = 13 dBm
//; Tx Power                  = +13 dBm
//; Gaussian BT               = NA
//; Bandwidth                 = Auto-Select kHz
//; CDR Type                  = Counting
//; CDR DR Range              = NA
//; AFC                       = On
//; AFC Method                = Auto-Select
//; Data Representation       = 0:F-low 1:F-high
//; Rx Duty-Cycle             = Off
//; Tx Duty-Cycle             = Off
//; Sleep Timer               = Off
//; Sleep Time                = NA
//; Rx Timer                  = Off
//; Rx Time T1                = NA
//; Rx Time T2                = NA
//; Rx Exit State             = STBY
//; Tx Exit State             = STBY
//; SLP Mode                  = Disable
//; RSSI Valid Source         = PJD
//; PJD Window                = 8 Jumps
//; LFOSC Calibration         = On
//; Xtal Stable Time          = 155 us
//; RSSI Compare TH           = NA
//; Data Mode                 = Packet
//; Whitening                 = Disable
//; Whiten Type               = NA
//; Whiten Seed Type          = NA
//; Whiten Seed               = NA
//; Manchester                = Disable
//; Manchester Type           = NA
//; FEC                       = Disable
//; FEC Type                  = NA
//; Tx Prefix Type            = 0
//; Tx Packet Number          = 1
//; Tx Packet Gap             = 32
//; Packet Type               = Variable Length
//; Node-Length Position      = First Node, then Length
//; Payload Bit Order         = Start from msb
//; Preamble Rx Size          = 2
//; Preamble Tx Size          = 8
//; Preamble Value            = 170
//; Preamble Unit             = 8-bit
//; Sync Size                 = 3-byte
//; Sync Value                = 11152852
//; Sync Tolerance            = None
//; Sync Manchester           = Disable
//; Node ID Size              = NA
//; Node ID Value             = NA
//; Node ID Mode              = None
//; Node ID Err Mask          = Disable
//; Node ID Free              = Disable
//; Payload Length            = 32
//; CRC Options               = None
//; CRC Seed                  = NA
//; CRC Range                 = NA
//; CRC Swap                  = NA
//; CRC Bit Invert            = NA
//; CRC Bit Order             = NA
//; Dout Mute                 = Off
//; Dout Adjust Mode          = Disable
//; Dout Adjust Percentage    = NA
//; Collision Detect          = Off
//; Collision Detect Offset   = NA
//; RSSI Detect Mode          = Always
//; RSSI Filter Setting       = No Filtering
//; RF Performance            = High
//; LBD Threshold             = 2.4 V
//; RSSI Offset               = 26
//; RSSI Offset Sign          = 1
//

#ifndef __CMT2300_PARAMS_H
#define __CMT2300_PARAMS_H



/* [CMT Bank] */
const u8 g_cmt2300CmtBank[CMT2300_CMT_BANK_SIZE] = {
    0x00,
    0x66,
    0xEC,
    0x1D,//20dbm //0x1C,
    0xF0,
    0x80,
    0x14,
    0x08,
    0x91,
    0x02,
    0x02,
    0xD0,
};

/* [System Bank] */
const u8 g_cmt2300SystemBank[CMT2300_SYSTEM_BANK_SIZE] = {
    0xA5,
    0xE0,
    0x35,
    0x00,
    0x00,
    0xF4,
    0x10,
    0xE2,
    0x42,
    0x20,
    0x00,
    0x81,
};

/* [Frequency Bank] */
//433.92Mhz
const u8 g_cmt2300FrequencyBank[CMT2300_FREQUENCY_BANK_SIZE] = {
    0x42,
    0x71,
    0xCE,
    0x1C,
    0x42,
    0x5B,
    0x1C,
    0x1C,
};
/*
845.901 Mhz
const u8 g_cmt2300FrequencyBank[CMT2300_FREQUENCY_BANK_SIZE] = {
    0x41,
    0xED,
    0x74,
    0x81,
    0x41,
    0xE2,
    0x1B,
    0x11,
};
*/

/* [Data Rate Bank] */
//2.4Kbps
const u8 g_cmt2300DataRateBank[CMT2300_DATA_RATE_BANK_SIZE] = {
    0x32,
    0x18,
    0x10,
    0x99,
    0xF1,
    0x9B,
    0x2C,
    0x0A,
    0x9F,
    0x39,
    0x29,
    0x29,
    0xC0,
    0x51,
    0x2A,
    0x53,
    0x00,
    0x00,
    0xB4,
    0x00,
    0x00,
    0x01,
    0x00,
    0x00,
};

/* [Data Rate Bank] */
//40Kbps
/*
const u8 g_cmt2300DataRateBank[CMT2300_DATA_RATE_BANK_SIZE] = {
    0x4C,
    0x93,
    0x21,
    0x10,
    0xA2,
    0x12,
    0x0B,
    0x0A,
    0x9F,
    0x5A,
    0x29,
    0x28,
    0xC0,
    0x8A,
    0x02,
    0x53,
    0x18,
    0x00,
    0xB4,
    0x00,
    0x00,
    0x01,
    0x00,
    0x00,
};
*/
/* [Baseband Bank] */
const u8 g_cmt2300BasebandBank[CMT2300_BASEBAND_BANK_SIZE] = {
    0x12,
    0x08,
    0x00,
    0xAA,
    0x04,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xD4,
    0x2D,
    0xAA,
    0x00,//0x01,//必须为0，否则数据出错
    0x1F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x60,
    0xFF,
    0x00,
    0x00,
    0x1F,
    0x10,//Len payload
};


/* [Tx Bank] */
const u8 g_cmt2300TxBank[CMT2300_TX_BANK_SIZE] = {
    0x50,
    0x0E,
    0x16,
    0x00,
    0x42,
    0xB0,
    0x00,
    0x8A,
    0x18,
    0x3F,
    0x7F,
};

#endif
