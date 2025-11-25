#include "stm32f10x.h"

int16_t rotate_Count = 0;
uint16_t flag = 1;

/**
  * 函    数：旋转编码器初始化
  * 参    数：无
  * 返 回 值：无
  */
void rotate_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);        //开启GPIOB的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //开启AFIO的时钟
    
   
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    //禁用JTAG，保留SWD
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*AFIO选择中断引脚*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3); //PB3映射到EXTI3
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4); //PB4映射到EXTI4
    
    /*EXTI初始化*/
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;     //EXTI3和EXTI4
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿触发
    EXTI_Init(&EXTI_InitStructure);
    
    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 配置EXTI3中断 (PB3)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 配置EXTI4中断 (PB4)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}

int16_t rotate_Get(void)
{
    int16_t Temp;
    Temp = rotate_Count;
    rotate_Count = 0;
    flag = 1;
    return Temp;
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) == SET)		//判断是否是外部中断0号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)		//PB0的下降沿触发中断，此时检测另一相PB1的电平，目的是判断旋转方向
			{
				rotate_Count --;					//此方向定义为反转，计数变量自减
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line3);			//清除外部中断0号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}

/**
  * 函    数：EXTI1外部中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) == SET)		//判断是否是外部中断1号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)		//PB1的下降沿触发中断，此时检测另一相PB0的电平，目的是判断旋转方向
			{
				rotate_Count ++;					//此方向定义为正转，计数变量自增
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line4);			//清除外部中断1号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}

