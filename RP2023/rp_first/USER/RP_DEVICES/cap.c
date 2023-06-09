#include "cap.h"
/*
电容使用：
1、使用cap.setdata函数进行赋值
2、用can将Buff0x2E和Buff0x2F两个包发送出去
其中一个是数据包一个是控制包，控制的频率自己斟酌
*/

#define CAP_OUTPUT_LIMIT 150

static void CAP_setMessage(cap_t *self,float powerBuff,float powerLimit,float volt,float current,float outLimit);
static void CAP_rxMessage(cap_t *self,uint32_t canId, uint8_t *rxBuf);
static void cap_heart_beat(cap_t *self);

cap_t cap = {

	.info.canId = 0x30,
	
	.info.offline_max_cnt = 100,
	.info.offline_cnt = 0,
	.info.state = CAP_OFFLINE,

	.updata     = CAP_rxMessage,
	.setdata    = CAP_setMessage,
	.heart_beat = cap_heart_beat,
};

static void cap_heart_beat(cap_t *self)
{
	cap_info_t *info = &self->info;

	info->offline_cnt++;
	if(info->offline_cnt > info->offline_max_cnt)//每次等待一段时间后自动离线
	{
		info->offline_cnt = info->offline_max_cnt;
		info->state = CAP_OFFLINE;
	} 
	else //每次接收成功就清空计数
	{
		/* 离线->在线 */
		if(info->state == CAP_OFFLINE)
		{
			info->state = CAP_ONLINE;
		}
	}
}	


int16_t float_to_int16(float a, float a_max, float a_min, int16_t b_max, int16_t b_min)
{
    int16_t b = (a - a_min) / (a_max - a_min) * (float)(b_max - b_min) + (float)b_min + 0.5f;   //加0.5使向下取整变成四舍五入
    return b;
}

float int16_to_float(int16_t a, int16_t a_max, int16_t a_min, float b_max, float b_min)
{
    float b = (float)(a - a_min) / (float)(a_max - a_min) * (b_max - b_min) + b_min;
    return b;
}

//裁判系统发送功率缓冲的频率为50Hz，该函数需要每20ms调用一次。
void CAP_setBuff0x2E(cap_t *self)
{
    self->data.Buff0x2E[0] = self->data.tx.chassis_power_buffer;    //底盘功率缓冲，0~60J
    self->data.Buff0x2E[1] = self->data.tx.chassis_volt;    	//底盘输出电压 单位 毫伏 **
    self->data.Buff0x2E[2] = self->data.tx.chassis_current;    //底盘输出电流 单位 毫安 **
}

void CAP_setBuff0x2F(cap_t *self)
{
    self->data.Buff0x2F[0] = self->data.tx.chassis_power_limit;   //底盘功率限制上限，0~120W
    self->data.Buff0x2F[1] = self->data.tx.output_power_limit;    //电容放电功率限制，-120~300W
    self->data.Buff0x2F[2] = self->data.tx.input_power_limit;     //电容充电功率限制，0~150W
    self->data.Buff0x2F[3] = self->data.tx.cap_control.all;       //电容开关，0（关闭）、1（开启）

}

void CAP_setMessage(cap_t *self,float powerBuff,float powerLimit,float volt,float current,float outLimit)
{
    self->data.tx.chassis_power_buffer = powerBuff;
    self->data.tx.chassis_power_limit  = powerLimit;
    self->data.tx.chassis_volt         = volt;
    self->data.tx.chassis_current      = current;
		self->data.tx.output_power_limit   = outLimit;
    self->data.tx.input_power_limit    = 150;
	
    self->data.tx.cap_control.bit.cap_switch = 1;
    self->data.tx.cap_control.bit.cap_record = 0;
	
		CAP_setBuff0x2E(self);
		CAP_setBuff0x2F(self);
}



void CAP_rxMessage(cap_t *self,uint32_t canId, uint8_t *rxBuf)
{
    if(canId == 0x30)
    {
        self->data.rx.cap_Ucr = int16_to_float(((uint16_t)rxBuf[0] << 8| rxBuf[1]), 32000, -32000, 30, 0);
        self->data.rx.cap_I   = int16_to_float(((uint16_t)rxBuf[2] << 8| rxBuf[3]), 32000, -32000, 20, -20);
        self->data.rx.cap_state.state = ((uint16_t)rxBuf[4] << 8| rxBuf[5]);    
			
        self->info.offline_cnt = 0;			
    }
}









