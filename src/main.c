#include "stm32f4xx.h"
#include "macros_utiles.h"

#define LED_VERTE					(BIT12)
#define LED_ORANGE					(BIT13)
#define LED_ROUGE					(BIT14)
#define LED_BLEUE					(BIT15)

#define SYSTICK_PERIOD_SECONDS		5e-4
#define SYSTICK_PRESCALER			8
#define SYSTICK_LOAD_RELOAD 		(SystemCoreClock / SYSTICK_PRESCALER) * SYSTICK_PERIOD_SECONDS

#define TIM2_PERIOD_SECONDS			0.5
#define TIM2_PREVIOUS_PRESCALERS 	2
#define TIM2_PRESCALER 				1
#define TIM2_AUTO_RELOAD			(SystemCoreClock / (TIM2_PREVIOUS_PRESCALERS * TIM2_PRESCALER)) * TIM2_PERIOD_SECONDS

static volatile uint32_t systickCycleCounter = 0;
static volatile uint32_t tim2CycleCounter = 0;


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
	SysTick->LOAD = SYSTICK_LOAD_RELOAD;
	SysTick->VAL = SYSTICK_LOAD_RELOAD;

	// Makes sure we are using the ARM core's external clock, that is, the clock provided by the STM32 MCU
	// (referred to as "to Cortex System timer" in the Clock Tree of the STM32F407's ref manual
	SysTick->CTRL &= ~BIT2;

	SysTick->CTRL |= BIT1; //SysTick exception request enable
	SysTick->CTRL |= BIT0; //Counter enable
}

void configureTIM2(){
	RCC->APB1ENR |= BIT0; //TIM2 clock enable
	TIM2->PSC = TIM2_PRESCALER - 1;
	TIM2->ARR = TIM2_AUTO_RELOAD - 1; //Needs to be 0.5 sec to work with the if conditions
	TIM2->DIER |= BIT0; //Update interrupt enable
	NVIC->ISER[0] = BIT28; //Enable TIM2 interrupt
	TIM2->CR1 |= BIT0; //TIM2 counter enable
}

void TIM2_IRQHandler(){
	TIM2->SR &= ~BIT0;
	if (tim2CycleCounter != 3) {
		GPIOD->ODR |= LED_VERTE;
		GPIOD->ODR |= LED_BLEUE;
	} else {
		GPIOD->ODR &= ~(LED_VERTE);
		GPIOD->ODR &= ~(LED_BLEUE);
	}
	tim2CycleCounter = (tim2CycleCounter + 1) % 4;
}


void SysTick_Handler(void)
{
	if (systickCycleCounter == 3) {
		GPIOD->ODR &= ~(LED_ORANGE);
		GPIOD->ODR &= ~(LED_ROUGE);
	} else {
		GPIOD->ODR |= LED_ORANGE;
		GPIOD->ODR |= LED_ROUGE;
	}
	systickCycleCounter = (systickCycleCounter + 1) % 4;
}

int main(void)
{
	configureLEDs();
	configureSysTick();
	configureTIM2();
	while (1);
}
