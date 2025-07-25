#include "oled.h"

#include "bmp.h"
#include "oled_spi.h"
#include "oled_Library.h"
#include "chinese_characters.h"

#include "delay.h"

uint8_t OLED_SRAM[8][128]; // 图像储存在SRAM里


/**
 * @brief OLED屏幕ISP接口初始化
 *
 */
void OLED_Init(void)
{
    OLED_SPI_Init();
    OLED_Reg_Init();
}


/**
 * @brief OLED初始化函数
 *
 */
void OLED_Reg_Init(void)
{
    OLED_RST_OFF(); // OLED复位
    delay_ms(10);   // 复位延时
    OLED_RST_ON();  // 结束复位

    OLED_SendCmd(0xae); // 关闭显示

    OLED_SendCmd(0xd5); // 设置时钟分频因子,震荡频率
    OLED_SendCmd(0x80); //[3:0],分频因子;[7:4],震荡频率

    OLED_SendCmd(0x81); // 设置对比度
    OLED_SendCmd(0x7f); // 128

    OLED_SendCmd(0x8d); // 设置电荷泵开关
    OLED_SendCmd(0x14); // 开

    OLED_SendCmd(0x20); // 设置模式
    OLED_SendCmd(0x00); // 设置为水平地址模式

    OLED_SendCmd(0x21); // 设置列地址的起始和结束的位置
    OLED_SendCmd(0x00); // 0
    OLED_SendCmd(0x7f); // 127

    OLED_SendCmd(0x22); // 设置页地址的起始和结束的位置
    OLED_SendCmd(0x00); // 0
    OLED_SendCmd(0x07); // 7

    OLED_SendCmd(0xc8); // 0xc9上下反置 0xc8正常
    OLED_SendCmd(0xa1); // 0xa0左右反置 0xa1正常

    // OLED_SendCmd(0xC0);
    // OLED_SendCmd(0xA0); // 旋转180度显示

    OLED_SendCmd(0xa4); // 全局显示开启;0xa4正常,0xa5无视命令点亮全屏
    OLED_SendCmd(0xa6); // 设置显示方式;bit0:1,反相显示;0,正常显示

    OLED_SendCmd(0xaf); // 开启显示
    OLED_SendCmd(0x56);

    OLED_DMA_Init();
}

/**
 * @brief 发送命令
 *
 * @param TxData 命令
 */
void OLED_SendCmd(u8 TxData)
{
    OLED_DC_CMD(); // 命令模式

    while (SPI_I2S_GetStatus(SPI1, SPI_I2S_TE_FLAG) == RESET) // 检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        for (uint8_t retry = 0; retry < 200; retry++) // 等待回复
            ;
        return; // 返回
    }

    delay_ms(100); // 延时100毫秒

    SPI_I2S_TransmitData(SPI1, TxData); // 通过外设 SPIx 发送一个数据

    OLED_DC_DAT(); // 数据模式
}

/**
 * @brief OLED反色显示
 *
 * @param i 0-正常，1-反色
 */
void OLED_ColorTurn(uint8_t i)
{
    if (i)
        OLED_SendCmd(0xA7); // 反色显示
    else
        OLED_SendCmd(0xA6); // 正常显示
}

/**
 * @brief OLED屏幕旋转180
 *
 * @param i 0-正常显示，1-旋转180度显示
 */
void OLED_DisplayTurn(uint8_t i)
{
    if (i)
    {
        OLED_SendCmd(0xC0);
        OLED_SendCmd(0xA0); // 旋转180度显示
    }
    else
    {
        OLED_SendCmd(0xC8);
        OLED_SendCmd(0xA1); // 正常显示
    }
}

/**
 * @brief 开启OLED显示，功耗 30 mW
 *
 */
void OLED_DisPlay_On(void)
{
    OLED_SendCmd(0x8D); // 电荷泵使能
    OLED_SendCmd(0x14); // 开启电荷泵
    OLED_SendCmd(0xAF); // 点亮屏幕
}

/**
 * @brief 关闭OLED显示，功耗 29 mW
 *
 */
void OLED_DisPlay_Off(void)
{
    OLED_SendCmd(0x8D); // 电荷泵使能
    OLED_SendCmd(0x10); // 关闭电荷泵
    OLED_SendCmd(0xAF); // 关闭屏幕
}

