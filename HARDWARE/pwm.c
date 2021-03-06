


//文件名：pwm.c
//作者：zqw
//功能：脉冲调制波输出。PA6脉冲调制波输出。


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


void gpio_tim3_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void tim3_config(u16 data1)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	u16 CCR1_Val = data1; 
	u16 T=2*data1;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	/* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = T;       							//当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    								//设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数模式

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel2 */
//  // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

//   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
//   TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2    
//   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel3 */
//  // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel4 */
//  // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4
//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
}

void tim3_init()
{
	gpio_tim3_config();
	tim3_config(0);
}

void set_pwm(u16 data)
{
	TIM3->CCR1=data;
	TIM3->ARR=2*data;
}










