#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void initTimer(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef pinTimer;
	pinTimer.GPIO_Pin   = GPIO_Pin_0;
	pinTimer.GPIO_Mode  = GPIO_Mode_AF_PP;  // Режим работы для ножки - альтернативная функция, push pull
	pinTimer.GPIO_Speed = GPIO_Speed_50MHz; //
	GPIO_Init(GPIOA, &pinTimer);



    /* Настроечные структурки
     1) TIM_TimeBaseInitTypeDef -- Основные настройки таймера
     2) TIM_OCInitTypeDef  -- Настройки для управление каналами настроенные как ВЫХОД
     3) TIM_ICInitTypeDef  -- Настройки для управление каналами настроенные как ВХОД
     4) TIM_BDTRInitTypeDef  -- Дополнительные разные настройки    */


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_CounterMode   = TIM_CounterMode_Up;
	timer.TIM_Prescaler     = 24000 - 1;
	timer.TIM_Period        = 100;
	timer.TIM_ClockDivision = TIM_CKD_DIV1; // CKD[1:0]: Clock division определяем частоту для фильтров (tDTS)
	// baseTimer.TIM_RepetitionCounter = 1; //Этот пораметр только для Т1 и Т8
	TIM_TimeBaseInit(TIM2, &timer); // Засовываем настройи в регистры

	// Настройка первого канала таймера
	 TIM_OCInitTypeDef timer_OCI;                          // создаём переменную (структуру) для определения режима работы таймера
	 timer_OCI.TIM_OCMode       = TIM_OCMode_PWM1;         // Режим установки ножки при совпадении
	 timer_OCI.TIM_OutputState  = TIM_OutputNState_Enable; // Подключаем нашу ножку к логике таймера
	 timer_OCI.TIM_Pulse        = 1000;                    // регистр TIMx->CCR1 (число сравнения в канале) число между  0x0000 и 0xFFFF
	 timer_OCI.TIM_OCPolarity   = TIM_OCNPolarity_High;    // рег TIMx->CCER бит CC1P Полярность выхода
	 TIM_OC1Init(TIM2, &timer_OCI);

	 TIM2->DIER |= TIM_DIER_CC1IE;

	 NVIC_EnableIRQ(TIM2_IRQn);
	 TIM_Cmd(TIM2, ENABLE); // Пуск!

}

void TIM2_IRQHandler(void) {
	int a;
	TIM_ClearITPendingBit(TIM6, TIM_IT_CC1);
}


int main(void){
	initTimer();


    while(1)
    {
    }
}