/**
 * @brief 清屏--全灭
 *
 */
void OLED_Clear(uint8_t led)
{
    for (uint8_t i = 0; i < 8; i++)
        for (uint8_t n = 0; n < 128; n++)
            if (led)
                OLED_SRAM[i][n] = 0xFF; // 全屏点亮
            else
                OLED_SRAM[i][n] = 0; // 清除所有数据
}

/**
 * @brief 画点
 *
 * @param x x坐标（0-128）
 * @param y y坐标（0-63）
 */
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
    OLED_SRAM[y / 8][x] |= (1 << (y % 8));
}

/**
 * @brief 清除点
 *
 * @param x x坐标（0-128）
 * @param y y坐标（0-63）
 */
void OLED_ClearPoint(uint8_t x, uint8_t y)
{
    OLED_SRAM[y / 8][x] = ~OLED_SRAM[y / 8][x];
    OLED_SRAM[y / 8][x] |= (1 << (y % 8));
    OLED_SRAM[y / 8][x] = ~OLED_SRAM[y / 8][x];
}

/**
 * @brief 画线
 *
 * @param x1 起点x坐标（0-128）
 * @param y1 起点y坐标（0-63）
 * @param x2 终点x坐标（0-128）
 * @param y2 终点y坐标（0-63）
 */
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if (x1 == x2) // 画竖线
        for (uint8_t i = 0; i < y2 - y1; i++)
            OLED_DrawPoint(x1, y1 + i);
    else if (y1 == y2) // 画横线
        for (uint8_t j = 0; j < x2 - x1; j++)
            OLED_DrawPoint(x1 + j, y1);
    else // 画斜线
        for (uint8_t z = 0; z < x2 - x1; z++)
            OLED_DrawPoint(x1 + z, y1 + z * ((y2 - y1) * 10 / (x2 - x1)) / 10);
}

/**
 * @brief 清除线
 *
 * @param x1 起点x坐标（0-128）
 * @param y1 起点y坐标（0-63）
 * @param x2 终点x坐标（0-128）
 * @param y2 终点y坐标（0-63）
 */
void OLED_ClearLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if (x1 == x2) // 清除竖线
        for (uint8_t i = 0; i < y2 - y1; i++)
            OLED_ClearPoint(x1, y1 + i);
    else if (y1 == y2) // 清除横线
        for (uint8_t j = 0; j < x2 - x1; j++)
            OLED_ClearPoint(x1 + j, y1);
    else // 清除斜线
        for (uint8_t z = 0; z < x2 - x1; z++)
            OLED_ClearPoint(x1 + z, y1 + z * ((y2 - y1) * 10 / (x2 - x1)) / 10);
}

/**
 * @brief 画圆
 *
 * @param x 圆心x坐标（0-128）
 * @param y 圆心y坐标（0-63）
 * @param r 圆的半径
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    a = 0;
    b = r;

    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;

        num = (a * a + b * b) - r * r; // 计算画的点离圆心的距离

        if (num > 0)
            b--, a--;
    }
}

/**
 * @brief 清除圆
 *
 * @param x 圆心x坐标（0-128）
 * @param y 圆心y坐标（0-63）
 * @param r 圆的半径
 */
void OLED_ClearCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a = 0, b = r, num;

    while (2 * b * b >= r * r)
    {
        OLED_ClearPoint(x + a, y - b);
        OLED_ClearPoint(x - a, y - b);
        OLED_ClearPoint(x - a, y + b);
        OLED_ClearPoint(x + a, y + b);

        OLED_ClearPoint(x + b, y + a);
        OLED_ClearPoint(x + b, y - a);
        OLED_ClearPoint(x - b, y - a);
        OLED_ClearPoint(x - b, y + a);

        a++;

        num = (a * a + b * b) - r * r; // 计算画的点离圆心的距离

        if (num > 0)
            b--, a--;
    }
}

/**
 * @brief 指定位置显示单字符
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param ascii 单字符
 * @param Font_Size 字符大小：Font_Size_F6X8 或 Font_Size_F8x16
 */
