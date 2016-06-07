#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

//--------------------------- Настройка светодиодов ----------------------------//
void initPin(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // Включаем тактирование порта

	GPIO_InitTypeDef myPin; // Создаем структуру для настройки ножки
	myPin.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_8; // Определяем ножки для работы
	myPin.GPIO_Mode  = GPIO_Mode_Out_PP; // Настройка режима работы
	myPin.GPIO_Speed = GPIO_Speed_50MHz; // Частота работы

	GPIO_Init(GPIOC, &myPin); // Инициализация
}
//------------------------------------------------------------------------------//


//--------------------- Настройка таймера на прерывание ------------------------//
void initTimer(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // Включаю тактирование таймера 6

	TIM_TimeBaseInitTypeDef baseTimer;   // Структура с настройками таймера
	baseTimer.TIM_Prescaler = 24000 - 1; // Настройка предделителя  на 1 мс
	baseTimer.TIM_Period    = 1000;      // Период - 1000 мс, т.е. 1 секунда
	TIM_TimeBaseInit(TIM6, &baseTimer);  // Заполнение регистров таймера

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // Разрешаем прерывания по обновлению счетчика таймера TIM6
	NVIC_EnableIRQ(TIM6_DAC_IRQn);             // Разрешаю обработку прерывания по переполнению счетчика

	TIM_Cmd(TIM6, ENABLE);                     // Включаю таймер
}
//------------------------------------------------------------------------------//


//--------------------- Настройка таймера на прерывание ------------------------//
void TIM6_DAC_IRQHandler() {
	static uint8_t flag = 0; //

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) { // Проверяем, что прерывание пришло от таймера
		flag = ~flag;

		if(flag) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
		} else {
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
		}
		/* Очищаем бит обрабатываемого прерывания */
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
