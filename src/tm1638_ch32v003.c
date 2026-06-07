/********************************** (C) COPYRIGHT  *******************************
 * File Name          : tm1638_ch32v003.c
 * Author             : Kerd
 * Version            : V1.0.0
 * Date               : 2026.06.10
 * Description        : Library for TM1638 8 segments 8 gigit 8 keys display module
 *********************************************************************************/

#include <ch32v00x.h>
#include <ch32v00x_rcc.h>
#include <tm1638.h>

#define INSTRUCTION_WRITE_DATA 0x40
#define INSTRUCTION_READ_KEY 0x42
#define INSTRUCTION_ADDRESS_AUTO 0x40
#define INSTRUCTION_ADDRESS_FIXED 0x44
#define INSTRUCTION_NORMAL_MODE 0x40
#define INSTRUCTION_TEST_MODE 0x48

#define DISPLAY_START_ADDRESS 0xC0

#define DISPLAY_TURN_OFF 0x80
#define DISPLAY_TURN_ON 0x88

#define STB_L() TM1638_STB_PORT->BCR  = (1 << TM1638_STB_PIN)
#define STB_H() TM1638_STB_PORT->BSHR = (1 << TM1638_STB_PIN)
#define CLK_L() TM1638_CLK_PORT->BCR  = (1 << TM1638_CLK_PIN)
#define CLK_H() TM1638_CLK_PORT->BSHR = (1 << TM1638_CLK_PIN)
#define DIO_L() TM1638_DIO_PORT->BCR  = (1 << TM1638_DIO_PIN)
#define DIO_H() TM1638_DIO_PORT->BSHR = (1 << TM1638_DIO_PIN)
#define DIO_R() (TM1638_DIO_PORT->INDR & (1 << TM1638_DIO_PIN))

#ifndef GPIO_CNF_IN_FLOATING
    #define GPIO_CNF_IN_FLOATING 4
    #define GPIO_CNF_OUT_PP      0
    #define GPIO_CNF_OUT_OD      4
#endif

/*
    Segments mapping

     -- 0 --
    |       |
    5       1
     -- 6 --
    4       2
    |       |
     -- 3 --  .7
*/

/* 
   Fonts Copyright (C) 2011 Ricardo Batista (rjbatista <at> gmail <dot> com)
   from tm1638 arduino library.
*/
// definition for standard hexadecimal numbers
const uint8_t NUMBER_FONT[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001  // F
};

