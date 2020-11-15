#ifndef NRF24L01REGS_H_
#define NRF24L01REGS_H_

// Number of registers
#define NUM_REGISTERS   26

// Registers
#define REG_CONFIG      0x00
#define REG_EN_AA       0x01
#define REG_EN_RXADDR   0x02
#define REG_SETUP_AW    0x03
#define REG_SETUP_RETR  0x04
#define REG_RF_CH       0x05
#define REG_RF_SETUP    0x06
#define REG_STATUS      0x07
#define REG_OBSERVE_TX  0x08
#define REG_RPD         0x09
#define REG_RX_ADDR_P0  0x0A
#define REG_RX_ADDR_P1  0x0B
#define REG_RX_ADDR_P2  0x0C
#define REG_RX_ADDR_P3  0x0D
#define REG_RX_ADDR_P4  0x0E
#define REG_RX_ADDR_P5  0x0F
#define REG_TX_ADDR     0x10
#define REG_RX_PW_P0    0x11
#define REG_RX_PW_P1    0x12
#define REG_RX_PW_P2    0x13
#define REG_RX_PW_P3    0x14
#define REG_RX_PW_P4    0x15
#define REG_RX_PW_P5    0x16
#define REG_FIFO_STATUS 0x17
#define REG_DYNPD       0x1C
#define REG_FEATURE     0x1D

// Register bit defines
#define CONFIG_MASK_RX_DR  6
#define CONFIG_MASK_TX_DS  5
#define CONFIG_MASK_MAX_RT 4
#define CONFIG_EN_CRC      3
#define CONFIG_CRCO        2
#define CONFIG_PWR_UP      1
#define CONFIG_PRIM_RX     0

#define STATUS_RX_DR       6
#define STATUS_TX_DS       5
#define STATUS_MAX_RT      4
#define STATUS_RX_P_NO     1
#define STATUS_TX_FULL     0

#define FIFO_STATUS_TX_REUSE      6
#define FIFO_STATUS_TX_FULL       5
#define FIFO_STATUS_TX_FIFO_EMPTY 4
#define FIFO_STATUS_RX_FULL       1
#define FIFO_STATUS_RX_FIFO_EMPTY 0

#define FEATURE_EN_DPL        2
#define FEATURE_EN_ACK_PAY    1
#define FEATURE_EN_DYN_ACK    0

// Instruction Mnemonics
#define CMD_R_REGISTER      0x00
#define CMD_W_REGISTER      0x20
#define CMD_ACTIVATE        0x50
#define CMD_R_RX_PL_WID     0x60
#define CMD_R_RX_PAYLOAD    0x61
#define CMD_W_TX_PAYLOAD    0xA0
#define CMD_W_ACK_PAYLOAD   0xA8
#define CMD_W_TX_PAYLOAD_NO_ACK  0xB0
#define CMD_FLUSH_TX        0xE1
#define CMD_FLUSH_RX        0xE2
#define CMD_REUSE_TX_PL     0xE3
#define CMD_NOP             0xFF

#endif /* NRF24L01REGS_H_ */
