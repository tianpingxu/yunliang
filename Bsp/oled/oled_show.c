#include "oled_show.h"
#include "oled.h"

DEV_STATE dev_s;

char OLED_zfc[] = {0}; // 字符转化为字符串储存于此数组

void oled_show(void) {
    OLED_ZFC(8, 0, (uint8_t *)"HJD-2 ", Font_Size_F8x16); // 在OLED屏幕的X轴为0，Y轴为0，显示8*16大小的字符串
    OLED_Long_HZ(60, 0, 1, 0, 3); // OLED显示汉字“驱动系统”

    delay_ms(1000); // 延时一秒
    OLED_Clear(0);  // 清屏
    OLED_ShowPicture(0, 0, 128, 8, 1); // OLED显示图片
}

void oled_show_line1(void) {
    OLED_ZFC(8, 0, (uint8_t *)"HJD-2 ", Font_Size_F8x16); // 在OLED屏幕的X轴为0，Y轴为0，显示8*16大小的字符串
    OLED_Long_HZ(60, 0, 1, 0, 3); // OLED显示汉字“驱动系统”
}

void oled_show_line2(uint16_t vol) {
    if((vol % 10) >= 5) {
        vol = (vol / 10 + 1);
    }else {
        vol /= 10;
    }
    uint8_t data_h, data_l;
    dev_s.dev_vol_now = vol;
    if(dev_s.dev_vol_old != dev_s.dev_vol_now) {
        dev_s.dev_vol_old = dev_s.dev_vol_now;

        OLED_Clear(0); // 清屏

        oled_show_line1();

        data_h = vol / 10;
        sprintf(OLED_zfc, "%d", data_h);
        if(data_h < 10) {
            OLED_ZFC(16, 3, OLED_zfc, Font_Size_F8x16);
        }else {
            OLED_ZFC(8, 3, OLED_zfc, Font_Size_F8x16);
        }

        OLED_ZFC(24, 3, (uint8_t *)".", Font_Size_F8x16);

        data_l = vol % 10;
        sprintf(OLED_zfc, "%d", data_l);
        OLED_ZFC(32, 3, OLED_zfc, Font_Size_F8x16);
        OLED_ZFC(42, 3, (uint8_t *)"V ", Font_Size_F8x16);

        OLED_Long_HZ(60, 3, 1, 4, 5); // OLED显示汉字“电压”
        if(vol < LOW_VOL) {
            OLED_Long_HZ(92, 3, 1, 10, 11); // OLED显示汉字“过低”
            dev_vol_low_flag = 1;
            SET_BIT(DEV_ERR_STATE, 4);
        }else if (vol > HIGH_VOL) {
            OLED_Long_HZ(92, 3, 1, 8, 9); // OLED显示汉字“过高”
            dev_vol_high_flag = 1;
            SET_BIT(DEV_ERR_STATE, 5);
        }else {
            OLED_Long_HZ(92, 3, 1, 6, 7); // OLED显示汉字“正常”
            CLEAR_BIT(DEV_ERR_STATE, 4);
            CLEAR_BIT(DEV_ERR_STATE, 5);
        }

        // test
        // dev_s.dev_err_now = COMMUNICATION_NOMAL;

        if(dev_s.dev_err_now == READY_COMPLETE) {
            OLED_Long_HZ(30, 6, 2, 0, 3); // OLED显示汉字“准备完成”
        }else if(dev_s.dev_err_now == COMMUNICATION_NOMAL) {
            OLED_Long_HZ(30, 6, 2, 4, 7); // OLED显示汉字“通讯正常”
        }else if(dev_s.dev_err_now == COMMUNICATION_DISCONNECT) {
            OLED_Long_HZ(30, 6, 2, 8, 11); // OLED显示汉字“通讯断开”
        }else if(dev_s.dev_err_now == MOTOR_BLOCKED) {
            OLED_Long_HZ(30, 6, 2, 12, 15); // OLED显示汉字“电机堵转”
        }else if(dev_s.dev_err_now == NAIL_LOCATION_EER) {
            OLED_Long_HZ(10, 6, 2, 16, 21); // OLED显示汉字“钉网位置有误”
        }else if(dev_s.dev_err_now == HARDWARE_EER) {
            OLED_Long_HZ(30, 6, 2, 22, 25); // OLED显示汉字“硬件故障”
        }else if(dev_s.dev_err_now == EER_CODE) {
            OLED_Long_HZ(10, 6, 2, 24, 26); // OLED显示汉字“故障码”
            sprintf(OLED_zfc, "%s", dev_s.dev_err_code_now);
            OLED_ZFC(40, 6, OLED_zfc, Font_Size_F8x16);
        }else if(dev_s.dev_err_now == NAIL_UP_OK) {
            OLED_Long_HZ(30, 6, 2, 27, 30); // OLED显示汉字“起订完成”
        }else {
        }
    }
}

void oled_show_line3(uint8_t dev_info, uint8_t err_code) {
    dev_s.dev_err_now = dev_info;
    dev_s.dev_err_code_now = err_code;
    if((dev_s.dev_err_old != dev_s.dev_err_now) || (dev_s.dev_err_code_old != dev_s.dev_err_code_now )) {
        dev_s.dev_err_old = dev_s.dev_err_now;
        dev_s.dev_err_code_old = dev_s.dev_err_code_now;

        dev_s.dev_vol_old = 0;
        oled_show_line2(dev_s.dev_vol_now);
    }
}

