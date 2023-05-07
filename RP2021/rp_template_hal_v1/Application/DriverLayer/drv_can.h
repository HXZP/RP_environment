/**
 *  @file       drv_can.h
 *  @author     @RobotPilots
 *  @version    v1.1.6
 *  @brief      CAN Driver Package(Based on HAL).
 *  @update
 *              v1.0(20-August-2020)
 *              v1.1(26-October-2021)
 *                  [*]1.�޸�CANx_rxDataHandler()�β�canId->rxId
 *              v1.1.1(8-November-2021)
 *                  [+]1.�����������
 *              v1.1.2(13-November-2021)
 *                  [*]1.�޸�����tx_port��tx_periodһֱΪĬ��ֵ��bug
 *              v1.1.3(20-November-2021)
 *                  [+]1.CAN_MailboxTypeDef�ṹ������auto_tx��Ա������������
 *                       ���Զ�����
 *                  [*]2.�޸�CAN_MailboxReadyForTx()�����ķ��������ж�����
 *                  [*]3.CAN_MailboxTypeDef�ṹ���޸�wait_tx_port_list������
 *                       Ϊwait_tx_port_fifo
 *              v1.1.4(27-November-2021)
 *                  [+]1.CAN_TxPortTypeDef�ṹ������delta_tx_time��Ա������
 *                       ʵ�ʷ��ͼ��
 *              v1.1.5(26-Feburary-2022)
 *                  [+]1.����can�����Լ�can����Ĵ�������¼
 *                  [*]2.�޸Ĳ��ֺ����ı���������ɾ����ʱ���Ա���
 *                  [+]3.���Ӻ���ע��
 *              v1.1.6(9-June-2022)
 *                  [+]1.can���Ͷ˿�����state��Ա�������Ƿ�������Ϣ���ͣ�
 *                       ����mode��Ա�����õ���/��������(Ĭ����������)
 */
#ifndef __DRV_CAN_H
#define __DRV_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "rp_driver_config.h"
/* Exported macro ------------------------------------------------------------*/
// ��������˿ڵķ��ͼ��
#define MAX_WAIT_TX_PORT_CNT 4  // �ȴ����͵����˿���Ŀ
/* Exported types ------------------------------------------------------------*/
typedef enum {
    CAN_MAILBOX_OK,
    CAN_MAILBOX_ERR,
    CAN_MAILBOX_BUSY
} CAN_MailboxErrnoTypeDef;

typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t             data[8];
} CAN_RxFrameTypeDef;

typedef struct {
    CAN_TxHeaderTypeDef header;
    uint8_t             data[8];
} CAN_TxFrameTypeDef;

typedef struct CAN_RxPortType {
    uint32_t rx_id;
    uint8_t  rx_buff[8];
    struct CAN_RxPortType *next;
} CAN_RxPortTypeDef;

typedef struct CAN_TxPortType {
    uint32_t                tx_id;
    uint8_t                 tx_buff[8];
    uint16_t                tx_period;
    uint32_t                last_tx_time;
    uint32_t                delta_tx_time;
    FunctionalState         state;
    CAN_TxModeTypeDef       mode;
    void                    *mailbox;
    struct CAN_TxPortType   *next;
} CAN_TxPortTypeDef;

typedef struct {
    CAN_MailboxErrnoTypeDef errno;
    uint32_t            err_cnt;
    //CAN_RxPortTypeDef *rx_port;
    CAN_TxPortTypeDef   *tx_port;
    uint8_t             tx_port_cnt;
    CAN_TxPortTypeDef   *wait_tx_port_fifo[MAX_WAIT_TX_PORT_CNT];
    uint8_t             wait_tx_port_cnt;
    FunctionalState     auto_tx;
    // MailboxState
} CAN_MailboxTypeDef;

extern CAN_MailboxTypeDef hcan1Mailbox;
extern CAN_MailboxTypeDef hcan2Mailbox;

/* Exported functions --------------------------------------------------------*/
void CAN1_Init(void);
void CAN2_Init(void);
//uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint32_t stdId, int16_t *dat);
//uint8_t CAN1_SendData(uint32_t stdId, int16_t *dat);
//uint8_t CAN2_SendData(uint32_t stdId, int16_t *dat);
HAL_StatusTypeDef CAN_StartTxByTxPort(CAN_TxPortTypeDef *tx_port);
HAL_StatusTypeDef CAN_StartTxByDriver(struct drv_can *drv, uint8_t *data);
void CAN_AddMsgByDriver(struct drv_can *drv, uint8_t *data, uint8_t data_cnt);
FunctionalState CAN_GetTxStateByDriver(struct drv_can *drv);
void CAN_SetTxStateByDriver(struct drv_can *drv, FunctionalState state);
void CAN_SetTxModeByDriver(struct drv_can *drv, CAN_TxModeTypeDef mode);
bool CAN_MailboxReadyForTx(CAN_MailboxTypeDef *mailbox);
void CAN_AutoTx(CAN_MailboxTypeDef *mailbox);

#endif
