#ifndef DEBUG_UART_DEBUG_UART_H_
#define DEBUG_UART_DEBUG_UART_H_

#define DEBUG_UART_BUF_TX_SIZE 128
#define DEBUG_UART_BUF_TX_MASK (DEBUG_UART_BUF_TX_SIZE-1)

#define DEBUG_UART_BUF_RX_SIZE 128
#define DEBUG_UART_BUF_RX_MASK (DEBUG_UART_BUF_RX_SIZE-1)

void debug_uart_init();
void debug_uart_putc(char c);
void debug_uart_puts(char * str);
void DEBUG_UART_EVENT(char * buf);
void register_debug_uart_event_callback(void (*callback)(char * buf));

#endif /* DEBUG_UART_DEBUG_UART_H_ */
