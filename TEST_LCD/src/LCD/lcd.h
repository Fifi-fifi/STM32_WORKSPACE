#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#define LCD_SEG_1A      1, 22
#define LCD_SEG_1B      0, 22
#define LCD_SEG_1C      1, 1
#define LCD_SEG_1D      1, 0
#define LCD_SEG_1E      0, 0
#define LCD_SEG_1F      1, 23
#define LCD_SEG_1G      0, 23
#define LCD_SEG_1H      3, 23
#define LCD_SEG_1J      3, 22
#define LCD_SEG_1K      2, 22
#define LCD_SEG_1M      0, 1
#define LCD_SEG_1N      3, 0
#define LCD_SEG_1P      2, 0
#define LCD_SEG_1Q      2, 23
#define LCD_SEG_1DP     3, 1
#define LCD_SEG_1COLON  2, 1

#define LCD_SEG_2A      1, 20
#define LCD_SEG_2B      0, 20
#define LCD_SEG_2C      1, 3
#define LCD_SEG_2D      1, 2
#define LCD_SEG_2E      0, 2
#define LCD_SEG_2F      1, 21
#define LCD_SEG_2G      0, 21
#define LCD_SEG_2H      3, 21
#define LCD_SEG_2J      3, 20
#define LCD_SEG_2K      2, 20
#define LCD_SEG_2M      0, 3
#define LCD_SEG_2N      3, 2
#define LCD_SEG_2P      2, 2
#define LCD_SEG_2Q      2, 21
#define LCD_SEG_2DP     3, 3
#define LCD_SEG_2COLON  2, 3

#define LCD_SEG_3A      1, 18
#define LCD_SEG_3B      0, 18
#define LCD_SEG_3C      1, 5
#define LCD_SEG_3D      1, 4
#define LCD_SEG_3E      0, 4
#define LCD_SEG_3F      1, 19
#define LCD_SEG_3G      0, 19
#define LCD_SEG_3H      3, 19
#define LCD_SEG_3J      3, 18
#define LCD_SEG_3K      2, 18
#define LCD_SEG_3M      0, 5
#define LCD_SEG_3N      3, 4
#define LCD_SEG_3P      2, 4
#define LCD_SEG_3Q      2, 19
#define LCD_SEG_3DP     3, 5
#define LCD_SEG_3COLON  2, 5

#define LCD_SEG_4A      1, 16
#define LCD_SEG_4B      0, 16
#define LCD_SEG_4C      1, 7
#define LCD_SEG_4D      1, 6
#define LCD_SEG_4E      0, 6
#define LCD_SEG_4F      1, 17
#define LCD_SEG_4G      0, 17
#define LCD_SEG_4H      3, 17
#define LCD_SEG_4J      3, 16
#define LCD_SEG_4K      2, 16
#define LCD_SEG_4M      0, 7
#define LCD_SEG_4N      3, 6
#define LCD_SEG_4P      2, 6
#define LCD_SEG_4Q      2, 17
#define LCD_SEG_4DP     3, 7
#define LCD_SEG_4COLON  2, 7

#define LCD_SEG_5A      1, 14
#define LCD_SEG_5B      0, 14
#define LCD_SEG_5C      1, 9
#define LCD_SEG_5D      1, 8
#define LCD_SEG_5E      0, 8
#define LCD_SEG_5F      1, 15
#define LCD_SEG_5G      0, 15
#define LCD_SEG_5H      3, 15
#define LCD_SEG_5J      3, 14
#define LCD_SEG_5K      2, 14
#define LCD_SEG_5M      0, 9
#define LCD_SEG_5N      3, 8
#define LCD_SEG_5P      2, 8
#define LCD_SEG_5Q      2, 15
#define LCD_SEG_5DP     3, 9   // BAR2
#define LCD_SEG_5COLON  2, 9   // BAR3

#define LCD_SEG_6A      1, 12
#define LCD_SEG_6B      0, 12
#define LCD_SEG_6C      1, 11
#define LCD_SEG_6D      1, 10
#define LCD_SEG_6E      0, 10
#define LCD_SEG_6F      1, 13
#define LCD_SEG_6G      0, 13
#define LCD_SEG_6H      3, 13
#define LCD_SEG_6J      3, 12
#define LCD_SEG_6K      2, 12
#define LCD_SEG_6M      0, 11
#define LCD_SEG_6N      3, 10
#define LCD_SEG_6P      2, 10
#define LCD_SEG_6Q      2, 13
#define LCD_SEG_6DP     3, 11  // BAR0
#define LCD_SEG_6COLON  2, 11  // BAR1

