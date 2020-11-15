#include "stm32f0xx.h"

void LoopDelay(volatile uint32_t n) {
	while(n > 0) n--;
}

int main(void){

	RCC->CR |= RCC_CR_HSION; 					//enable high-speed internal oscillator
	while (!(RCC->CR & RCC_CR_HSIRDY)); 		//wait until oscillator starts

	RCC->CFGR |= (RCC_CFGR_SW);					//set system clock source to hsi
	RCC->CFGR &= ~(RCC_CFGR_HPRE);				//set ahb prescaler to not divide

	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN);		//enable clock for PORTA peripheral

	GPIOA->MODER |= (GPIO_MODER_MODER5_0); 		//set PB5 mode to general purpose out
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_5);		//set PB5 type to push-pull
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR5_1);	//set PB5 speed to high
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5_0);		//set PB5 pu/pd to no pull

	while (1){

		GPIOA->BSRR |= (GPIO_BSRR_BS_5);
		LoopDelay(20000);
		GPIOA->BSRR |= (GPIO_BSRR_BR_5);
		LoopDelay(20000);

	}
}
