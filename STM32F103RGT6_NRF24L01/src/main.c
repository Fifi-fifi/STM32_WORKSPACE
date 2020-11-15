#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

#include "DEBUG_UART/debug_uart.h"
#include "NRF24L01regs.h"
//#define RX
#define CSN_HIGH	GPIOA->BSRR = GPIO_BSRR_BS4
#define CSN_LOW		GPIOA->BSRR = GPIO_BSRR_BR4

#define CE_HIGH		GPIOA->BSRR = GPIO_BSRR_BS3
#define CE_LOW		GPIOA->BSRR = GPIO_BSRR_BR3

char debug_uart_framebuf[100];

//just simple delay for demonstration
void LoopDelay(volatile uint64_t n) {
	while(n > 0) n--;
}

uint8_t SPI_transmit_recieve( uint8_t byte ){
	SPI1->DR = byte;
	while(!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
}

void SPI_write_buf( uint8_t* data, uint8_t len ){
	for(uint8_t i = 0; i<len; i++){
		SPI1->DR = data[i];
		while(!(SPI1->SR & SPI_SR_RXNE));
		uint8_t a = SPI1->DR;
	}
}

void SPI_read_buf( uint8_t* data, uint8_t len ){
	for(uint8_t i = 0; i<len; i++){
		SPI1->DR = 0x00;
		while(!(SPI1->SR & SPI_SR_RXNE));
		data[i] = SPI1->DR;
	}
}

void NRF_write_command( uint8_t command , uint8_t* data, uint8_t len){
	CSN_LOW;
	SPI_transmit_recieve(command);
	SPI_write_buf(data, len);
	CSN_HIGH;
}

void NRF_read_command( uint8_t command , uint8_t* data, uint8_t len){
	CSN_LOW;
	SPI_transmit_recieve(command);
	SPI_read_buf(data, len);
	CSN_HIGH;
}

void NRF_write_reg8( uint8_t reg, uint8_t byte){
	CSN_LOW;
	SPI_transmit_recieve(CMD_W_REGISTER | reg);
	SPI_transmit_recieve(byte);
	CSN_HIGH;
}

uint8_t NRF_read_reg8( uint8_t reg ){
	CSN_LOW;
	SPI_transmit_recieve(CMD_R_REGISTER | reg);
	uint8_t byte = SPI_transmit_recieve(0);
	CSN_HIGH;
	return byte;
}

uint8_t NRF_is_connected(){
	uint8_t old =  NRF_read_reg8(0);
	NRF_write_reg8(0, 0x0f);
	uint8_t read = NRF_read_reg8(0);
	NRF_write_reg8(0,old);
	if(read == 0x0f) return 1; else return 0;
}
#ifdef RX
uint8_t rx_addr0[5] = {0xf0,0xf0,0xf0,0xf0,0xf0};
uint8_t rx_addr1[5] = {0xf0,0xf0,0xf0,0xf0,0xf0};
uint8_t rx_addr2[5] = {0x02,0x02};
uint8_t rx_addr3[5] = {0x03,0x03};
uint8_t rx_addr4[5] = {0x04,0x04};
uint8_t rx_addr5[5] = {0x05,0x05};

uint8_t tx_addr[5] = {0xf0,0xf0,0xf0,0xf0,0xf0};
#else
uint8_t rx_addr0[5] = {0xf0,0xf0,0xf0,0xf0,0xf0};
uint8_t rx_addr1[5] = {0x01,0x01,0x01,0x01,0x01};
uint8_t rx_addr2[5] = {0x02,0x02};
uint8_t rx_addr3[5] = {0x03,0x03};
uint8_t rx_addr4[5] = {0x04,0x04};
uint8_t rx_addr5[5] = {0x05,0x05};

uint8_t tx_addr[5] = {0xf0,0xf0,0xf0,0xf0,0xf0};
#endif
void NRF_RX_mode(){
	CE_LOW;
	NRF_write_reg8(REG_CONFIG, NRF_read_reg8(REG_CONFIG) | (1<<CONFIG_PRIM_RX) );
	CE_HIGH;
	LoopDelay(10000);
}

void NRF_TX_mode(){
	CE_LOW;
	NRF_write_reg8(REG_CONFIG, NRF_read_reg8(REG_CONFIG) & (~(1<<CONFIG_PRIM_RX)) );
	NRF_write_reg8(REG_CONFIG, NRF_read_reg8(REG_CONFIG) | ((1<<CONFIG_PWR_UP)) );
	LoopDelay(10000);
}

void NRF_send_packet( uint8_t* data, uint8_t len){
	NRF_write_command(CMD_W_TX_PAYLOAD, data, len);
}

void NRF_init(){
	LoopDelay(100000);
	//
#ifndef RX
//	NRF_write_reg8(REG_CONFIG, 0x5A);//Register : 0x00 (CONFIG) 0x5A
//	NRF_write_reg8(REG_EN_AA, 0x3F);//Register : 0x01 (EN_AA) 0x3F
//	NRF_write_reg8(REG_EN_RXADDR, 0x03);//Register : 0x02 (EN_RXADDR) 0x03
//	NRF_write_reg8(REG_SETUP_AW, 0x03);//Register : 0x03 (SETUP_AW) 0x03
//	NRF_write_reg8(REG_SETUP_RETR, 0x03);//Register : 0x04 (SETUP_RETR) 0x03
//	NRF_write_reg8(REG_RF_CH, 0x1f);//Register : 0x05 (RF_CH) 0x1F
//	NRF_write_reg8(REG_RF_SETUP, 0x0f);//Register : 0x06 (RF_SETUP) 0x0F
//	NRF_write_reg8(REG_STATUS, 0x1e);//Register : 0x07 (STATUS) 0x1E
//	Register : 0x08 (OBSERVE_TX) 0x03
//	Register : 0x09 (CD) 0x00
//	Register : 0x0A (RX_ADDR_P0) 0xF0
//	Register : 0x0B (RX_ADDR_P1) 0xC2
//	Register : 0x0C (RX_ADDR_P2) 0xC3
//	Register : 0x0D (RX_ADDR_P3) 0xC4
//	Register : 0x0E (RX_ADDR_P4) 0xC5
//	Register : 0x0F (RX_ADDR_P5) 0xC6
//	Register : 0x10 (TX_ADDR) 0xF0
//	Register : 0x11 (RX_PW_P0) 0x08
//	Register : 0x12 (RX_PW_P1) 0x37
//	Register : 0x13 (RX_PW_P2) 0x00
//	Register : 0x14 (RX_PW_P3) 0x00
//	Register : 0x15 (RX_PW_P4) 0x00
//	Register : 0x16 (RX_PW_P5) 0x00
//	Register : 0x17 (FIFO_STATUS) 0x11
//	Register : 0x1C (DYNPD) 0x00
//	Register : 0x1D (FEATURE) 0x00
	//
#endif
	NRF_write_reg8(REG_EN_AA, 0x3F);		//enable AA on all pipes
	NRF_write_reg8(REG_EN_RXADDR, 0x3F);	//enable all RX adresses
	NRF_write_reg8(REG_SETUP_AW, 0x03);		//set adress witdh to 5bytes
	NRF_write_reg8(REG_SETUP_RETR, 0xe6);	//set retransimition data
	NRF_write_reg8(REG_RF_CH, 80);		//set rf channel
	NRF_write_reg8(REG_RF_SETUP, 0x00);		//set power to max
	NRF_write_reg8(REG_STATUS, 0x70);		//clear interrupts
	NRF_write_command(CMD_W_REGISTER | REG_RX_ADDR_P0, rx_addr0, 5);
	NRF_write_command(CMD_W_REGISTER | REG_RX_ADDR_P1, rx_addr1, 5);
	NRF_write_command(CMD_W_REGISTER | REG_RX_ADDR_P2, rx_addr2, 2);
	NRF_write_command(CMD_W_REGISTER | REG_RX_ADDR_P3, rx_addr3, 2);
	NRF_write_command(CMD_W_REGISTER | REG_RX_ADDR_P4, rx_addr4, 2);
	NRF_write_command(CMD_W_REGISTER | REG_RX_ADDR_P5, rx_addr5, 2);
	NRF_write_command(CMD_W_REGISTER | REG_TX_ADDR, tx_addr, 5);
	NRF_write_reg8(REG_DYNPD, 0x3f);	//3f	//all pipes variable length
	NRF_write_reg8(REG_FEATURE, 0x04);	//04	//enable features

	NRF_write_reg8(REG_CONFIG, NRF_read_reg8(REG_CONFIG) | (1<<CONFIG_PWR_UP) );		//enable features
	LoopDelay(65000);

	NRF_RX_mode();
}

void debug_uart_str_event(char * str){
	debug_uart_puts(str);
	debug_uart_puts("\r\n");
}

int main(void){

	//processor starts up with pll configured for 72Mhz. (we will change it in future to make it fail-save) Let's change that for now to 36Mhz

	RCC->CR |= (RCC_CR_HSION);					//turn on HSI oscillator (8Mhz)
	while(!(RCC->CR & RCC_CR_HSIRDY));			//wait until oscillator starts

	RCC->CFGR &= ~(RCC_CFGR_SW);				//set SYSCLK to HSI (for the PLL configuration)
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);	//wait until it switches

	RCC->CR &= ~(RCC_CR_PLLON);					//turn off PLL (to be able to change parameters)

	RCC->CFGR &= ~(RCC_CFGR_PLLSRC);			//select PLL input as HSI/2
	RCC->CFGR |= (RCC_CFGR_PLLMULL9);			//set PLL multiplier to x9

	RCC->CR |= RCC_CR_PLLON;					//turn PLL back on
	while(!(RCC->CR & RCC_CR_PLLRDY));			//wait until it locks

	RCC->CFGR = (RCC->CFGR | RCC_CFGR_SW_1) | (RCC->CFGR & ~(RCC_CFGR_SW_0)); 	//set SYSCLK to PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	//wait until it switches

	RCC->CFGR |= (RCC_CFGR_MCO_SYSCLK);			//set MCO to output sysclk

	RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN);		//enable Clock for GPIOC periph
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN);		//enable Clock for GPIOA periph

	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);		//enable Clock for GPIOA periph

	//mco output
	GPIOA->CRH |= GPIO_CRH_MODE8;				//set PA8 as output with drive strenght to 50Mhz
	GPIOA->CRH = ((GPIOA->CRH | GPIO_CRH_CNF8_1) & ~(GPIO_CRH_CNF8_0));	//set PA8 to push-pull alternate function

	//uart i/o
	GPIOA->CRH |= GPIO_CRH_MODE9;				//set PA9 as output with drive strenght to 50Mhz
	GPIOA->CRH = ((GPIOA->CRH | GPIO_CRH_CNF9_1) & ~(GPIO_CRH_CNF9_0));	//set PA9 to push-pull alternate function
	GPIOA->CRH &= ~(GPIO_CRH_MODE10);				//set PA10 as output with drive strenght to 50Mhz
	GPIOA->CRH = ((GPIOA->CRH | GPIO_CRH_CNF10_1) & ~(GPIO_CRH_CNF10_0));	//set PA10 to push-pull alternate function

	//onboard led
	GPIOC->CRH |= GPIO_CRH_MODE8_0;				//set PC8 as output with drive strenght to 10Mhz
	GPIOC->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF8_1);	//set PC8 to push-pull normal operation

	//spi to alternate
	GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7;	//output 50Mhz
	GPIOA->CRL = ((GPIOA->CRL | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1) & ~((GPIO_CRL_CNF5_0) | (GPIO_CRL_CNF6_0) | (GPIO_CRL_CNF7_0)));	//alternate funcion push-pull

	//ce csn to output
	GPIOA->CRL |= GPIO_CRL_MODE3_0 | GPIO_CRL_MODE4_0;	//output 50Mhz
	GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_CNF4);	//push-pull normal i/o

	//irq to input
	GPIOA->CRL &= ~(GPIO_CRL_MODE2);	//input
	GPIOA->CRL &= ~(GPIO_CRL_CNF2_0);
	GPIOA->CRL |= (GPIO_CRL_CNF2_1);	//input pupd
	GPIOA->ODR |= (GPIO_ODR_ODR2);		//pullup

	CSN_HIGH;

	//spi config
	SPI1->CR1 |= ( SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_BR_1 );	//set internal ss pin to high, master mode, and baud to /8
	SPI1->CR1 |= SPI_CR1_SPE;	//enable spi

	debug_uart_init();
	register_debug_uart_event_callback(debug_uart_str_event);
	NVIC_EnableIRQ(USART1_IRQn);

	if(NRF_is_connected()){
		for(uint8_t i = 0; i<10; i++){
			GPIOC->ODR ^= GPIO_ODR_ODR8;
			LoopDelay(100000);
		}

		NRF_init();

	}else{
		for(uint8_t i = 0; i<8; i++){
			GPIOC->ODR ^= GPIO_ODR_ODR8;
			LoopDelay(1000000);
		}
	}
	debug_uart_puts("start\r\n");
	while (1){

#ifdef RX
		while(GPIOA->IDR & GPIO_IDR_IDR2);
		LoopDelay(100000);
		if(NRF_read_reg8(REG_STATUS) & (1<<STATUS_RX_DR)){
			LoopDelay(10000);
			//uint8_t asd[6];
			NRF_read_command(CMD_FLUSH_RX, 0, 0);
			NRF_write_reg8(REG_STATUS, 0x70);		//clear interrupts
			//NRF_RX_mode();
			GPIOC->ODR ^= GPIO_ODR_ODR8;

		}
#else

		uint8_t data[3] = {0xaa,0xff,0x55};

		NRF_write_command(CMD_FLUSH_TX, 0, 0);
		NRF_TX_mode();

		NRF_send_packet(data, 3);


		CE_HIGH;
		LoopDelay(20);
		CE_LOW;

		LoopDelay(500);

		while(GPIOA->IDR & GPIO_IDR_IDR2)LoopDelay(10000);;
		LoopDelay(10000);
		uint8_t ob1 = NRF_read_reg8(REG_OBSERVE_TX);
		uint8_t stat = NRF_read_reg8(REG_STATUS);

		if(stat & (1<<STATUS_MAX_RT) ){
			debug_uart_puts("max\r\n");
			debug_uart_putc(ob1);
			for(uint8_t i = 0; i<2; i++){
				GPIOC->ODR ^= GPIO_ODR_ODR8;
				LoopDelay(100000);
			}
		}
		if(stat & (1<<STATUS_TX_DS) ){
			debug_uart_puts("ack\r\n");
			for(uint8_t i = 0; i<4; i++){
				GPIOC->ODR ^= GPIO_ODR_ODR8;
				LoopDelay(100000);
			}
		}
		NRF_write_reg8(REG_STATUS, 0x10);		//clear interrupts
		NRF_write_reg8(REG_STATUS, 0x20);		//clear interrupts
		NRF_write_reg8(REG_STATUS, 0x40);		//clear interrupts
		LoopDelay(200000);

#endif

		//GPIOC->BSRR = GPIO_BSRR_BS8;
		//LoopDelay(650000);
		//GPIOC->BSRR = GPIO_BSRR_BR8;

		//DEBUG_UART_EVENT(debug_uart_framebuf);

	}
}