/********************************** (C) COPYRIGHT  *******************************
 * File Name          : tm1638.h
 * Author             : Kerd
 * Version            : V1.0.0
 * Date               : 2026.06.10
 * Description        : Library for TM1638 8 segments 8 gigit 8 keys display module
 *********************************************************************************/

#ifndef INC_TM1638_H_
#define INC_TM1638_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TM1638_DELAY    0    // number of iterations for short protocol delay
#define TM1638_DISPLAY_LEN 8  // length of display (number of 7 segments indicators)

/* Modify GPIOs here */
#define TM1638_STB_PORT GPIOC
#define TM1638_STB_PIN  3
#define TM1638_CLK_PORT GPIOC
#define TM1638_CLK_PIN  4
#define TM1638_DIO_PORT GPIOC
#define TM1638_DIO_PIN  6

#define TM1638_PRINT_DOT            0b00000001  // print dot at most right position
#define TM1638_PRINT_LEADING_ZERO   0b00000010  // print leading zeros

/*********************************************************************
 * @fn      tm1638_init
 *
 * @brief   Initialize tm1638 and gpio's. GPIO peripheral clock must be
 *          enabled by the caller.
 *
 * @return  none
 */
void tm1638_init();

/*********************************************************************
 * @fn      tm1638_clear
 *
 * @brief   Clear display memory
 *
 * @return  none
 */
void tm1638_clear();

/*********************************************************************
 * @fn      tm1638_setLed
 *
 * @brief   Set led ON/OFF
 *
 * @param index - led index (0-7)
 * @param state - led state (0 - OFF, 1 - ON)
 *
 * @return  none
 */
void tm1638_setLed(uint8_t index, uint8_t state);

/*********************************************************************
 * @fn      tm1638_displayOn
 *
 * @brief   Turn display On
 *
 * @return  none
 */
void tm1638_displayOn();

/*********************************************************************
 * @fn      tm1638_displayOff
 *
 * @brief   Turn display Off
 *
 * @return  none
 */
void tm1638_displayOff();

/*********************************************************************
 * @fn      tm1638_displayBrightness
 *
 * @brief   Set display brightness
 *
 * @param brightness - display brightness (0-7)
 *
 * @return  none
 */
void tm1638_displayBrightness(uint8_t brightness);

/*********************************************************************
 * @fn      tm1638_displayMode
 *
 * @brief   Set display brightness and on/off state
 *
 * @param _new_mode - tm1638 mode
 *
 * @return  none
 */
void tm1638_displayMode(uint8_t _new_mode);

/*********************************************************************
 * @fn      tm1638_printGlyph
 *
 * @brief   Set indicator segments at specified position
 *
 * @param pos - indicator position
 * @param data - indicator segments
 * @param dot - enable dot segment at specified position
 *
 * @return  none
 */
void tm1638_printGlyph(uint8_t pos, uint8_t data, uint8_t dot);

/*********************************************************************
 * @fn      tm1638_printDigit
 *
 * @brief   Print digit at specified position
 *
 * @param pos - indicator position
 * @param digit - digit to print (0-15)
 * @param dot - enable dot segment at specified position
 *
 * @return  none
 */
void tm1638_printDigit(uint8_t pos, uint8_t digit, uint8_t dot);

/*********************************************************************
 * @fn      tm1638_printChar
 *
 * @brief   Print character at specified position
 *
 * @param pos - indicator position
 * @param ch - character to print (starting from ' ')
 * @param dot - enable dot segment at specified position
 *
 * @return  none
 */
void tm1638_printChar(uint8_t pos, char ch, uint8_t dot);

/*********************************************************************
 * @fn      tm1638_print
 *
 * @brief   Print string at specified position
 *
 * @param pos - start position
 * @param str - string to print
 *
 * @return  none
 */
void tm1638_print(uint8_t pos, const char* str);

/*********************************************************************
 * @fn      tm1638_printHex
 *
 * @brief   Print hexadecimal number at specified position
 *
 * @param pos - start position
 * @param val - value to print
 * @param len - number of digits to print
 * @param flags - TM1638_PRINT_* flags
 *
 * @return  none
 */
void tm1638_printHex(uint8_t pos, uint32_t val, uint8_t len, uint8_t flags);

/*********************************************************************
 * @fn      tm1638_printDec
 *
 * @brief   Print decimal number at specified position
 *
 * @param pos - start position
 * @param val - value to print
 * @param len - number of digits to print
 * @param flags - TM1638_PRINT_* flags
 *
 * @return  none
 */
void tm1638_printDec(uint8_t pos, uint32_t val, uint8_t len, uint8_t flags);

/*********************************************************************
 * @fn      tm1638_getButtons
 *
 * @brief   get buttons values as bit set
 *
 * @return  buttons on/off bit set
 */
uint8_t tm1638_getButtons(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_TM1638_H_ */
