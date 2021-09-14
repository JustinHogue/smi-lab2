#include "stm32f4xx.h"
#include "macros_utiles.h"

#define LED_VERTE			(BIT12)
#define LED_ORANGE			(BIT13)
#define LED_ROUGE			(BIT14)
#define LED_BLEUE			(BIT15)

volatile uint32_t tick = 0;
volatile uint32_t decount = 0;

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

void turnOnTheLights(){
	GPIOD->ODR |= LED_VERTE;
	GPIOD->ODR |= LED_ORANGE;
	GPIOD->ODR |= LED_ROUGE;
	GPIOD->ODR |= LED_BLEUE;
}

void turnOffTheLights(){
	GPIOD->ODR &= ~(LED_VERTE);
	GPIOD->ODR &= ~(LED_ORANGE);
	GPIOD->ODR &= ~(LED_ROUGE);
	GPIOD->ODR &= ~(LED_BLEUE);
}

void configureSysTickTimer(){
	SysTick_Config(SystemCoreClock / 1000);
}

void SysTick_Handler(void)
{
	if (decount == 7) {
		decount = 0;
	} else if (decount == 6){
		turnOffTheLights();
		decount++;
	} else {
		turnOnTheLights();
		decount++;
	}
}

int main(void)
{
	configureLEDs();
	SysTick_Config(SystemCoreClock / 4);
	while (1);
}
