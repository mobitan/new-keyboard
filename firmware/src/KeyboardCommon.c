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

#include "Keyboard.h"
#include "Mouse.h"

#include <stdint.h>
#include <string.h>

#ifdef __XC8
#include <xc.h>
#include <system.h>
#include <plib/adc.h>
#endif

#ifdef NRF51
#include <stdio.h>
#include "nisse.h"
#endif

__EEPROM_DATA(BASE_QWERTY, KANA_ROMAJI, OS_PC, 1 /* delay */, 0 /* mod */, LED_DEFAULT, IME_MS, 0 /* mouse */);

uint8_t os;
uint8_t mod;
uint8_t prefix_shift;
uint8_t prefix;

#define MAX_OS_KEY_NAME     5

static uint8_t const osKeys[OS_MAX + 1][MAX_OS_KEY_NAME] =
{
    {KEY_P, KEY_C, KEY_ENTER},
    {KEY_M, KEY_A, KEY_C, KEY_ENTER},
    {KEY_1, KEY_0, KEY_4, KEY_A, KEY_ENTER},
    {KEY_1, KEY_0, KEY_4, KEY_B, KEY_ENTER},
    {KEY_1, KEY_0, KEY_9, KEY_A, KEY_ENTER},
    {KEY_1, KEY_0, KEY_9, KEY_B, KEY_ENTER},
    {KEY_A, KEY_MINUS, KEY_S, KEY_P, KEY_ENTER},
    {KEY_S, KEY_MINUS, KEY_S, KEY_P, KEY_ENTER},
};

#define MAX_MOD             5
#define MAX_MOD_KEY_NAME    6
#define MAX_MOD_KEYS        7

#define isMacMod()  (mod == 2 || mod == 5)

static uint8_t const modMap[MAX_MOD + 1][MAX_MOD_KEYS] =
{
    {KEY_LEFTCONTROL, KEY_LEFTSHIFT, KEY_LEFT_GUI, KEY_LEFTALT, KEY_RIGHTALT, KEY_RIGHTSHIFT, KEY_RIGHTCONTROL },
    {KEY_LEFTCONTROL, KEY_LEFTSHIFT, KEY_LEFTALT, KEY_LANG2, KEY_LANG1, KEY_RIGHTSHIFT, KEY_RIGHTCONTROL },
    {KEY_LEFTCONTROL, KEY_LEFTSHIFT, KEY_LEFT_GUI, KEY_LANG2, KEY_LANG1, KEY_RIGHTSHIFT, KEY_RIGHTCONTROL },
    {KEY_LEFTSHIFT, KEY_LEFTCONTROL, KEY_LEFT_GUI, KEY_LEFTALT, KEY_RIGHTALT, KEY_RIGHTCONTROL, KEY_RIGHTSHIFT },
    {KEY_LEFTSHIFT, KEY_LEFTCONTROL, KEY_LEFTALT, KEY_LANG2, KEY_LANG1, KEY_RIGHTCONTROL, KEY_RIGHTSHIFT },
    {KEY_LEFTSHIFT, KEY_LEFTCONTROL, KEY_LEFT_GUI, KEY_LANG2, KEY_LANG1, KEY_RIGHTCONTROL, KEY_RIGHTSHIFT },
};

static uint8_t const modKeys[MAX_MOD + 1][MAX_MOD_KEY_NAME] =
{
    {KEY_C, KEY_ENTER},
    {KEY_C, KEY_J, KEY_ENTER},
    {KEY_C, KEY_J, KEY_M, KEY_A, KEY_C, KEY_ENTER},
    {KEY_S, KEY_ENTER},
    {KEY_S, KEY_J, KEY_ENTER},
    {KEY_S, KEY_J, KEY_M, KEY_A, KEY_C, KEY_ENTER},
};

