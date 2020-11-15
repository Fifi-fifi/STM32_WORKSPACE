#include "main.h"
#include "stm32f0xx.h"

#define RFM_CE_HIGH		GPIOA->BSRR |= GPIO_BSRR_BS_3
#define RFM_CE_LOW		GPIOA->BSRR |= GPIO_BSRR_BR_3

#define RFM_CSN_HIGH	GPIOA->BSRR |= GPIO_BSRR_BS_2
#define RFM_CSN_LOW		GPIOA->BSRR |= GPIO_BSRR_BR_2

#define SPI_ON			SPI1->CR1 |= (SPI_CR1_SPE)
#define SPI_OFF			SPI1->CR1 &= ~(SPI_CR1_SPE)

char out_buf[ 8 ] = "mirekk36";

const uint32_t Bank1_Reg0_13[] = {
	0xE2014B40,
	0x00004BC0,
	0x028CFCD0,
	0x41390099,
	0x1B8296d9,
	0xA67F0224,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00127300,
	0x46B48000
};

const uint8_t Bank1_Reg14[] = {
		0x41,0x20,0x08,0x04,0x81,0x20,0xCF,0xF7,0xFE,0xFF,0xFF
};

const uint8_t Bank0_Reg[][2] = {
		{0,0x0F},//reflect RX_DR\TX_DS\MAX_RT,Enable CRC ,2byte,POWER UP,PRX
		{1,0x3F},//Enable auto acknowledgement data pipe5\4\3\2\1\0
		{2,0x3F},//Enable RX Addresses pipe5\4\3\2\1\0
		{3,0x03},//RX/TX address field width 5byte
		{4,0xff},//auto retransmission dalay (4000us),auto retransmission count(15)
		{5,0x17},//23 channel
		{6,0x07},//air data rate-1M,out power 0dbm,setup LNA gain
		{7,0x07},//
		{8,0x00},//
		{9,0x00},
		{12,0xc3},//only LSB Receive address data pipe 2, MSB bytes is equal to RX_ADDR_P1[39:8]
		{13,0xc4},//only LSB Receive address data pipe 3, MSB bytes is equal to RX_ADDR_P1[39:8]
		{14,0xc5},//only LSB Receive address data pipe 4, MSB bytes is equal to RX_ADDR_P1[39:8]
		{15,0xc6},//only LSB Receive address data pipe 5, MSB bytes is equal to RX_ADDR_P1[39:8]
		{17,0x20},//Number of bytes in RX payload in data pipe0(32 byte)
		{18,0x20},//Number of bytes in RX payload in data pipe1(32 byte)
		{19,0x20},//Number of bytes in RX payload in data pipe2(32 byte)
		{20,0x20},//Number of bytes in RX payload in data pipe3(32 byte)
		{21,0x20},//Number of bytes in RX payload in data pipe4(32 byte)
		{22,0x20},//Number of bytes in RX payload in data pipe5(32 byte)
		{23,0x00},//fifo status
		{28,0x3F},//Enable dynamic payload length data pipe5\4\3\2\1\0
		{29,0x07}//Enables Dynamic Payload Length,Enables Payload with ACK,Enables the W_TX_PAYLOAD_NOACK command
};

const uint8_t RX0_Address[] = {0x34,0x43,0x10,0x10,0x01};//Receive address data pipe 0
const uint8_t RX1_Address[] = {0x39,0x38,0x37,0x36,0xc2};//Receive address data pipe 1

//returns "status" reg
uint8_t RFM_write_reg( uint8_t reg , uint8_t value ){
	RFM_CSN_LOW;
	SPI1->DR = (uint16_t)(value<<8)+reg;
	while(SPI1->SR & SPI_SR_BSY);
	RFM_CSN_HIGH;
	return SPI1->DR;
}

void RFM_write_buf(uint8_t reg, uint8_t *pBuf, uint8_t len) {
	for(uint8_t i=0; i<len; i++){
		RFM_write_reg(i+reg,pBuf[i]);
	}
}

uint8_t RFM_read_reg( uint8_t reg ){
	RFM_CSN_LOW;
	SPI1->DR = (uint16_t)(0<<8)+reg;
	while(SPI1->SR & SPI_SR_BSY);
	RFM_CSN_HIGH;
	return SPI1->DR >> 8;
}

