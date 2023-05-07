/**
 *  @file       rp_config.h
 *  @author     @RobotPilots
 *  @version    v1.1
 *  @brief      RobotPilots Robots' Configuration.
 *  @update
 *              v1.0(9-September-2020)
 *              v1.1(7-November-2021)
 *                  1.�Ż��豸����Ϣ��ṹ��ı������壬����volatile/const�ؼ���
 *                  2.��rp_config.h�ֳ�driver_config.h, device_config.h, user_config.h����ͷ�ļ�    
 */

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
    WFI;          
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
    CPSID   I
    BX      LR      
}
//���������ж�
__asm void INTX_ENABLE(void)
{
    CPSIE   I
    BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0             //set Main Stack value
    BX r14
}
