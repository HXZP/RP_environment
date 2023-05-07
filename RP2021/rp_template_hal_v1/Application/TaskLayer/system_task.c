/**
 * @file        system_task.c
 * @author      @RobotPilots
 * @version     v1.2
 * @brief       Decision Center.
 *  @update     
 *              v1.0(27-October-2020)
 *              v1.1(9-Feburary-2022)
 *                  [+]1.����ģ��rc
 *                  [*]2.�޸�ϵͳ���������������ȴ�
 *              v1.2(6-March-2022)
 *                  [+]1.system_task����ʼʱ����rc.init()
 */

/* Includes ------------------------------------------------------------------*/
#include "system_task.h"

#include "cmsis_os.h"
#include "rc.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
flag_t flag = {
    .gimbal = {
        .reset_start = false,
        .reset_ok = false,
    },
};

system_t sys = {
    .remote_mode = RC,
    .state = SYS_STATE_RCLOST,
    .mode = SYS_MODE_NORMAL,
};

/* Private functions ---------------------------------------------------------*/
/**
 *  @brief  ����ң�����л����Ʒ�ʽ
 */
static void system_ctrl_mode_switch(void)
{
    
}

/**
 *  @brief  ����ң�����л�ϵͳ��Ϊ
 */
static void system_mode_act_switch(void)
{
//    if(rc.info->W.flip == PRESS_TO_RELEASE) {
//        
//    }
}

static void system_state_machine(void)
{
    // ���Ʒ�ʽ�л�
    system_ctrl_mode_switch();
    // ϵͳģʽ�л�(����ģʽ�²������л�)
    if(sys.remote_mode == KEY)
        system_mode_act_switch();
}

/* Exported functions --------------------------------------------------------*/
/**
 *  @brief  ϵͳ��������
 */
void StartSystemTask(void const * argument)
{   
    static uint8_t reconnect_cnt = 0;
    
    rc.init();
    for(;;)
    {
        portENTER_CRITICAL();
        
        /* ң��ʧ�� */
        if(rc.dev->work_state == DEV_OFFLINE) {
            // ����rc������
            rc.dev->reset(rc.dev);
            // ����rcģ��
            rc.reset();
            // ������������
            reconnect_cnt = 0;
            // ϵͳ��ң��ʧ��
            sys.state = SYS_STATE_RCLOST;
        }
        /* ң������ */
        else {
            // ����rcģ��
            rc.update();  
            /* ң�س��� */
            if(rc.dev->errno != NONE_ERR) {
                __set_FAULTMASK(1);
                NVIC_SystemReset();
            } 
            /* ң������ */
            else {
                /* ʧ���ָ� */
                if(sys.state == SYS_STATE_RCLOST) {
                    if(reconnect_cnt < 2) {
                        reconnect_cnt++;
                    } else {
                        // ���ڴ˴�ͬ����̨��λ��־λ
                        flag.gimbal.reset_start = true;
                        flag.gimbal.reset_ok = false;
                        // ϵͳ��������λ
                        sys.remote_mode = RC;
                        sys.state = SYS_STATE_NORMAL;
                        sys.mode = SYS_MODE_NORMAL;
                    }
                }
                /* ϵͳ���� */
                else if(sys.state == SYS_STATE_NORMAL) {
                    // ���ڴ˴��ȴ���̨��λ��������л�״̬
                    if(flag.gimbal.reset_ok == true)
                        system_state_machine();
                }
            }
        }
        
        portEXIT_CRITICAL();
        
        osDelay(2);
    }
}