void RFM_read_buf(uint8_t reg, uint8_t *pBuf, uint8_t len) {
	uint8_t i;

	for(i=0;i<len;i++){
		pBuf[i] = RFM_read_reg(i+reg);
	}
}

uint8_t RFM_is_connected(){
	uint8_t old = RFM_read_reg(0x00);

	RFM_write_reg(0x20, 0x0f);
	uint8_t read = RFM_read_reg(0x00);

	RFM_write_reg(0x20, old);

	if(read == 0x0f){
		return 1;
	}else{
		return 0;
	}

}

void RFM_power_down( void ) {

	uint8_t value;

	RFM_CE_LOW;

	value = RFM_read_reg(CONFIG);
	value = value & 0xfd;
	RFM_write_reg(WRITE_REGISTER| CONFIG, value);

	RFM_CE_HIGH;
}

void RFM_RX_mode() {
	uint8_t value;

	RFM_write_reg(FLUSH_RX,0);

	value = RFM_read_reg(STATUS);
	RFM_write_reg(WRITE_REGISTER|STATUS,value);

	RFM_CE_LOW;

	value = RFM_read_reg(CONFIG);
	value = value | 0x01;
	RFM_write_reg(WRITE_REGISTER | CONFIG, value);

  	RFM_CE_HIGH;
}

void RFM_TX_mode() {
	uint8_t value;

	RFM_CE_LOW;

	RFM_write_reg(FLUSH_TX,0);
	value = RFM_read_reg(CONFIG);
	value = value & 0xfe;
	RFM_write_reg(WRITE_REGISTER | CONFIG, value);

	RFM_CE_HIGH;
}

void RFM_Switch_CFG(char cfg) {
	uint8_t Tmp;

	Tmp = RFM_read_reg(7);
	Tmp = Tmp & 0x80;

	if( ( (Tmp)&&(cfg==0) ) || ( ((Tmp)==0)&&(cfg) ) )
		RFM_write_reg(ACTIVATE_CMD,0x53);
}

void RFM_set_channel(uint8_t ch) {
	RFM_write_reg((uint8_t)(WRITE_REGISTER|5),(uint8_t)(ch));
}

void RFM_Init() {
	uint8_t i,j;
 	uint8_t WriteArr[12];

#if RFM7x_UseIRQ == 1

	// inicjalizacja przerwania asynchronicznego INT2
	RF_IRQDDR 	&= ~IRQ;
	RF_IRQPORT 	|= IRQ;
	GICR 		|= (1<<INT2);
	MCUCSR 		&= ~(1<<ISC2);	// zbocze opadaj¹ce

	// inicjalizacja przerwania synchronicznego INT0 / INT1
//	RF_IRQDDR 	&= ~IRQ;
//	RF_IRQPORT 	|= IRQ;
//	SPI_PORT 	|= IRQ;
//	GICR 		|= (1<<INT0); //(1<<INT1);
//	MCUCR 		&= ~(1<<ISC01); //(1<<ISC11);	// zbocze opadaj¹ce

#endif

	RFM_Switch_CFG(0);

	for(i=0;i<20;i++) {
		RFM_write_reg((WRITE_REGISTER | Bank0_Reg[i][0] ),Bank0_Reg[i][1] );
	}

//reg 10 - Rx0 addr
	for(j=0;j<5;j++) {
		WriteArr[j] = RX0_Address[j];
	}
	RFM_write_buf((WRITE_REGISTER|10),&(WriteArr[0]),5);

//REG 11 - Rx1 addr
	for(j=0;j<5;j++) {
		WriteArr[j] = RX1_Address[j];
	}
	RFM_write_buf((WRITE_REGISTER|11),&(WriteArr[0]),5);

//REG 16 - TX addr
	for(j=0;j<5;j++) {
		WriteArr[j] = RX0_Address[j];
	}
	RFM_write_buf((WRITE_REGISTER|16),&(WriteArr[0]),5);


	i=RFM_read_reg(29);
	if(i==0) RFM_write_reg(ACTIVATE_CMD,0x73);// Active
	for(i=22;i>=21;i--)	{
		RFM_write_reg((WRITE_REGISTER|Bank0_Reg[i][0] ),Bank0_Reg[i][1]);
	}

	RFM_Switch_CFG(1);
	//reverse
	for(i=0;i<=8;i++) {
		for(j=0;j<4;j++) WriteArr[j] = (Bank1_Reg0_13[i] >> (8*(j) ) )&0xff;
		RFM_write_buf((WRITE_REGISTER|i),&(WriteArr[0]),4);
	}

	for(i=9;i<=13;i++) {
		for(j=0;j<4;j++) WriteArr[j]=( Bank1_Reg0_13[i]  >> (8*(3-j) ) )&0xff;
		RFM_write_buf((WRITE_REGISTER|i),&(WriteArr[0]),4);
	}

	for(j=0;j<11;j++) {
		WriteArr[j] = Bank1_Reg14[j];
	}
	RFM_write_buf((WRITE_REGISTER|14),&(WriteArr[0]),11);

//toggle REG4<25,26>
	for(j=0;j<4;j++) WriteArr[j]=(Bank1_Reg0_13[4] >> (8*(j) ) )&0xff;

	WriteArr[0]=WriteArr[0]|0x06;
	RFM_write_buf((WRITE_REGISTER|4),&(WriteArr[0]),4);

	WriteArr[0]=WriteArr[0]&0xf9;
	RFM_write_buf((WRITE_REGISTER|4),&(WriteArr[0]),4);

	RFM_Switch_CFG(0);
	RFM_RX_mode();
}