void OLED_ZF(uint8_t x, uint8_t y, uint8_t *ascii, uint8_t Font_Size)
{
    if (Font_Size)
        for (uint8_t i = 0; i < 6; i++)
            OLED_SRAM[y][x + i] = YIN_F6X8[(*ascii - 32) * 6 + 1 + i];
    else
        for (uint8_t i1 = 0; i1 < 2; i1++)
            for (uint8_t i2 = 0; i2 < 8; i2++)
                OLED_SRAM[y + i1][x + i2] = YIN_F8X16[(*ascii - 32) * 16 + 8 * i1 + i2];
}
/**
 * @brief OLED显示多个字符
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param str 字符串
 * @param Font_Size 字符大小：Font_Size_F6X8 或 Font_Size_F8x16
 */
void OLED_ZFC(uint8_t x, uint8_t y, uint8_t *str, uint8_t Font_Size)
{
    uint8_t j = 0;

    while (str[j] != '\0')
    {
        if (Font_Size)
        {
            for (uint8_t i = 0; i < 6; i++)
                OLED_SRAM[y][x + i] = YIN_F6X8[(str[j] - 32) * 6 + 1 + i];

            x += 6;

            if (x > 120) // 自动换行
                x = 0, y++;
        }
        else
        {
            for (uint8_t i1 = 0; i1 < 2; i1++)
                for (uint8_t i2 = 0; i2 < 8; i2++)
                    OLED_SRAM[y + i1][x + i2] = YIN_F8X16[(str[j] - 32) * 16 + 8 * i1 + i2];

            x += 8;

            if (x > 120) // 自动换行
                x = 0, y += 2;
        }

        j++;
    }
}

/**
 * @brief OLED显示字符串水平居中
 *
 * @param y 纵坐标
 * @param str 字符串
 * @param Font_Size 字符大小：Font_Size_F6X8 或 Font_Size_F8x16
 */
void OLED_ZFC_Horizontal_center(uint8_t y, uint8_t *str, uint8_t Font_Size)
{
    uint8_t num = 0, j = 0;

    // 读取字符个数
    while (str[num] != '\0')
        num++;

    if (num < 22)
    {
        if (Font_Size)
            num = ((21 - num) * 6) / 2;
        else
            num = ((16 - num) * 8) / 2;

        while (str[j] != '\0')
        {
            if (Font_Size)
            {
                for (uint8_t i = 0; i < 6; i++)
                    OLED_SRAM[y][num + i] = YIN_F6X8[(str[j] - 32) * 6 + 1 + i];

                num += 6;

                if (num > 120) // 自动换行
                    num = 0, y++;
            }
            else
            {
                for (uint8_t i1 = 0; i1 < 2; i1++)
                    for (uint8_t i2 = 0; i2 < 8; i2++)
                        OLED_SRAM[y + i1][num + i2] = YIN_F8X16[(str[j] - 32) * 16 + 8 * i1 + i2];

                num += 8;

                if (num > 120) // 自动换行
                    num = 0, y += 2;
            }

            j++; // 显示下一个字符;
        }
    }
    else
        OLED_ZFC_Horizontal_center(y, (uint8_t *)"ERROR", Font_Size);
}

/**
 * @brief OLED显示字符串垂直居中
 *
 * @param x 横坐标
 * @param str 字符串
 * @param Font_Size 字符大小：Font_Size_F6X8 或 Font_Size_F8x16
 */
void OLED_ZFC_Vorizontal_center(uint8_t x, uint8_t *str, uint8_t Font_Size)
{
    uint8_t num = 0, j = 0, y;

    while (str[num] != '\0') // 读取字符个数
        num++;

    if (Font_Size)
    {
        if (num < 43)
            y = 3;
        else if (num < 85)
            y = 2;
        else if (num < 148)
            y = 1;
        else
            y = 0;
    }
    else
    {
        if (num < 33)
            y = 2;
        else if (num < 49)
            y = 1;
        else
            y = 0;
    }

    while (str[j] != '\0')
    {
        if (Font_Size)
        {
            for (uint8_t i = 0; i < 6; i++)
                OLED_SRAM[y][x + i] = YIN_F6X8[(str[j] - 32) * 6 + 1 + i];

            x += 6;

            if (x > 120) // 自动换行
                x = 0, y++;
        }
        else
        {
            for (uint8_t i1 = 0; i1 < 2; i1++)
                for (uint8_t i2 = 0; i2 < 8; i2++)
                    OLED_SRAM[y + i1][x + i2] = YIN_F8X16[(str[j] - 32) * 16 + 8 * i1 + i2];

            x += 8;

            if (x > 120) // 自动换行
                x = 0, y += 2;
        }

        j++;
    }
}

