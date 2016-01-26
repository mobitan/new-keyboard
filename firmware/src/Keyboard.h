/*
 * Copyright 2013-2015 Esrille Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// cf. http://www.usb.org/developers/hidpage/Hut1_12v2.pdf pp.53-60

                                        // 109.nodoka
#define KEY_ERRORROLLOVER       0x01
#define KEY_POSTFAIL            0x02
#define KEY_ERRORUNDEFINED      0x03
#define KEY_A                   0x04    // 0x1e   A
#define KEY_B                   0x05    // 0x30   B
#define KEY_C                   0x06    // 0x2e   C
#define KEY_D                   0x07    // 0x20   D
#define KEY_E                   0x08    // 0x12   E
#define KEY_F                   0x09    // 0x21   F
#define KEY_G                   0x0A    // 0x22   G
#define KEY_H                   0x0B    // 0x23   H
#define KEY_I                   0x0C    // 0x17   I
#define KEY_J                   0x0D    // 0x24   J
#define KEY_K                   0x0E    // 0x25   K
#define KEY_L                   0x0F    // 0x26   L
#define KEY_M                   0x10    // 0x32   M
#define KEY_N                   0x11    // 0x31   N
#define KEY_O                   0x12    // 0x18   O
#define KEY_P                   0x13    // 0x19   P
#define KEY_Q                   0x14    // 0x10   Q
#define KEY_R                   0x15    // 0x13   R
#define KEY_S                   0x16    // 0x1f   S
#define KEY_T                   0x17    // 0x14   T
#define KEY_U                   0x18    // 0x16   U
#define KEY_V                   0x19    // 0x2f   V
#define KEY_W                   0x1A    // 0x11   W
#define KEY_X                   0x1B    // 0x2d   X
#define KEY_Y                   0x1C    // 0x15   Y
#define KEY_Z                   0x1D    // 0x2c   Z
#define KEY_1                   0x1E    // 0x02   _1
#define KEY_2                   0x1F    // 0x03   _2
#define KEY_3                   0x20    // 0x04   _3
#define KEY_4                   0x21    // 0x05   _4
#define KEY_5                   0x22    // 0x06   _5
#define KEY_6                   0x23    // 0x07   _6
#define KEY_7                   0x24    // 0x08   _7
#define KEY_8                   0x25    // 0x09   _8
#define KEY_9                   0x26    // 0x0a   _9
#define KEY_0                   0x27    // 0x0b   _0
#define KEY_ENTER               0x28    // 0x1c   Enter
#define KEY_ESCAPE              0x29    // 0x01   Esc
#define KEY_BACKSPACE           0x2A    // 0x0e   BackSpace
#define KEY_TAB                 0x2B    // 0x0f   Tab
#define KEY_SPACEBAR            0x2C    // 0x39   Space
#define KEY_MINUS               0x2D    // 0x0c   HyphenMinus
#define KEY_EQUAL               0x2E    // 0x0d   CircumflexAccent
#define KEY_LEFT_BRACKET        0x2F    // 0x1a   CommercialAt
#define KEY_RIGHT_BRACKET       0x30    // 0x1b   LeftSquareBracket
#define KEY_BACKSLASH           0x31    // 0x2b   RightSquareBracket
#define KEY_NON_US_HASH         0x32    // 0x2b   RightSquareBracket
#define KEY_SEMICOLON           0x33    // 0x27   Semicolon
#define KEY_QUOTE               0x34    // 0x28   Colon
#define KEY_GRAVE_ACCENT        0x35    // 0x29   Kanji
#define KEY_COMMA               0x36    // 0x33   Comma
#define KEY_PERIOD              0x37    // 0x34   FullStop
#define KEY_SLASH               0x38    // 0x35   Solidus
#define KEY_CAPS_LOCK           0x39    // 0x3a   Eisuu
#define KEY_F1                  0x3A    // 0x3b   F1
#define KEY_F2                  0x3B    // 0x3c   F2
#define KEY_F3                  0x3C    // 0x3d   F3
#define KEY_F4                  0x3D    // 0x3e   F4
#define KEY_F5                  0x3E    // 0x3f   F5
#define KEY_F6                  0x3F    // 0x40   F6
#define KEY_F7                  0x40    // 0x41   F7
#define KEY_F8                  0x41    // 0x42   F8
#define KEY_F9                  0x42    // 0x43   F9
#define KEY_F10                 0x43    // 0x44   F10
#define KEY_F11                 0x44    // 0x57   F11
#define KEY_F12                 0x45    // 0x58   F12
#define KEY_PRINTSCREEN         0x46    // E0-E0E1-0x37   PrintScreen
#define KEY_SCROLL_LOCK         0x47    // 0x46   ScrollLock
#define KEY_PAUSE               0x48    // E1-E0E1-0x1d 0x45   Pause
#define KEY_INSERT              0x49    // E0-E0E1-0x52   Insert
#define KEY_HOME                0x4A    // E0-E0E1-0x47   Home
#define KEY_PAGEUP              0x4B    // E0-E0E1-0x49   PageUp
#define KEY_DELETE              0x4C    // E0-E0E1-0x53   Delete
#define KEY_END                 0x4D    // E0-E0E1-0x4f   End
#define KEY_PAGEDOWN            0x4E    // E0-E0E1-0x51   PageDown
#define KEY_RIGHTARROW          0x4F    // E0-E0E1-0x4d   Right
#define KEY_LEFTARROW           0x50    // E0-E0E1-0x4b   Left
#define KEY_DOWNARROW           0x51    // E0-E0E1-0x50   Down
#define KEY_UPARROW             0x52    // E0-E0E1-0x48   Up
#define KEYPAD_NUM_LOCK         0x53    // 0x45   NumLock
#define KEYPAD_DIVIDE           0x54    // E0-E0E1-0x35   NumSolidus
#define KEYPAD_MULTIPLY         0x55    // 0x37   NumAsterisk
#define KEYPAD_SUBTRACT         0x56    // 0x4a   NumHyphenMinus
#define KEYPAD_ADD              0x57    // 0x4e   NumPlusSign
#define KEYPAD_ENTER            0x58    // 0x1c   Enter
#define KEYPAD_1                0x59    // 0x4f   Num1
#define KEYPAD_2                0x5A    // 0x50   Num2
#define KEYPAD_3                0x5B    // 0x51   Num3
#define KEYPAD_4                0x5C    // 0x4b   Num4
#define KEYPAD_5                0x5D    // 0x4c   Num5
#define KEYPAD_6                0x5E    // 0x4d   Num6
#define KEYPAD_7                0x5F    // 0x47   Num7
#define KEYPAD_8                0x60    // 0x48   Num8
#define KEYPAD_9                0x61    // 0x49   Num9
#define KEYPAD_0                0x62    // 0x52   Num0
#define KEYPAD_DOT              0x63    // 0x53   NumFullStop
#define KEY_NON_US_BACKSLASH    0x64    // 0x56
#define KEY_APPLICATION         0x65    // E0-E0E1-0x5d   Applications
#define KEY_POWER               0x66    // E0-E0E1-0x5e   PowerOff
#define KEYPAD_EQUAL            0x67    // 0x59
#define KEY_F13                 0x68    // 0x64
#define KEY_F14                 0x69    // 0x65
#define KEY_F15                 0x6A    // 0x66
#define KEY_F16                 0x6B    // 0x67
#define KEY_F17                 0x6C    // 0x68
#define KEY_F18                 0x6D    // 0x69
#define KEY_F19                 0x6E    // 0x6a   PenButton2
#define KEY_F20                 0x6F    // 0x6b   PenButton
#define KEY_F21                 0x70    // 0x6c
#define KEY_F22                 0x71    // 0x6d
#define KEY_F23                 0x72    // 0x6e
#define KEY_F24                 0x73    // 0x76
#define KEY_EXECUTE             0x74    // (ignored)
#define KEY_HELP                0x75    // (ignored)
#define KEY_MENU                0x76    // (ignored)
#define KEY_SELECT              0x77    // (ignored)
#define KEY_STOP                0x78    // (ignored)
#define KEY_AGAIN               0x79    // (ignored)
#define KEY_UNDO                0x7A    // (ignored)
#define KEY_CUT                 0x7B    // (ignored)
#define KEY_COPY                0x7C    // (ignored)
#define KEY_PASTE               0x7D    // (ignored)
#define KEY_FIND                0x7E    // (ignored)
#define KEY_MUTE                0x7F    // (ignored)
#define KEY_VOLUME_UP           0x80    // (ignored)
#define KEY_VOLUME_DOWN         0x81    // (ignored)
#define KEY_LOCKING_CAPS_LOCK   0x82
#define KEY_LOCKING_NUM_LOCK    0x83
#define KEY_LOCKING_SCROLL_LOCK 0x84
#define KEYPAD_COMMA            0x85    // 0x7e
#define KEYPAD_EQUAL_SIGN       0x86    // (ignored)
#define KEY_INTERNATIONAL1      0x87    // 0x73   ReverseSolidus
#define KEY_INTERNATIONAL2      0x88    // 0x70   Hiragana
#define KEY_INTERNATIONAL3      0x89    // 0x7d   YenSign
#define KEY_INTERNATIONAL4      0x8A    // 0x79   Convert
#define KEY_INTERNATIONAL5      0x8B    // 0x7b   NonConvert
#define KEY_INTERNATIONAL6      0x8C    // 0x5c
#define KEY_INTERNATIONAL7      0x8D    // (ignored)
#define KEY_INTERNATIONAL8      0x8E    // (ignored)
#define KEY_INTERNATIONAL9      0x8F    // (ignored)
#define KEY_LANG1               0x90    // (used internally)
#define KEY_LANG2               0x91    // (used internally)
#define KEY_LANG3               0x92    // 0x78
#define KEY_LANG4               0x93    // 0x77
#define KEY_LANG5               0x94    // 0x76
#define KEY_LANG6               0x95    // (ignored)
#define KEY_LANG7               0x96    // (ignored)
#define KEY_LANG8               0x97    // (ignored)
#define KEY_LANG9               0x98    // (ignored)
#define KEY_ALTERNATE_ERASE     0x99    // (ignored)
#define KEY_SYSREQ_ATTENTION    0x9A    // (ignored)
#define KEY_CANCEL              0x9B    // (ignored)
#define KEY_CLEAR               0x9C    // (ignored)
#define KEY_PRIOR               0x9D    // (ignored)
#define KEY_RETURN              0x9E    // (ignored)
#define KEY_SEPARATOR           0x9F    // (ignored)
#define KEY_OUT                 0xA0    // (ignored)
#define KEY_OPER                0xA1    // (ignored)
#define KEY_CLEAR_AGAIN         0xA2    // (ignored)
#define KEY_CRSEL_PROPS         0xA3    // (ignored)
#define KEY_EXSEL               0xA4    // (ignored)
#define KEYPAD_00               0xB0    // (ignored)
#define KEYPAD_000              0xB1    // (ignored)
#define THOUSANDS_SEPARATOR     0xB2    // (ignored)
#define DECIMAL_SEPARATOR       0xB3    // (ignored)
#define CURRENCY_UNIT           0xB4    // (ignored)
#define CURRENCY_SUB_UNIT       0xB5    // (ignored)
#define KEYPAD_LEFT_PAREN       0xB6    // (ignored)
#define KEYPAD_RIGHT_PAREN      0xB7    // (ignored)
#define KEYPAD_LEFT_BRACE       0xB8    // (ignored)
#define KEYPAD_RIGHT_BRACE      0xB9    // (ignored)
#define KEYPAD_TAB              0xBA    // (ignored)
#define KEYPAD_BACKSPACE        0xBB    // (ignored)
#define KEYPAD_A                0xBC    // (ignored)
#define KEYPAD_B                0xBD    // (ignored)
#define KEYPAD_C                0xBE    // (ignored)
#define KEYPAD_D                0xBF    // (ignored)
#define KEYPAD_E                0xC0    // (ignored)
#define KEYPAD_F                0xC1    // (ignored)
#define KEYPAD_XOR              0xC2    // (ignored)
#define KEYPAD_HAT              0xC3    // (ignored)
#define KEYPAD_PERCENT          0xC4    // (ignored)
#define KEYPAD_LESS             0xC5    // (ignored)
#define KEYPAD_MORE             0xC6    // (ignored)
#define KEYPAD_AND              0xC7    // (ignored)
#define KEYPAD_LOGICAL_AND      0xC8    // (ignored)
#define KEYPAD_OR               0xC9    // (ignored)
#define KEYPAD_LOGICAL_OR       0xCA    // (ignored)
#define KEYPAD_COLON            0xCB    // (ignored)
#define KEYPAD_HASH             0xCC    // (ignored)
#define KEYPAD_SPACE            0xCD    // (ignored)
#define KEYPAD_ATMARK           0xCE    // (ignored)
#define KEYPAD_NOT              0xCF    // (ignored)
#define KEYPAD_MEMORY_STORE     0xD0    // (ignored)
#define KEYPAD_MEMORY_RECALL    0xD1    // (ignored)
#define KEYPAD_MEMORY_CLEAR     0xD2    // (ignored)
#define KEYPAD_MEMORY_ADD       0xD3    // (ignored)
#define KEYPAD_MEMORY_SUBTRACT  0xD4    // (ignored)
#define KEYPAD_MEMORY_MULTIPLY  0xD5    // (ignored)
#define KEYPAD_MEMORY_DIVIDE    0xD6    // (ignored)
#define KEYPAD_SIGN             0xD7    // (ignored)
#define KEYPAD_CLEAR            0xD8    // (ignored)
#define KEYPAD_CLEAR_ENTRY      0xD9    // (ignored)
#define KEYPAD_BINARY           0xDA    // (ignored)
#define KEYPAD_OCTAL            0xDB    // (ignored)
#define KEYPAD_DECIMAL          0xDC    // (ignored)
#define KEYPAD_HEXADECIMAL      0xDD    // (ignored)
#define KEY_LEFTCONTROL         0xE0    // 0x1d   LeftControl
#define KEY_LEFTSHIFT           0xE1    // 0x2a   LeftShift
#define KEY_LEFTALT             0xE2    // 0x38   LeftAlt
#define KEY_LEFT_GUI            0xE3    // E0-E0E1-0x5b   LeftWindows
#define KEY_RIGHTCONTROL        0xE4    // E0-E0E1-0x1d   RightControl
#define KEY_RIGHTSHIFT          0xE5    // 0x36   RightShift
#define KEY_RIGHTALT            0xE6    // E0-E0E1-0x38   RightAlt
#define KEY_RIGHT_GUI           0xE7    // E0-E0E1-0x5c   RightWindows

#define KEY_CALC        	0xFB

#define MOD_LEFTALT         (1u << (KEY_LEFTALT - KEY_LEFTCONTROL))
#define MOD_RIGHTALT        (1u << (KEY_RIGHTALT - KEY_LEFTCONTROL))
#define MOD_ALT             (MOD_LEFTALT | MOD_RIGHTALT)

#define MOD_LEFTSHIFT       (1u << (KEY_LEFTSHIFT - KEY_LEFTCONTROL))
#define MOD_RIGHTSHIFT      (1u << (KEY_RIGHTSHIFT - KEY_LEFTCONTROL))
#define MOD_SHIFT           (MOD_LEFTSHIFT | MOD_RIGHTSHIFT)

#define MOD_LEFTCONTROL     (1u << (KEY_LEFTCONTROL - KEY_LEFTCONTROL))
#define MOD_RIGHTCONTROL    (1u << (KEY_RIGHTCONTROL - KEY_LEFTCONTROL))
#define MOD_CONTROL         (MOD_LEFTCONTROL | MOD_RIGHTCONTROL)

#define MOD_LEFTGUI         (1u << (KEY_LEFT_GUI - KEY_LEFTCONTROL))
#define MOD_RIGHTGUI        (1u << (KEY_RIGHT_GUI - KEY_LEFTCONTROL))
#define MOD_GUI             (MOD_LEFTGUI | MOD_RIGHTGUI)

#define LED_NUM_LOCK    0x01
#define LED_CAPS_LOCK   0x02
#define LED_SCROLL_LOCK 0x04

//
// Kana
//

#define ROMA_NONE	0

#define ROMA_A		1
#define ROMA_I		2
#define ROMA_U		3
#define ROMA_E		4
#define ROMA_O		5

#define ROMA_K		7
#define ROMA_KA		8
#define ROMA_KI		9
#define ROMA_KU		10
#define ROMA_KE		11
#define ROMA_KO		12
#define ROMA_KY		13

#define ROMA_S		14
#define ROMA_SA		15
#define ROMA_SI		16
#define ROMA_SU		17
#define ROMA_SE		18
#define ROMA_SO		19
#define ROMA_SY		20

#define ROMA_T		21
#define ROMA_TA		22
#define ROMA_TI		23
#define ROMA_TU		24
#define ROMA_TE		25
#define ROMA_TO		26
#define ROMA_TY		27

#define ROMA_N		28
#define ROMA_NA		29
#define ROMA_NI		30
#define ROMA_NU		31
#define ROMA_NE		32
#define ROMA_NO		33
#define ROMA_NY		34

#define ROMA_H		35
#define ROMA_HA		36
#define ROMA_HI		37
#define ROMA_HU		38
#define ROMA_HE		39
#define ROMA_HO		40
#define ROMA_HY		41

#define ROMA_M		42
#define ROMA_MA		43
#define ROMA_MI		44
#define ROMA_MU		45
#define ROMA_ME		46
#define ROMA_MO		47
#define ROMA_MY		48

#define ROMA_Y		49
#define ROMA_YA		50
#define ROMA_YU		52
#define ROMA_YO		54

#define ROMA_R		56
#define ROMA_RA		57
#define ROMA_RI		58
#define ROMA_RU		59
#define ROMA_RE		60
#define ROMA_RO		61
#define ROMA_RY		62

#define ROMA_W		63
#define ROMA_WA		64
#define ROMA_WO		68

#define ROMA_P		70
#define ROMA_PA		71
#define ROMA_PI		72
#define ROMA_PU		73
#define ROMA_PE		74
#define ROMA_PO		75
#define ROMA_PY		76

#define ROMA_G		77
#define ROMA_GA		78
#define ROMA_GI		79
#define ROMA_GU		80
#define ROMA_GE		81
#define ROMA_GO		82
#define ROMA_GY		83

#define ROMA_Z		84
#define ROMA_ZA		85
#define ROMA_ZI		86
#define ROMA_ZU		87
#define ROMA_ZE		88
#define ROMA_ZO		89
#define ROMA_ZY		90

#define ROMA_D		91
#define ROMA_DA		92
#define ROMA_DI		93
#define ROMA_DU		94
#define ROMA_DE		95
#define ROMA_DO		96
#define ROMA_DY		97

#define ROMA_B		98
#define ROMA_BA		99
#define ROMA_BI		100
#define ROMA_BU		101
#define ROMA_BE		102
#define ROMA_BO		103
#define ROMA_BY		104

#define ROMA_X		105
#define ROMA_XA		106
#define ROMA_XI		107
#define ROMA_XU		108
#define ROMA_XE		109
#define ROMA_XO		110

#define ROMA_XK		112
#define ROMA_XKA	113
#define ROMA_XKE	116

#define ROMA_XT		119
#define ROMA_XTU	122

#define ROMA_XY		126
#define ROMA_XYA	127
#define ROMA_XYU	129
#define ROMA_XYO	131

#define ROMA_XW		133
#define ROMA_XWA	134

#define ROMA_WY		140
#define ROMA_WYI	142
#define ROMA_WYE	144

#define ROMA_V		147
#define ROMA_VU		150

#define ROMA_L		154
#define ROMA_LA		155
#define ROMA_LI		156
#define ROMA_LU		157
#define ROMA_LE		158
#define ROMA_LO		159

// M-type
#define ROMA_ANN	161
#define ROMA_AKU	162
#define ROMA_ATU	163
#define ROMA_AI		164
#define ROMA_INN	165
#define ROMA_IKU	166
#define ROMA_ITU	167
#define ROMA_UNN	168
#define ROMA_UKU	169
#define ROMA_UTU	170
#define ROMA_ENN	171
#define ROMA_EKI	172
#define ROMA_ETU	173
#define ROMA_EI		174
#define ROMA_ONN	175
#define ROMA_OKU	176
#define ROMA_OTU	177
#define ROMA_OU		178
#define ROMA_C		179
#define ROMA_F		180
#define ROMA_J		181
#define ROMA_Q		182

// Common
#define ROMA_NN		200
#define ROMA_CHOUON	201
#define ROMA_DAKUTEN	202
#define ROMA_HANDAKU	203
#define ROMA_QUESTION	204
#define ROMA_TOUTEN	205     // 、
#define ROMA_KUTEN	206     // 。
#define ROMA_LAB	207	// <
#define ROMA_RAB	208	// >

// Stickney Next
#define KANA_DAKUTEN    209
#define KANA_HANDAKU    210
#define KANA_LCB        211
#define KANA_RCB        212
#define KANA_KE         213     // '
#define KANA_SE         ROMA_P
#define KANA_SO         ROMA_C
#define KANA_HE         214     // =
#define KANA_HO         215     // -
#define KANA_NU         216     // 1
#define KANA_ME         217     // /
#define KANA_MU         218     //KEY_NON_US_HASH
#define KANA_WO         219
#define KANA_RO         220     // KEY_INTERNATIONAL1
#define KANA_TOUTEN     221
#define KANA_KUTEN      222
#define KANA_NAKAGURO   223
#define KANA_CHOUON     224

// Common - Extra
#define ROMA_BANG       225     // !

// Non-common (US, JP, IME)
#define ROMA_LCB        230     // 「
#define ROMA_RCB        231     // 」
#define ROMA_LWCB       232     // 『
#define ROMA_RWCB       233     // 』
#define ROMA_LSB        234     // [
#define ROMA_RSB        235     // ]
#define ROMA_NAKAGURO   236     // ・
#define ROMA_SLASH      237     // ／
#define ROMA_SANTEN     238     // …
#define ROMA_COMMA      239     // ，
#define ROMA_PERIOD     240     // ．
#define ROMA_NAMI       241     // 〜

//
//
//

#define EEPROM_BASE     0
#define EEPROM_KANA     1
#define EEPROM_OS       2
#define EEPROM_DELAY	3
#define EEPROM_MOD      4
#define EEPROM_LED      5
#define EEPROM_IME      6
#define EEPROM_MOUSE    7
#define EEPROM_PREFIX   8

void initKeyboard(void);
void initKeyboardBase(void);
void initKeyboardKana(void);

#define KEY_LEFTFN      0xF0
#define KEY_RIGHTFN     0xF1
#define KEY_DAKUTEN     0xF3
#define KEY_HANDAKU     0xF4

#define MOD_FN          1u
#define MOD_PAD         4u      // Touch sensor

#define BASE_QWERTY     0
#define BASE_DVORAK     1
#define BASE_COLEMAK    2
#define BASE_JIS        3
#define BASE_NICOLA_F   4
#define BASE_MAX        4
void emitBaseName(void);
void switchBase(void);

int8_t isDigit(uint8_t code);
int8_t isJP(void);

#define KANA_ROMAJI     0
#define KANA_NICOLA     1
#define KANA_MTYPE      2
#define KANA_TRON       3
#define KANA_STICKNEY   4
#define KANA_X6004      5
#define KANA_MAX        5
void emitKanaName(void);
void switchKana(void);

#define OS_PC           0   // F13 / F14
#define OS_MAC          1   // Kana / Eisuu
#define OS_104A         2   // Shift-Ctrl-Space / Shift-Ctlr-Backspace
#define OS_104B         3   // Alt-`
#define OS_109A         4   // Shift-Ctrl-Henkan / Shift-Ctlr-Muhenkan
#define OS_109B         5   // `
#define OS_ALT_SP       6   // Alt-Space
#define OS_SHIFT_SP     7   // Shift-Space
#define OS_MAX          7
void emitOSName(void);
void switchOS(void);

#define is109()     (os == OS_109A || os == OS_109B)

void emitModName(void);
void switchMod(void);

#define MAX_DELAY       4

void emitDelayName(void);
void switchDelay(void);

#define LED_LEFT            0
#define LED_CENTER          1
#define LED_RIGHT           2
#define LED_LEFT_NUM        3
#define LED_CENTER_CAPS     4
#define LED_RIGHT_SCROLL    5
#define LED_OFF             6
#define LED_MAX             6

#define LED_DEFAULT         LED_CENTER_CAPS

void emitLEDName(void);
void switchLED(void);

#define IME_MS          0
#define IME_ATOK        1
#define IME_GOOGLE      2
#define IME_APPLE       3
#define IME_MAX         3
void emitIMEName(void);
void switchIME(void);

#define PREFIXSHIFT_OFF 0
#define PREFIXSHIFT_ON  1
#define PREFIXSHIFT_LED 2
#define PREFIXSHIFT_MAX 2

void emitPrefixShift(void);
void switchPrefixShift(void);

#define VOID_KEY        14  // A key matrix index at which no key is assigned

#define XMIT_NONE       0
#define XMIT_NORMAL     1
#define XMIT_BRK        2
#define XMIT_IN_ORDER   3
#define XMIT_MACRO      4

void onPressed(int8_t row, uint8_t column);
int8_t makeReport(uint8_t* report);

uint8_t processModKey(uint8_t key);

int8_t isKanaMode(const uint8_t* current);
uint8_t toggleKanaMode(uint8_t key, uint8_t mod, int8_t make);

int8_t processKeysBase(const uint8_t* current, const uint8_t* processed, uint8_t* report);
int8_t processKeysKana(const uint8_t* current, const uint8_t* processed, uint8_t* report);

uint8_t getLED(void);
uint8_t controlLED(uint8_t report);
uint8_t controlKanaLED(uint8_t report);

uint8_t getKeyNumLock(uint8_t code);
uint8_t getKeyBase(uint8_t code);

#ifdef __XC8
#define MAX_MACRO_SIZE  132
#else
#define MAX_MACRO_SIZE  160
#endif

uint8_t beginMacro(uint8_t max);
uint8_t peekMacro(void);
uint8_t getMacro(void);
void emitKey(uint8_t key);
void emitString(const uint8_t s[]);
void emitStringN(const uint8_t s[], uint8_t len);

extern uint8_t os;
extern uint8_t prefix_shift;
extern uint8_t prefix;

#ifndef __XC8

#define BOARD_REV_VALUE         5
#define APP_VERSION_VALUE       0x0016  // BCD

uint8_t eeprom_read(uint8_t index);
void eeprom_write(uint8_t index, uint8_t val);

#endif

#endif  // #ifndef KEYBOARD_H