uint8_t Send_Packet(uint8_t type, void* pbuf, uint8_t len) {
	uint16_t tmo=0xffff;

	while( (RFM_read_reg(FIFO_STATUS) & FIFO_STATUS_TX_FULL) && tmo-- );

	RFM_TX_mode();  //switch to tx mode

#if RFM7x_UseIRQ == 1
	rfm7x_TX_flag=1;
#endif

	RFM_write_buf(type, pbuf, len); // Writes data to buffer

#if RFM7x_UseIRQ == 1
	//---- right way for RFM70 and RFM73 using external IRQ
	tmo=0xffff;
	while( rfm7x_TX_flag && tmo-- );
#else
	//---- right way for RFM70 and RFM73 without using external IRQ
	uint8_t value;
	do {
		value = RFM_read_reg(READ_REGISTER|STATUS);
	} while( !(value & STATUS_TX_DS) );

	RFM_write_reg(WRITE_REGISTER | STATUS, value|STATUS_TX_DS);
#endif

	RFM_RX_mode();

	return 0;
}

uint8_t Receive_Packet(void * pBuf) {

	uint8_t len, len1=0, sta, fifo_sta;
	uint8_t *wsk = pBuf;

	sta = RFM_read_reg( READ_REGISTER|STATUS );

	if( (sta & STATUS_RX_DR) == STATUS_RX_DR ) {

		do {
			len=RFM_read_reg(R_RX_PL_WID_CMD);	// read len

			if(len<=32) {
				RFM_read_buf(RD_RX_PLOAD, pBuf, len);// read receive payload from RX_FIFO buffer
				len1=len;
			} else {
				RFM_write_reg( FLUSH_RX, 0);//flush Rx
			}

			fifo_sta=RFM_read_reg( READ_REGISTER|FIFO_STATUS );	// read register FIFO_STATUS's value

		} while( (fifo_sta&FIFO_STATUS_RX_EMPTY)==0 ); //while not empty

		wsk[len1]=0;  // automatycznie zapisanie zera na potrzeby transmisji tekstu

		RFM_write_reg( WRITE_REGISTER|STATUS, sta );// clear RX_DR or TX_DS or MAX_RT interrupt flag

	}

	return len1;
}

uint8_t rfm7x_RX_buf[10];