/**
 * @brief OLED显示一个文字
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param page 文字所在页码
 * @param num 文字序号
 */
void OLED_ONE_HZ(uint8_t x, uint8_t y, uint8_t page, uint8_t num)
{
    switch (page)
    {
    case 1:
        for (uint8_t i1 = 0; i1 < 2; i1++)
            for (uint8_t i2 = 0; i2 < 16; i2++)
                OLED_SRAM[y + i1][x + i2] = (HZ1[num * 2 + i1][i2]);
        break;
    case 2:
        for (uint8_t i3 = 0; i3 < 2; i3++)
            for (uint8_t i4 = 0; i4 < 16; i4++)
                OLED_SRAM[y + i3][x + i4] = (HZ2[num * 2 + i3][i4]);
        break;
    default:
        break;
    }
}

/**
 * @brief OLED显示一句话
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param page 文字所在页码
 * @param num1 文字开始序号
 * @param num2 文字结束序号
 */
void OLED_Long_HZ(uint8_t x, uint8_t y, uint8_t page, uint8_t num1, uint8_t num2)
{
    for (uint8_t num = num1; num < num2 + 1; num++)
    {
        if (x % 16)
        {
            if (x + (num - num1 + 1) * 16 < 128)
                OLED_ONE_HZ(x + (num - num1) * 16, y, page, num);
            else if (x + (num - num1 + 1) * 16 < 256)
                OLED_ONE_HZ((num - num1 - 8 + (x / 16) + 1) * 16, y + 2, page, num);
            else if (x + (num - num1 + 1) * 16 < 384)
                OLED_ONE_HZ((num - num1 - 16 + (x / 16) + 1) * 16, y + 4, page, num);
            else if (x + (num - num1 + 1) * 16 < 512)
                OLED_ONE_HZ((num - num1 - 24 + (x / 16) + 1) * 16, y + 6, page, num);
        }
        else
        {
            if (x + (num - num1) * 16 < 128)
                OLED_ONE_HZ(x + (num - num1) * 16, y, page, num);
            else if (x + (num - num1) * 16 < 256)
                OLED_ONE_HZ((num - num1 - 8 + (x / 16)) * 16, y + 2, page, num);
            else if (x + (num - num1) * 16 < 384)
                OLED_ONE_HZ((num - num1 - 16 + (x / 16)) * 16, y + 4, page, num);
            else if (x + (num - num1) * 16 < 512)
                OLED_ONE_HZ((num - num1 - 24 + (x / 16)) * 16, y + 6, page, num);
        }
    }
}

/**
 * @brief OLED显示一句话水平居中
 *
 * @param y 纵坐标
 * @param str 字符串
 * @param num1 文字开始序号
 * @param num2 文字结束序号
 */