// now to glass-SEG-to-STM_SEG mapping
// COM1-COM4 on glass map straigh to COM1-COM4 on STM, so we don't need to remap those

#define LCD_GLASS_SEG23  29
#define LCD_GLASS_SEG0   0
#define LCD_GLASS_SEG10  14
#define LCD_GLASS_SEG21  27
#define LCD_GLASS_SEG2   2
#define LCD_GLASS_SEG11  15
#define LCD_GLASS_SEG12  17
#define LCD_GLASS_SEG20  26
#define LCD_GLASS_SEG3   7
#define LCD_GLASS_SEG19  25
#define LCD_GLASS_SEG4   8
#define LCD_GLASS_SEG22  28//4
#define LCD_GLASS_SEG1   1
#define LCD_GLASS_SEG14  18
#define LCD_GLASS_SEG9   13
#define LCD_GLASS_SEG13  16
#define LCD_GLASS_SEG18  24
#define LCD_GLASS_SEG5   9
#define LCD_GLASS_SEG17  21
#define LCD_GLASS_SEG6   10
#define LCD_GLASS_SEG16  20
#define LCD_GLASS_SEG7   11
#define LCD_GLASS_SEG15  19
#define LCD_GLASS_SEG8   12

#define PERIPH_BB(a, bit) (*(volatile uint32_t *)(((((uint32_t)&(a))-PERIPH_BASE) * 32) + (bit * 4) + PERIPH_BB_BASE))
#define LCD_GET_BB_ADDR(com, seg)  PERIPH_BB(*((uint8_t *)&LCD->RAM[0] + 8 * com + seg / 8), (seg) % 8)
#define LCD_GET_BB_ADDR1(com, seg) LCD_GET_BB_ADDR(com, GLUE(LCD_GLASS_SEG, seg))
#define LCD_GET_BB_ADDR2(...)      LCD_GET_BB_ADDR1(__VA_ARGS__)
#define LCD_SET_SEG(segment, val)  do { LCD_GET_BB_ADDR2(GLUE(LCD_SEG_, segment)) = val; } while(0)

#define LCD_GET_SEG_NR(com, seg) GLUE(LCD_GLASS_SEG, seg)
#define LCD_GET_SEG_NR2(...) LCD_GET_SEG_NR(__VA_ARGS__)
#define LCD_S_(segment) LCD_GET_SEG_NR2(GLUE(LCD_SEG_, segment))

#define LCD_GET_COM_NR(com, seg) com
#define LCD_GET_COM_NR2(...) LCD_GET_COM_NR(__VA_ARGS__)
#define LCD_C_(segment) LCD_GET_COM_NR2(GLUE(LCD_SEG_, segment))

#define LCD_DIGIT_NR  6
#define LCD_SEG_NR    (14 + 2)

#define LCD_SEG(digit, segment)  LCD_GET_BB_ADDR(lcdSeg[digit][segment].com, lcdSeg[digit][segment].seg)


typedef struct __attribute__((packed)) {
  uint8_t com;
  uint8_t seg;
} TLcdSeg;

enum Lcd_dots {no_dots = 0, dot=1, colon=2, dot_colon = 3};
typedef enum Lcd_dots T_Lcd_dots;

extern CODE TLcdSeg lcdSeg[LCD_DIGIT_NR][LCD_SEG_NR];
extern CODE uint16_t lcdAlphaTab[26];
extern CODE uint16_t lcdDigitTab[10];

void LcdIoInit();									//initializes i/os for LCD

void LcdPrintChar(uint8_t digit, char c);			//prints single character on LCD
void LcdPrintString( uint8_t start, char * str);	//prints whole string on LCD
void LcdPrintDot( uint8_t digit, T_Lcd_dots dots );	//prints decimal dot or colon using enum
void LcdPrintBar( uint8_t val );					//print binary value (0-15) to bar on LCD

void LcdUpdate();									//updates text on LCD (swap buffers)

#endif /* LCD_LCD_H_ */
