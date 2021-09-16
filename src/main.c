#include "stm32f4xx.h"
#include "macros_utiles.h"

#define LED_VERTE			(BIT12)
#define LED_ORANGE			(BIT13)
#define LED_ROUGE			(BIT14)
#define LED_BLEUE			(BIT15)
#define SysTick_LOAD_RELOAD ((u32)0x00668A00)//TODO: Find frequency to get an interruption every 0.5ms with the if

volatile uint32_t decountForSeconds = 0;
volatile uint32_t decountForMilliSeconds = 0;


void configureLEDs(){
	RCC->AHB1ENR |= BIT3;
	GPIOD->MODER |= BIT24;
	GPIOD->MODER &= ~BIT25;
	GPIOD->MODER |= BIT26;
	GPIOD->MODER &= ~BIT27;
	GPIOD->MODER |= BIT28;
	GPIOD->MODER &= ~BIT29;
	GPIOD->MODER |= BIT30;
	GPIOD->MODER &= ~BIT31;
}

void configureSysTick(){
	SysTick->LOAD=SysTick_LOAD_RELOAD;
	SysTick->VAL = 0;
	SysTick->CTRL |= BIT2; //Clock source selection as 1: Processor clock (AHB)
	SysTick->CTRL |= BIT1; //SysTick exception request enable
	SysTick->CTRL |= BIT0; //Counter enable
}

void configureTIM2(){
	RCC->APB1ENR |= BIT0; //TIM2 clock enable
	TIM2->PSC = 2*(SystemCoreClock/4)/10000-1; //Base of 10 kHz
	TIM2->ARR = 5000-1; //Needs to be 0.5 sec to work with the if conditions
	TIM2->DIER |= BIT0; //Update interrupt enable
	NVIC->ISER[0]=BIT28; //Enable TIM2 interrupt
	TIM2->CR1 |= BIT0; //TIM2 counter enable
}

void TIM2_IRQHandler(){
	if (decountForMilliSeconds != 3) {
		GPIOD->ODR |= LED_VERTE;
		GPIOD->ODR |= LED_BLEUE;
		decountForMilliSeconds++;
	} else {
		GPIOD->ODR &= ~(LED_VERTE);
		GPIOD->ODR &= ~(LED_BLEUE);
		decountForMilliSeconds = 0;
	}
	TIM2->SR &= ~BIT0;
	TIM2->CR1 |= BIT0; //TIM2 counter enable
}


void SysTick_Handler(void)
{
	if (decountForSeconds == 3) {
		GPIOD->ODR &= ~(LED_ORANGE);
		GPIOD->ODR &= ~(LED_ROUGE);
		decountForSeconds = 0;
	} else {
		GPIOD->ODR |= LED_ORANGE;
		GPIOD->ODR |= LED_ROUGE;
		decountForSeconds++;
	}
}

int main(void)
{
	configureLEDs();
	configureSysTick();
	configureTIM2();
	while (1);
}