// definition for the displayable ASCII chars
const uint8_t FONT_DEFAULT[] = {
  0b00000000, // (32)  <space>
  0b10000110, // (33)	!
  0b00100010, // (34)	"
  0b01111110, // (35)	#
  0b01101101, // (36)	$
  0b00000000, // (37)	%
  0b00000000, // (38)	&
  0b00000010, // (39)	'
  0b00110000, // (40)	(
  0b00000110, // (41)	)
  0b01100011, // (42)	*
  0b00000000, // (43)	+
  0b00000100, // (44)	,
  0b01000000, // (45)	-
  0b10000000, // (46)	.
  0b01010010, // (47)	/
  0b00111111, // (48)	0
  0b00000110, // (49)	1
  0b01011011, // (50)	2
  0b01001111, // (51)	3
  0b01100110, // (52)	4
  0b01101101, // (53)	5
  0b01111101, // (54)	6
  0b00100111, // (55)	7
  0b01111111, // (56)	8
  0b01101111, // (57)	9
  0b00000000, // (58)	:
  0b00000000, // (59)	;
  0b00000000, // (60)	<
  0b01001000, // (61)	=
  0b00000000, // (62)	>
  0b01010011, // (63)	?
  0b01011111, // (64)	@
  0b01110111, // (65)	A
  0b01111111, // (66)	B
  0b00111001, // (67)	C
  0b00111111, // (68)	D
  0b01111001, // (69)	E
  0b01110001, // (70)	F
  0b00111101, // (71)	G
  0b01110110, // (72)	H
  0b00000110, // (73)	I
  0b00011111, // (74)	J
  0b01101001, // (75)	K
  0b00111000, // (76)	L
  0b00010101, // (77)	M
  0b00110111, // (78)	N
  0b00111111, // (79)	O
  0b01110011, // (80)	P
  0b01100111, // (81)	Q
  0b00110001, // (82)	R
  0b01101101, // (83)	S
  0b01111000, // (84)	T
  0b00111110, // (85)	U
  0b00101010, // (86)	V
  0b00011101, // (87)	W
  0b01110110, // (88)	X
  0b01101110, // (89)	Y
  0b01011011, // (90)	Z
  0b00111001, // (91)	[
  0b01100100, // (92)	\ (this can't be the last char on a line, even in comment or it'll concat)
  0b00001111, // (93)	]
  0b00000000, // (94)	^
  0b00001000, // (95)	_
  0b00100000, // (96)	`
  0b01011111, // (97)	a
  0b01111100, // (98)	b
  0b01011000, // (99)	c
  0b01011110, // (100)	d
  0b01111011, // (101)	e
  0b00110001, // (102)	f
  0b01101111, // (103)	g
  0b01110100, // (104)	h
  0b00000100, // (105)	i
  0b00001110, // (106)	j
  0b01110101, // (107)	k
  0b00110000, // (108)	l
  0b01010101, // (109)	m
  0b01010100, // (110)	n
  0b01011100, // (111)	o
  0b01110011, // (112)	p
  0b01100111, // (113)	q
  0b01010000, // (114)	r
  0b01101101, // (115)	s
  0b01111000, // (116)	t
  0b00011100, // (117)	u
  0b00101010, // (118)	v
  0b00011101, // (119)	w
  0b01110110, // (120)	x
  0b01101110, // (121)	y
  0b01000111, // (122)	z
  0b01000110, // (123)	{
  0b00000110, // (124)	|
  0b01110000, // (125)	}
  0b00000001, // (126)	~
};

uint8_t _display_state = 0;

static
void _delay(void) {
    for(volatile uint8_t i=0; i<TM1638_DELAY; i++);
}

static
void _set_dio_write(uint8_t write) {
    uint8_t mode = write ? (GPIO_Speed_50MHz | GPIO_CNF_OUT_OD) : GPIO_CNF_IN_FLOATING;
    TM1638_DIO_PORT->CFGLR = (TM1638_DIO_PORT->CFGLR & (~(0xf<<(4*TM1638_DIO_PIN)))) | (mode<<(4*TM1638_DIO_PIN));
}

static void _send(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        CLK_L(); _delay();
        if (data & 1) { DIO_H(); } else { DIO_L(); }; _delay();
        CLK_H(); _delay();
        data >>= 1;
    }
}

static void _send_cmd(uint8_t cmd) {
    STB_L();
    _send(cmd);
    STB_H();
}

static void _send_data(uint8_t address, uint8_t data) {
    _send_cmd(INSTRUCTION_ADDRESS_FIXED);
    STB_L();
    _send(DISPLAY_START_ADDRESS | address);
    _send(data);
    STB_H();
}

static uint8_t _read() {
    uint8_t data = 0;
    _set_dio_write(0);
    for (uint8_t i = 0; i < 8; i++) {
        CLK_L(); _delay();
        data >>= 1;
        if (DIO_R()) data |= 0x80;
        CLK_H(); _delay();
    }
    _set_dio_write(1);
    return data;
}

void tm1638_init()
{
    uint8_t mode = GPIO_Speed_50MHz | GPIO_CNF_OUT_OD;
    TM1638_STB_PORT->CFGLR = (TM1638_STB_PORT->CFGLR & (~(0xf<<(4*TM1638_STB_PIN)))) | (mode<<(4*TM1638_STB_PIN));
    TM1638_CLK_PORT->CFGLR = (TM1638_CLK_PORT->CFGLR & (~(0xf<<(4*TM1638_CLK_PIN)))) | (mode<<(4*TM1638_CLK_PIN));
    TM1638_DIO_PORT->CFGLR = (TM1638_DIO_PORT->CFGLR & (~(0xf<<(4*TM1638_DIO_PIN)))) | (mode<<(4*TM1638_DIO_PIN));
    STB_H(); CLK_H(); DIO_H();
    _send_cmd(0x8F);
    tm1638_clear();
}