#if 0
static uint8_t const matrixFn[8][12][3] =
{
    {{KEY_INSERT}, {KEY_F2}, {KEY_F3}, {KEY_F4}, {KEY_F5}, {KEY_F6}, {KEY_F7}, {KEY_F8}, {KEY_F9}, {KEY_MUTE}, {KEY_VOLUME_DOWN}, {KEY_PAUSE}},
    {{KEY_LEFTCONTROL, KEY_DELETE}, {KEY_F1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {KEY_VOLUME_UP}, {KEY_SCROLL_LOCK}},
    {{KEY_LEFTCONTROL, KEY_LEFTSHIFT, KEY_Z}, {KEY_LEFTCONTROL, KEY_1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {KEY_LEFTCONTROL, KEY_0}, {KEY_PRINTSCREEN}},
    {{KEY_DELETE}, {KEY_LEFTCONTROL, KEY_2}, {KEY_LEFTCONTROL, KEY_3}, {KEY_LEFTCONTROL, KEY_4}, {KEY_LEFTCONTROL, KEY_5}, {0}, {0}, {KEY_LEFTCONTROL, KEY_6}, {KEY_LEFTCONTROL, KEY_LEFTSHIFT, KEY_LEFTARROW}, {KEY_LEFTSHIFT, KEY_UPARROW}, {KEY_LEFTCONTROL, KEY_LEFTSHIFT, KEY_RIGHTARROW}, {KEYPAD_NUM_LOCK}},
    {{KEY_LEFTCONTROL, KEY_Q}, {KEY_LEFTCONTROL, KEY_W}, {KEY_PAGEUP}, {KEY_LEFTCONTROL, KEY_R}, {KEY_LEFTCONTROL, KEY_T}, {0}, {0}, {KEY_LEFTCONTROL, KEY_HOME}, {KEY_LEFTCONTROL, KEY_LEFTARROW}, {KEY_UPARROW}, {KEY_LEFTCONTROL, KEY_RIGHTARROW}, {KEY_LEFTCONTROL, KEY_END}},
    {{KEY_LEFTCONTROL, KEY_A}, {KEY_LEFTCONTROL, KEY_S}, {KEY_PAGEDOWN}, {KEY_LEFTCONTROL, KEY_F}, {KEY_LEFTCONTROL, KEY_G}, {KEY_ESCAPE}, {KEY_CAPS_LOCK}, {KEY_HOME}, {KEY_LEFTARROW}, {KEY_DOWNARROW}, {KEY_RIGHTARROW}, {KEY_END}},
    {{KEY_LEFTCONTROL, KEY_Z}, {KEY_LEFTCONTROL, KEY_X}, {KEY_LEFTCONTROL, KEY_C}, {KEY_LEFTCONTROL, KEY_V}, {KEY_LANG2}, {KEY_TAB}, {KEY_ENTER}, {KEY_LANG1}, {KEY_LEFTSHIFT, KEY_LEFTARROW}, {KEY_LEFTSHIFT, KEY_DOWNARROW}, {KEY_LEFTSHIFT, KEY_RIGHTARROW}, {KEY_LEFTSHIFT, KEY_END}},
    {{0}, {0}, {0}, {0}, {KEY_LEFTCONTROL, KEY_BACKSPACE}, {0}, {0}, {KEY_LEFTCONTROL, KEY_SPACEBAR}, {0}, {0}, {0}, {0}}
};
static uint8_t const matrixFn109[4][3] =
{
    {KEY_INTERNATIONAL5},   // no-convert
    {KEY_INTERNATIONAL4},   // convert
    {KEY_INTERNATIONAL2},   // hiragana
    {KEY_GRAVE_ACCENT}      // zenkaku
};
#endif
static uint8_t const matrixFn[8][12][3] =
{
    {{KEY_PRINTSCREEN}, {KEY_F2}, {KEY_F3}, {KEY_F4}, {KEY_F5}, {KEY_F6}, {KEY_F7}, {KEY_F8}, {KEY_F9}, {KEY_MUTE}, {KEY_VOLUME_DOWN}, {KEY_PAUSE}},
    {{KEY_GRAVE_ACCENT}, {KEY_F1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {KEY_VOLUME_UP}, {KEY_BACKSLASH}},
    {{KEY_SCROLL_LOCK}, {KEY_LEFTALT, KEY_1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {KEY_LEFTALT, KEY_0}, {KEYPAD_NUM_LOCK}},
    {{KEY_CAPS_LOCK}, {KEY_LEFTALT, KEY_2}, {KEY_LEFTALT, KEY_3}, {KEY_LEFTALT, KEY_4}, {KEY_LEFTALT, KEY_5}, {0}, {0}, {KEY_LEFTALT, KEY_6}, {KEY_LEFTALT, KEY_7}, {KEY_LEFTALT, KEY_8}, {KEY_LEFTALT, KEY_9}, {KEY_QUOTE}},
    {{KEY_INTERNATIONAL2}, {KEY_INTERNATIONAL4}, {KEY_PAGEUP}, {KEY_INTERNATIONAL5}, {KEY_INTERNATIONAL6}, {0}, {0}, {KEY_LANG3}, {KEY_NON_US_BACKSLASH}, {KEY_UPARROW}, {KEYPAD_EQUAL}, {KEYPAD_COMMA}},
    {{KEY_INTERNATIONAL1}, {KEY_INSERT}, {KEY_PAGEDOWN}, {KEY_DELETE}, {KEY_INTERNATIONAL3}, {KEY_ESCAPE}, {KEY_APPLICATION}, {KEY_HOME}, {KEY_LEFTARROW}, {KEY_DOWNARROW}, {KEY_RIGHTARROW}, {KEY_END}},
    {{KEY_F15}, {KEY_F16}, {KEY_F17}, {KEY_F18}, {KEY_F19}, {KEY_TAB}, {KEY_ENTER}, {KEY_F20}, {KEY_F21}, {KEY_F22}, {KEY_F23}, {KEY_F24}},
    {{0}, {0}, {0}, {0}, {KEY_BACKSPACE}, {0}, {0}, {KEY_SPACEBAR}, {0}, {0}, {0}, {0}}
};

static uint8_t const matrixNumLock[8][5] =
{
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, KEYPAD_MULTIPLY, 0,
    KEY_CALC, 0, KEYPAD_EQUAL, KEYPAD_DIVIDE, 0,
    0, KEYPAD_7, KEYPAD_8, KEYPAD_9, KEYPAD_SUBTRACT,
    0, KEYPAD_4, KEYPAD_5, KEYPAD_6, KEYPAD_ADD,
    0, KEYPAD_1, KEYPAD_2, KEYPAD_3, KEY_ENTER,
    0, 0, KEYPAD_0, KEYPAD_DOT, 0,
};

#define MAX_DELAY_KEY_NAME  4

static uint8_t const delayKeyNames[MAX_DELAY + 1][MAX_DELAY_KEY_NAME] =
{
    {KEY_D, KEY_0, KEY_ENTER},
    {KEY_D, KEY_1, KEY_2, KEY_ENTER},
    {KEY_D, KEY_2, KEY_4, KEY_ENTER},
    {KEY_D, KEY_3, KEY_6, KEY_ENTER},
    {KEY_D, KEY_4, KEY_8, KEY_ENTER},
};

#define MAX_PREFIX_KEY_NAME  4

static uint8_t const prefixKeyNames[PREFIXSHIFT_MAX + 1][MAX_PREFIX_KEY_NAME] =
{
    {KEY_O, KEY_F, KEY_F, KEY_ENTER},
    {KEY_O, KEY_N, KEY_ENTER},
    {KEY_L, KEY_E, KEY_D, KEY_ENTER},
};

static uint8_t const codeRev2[8][12] =
{
    13, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 22,
    36, 0, VOID_KEY, VOID_KEY, VOID_KEY, VOID_KEY, VOID_KEY, VOID_KEY, VOID_KEY, VOID_KEY, 11, 47,
    84, 24, VOID_KEY, VOID_KEY, VOID_KEY, 65, 66, VOID_KEY, VOID_KEY, VOID_KEY, 35, 95,
    85, 12, VOID_KEY, VOID_KEY, VOID_KEY, 77, 78, VOID_KEY, VOID_KEY, VOID_KEY, 23, 94,
    86, 25, 37, 38, 39, 40, 43, 44, 45, 46, 34, 93,
    87, 48, 49, 50, 51, 52, 55, 56, 57, 58, 59, 92,
    88, 60, 61, 62, 63, 64, 67, 68, 69, 70, 71, 91,
    89, 72, 73, 74, 75, 76, 79, 80, 81, 82, 83, 90,
};

typedef struct Keys {
    uint8_t keys[6];
} Keys;

static uint8_t ordered_keys[MAX_MACRO_SIZE];
static uint8_t ordered_pos = 0;
static uint8_t ordered_max;

static uint8_t currentDelay;
static Keys keys[MAX_DELAY + 2];
static int8_t currentKey = 0;

#ifdef __XC8
static uint8_t tick;
#endif
static uint8_t processed[8];

static uint8_t modifiers;
static uint8_t modifiersPrev;
static uint8_t current[8];
static int8_t count;
static uint8_t rowCount[8];
static uint8_t columnCount[12];

static uint8_t led;

void initKeyboard(void)
{
    memset(keys, VOID_KEY, sizeof keys);
    currentKey = 0;
    memset(current, 0, 8);
    memset(processed, 0, 2);
    memset(processed + 2, VOID_KEY, 6);
    modifiers = modifiersPrev = 0;
    count = 2;

    os = eeprom_read(EEPROM_OS);
    if (OS_MAX < os)
        os = 0;
    mod = eeprom_read(EEPROM_MOD);
    if (MAX_MOD < mod)
        mod = 0;
    currentDelay = eeprom_read(EEPROM_DELAY);
    if (MAX_DELAY < currentDelay)
        currentDelay = 0;
    prefix_shift = eeprom_read(EEPROM_PREFIX);
    if (PREFIXSHIFT_MAX < prefix_shift)
        prefix_shift = 0;
    initKeyboardBase();
    initKeyboardKana();
}

void emitOSName(void)
{
    emitStringN(osKeys[os], MAX_OS_KEY_NAME);
}

void switchOS(void)
{
    ++os;
    if (OS_MAX < os)
        os = 0;
    eeprom_write(EEPROM_OS, os);
    emitOSName();
}

void emitModName(void)
{
    emitStringN(modKeys[mod], MAX_MOD_KEY_NAME);
}

void switchMod(void)
{
    ++mod;
    if (MAX_MOD < mod)
        mod = 0;
    eeprom_write(EEPROM_MOD, mod);
    emitModName();
}

void emitDelayName(void)
{
    emitStringN(delayKeyNames[currentDelay], MAX_DELAY_KEY_NAME);
}

void switchDelay(void)
{
    ++currentDelay;
    if (MAX_DELAY < currentDelay)
        currentDelay = 0;
    eeprom_write(EEPROM_DELAY, currentDelay);
    emitDelayName();
}

void emitPrefixShift(void)
{
    emitStringN(prefixKeyNames[prefix_shift], MAX_PREFIX_KEY_NAME);
}

void switchPrefixShift(void)
{
    ++prefix_shift;
    if (PREFIXSHIFT_MAX < prefix_shift)
        prefix_shift = 0;
    eeprom_write(EEPROM_PREFIX, prefix_shift);
    emitPrefixShift();
}

void onPressed(int8_t row, uint8_t column)
{
    uint8_t key;
    uint8_t code;

    if (2 <= BOARD_REV_VALUE)
        code = codeRev2[row][column];
    else
        code = 12 * row + column;
    ++columnCount[column];
    ++rowCount[row];
    key = getKeyBase(code);
    if (KEY_LEFTCONTROL <= key && key <= KEY_RIGHT_GUI) {
        modifiers |= 1u << (key - KEY_LEFTCONTROL);
        return;
    }
    if (KEY_FN == key) {
        current[1] |= MOD_FN;
        return;
    }
    if (count < 8)
        current[count++] = code;
}

static int8_t detectGhost(void)
{
    uint8_t i;
    int8_t detected;
    uint8_t rx = 0;
    uint8_t cx = 0;

    for (i = 0; i < sizeof rowCount; ++i) {
        if (2 <= rowCount[i])
            ++rx;
    }
    for (i = 0; i < sizeof columnCount; ++i) {
        if (2 <= columnCount[i])
            ++cx;
    }
    detected = (2 <= rx && 2 <= cx);
    memset(rowCount, 0, sizeof rowCount);
    memset(columnCount, 0, sizeof columnCount);
    return detected;
}

uint8_t beginMacro(uint8_t max)
{
    ordered_pos = 1;
    ordered_max = max;
    return ordered_keys[0];
}

uint8_t peekMacro(void)
{
    if (ordered_max <= ordered_pos)
        return 0;
    return ordered_keys[ordered_pos];
}

uint8_t getMacro(void)
{
    if (ordered_max <= ordered_pos)
        return 0;
    uint8_t key = ordered_keys[ordered_pos++];
    if (key == 0) {
        ordered_pos = 0;
        ordered_max = 0;
    }
    return key;
}

void emitKey(uint8_t c)
{
    if (ordered_pos < sizeof ordered_keys)
        ordered_keys[ordered_pos++] = c;
    if (ordered_pos < sizeof ordered_keys)
        ordered_keys[ordered_pos] = 0;
}

void emitString(const uint8_t s[])
{
    uint8_t i = 0;
    uint8_t c;
    for (c = s[i]; c; c = s[++i])
        emitKey(c);
}

void emitStringN(const uint8_t s[], uint8_t len)
{
    uint8_t i = 0;
    uint8_t c;
    for (c = s[i]; i < len && c; c = s[++i])
        emitKey(c);
}

static uint8_t getNumKeycode(unsigned int n)
{
    if (n == 0)
        return KEY_0;
    if (1 <= n && n <= 9)
        return KEY_1 - 1 + n;
    return KEY_SPACEBAR;
}

static const uint8_t about1[] = {
    KEY_E, KEY_S, KEY_R, KEY_I, KEY_L, KEY_L, KEY_E, KEY_SPACEBAR, KEY_N, KEY_I, KEY_S, KEY_S, KEY_E,
#ifdef NRF51
    KEY_SPACEBAR, KEY_B, KEY_L, KEY_E,
#endif
    KEY_ENTER,
    KEY_R, KEY_E, KEY_V, KEY_PERIOD, KEY_SPACEBAR, 0
};
static const uint8_t about2[] = {
    KEY_V, KEY_E, KEY_R, KEY_PERIOD, KEY_SPACEBAR, 0
};
static const uint8_t about3[] = {
    KEY_C, KEY_O, KEY_P, KEY_Y, KEY_R, KEY_I, KEY_G, KEY_H, KEY_T, KEY_SPACEBAR, KEY_2, KEY_0, KEY_1, KEY_3, KEY_MINUS, KEY_2, KEY_0, KEY_1, KEY_5, KEY_SPACEBAR,
    KEY_E, KEY_S, KEY_R, KEY_I, KEY_L, KEY_L, KEY_E, KEY_SPACEBAR, KEY_I, KEY_N, KEY_C, KEY_PERIOD, KEY_ENTER,
    KEY_F, KEY_2, KEY_SPACEBAR, 0
};
static const uint8_t about4[] = {
    KEY_F, KEY_3, KEY_SPACEBAR, 0
};
static const uint8_t about5[] = {
    KEY_F, KEY_4, KEY_SPACEBAR, 0
};
static const uint8_t about6[] = {
    KEY_F, KEY_5, KEY_SPACEBAR, 0
};
static const uint8_t about7[] = {
    KEY_F, KEY_6, KEY_SPACEBAR, 0
};
static const uint8_t about8[] = {
    KEY_F, KEY_7, KEY_SPACEBAR, 0
};
static const uint8_t about9[] = {
    KEY_F, KEY_8, KEY_SPACEBAR, 0
};
static const uint8_t about10[] = {
    KEY_F, KEY_9, KEY_SPACEBAR, 0
};

static void about(void)
{
    // REV.
    emitString(about1);
    emitKey(getNumKeycode(BOARD_REV_VALUE));
    emitKey(KEY_ENTER);

    // VER.
    emitString(about2);
    emitKey(getNumKeycode((APP_VERSION_VALUE >> 8) & 0xf));
    emitKey(KEY_PERIOD);
    emitKey(getNumKeycode((APP_VERSION_VALUE >> 4) & 0xf));
    emitKey(getNumKeycode(APP_VERSION_VALUE & 0xf));
    emitKey(KEY_ENTER);

    // F2 OS
    emitString(about3);
    emitOSName();

    // F3 Layout
    emitString(about4);
    emitBaseName();

    // F4 Kana Layout
    emitString(about5);
    emitKanaName();

    // F5 Delay
    emitString(about6);
    emitDelayName();

    // F6 Modifiers
    emitString(about7);
    emitModName();

    // F7 IME
    emitString(about8);
    emitIMEName();

    // F8 LED
    emitString(about9);
    emitLEDName();

    // F9 Prefix Shift
    emitString(about10);
    emitPrefixShift();

#ifdef ENABLE_MOUSE
    emitMouse();
#endif

#ifdef NRF51
{
    uint32_t voltage = battery_voltage_get();
    emitKey(getNumKeycode(voltage / 100));
    emitKey(KEY_PERIOD);
    voltage %= 100;
    emitKey(getNumKeycode(voltage / 10));
    emitKey(getNumKeycode(voltage % 10));
    emitKey(KEY_V);
    emitKey(KEY_ENTER);
}
#endif
}

static const uint8_t* getKeyFn(uint8_t code)
{
#if 0
    if (is109()) {
        if (12 * 6 + 8 <= code && code <= 12 * 6 + 11)
            return matrixFn109[code - (12 * 6 + 8)];
    }
#endif
    return matrixFn[code / 12][code % 12];
}

static int8_t processKeys(const uint8_t* current, uint8_t* processed, uint8_t* report)
{
    int8_t xmit;

    if (!memcmp(current, processed, 8))
        return XMIT_NONE;
    memset(report, 0, 8);
    if (current[1] & MOD_FN) {
        uint8_t modifiers = current[0];
        uint8_t count = 2;
        xmit = XMIT_NORMAL;
        for (int8_t i = 2; i < 8 && xmit != XMIT_MACRO; ++i) {
            uint8_t code = current[i];
            const uint8_t* a = getKeyFn(code);
            for (int8_t j = 0; j < 3 && count < 8; ++j) {
                uint8_t key = a[j];
                int8_t make = !memchr(processed + 2, code, 6);
                switch (key) {
                case 0:
                    break;
                case KEY_F1:
                    if (make) {
                        about();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F2:
                    if (make) {
                        switchOS();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F3:
                    if (make) {
                        switchBase();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F4:
                    if (make) {
                        switchKana();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F5:
                    if (make) {
                        switchDelay();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F6:
                    if (make) {
                        switchMod();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F7:
                    if (make) {
                        switchIME();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F8:
                    if (make) {
                        switchLED();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_F9:
                    if (make) {
                        switchPrefixShift();
                        xmit = XMIT_MACRO;
                    }
                    break;
                case KEY_LEFTCONTROL:
                    modifiers |= MOD_LEFTCONTROL;
                    break;
                case KEY_RIGHTCONTROL:
                    modifiers |= MOD_RIGHTCONTROL;
                    break;
                case KEY_LEFTSHIFT:
                    modifiers |= MOD_LEFTSHIFT;
                    break;
                case KEY_RIGHTSHIFT:
                    modifiers |= MOD_RIGHTSHIFT;
                    break;
                case KEY_LEFTALT:
                    modifiers |= MOD_LEFTALT;
                    break;
                case KEY_RIGHTALT:
                    modifiers |= MOD_RIGHTALT;
                    break;
                case KEY_LEFT_GUI:
                    modifiers |= MOD_LEFTGUI;
                    break;
                case KEY_RIGHT_GUI:
                    modifiers |= MOD_RIGHTGUI;
                    break;
#ifdef NRF51
                case KEY_ESCAPE:
                    if (make) {
                        set_event(BSP_EVENT_DISCONNECT);
                        xmit = XMIT_BRK;
                    }
                    break;
                case KEY_1:
                    if (make) {
                        set_event(BSP_EVENT_KEY_1);
                        modifiers &= ~MOD_LEFTCONTROL;
                        xmit = XMIT_BRK;
                    }
                    break;
                case KEY_2:
                    if (make) {
                        set_event(BSP_EVENT_KEY_2);
                        modifiers &= ~MOD_LEFTCONTROL;
                        xmit = XMIT_BRK;
                    }
                    break;
                case KEY_3:
                    if (make) {
                        set_event(BSP_EVENT_KEY_3);
                        modifiers &= ~MOD_LEFTCONTROL;
                        xmit = XMIT_BRK;
                    }
                    break;
#endif
                default:
                    key = toggleKanaMode(key, current[0], make);
                    report[count++] = key;
                    break;
                }
            }
        }
        report[0] = modifiers;
    } else if (isKanaMode(current))
        xmit = processKeysKana(current, processed, report);
    else
        xmit = processKeysBase(current, processed, report);
    if (xmit == XMIT_NORMAL || xmit == XMIT_IN_ORDER || xmit == XMIT_MACRO)
        memmove(processed, current, 8);
    return xmit;
}

static void processOSMode(uint8_t* report)
{
    for (int8_t i = 2; i < 8; ++i) {
        switch (os) {
        case OS_PC:
            switch (report[i]) {
            case KEY_LANG1:
                report[i] = KEY_F13;
                break;
            case KEY_LANG2:
                report[i] = KEY_F14;
                break;
#if 0
            case KEY_INTERNATIONAL4:
            case KEY_INTERNATIONAL5:
                report[i] = KEY_SPACEBAR;
                break;
#endif
            default:
                break;
            }
            break;
        case OS_MAC:
            switch (report[i]) {
#if 0
            case KEY_INTERNATIONAL4:
            case KEY_INTERNATIONAL5:
                report[i] = KEY_SPACEBAR;
                break;
#endif
            case KEY_APPLICATION:
                if (isMacMod()) {
                    report[0] |= MOD_LEFTALT;
                    memmove(report + i, report + i + 1, 7 - i);
                    report[7] = 0;
                    --i;
                }
                break;
            default:
                break;
            }
            break;
        case OS_104A:
            switch (report[i]) {
            case KEY_LANG1:
                report[i] = KEY_SPACEBAR;
                report[0] |= MOD_LEFTSHIFT | MOD_LEFTCONTROL;
                break;
            case KEY_LANG2:
                report[i] = KEY_BACKSPACE;
                report[0] |= MOD_LEFTSHIFT | MOD_LEFTCONTROL;
                break;
#if 0
            case KEY_INTERNATIONAL4:
            case KEY_INTERNATIONAL5:
                report[i] = KEY_SPACEBAR;
                break;
#endif
            default:
                break;
            }
            break;
        case OS_104B:
            switch (report[i]) {
            case KEY_LANG1:
            case KEY_LANG2:
                report[i] = KEY_GRAVE_ACCENT;
                report[0] |= MOD_LEFTALT;
                break;
#if 0
            case KEY_INTERNATIONAL4:
            case KEY_INTERNATIONAL5:
                report[i] = KEY_SPACEBAR;
                break;
#endif
            default:
                break;
            }
            break;
        case OS_109A:
            switch (report[i]) {
            case KEY_LANG1:
                report[i] = KEY_INTERNATIONAL4;
                report[0] |= MOD_LEFTSHIFT | MOD_LEFTCONTROL;
                break;
            case KEY_LANG2:
                report[i] = KEY_INTERNATIONAL5;
                report[0] |= MOD_LEFTSHIFT | MOD_LEFTCONTROL;
                break;
            default:
                break;
            }
            break;
        case OS_109B:
            switch (report[i]) {
            case KEY_LANG1:
            case KEY_LANG2:
                report[i] = KEY_GRAVE_ACCENT;
                break;
            default:
                break;
            }
            break;
        case OS_ALT_SP:
            switch (report[i]) {
            case KEY_LANG1:
            case KEY_LANG2:
                report[i] = KEY_SPACEBAR;
                report[0] |= MOD_LEFTALT;
                break;
            default:
                break;
            }
            break;
        case OS_SHIFT_SP:
            switch (report[i]) {
            case KEY_LANG1:
            case KEY_LANG2:
                report[i] = KEY_SPACEBAR;
                report[0] |= MOD_LEFTSHIFT;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

uint8_t processModKey(uint8_t key)
{
    const uint8_t* map = modMap[0];
    for (int8_t i = 0; i < MAX_MOD_KEYS; ++i) {
        if (key == map[i])
            return modMap[mod][i];
    }
    return key;
}

int8_t makeReport(uint8_t* report)
{
    int8_t xmit = XMIT_NONE;
    int8_t at;
    int8_t prev;

    if (!detectGhost()) {
        while (count < 8)
            current[count++] = VOID_KEY;
        memmove(keys[currentKey].keys, current + 2, 6);
        current[0] = modifiers;
        if (led & LED_SCROLL_LOCK)
            current[1] |= MOD_FN;
#ifdef ENABLE_MOUSE
        if (isMouseTouched())
            current[1] |= MOD_PAD;
#endif

        if (prefix_shift && isKanaMode(current)) {
            current[0] |= prefix;
            if (!(modifiersPrev & MOD_LEFTSHIFT) && (modifiers & MOD_LEFTSHIFT))
                prefix ^= MOD_LEFTSHIFT;
            if (!(modifiersPrev & MOD_RIGHTSHIFT) && (modifiers & MOD_RIGHTSHIFT))
                prefix ^= MOD_RIGHTSHIFT;
        }
        modifiersPrev = modifiers;

        // Copy keys that exist in both keys[prev] and keys[at] for debouncing.
        at = currentKey + MAX_DELAY + 2 - currentDelay;
        if (MAX_DELAY + 1 < at)
                at -= MAX_DELAY + 2;
        prev = at + MAX_DELAY + 1;
        if (MAX_DELAY + 1 < prev)
                prev -= MAX_DELAY + 2;
        count = 2;
        for (int8_t i = 0; i < 6; ++i) {
            uint8_t key = keys[at].keys[i];
            if (memchr(keys[prev].keys, key, 6))
                current[count++] = key;
        }
        while (count < 8)
            current[count++] = VOID_KEY;

#ifdef ENABLE_MOUSE
        if (current[1] == MOD_PAD)
            processMouseKeys(current, processed);
#endif

        if (memcmp(current, processed, 8)) {
            if (memcmp(current + 2, processed + 2, 6) || current[2] == VOID_KEY || current[1] || (current[0] & MOD_SHIFT)) {
                if (current[2] != VOID_KEY)
                    prefix = 0;
                xmit = processKeys(current, processed, report);
            } else if (processed[1] && !current[1] ||
                     (processed[0] & MOD_LEFTSHIFT) && !(current[0] & MOD_LEFTSHIFT) ||
                     (processed[0] & MOD_RIGHTSHIFT) && !(current[0] & MOD_RIGHTSHIFT))
            {
                /* empty */
            } else
                xmit = processKeys(current, processed, report);
        }
        processOSMode(report);
    } else {
        prev = currentKey + MAX_DELAY + 1;
        if (MAX_DELAY + 1 < prev)
                prev -= MAX_DELAY + 2;
        memmove(keys[currentKey].keys, keys[prev].keys, 6);
    }

    if (MAX_DELAY + 1 < ++currentKey)
        currentKey = 0;
    count = 2;
    modifiers = 0;
    current[1] = 0;

    return xmit;
}

uint8_t getLED(void)
{
    return led;
}

uint8_t controlLED(uint8_t report)
{
    led = report;
    report = controlKanaLED(report);
#ifdef ENABLE_MOUSE
    if (isMouseTouched())
        report |= LED_SCROLL_LOCK;
#endif
#ifdef __XC8
    if (BOARD_REV_VALUE < 3) {
        static int8_t tick;

        if (4 <= ++tick)
            tick = 0;
        else
            report &= ~LED_USB_DEVICE_HID_KEYBOARD_CAPS_LOCK;
    }
#endif
    return report;
}

uint8_t getKeyNumLock(uint8_t code)
{
    uint8_t col = code % 12;

    if ((led & LED_NUM_LOCK) && 7 <= col) {
        col -= 7;
        return matrixNumLock[code / 12][col];
    }
    return 0;
}
