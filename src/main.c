#include "stm32f4xx.h"
#include "macros_utiles.h"

#define LED_VERTE			(BIT12)
#define LED_ORANGE			(BIT13)
#define LED_ROUGE			(BIT14)
#define LED_BLEUE			(BIT15)

void configureLEDs(){
	RCC->AHB1ENR |= BIT3;
	GPIOD->MODER |= BIT24;
	GPIOD->MODER |= BIT26;
	GPIOD->MODER |= BIT28;
	GPIOD->MODER |= BIT30;
}

int main(void)
{
	volatile uint32_t i;
	configureLEDs();
	/* Infinite loop */
	while (1)
	{
		GPIOD->ODR |= LED_VERTE;
		GPIOD->ODR |= LED_ORANGE;
		GPIOD->ODR |= LED_ROUGE;
		GPIOD->ODR |= LED_BLEUE;
		for(i = 0; i < 5000000; i++);
		GPIOD->ODR &= ~(LED_VERTE);
		GPIOD->ODR &= ~(LED_ORANGE);
		GPIOD->ODR &= ~(LED_ROUGE);
		GPIOD->ODR &= ~(LED_BLEUE);
		for(i = 0; i < 5000000; i++);
	}
}