int main(void){

	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY));

	RCC->CFGR &= ~(RCC_CFGR_SW_HSI);
	while((RCC->CR & RCC_CFGR_SWS));

	RCC->CR &= ~(RCC_CR_PLLON);
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC);
	RCC->CFGR |= (RCC_CFGR_PLLSRC_HSI_DIV2);
	RCC->CFGR &= ~(RCC_CFGR_PLLMUL);
	RCC->CFGR |= (RCC_CFGR_PLLMUL12);
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR& RCC_CR_PLLRDY));

	RCC->CFGR |= (RCC_CFGR_SW_PLL);				//now clock is 48Mhz

	RCC->CFGR |= (RCC_CFGR_MCO_SYSCLK);			//set MCO to output sysclk

	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN);		//enable clock for PORTA peripherial
	RCC->AHBENR |= (RCC_AHBENR_GPIOCEN);		//enable clock for PORTC peripherial
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);		//enable clock for SPI peripherial

	//mco
	GPIOA->MODER &= ~(GPIO_MODER_MODER8);
	GPIOA->MODER |= GPIO_MODER_MODER8_1;		//set PA8 as alternate funcion
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_8);		//set PA8 to push-pull
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8);	//set PA8 as high-speed i/o

	//led
	GPIOC->MODER &= ~(GPIO_MODER_MODER8);
	GPIOC->MODER |= GPIO_MODER_MODER8_0;		//set PC8 as output
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8);		//set PC8 to push-pull
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8);	//set PC8 as high-speed i/o

	//spi to alternate
	GPIOA->MODER &= ~((GPIO_MODER_MODER5) | (GPIO_MODER_MODER6) | (GPIO_MODER_MODER7));
	GPIOA->MODER |= (GPIO_MODER_MODER5_1) | (GPIO_MODER_MODER6_1) | (GPIO_MODER_MODER7_1);
	GPIOA->OTYPER &= ~((GPIO_OTYPER_OT_5) | (GPIO_OTYPER_OT_6) | (GPIO_OTYPER_OT_7));
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR5) | (GPIO_OSPEEDER_OSPEEDR6) | (GPIO_OSPEEDER_OSPEEDR7);

	//ce and csn to out
	GPIOA->MODER &= ~((GPIO_MODER_MODER2) | (GPIO_MODER_MODER3));
	GPIOA->MODER |= (GPIO_MODER_MODER2_0) | (GPIO_MODER_MODER3_0);
	GPIOA->OTYPER &= ~((GPIO_OTYPER_OT_2) | (GPIO_OTYPER_OT_3));
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2) | (GPIO_OSPEEDER_OSPEEDR3);

	//spi config
	SPI1->CR1 &= ~((SPI_CR1_CPHA) | (SPI_CR1_CPOL) | (SPI_CR1_CRCEN) );						//set phase and polarity to 0
	SPI1->CR1 |= (SPI_CR1_MSTR) | (SPI_CR1_BR_1) | (SPI_CR1_SSM) | (SPI_CR1_SSI);			//set spi to master and clock to /8 and software slave to 1

	SPI1->CR2 = (SPI1->CR2 & (~SPI_CR2_DS)) | (SPI_CR2_DS_2) | (SPI_CR2_DS_1) | (SPI_CR2_DS_0); 	//spi 8-bit															//turn on spi

	SPI_ON;
	RFM_CSN_HIGH;

	RFM_Init();

	RFM_set_channel(41);

	while (1){

		if(RFM_is_connected()){
			Send_Packet(W_TX_PAYLOAD_NOACK_CMD, &out_buf, 8);
			GPIOC->BSRR |= GPIO_BSRR_BS_8;
			for(uint32_t i=0;i<1000;i++)for(uint32_t j=0;j<100;j++)__NOP();
			GPIOC->BSRR |= GPIO_BSRR_BR_8;
		}else{
			GPIOC->BSRR |= GPIO_BSRR_BR_8;
		}


		uint8_t len = Receive_Packet( rfm7x_RX_buf );
		if( len ){
			GPIOC->BSRR |= GPIO_BSRR_BS_8;
			for(uint32_t i=0;i<1000;i++)for(uint32_t j=0;j<1000;j++)__NOP();
			GPIOC->BSRR |= GPIO_BSRR_BR_8;
		}


		for(uint32_t i=0;i<1000;i++)for(uint32_t j=0;j<1000;j++)__NOP();

	}
}
