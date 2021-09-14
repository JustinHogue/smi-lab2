#include "stm32f4xx.h"
#include "macros_utiles.h"

#define LED_VERTE			(BIT12)
#define LED_ORANGE			(BIT13)
#define LED_ROUGE			(BIT14)
#define LED_BLEUE			(BIT15)
#define SysTick_LOAD_RELOAD ((u32)0x00CD1400)//¼ of processor frequency to get an interruption every 250ms

volatile uint32_t tick = 0;
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
	// TODO: Determine Prescaler and Auto-Reload values!
	//TIM2->PSC = ???;
	TIM2->ARR = 4; //Needs to be 0.5 ms
	TIM2->DIER |= BIT0; //Update interrupt enable
	NVIC->ISER[0]=BIT28; //Enable TIM2 interrupt
	TIM2->CR1 |= BIT0; //TIM2 counter enable
}

void TIM2_IRQHandler(){
	if (decountForMilliSeconds != 3) {
		GPIOD->ODR |= LED_ORANGE;
		GPIOD->ODR |= LED_ROUGE;
		decountForMilliSeconds++;
	} else {
		GPIOD->ODR &= ~(LED_ORANGE);
		GPIOD->ODR &= ~(LED_ROUGE);
		decountForMilliSeconds = 0;
	}
}


void SysTick_Handler(void)
{
	if (decountForSeconds == 7) {
		decountForSeconds = 0;
	} else if (decountForSeconds == 6){
		GPIOD->ODR &= ~(LED_VERTE);
		GPIOD->ODR &= ~(LED_BLEUE);
		decountForSeconds++;
	} else {
		GPIOD->ODR |= LED_VERTE;
		GPIOD->ODR |= LED_BLEUE;
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