void OLED_Long_HZ_Horizontal_center(uint8_t y, uint8_t page, uint8_t num1, uint8_t num2)
{
    uint8_t x, x1;

    if ((num2 - num1) < 7)
        x = ((7 - (num2 - num1)) * 16) / 2;
    else
        x = 0;

    for (uint8_t num = num1; num < num2 + 1; num++)
    {
        if (x % 16)
        {
            if (x + (num - num1 + 1) * 16 < 128)
                OLED_ONE_HZ(x + (num - num1) * 16, y, page, num);
            else if (x + (num - num1 + 1) * 16 < 256)
                OLED_ONE_HZ((num - num1 - 8 + (x1 / 16) + 1) * 16 + (((15 - (num2 - num1)) * 16) / 2), y + 2, page, num);
            else if (x + (num - num1 + 1) * 16 < 384)
                OLED_ONE_HZ((num - num1 - 16 + (x1 / 16) + 1) * 16 + (((23 - (num2 - num1)) * 16) / 2), y + 4, page, num);
            else if (x + (num - num1 + 1) * 16 < 512)
                OLED_ONE_HZ((num - num1 - 24 + (x1 / 16) + 1) * 16 + (((31 - (num2 - num1)) * 16) / 2), y + 6, page, num);
        }
        else
        {
            if (x + (num - num1) * 16 < 128)
                OLED_ONE_HZ(x + (num - num1) * 16, y, page, num);
            else if (x + (num - num1) * 16 < 256)
                OLED_ONE_HZ((num - num1 - 8 + (x1 / 16)) * 16 + (((15 - (num2 - num1)) * 16) / 2), y + 2, page, num);
            else if (x + (num - num1 + 1) * 16 < 384)
                OLED_ONE_HZ((num - num1 - 16 + (x1 / 16)) * 16 + (((23 - (num2 - num1)) * 16) / 2), y + 4, page, num);
            else if (x + (num - num1 + 1) * 16 < 512)
                OLED_ONE_HZ((num - num1 - 24 + (x1 / 16)) * 16 + (((31 - (num2 - num1)) * 16) / 2), y + 6, page, num);
        }
    }
}

/**
 * @brief OLED显示一句话垂直居中
 *
 * @param x 横坐标
 * @param str 字符串
 * @param num1 文字开始序号
 * @param num2 文字结束序号
 */
void OLED_Long_HZ_Vorizontal_center(uint8_t x, uint8_t page, uint8_t num1, uint8_t num2)
{
    uint8_t y;

    if ((num2 - num1) < 7)
        y = 3;
    else if ((num2 - num1) < 15)
        y = 2;
    else if ((num2 - num1) < 23)
        y = 1;
    else
        y = 0;

    for (uint8_t num = num1; num < num2 + 1; num++)
    {
        if (x % 16)
        {
            if (x + (num - num1 + 1) * 16 < 128)
                OLED_ONE_HZ(x + (num - num1) * 16, y, page, num);
            else if (x + (num - num1 + 1) * 16 < 256)
                OLED_ONE_HZ((num - num1 - 8 + (x / 16) + 1) * 16, y + 2, page, num);
            else if (x + (num - num1 + 1) * 16 < 384)
                OLED_ONE_HZ((num - num1 - 16 + (x / 16) + 1) * 16, y + 4, page, num);
            else if (x + (num - num1 + 1) * 16 < 512)
                OLED_ONE_HZ((num - num1 - 24 + (x / 16) + 1) * 16, y + 6, page, num);
        }
        else
        {
            if (x + (num - num1) * 16 < 128)
                OLED_ONE_HZ(x + (num - num1) * 16, y, page, num);
            else if (x + (num - num1) * 16 < 256)
                OLED_ONE_HZ((num - num1 - 8 + (x / 16)) * 16, y + 2, page, num);
            else if (x + (num - num1) * 16 < 384)
                OLED_ONE_HZ((num - num1 - 16 + (x / 16)) * 16, y + 4, page, num);
            else if (x + (num - num1) * 16 < 512)
                OLED_ONE_HZ((num - num1 - 24 + (x / 16)) * 16, y + 6, page, num);
        }
    }
}

/**
 * @brief OLED显示图片
 *
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标，最大是128
 * @param y2 终点y坐标，最大是8
 * @param BMP 图片选择
 */
void OLED_ShowPicture(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t BMP)
{
    uint32_t a = 0;

    switch (BMP)
    {
    case 1:
        for (uint8_t i = y1; i < (y2 - y1); i++)
            for (uint8_t n = x1; n < x2 - x1; n++, a++)
                OLED_SRAM[i][n] = BMP1[a];
        break;
    case 2:
        for (uint8_t j = y1; j < (y2 - y1); j++)
            for (uint8_t m = x1; m < x2 - x1; m++, a++)
                OLED_SRAM[j][m] = BMP2[a];
        break;
    case 3:
        for (uint8_t z = y1; z < (y2 - y1); z++)
            for (uint8_t l = x1; l < x2 - x1; l++, a++)
                OLED_SRAM[z][l] = BMP3[a];
        break;

    default:
        break;
    }
}