void tm1638_clear() {
    _send_cmd(0x40); // auto-increment
    STB_L();
    _send(0xC0); // addr 0
    for (uint8_t i = 0; i < 16; i++)
        _send(0x00);
    STB_H();
    tm1638_displayMode(DISPLAY_TURN_ON | 7);
}

void tm1638_displayOn() {
    tm1638_displayMode(DISPLAY_TURN_ON | (_display_state & 0x07));
}

void tm1638_displayOff() {
    tm1638_displayMode(DISPLAY_TURN_OFF | (_display_state & 0x07));
}

void tm1638_displayBrightness(uint8_t brightness) {
    tm1638_displayMode((_display_state & 0xF8) | (brightness & 0x07));
}

void tm1638_displayMode(uint8_t _new_mode) {
    _send_cmd((_display_state = _new_mode));
}

void tm1638_setLed(uint8_t index, uint8_t state) {
    if (index > 7) return;
    _send_data(1 + (index << 1), state ? 0xFF : 0x00);
}

void tm1638_printGlyph(uint8_t pos, uint8_t data, uint8_t dot) {
	_send_data(pos << 1, data | (dot ? 0b10000000 : 0));
}

void tm1638_printDigit(uint8_t pos, uint8_t digit, uint8_t dot) {
    _send_data(pos << 1, NUMBER_FONT[digit & 0x0F] | (dot ? 0b10000000 : 0));
}

void tm1638_printChar(uint8_t pos, char ch, uint8_t dot) {
    ch = (ch < ' ') ? 0 : ch - ' ';
    _send_data(pos << 1, FONT_DEFAULT[ch] | (dot ? 0b10000000 : 0));
}

void tm1638_print(uint8_t pos, const char* str) {
    for (uint8_t i = 0; (pos < 8) && str[i]; i++, pos++) {
        tm1638_printChar(pos, str[i], str[i+1] == '.');
        if ((str[i] != '.') && (str[i+1] == '.')) i++;
    }
}

void tm1638_printHex(uint8_t pos, uint32_t val, uint8_t len, uint8_t flags) {
    if (len == 0) return;
    if (pos + len > TM1638_DISPLAY_LEN) return;
    uint8_t dot = (flags & TM1638_PRINT_DOT) != 0;
    for (uint8_t i = pos + len; i > pos; ) {
        i--;
        if (val)
            tm1638_printDigit(i, val & 0xF, dot);
        else 
            tm1638_printGlyph(i, (flags & TM1638_PRINT_LEADING_ZERO) ? NUMBER_FONT[0] : 0, dot);
        dot = 0;
        val >>= 4;
    }
}

void tm1638_printDec(uint8_t pos, uint32_t val, uint8_t len, uint8_t flags) {
    if (len == 0) return;
    if (pos + len > TM1638_DISPLAY_LEN) return;
    uint8_t dot = (flags & TM1638_PRINT_DOT) != 0;
    for (uint8_t i = pos + len; i > pos; ) {
        i--;
        if (val)
            tm1638_printDigit(i, val % 10, dot);
        else 
            tm1638_printGlyph(i, (flags & TM1638_PRINT_LEADING_ZERO) ? NUMBER_FONT[0] : 0, dot);
        dot = 0;
        val /= 10;
    }
}

uint8_t tm1638_getButtons(void) {
    uint8_t buttons = 0;
	uint8_t i=0;
    STB_L();
    _send(INSTRUCTION_READ_KEY);
    for (i=0; i<4; i++) {
        buttons |= _read() << i;
    }
    STB_H();
    return buttons;
}
