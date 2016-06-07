#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

//--------------------------- ��������� ����������� ----------------------------//
void initPin(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // �������� ������������ �����

	GPIO_InitTypeDef myPin; // ������� ��������� ��� ��������� �����
	myPin.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_8; // ���������� ����� ��� ������
	myPin.GPIO_Mode  = GPIO_Mode_Out_PP; // ��������� ������ ������
	myPin.GPIO_Speed = GPIO_Speed_50MHz; // ������� ������

	GPIO_Init(GPIOC, &myPin); // �������������
}
//------------------------------------------------------------------------------//


//--------------------- ��������� ������� �� ���������� ------------------------//
void initTimer(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // ������� ������������ ������� 6

	TIM_TimeBaseInitTypeDef baseTimer;   // ��������� � ����������� �������
	baseTimer.TIM_Prescaler = 24000 - 1; // ��������� ������������  �� 1 ��
	baseTimer.TIM_Period    = 1000;      // ������ - 1000 ��, �.�. 1 �������
	TIM_TimeBaseInit(TIM6, &baseTimer);  // ���������� ��������� �������

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // ��������� ���������� �� ���������� �������� ������� TIM6
	NVIC_EnableIRQ(TIM6_DAC_IRQn);             // �������� ��������� ���������� �� ������������ ��������

	TIM_Cmd(TIM6, ENABLE);                     // ������� ������
}
//------------------------------------------------------------------------------//


//--------------------- ��������� ������� �� ���������� ------------------------//
void TIM6_DAC_IRQHandler() {
	static uint8_t flag = 0; //

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) { // ���������, ��� ���������� ������ �� �������
		flag = ~flag;

		if(flag) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
		} else {
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
		}
		/* ������� ��� ��������������� ���������� */
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}


}
//------------------------------------------------------------------------------//


int main(void) {
	initPin();
	initTimer();


	while (1);
	return 0;
}
