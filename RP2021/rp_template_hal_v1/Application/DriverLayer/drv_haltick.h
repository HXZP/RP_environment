/**
 *  @file       drv_haltick.c
 *  @author     @RobotPilots
 *  @version    v1.2
 *  @brief      Haltick driver
 *  @update
 *              v1.0(15-September-2020)
 *              v1.1(10-October-2021)
 *                  1.ͨ����дHAL_IncTick()������Ҫ��������micros()�Ÿ���
 *                    HalTick��ֵ�޸ĳ����ж������Զ�����(��/1ms)����micros()
 *                    �����ڵ���ʱ��ȷ���㵱ǰʱ��(us)��
 *              v1.2(8-Feburary-2022)
 *                  1.����millis()
 */
#ifndef __DRV_HALTICK_H
#define __DRV_HALTICK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

// Hal��ȫ��ʱ�����(Ĭ��1ms����һ��)
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;
/* Exported macro ------------------------------------------------------------*/
#define millis()    (uwTick)

/* Exported types ------------------------------------------------------------*/
extern volatile uint32_t HalTick;

/* Exported functions --------------------------------------------------------*/
uint32_t micros(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
