/**
 *  @file       rc.c
 *  @author     @RobotPilots
 *  @version    v1.0
 *  @brief      Rc Module.
 *  @update     
 *              v1.0(8-Feburary-2022)
 */
#ifndef __RC_H
#define __RC_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

#include "rc_sensor.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
    RELEASE = 0,// �ɿ�
    PRESS = 1   // ����
} button_state_t;

typedef enum {
    BTN_NONE_FLIP = 0,  // �޵�ƽ��ת
    PRESS_TO_RELEASE = 1,  // ����->�ɿ�
    RELEASE_TO_PRESS = 2   // �ɿ�->����
} button_flip_t;

typedef struct {
    button_state_t  state;      // ��ǰ״̬
    button_flip_t   flip;       // ��ת״̬
    uint32_t        update_time;// ����ʱ��(HALTICK: 1msΪ��λ)
    uint32_t        hold_time;  // ����ʱ��(HALTICK: 1msΪ��λ)
} button_t;

typedef enum {
    SW_UP = 1,
    SW_MID = 3,
    SW_DOWN = 2
} switch_state_t;

typedef enum {
    SW_NONE_FLIP = 0,
    SW_MID_TO_UP = 1,  
    SW_MID_TO_DOWN = 2,
    SW_UP_TO_MID = 3,
    SW_DOWN_TO_MID = 4
} switch_flip_t;

typedef struct {
    switch_state_t  state;      // ��ǰ״̬
    switch_flip_t   flip;       // ��ת״̬
    uint32_t        update_time;// ����ʱ��(HALTICK: 1msΪ��λ)
    uint32_t        hold_time;  // ����ʱ��(HALTICK: 1msΪ��λ)
} switch_t;

typedef struct {
    int16_t     val;    
} stick_t;

typedef struct {
    int16_t     val;
} mouse_speed_t;

typedef struct rc_info_struct {
    switch_t        SW1;
    switch_t        SW2;
    stick_t         CH0;
    stick_t         CH1;
    stick_t         CH2;
    stick_t         CH3;
    stick_t         THUMBWHEEL;
    button_t        W;
    button_t        S;
    button_t        A;
    button_t        D;
    button_t        SHIFT;
    button_t        CTRL;
    button_t        Q;
    button_t        E;
    button_t        R;
    button_t        F;
    button_t        G;
    button_t        Z;
    button_t        X;
    button_t        C;
    button_t        V;
    button_t        B;
    button_t        MOUSE_L;
    button_t        MOUSE_R;
    mouse_speed_t   MOUSE_VX;
    mouse_speed_t   MOUSE_VY;
    mouse_speed_t   MOUSE_VZ;
} rc_info_t;

typedef struct rc_struct {
    rc_sensor_t     *dev;
    rc_info_t       *info;
    void            (*init)(void);
    void            (*update)(void);
    void            (*reset)(void);
    button_flip_t   (*update_button)(button_t *btn, button_state_t new_btn_state);
    switch_flip_t   (*update_switch)(switch_t *sw, switch_state_t new_sw_state);
    void            (*update_stick)(stick_t *stick, int16_t val);
    void            (*update_mouse)(mouse_speed_t *mouse_speed, int16_t val);
    button_flip_t   (*copy_button)(button_t *dst, button_t *src);
    switch_flip_t   (*copy_switch)(switch_t *sw, switch_t *src);    
    void            (*reset_button)(button_t *btn, button_state_t new_btn_state);
    void            (*reset_switch)(switch_t *sw, switch_state_t new_sw_state);
    void            (*reset_stick)(stick_t *stick, int16_t val);
    void            (*reset_mouse)(mouse_speed_t *mouse_speed, int16_t val);    
} rc_t;

extern rc_info_t rc_info;
extern rc_t rc;

/* Exported functions --------------------------------------------------------*/
void rc_init(void);
void rc_update(void);
void rc_reset(void);

#endif
