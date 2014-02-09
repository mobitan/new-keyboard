/*
 * Copyright 2014 Esrille Inc.
 *
 * This file is a modified version of app_device_keyboard.c provided by
 * Microchip Technology, Inc. for using Esrille New Keyboard.
 * See the Software License Agreement below for the License.
 */

/*******************************************************************************
  USB device keyboard demo source file

  Company:
    Microchip Technology Inc.

  File Name:
    keyboard.c

  Summary:
    This demo will make the USB peripheral look like a USB keyboard.

  Description:
    This demo will make the USB peripheral look like a USB keyboard.  The code
    defines the behavior of the keyboard and how it operates.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <system.h>
#include <usb/usb.h>
#include <usb/usb_device_hid.h>

#include "app_device_keyboard.h"
#include "app_led_usb_status.h"

#include <Keyboard.h>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************

//Class specific descriptor - HID Keyboard
const struct{uint8_t report[HID_RPT01_SIZE];}hid_rpt01={
{   0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xFF,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0}                          // End Collection
};


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************

/* This typedef defines the only INPUT report found in the HID report
 * descriptor and gives an easy way to create the OUTPUT report. */
typedef struct __attribute__((packed))
{
    /* The union below represents the first byte of the INPUT report.  It is
     * formed by the following HID report items:
     *
     *  0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
     *  0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
     *  0x15, 0x00, //   LOGICAL_MINIMUM (0)
     *  0x25, 0x01, //   LOGICAL_MAXIMUM (1)
     *  0x75, 0x01, //   REPORT_SIZE (1)
     *  0x95, 0x08, //   REPORT_COUNT (8)
     *  0x81, 0x02, //   INPUT (Data,Var,Abs)
     *
     * The report size is 1 specifying 1 bit per entry.
     * The report count is 8 specifying there are 8 entries.
     * These entries represent the Usage items between Left Control (the usage
     * minimum) and Right GUI (the usage maximum).
     */
    union __attribute__((packed))
    {
        uint8_t value;
        struct __attribute__((packed))
        {
            unsigned leftControl    :1;
            unsigned leftShift      :1;
            unsigned leftAlt        :1;
            unsigned leftGUI        :1;
            unsigned rightControl   :1;
            unsigned rightShift     :1;
            unsigned rightAlt       :1;
            unsigned rightGUI       :1;
        } bits;
    } modifiers;

    /* There is one byte of constant data/padding that is specified in the
     * input report:
     *
     *  0x95, 0x01,                    //   REPORT_COUNT (1)
     *  0x75, 0x08,                    //   REPORT_SIZE (8)
     *  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
     */
    unsigned :8;

    /* The last INPUT item in the INPUT report is an array type.  This array
     * contains an entry for each of the keys that are currently pressed until
     * the array limit, in this case 6 concurent key presses.
     *
     *  0x95, 0x06,                    //   REPORT_COUNT (6)
     *  0x75, 0x08,                    //   REPORT_SIZE (8)
     *  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
     *  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
     *  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
     *  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
     *  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
     *
     * Report count is 6 indicating that the array has 6 total entries.
     * Report size is 8 indicating each entry in the array is one byte.
     * The usage minimum indicates the lowest key value (Reserved/no event)
     * The usage maximum indicates the highest key value (Application button)
     * The logical minimum indicates the remapped value for the usage minimum:
     *   No Event has a logical value of 0.
     * The logical maximum indicates the remapped value for the usage maximum:
     *   Application button has a logical value of 101.
     *
     * In this case the logical min/max match the usage min/max so the logical
     * remapping doesn't actually change the values.
     *
     * To send a report with the 'a' key pressed (usage value of 0x04, logical
     * value in this example of 0x04 as well), then the array input would be the
     * following:
     *
     * LSB [0x04][0x00][0x00][0x00][0x00][0x00] MSB
     *
     * If the 'b' button was then pressed with the 'a' button still held down,
     * the report would then look like this:
     *
     * LSB [0x04][0x05][0x00][0x00][0x00][0x00] MSB
     *
     * If the 'a' button was then released with the 'b' button still held down,
     * the resulting array would be the following:
     *
     * LSB [0x05][0x00][0x00][0x00][0x00][0x00] MSB
     *
     * The 'a' key was removed from the array and all other items in the array
     * were shifted down. */
    uint8_t keys[6];
} KEYBOARD_INPUT_REPORT;


/* This typedef defines the only OUTPUT report found in the HID report
 * descriptor and gives an easy way to parse the OUTPUT report. */
typedef union __attribute__((packed))
{
    /* The OUTPUT report is comprised of only one byte of data. */
    uint8_t value;
    struct
    {
        /* There are two report items that form the one byte of OUTPUT report
         * data.  The first report item defines 5 LED indicators:
         *
         *  0x95, 0x05,                    //   REPORT_COUNT (5)
         *  0x75, 0x01,                    //   REPORT_SIZE (1)
         *  0x05, 0x08,                    //   USAGE_PAGE (LEDs)
         *  0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
         *  0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
         *  0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
         *
         * The report count indicates there are 5 entries.
         * The report size is 1 indicating each entry is just one bit.
         * These items are located on the LED usage page
         * These items are all of the usages between Num Lock (the usage
         * minimum) and Kana (the usage maximum).
         */
        unsigned numLock        :1;
        unsigned capsLock       :1;
        unsigned scrollLock     :1;
        unsigned compose        :1;
        unsigned kana           :1;

        /* The second OUTPUT report item defines 3 bits of constant data
         * (padding) used to make a complete byte:
         *
         *  0x95, 0x01,                    //   REPORT_COUNT (1)
         *  0x75, 0x03,                    //   REPORT_SIZE (3)
         *  0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
         *
         * Report count of 1 indicates that there is one entry
         * Report size of 3 indicates the entry is 3 bits long. */
        unsigned                :3;
    } leds;
} KEYBOARD_OUTPUT_REPORT;


/* This creates a storage type for all of the information required to track the
 * current state of the keyboard. */
typedef struct
{
    USB_HANDLE lastINTransmission;
    USB_HANDLE lastOUTTransmission;
} KEYBOARD;

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Variables
// *****************************************************************************
// *****************************************************************************
static KEYBOARD keyboard;

#if !defined(KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
static KEYBOARD_INPUT_REPORT inputReport KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG;

#if !defined(KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
static volatile KEYBOARD_OUTPUT_REPORT outputReport KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG;


// *****************************************************************************
// *****************************************************************************
// Section: Private Prototypes
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************
void APP_KeyboardInit(void)
{
    initKeyboard();
    
    //initialize the variable holding the handle for the last
    // transmission
    keyboard.lastINTransmission = 0;
    
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    //Arm OUT endpoint so we can receive caps lock, num lock, etc. info from host
    keyboard.lastOUTTransmission = HIDRxPacket(HID_EP, (uint8_t*) &outputReport, sizeof(outputReport));
}

void APP_KeyboardTasks(void)
{
    static int8_t xmit = XMIT_NORMAL;
    int8_t row;
    uint8_t column;
    uint8_t on;

    if (xmit != XMIT_BRK)
        Delay10KTCYx(3);   // 5msec at 24MHz

    /* Check if the IN endpoint is busy, and if it isn't check if we want to send
     * keystroke data to the host. */
    if(!HIDTxHandleBusy(keyboard.lastINTransmission))
    {
        for (row = 7; 0 <= row; --row) {
            PORTA &= 0xC0;
            PORTE &= 0xFC;
            TRISA |= 0x3F;
            TRISE |= 0x03;
            switch (row) {
            case 0:
                TRISAbits.TRISA0 = 0;
                break;
            case 1:
                TRISAbits.TRISA1 = 0;
                break;
            case 2:
                TRISAbits.TRISA2 = 0;
                break;
            case 3:
                TRISAbits.TRISA3 = 0;
                break;
            case 4:
                TRISAbits.TRISA4 = 0;
                break;
            case 5:
                TRISAbits.TRISA5 = 0;
                break;
            case 6:
                TRISEbits.TRISE0 = 0;
                break;
            case 7:
                TRISEbits.TRISE1 = 0;
                break;
            }
            for (column = 0; column < 12; ++column) {
                switch (column) {
                case 0:
                    on = !PORTDbits.RD4;
                    break;
                case 1:
                    on = !PORTDbits.RD5;
                    break;
                case 2:
                    on = !PORTDbits.RD6;
                    break;
                case 3:
                    on = !PORTDbits.RD7;
                    break;
                case 4:
                    on = !PORTDbits.RD2;
                    break;
                case 5:
                    on = !PORTDbits.RD3;
                    break;
                case 6:
                    on = !PORTBbits.RB5;
                    break;
                case 7:
                    on = !PORTBbits.RB4;
                    break;
                case 8:
                    on = !PORTBbits.RB1;
                    break;
                case 9:
                    on = !PORTBbits.RB0;
                    break;
                case 10:
                    on = !PORTBbits.RB2;
                    break;
                case 11:
                    on = !PORTBbits.RB3;
                    break;
                }
                if (on)
                    onPressed(row, column);
            }
        }

        xmit = makeReport((uint8_t*) &inputReport);
        switch (xmit) {
        case XMIT_BRK:
            memset(&inputReport + 2, 0, 6);
            keyboard.lastINTransmission = HIDTxPacket(HID_EP, (uint8_t*) &inputReport, sizeof(inputReport));
            break;
        case XMIT_NORMAL:
            keyboard.lastINTransmission = HIDTxPacket(HID_EP, (uint8_t*) &inputReport, sizeof(inputReport));
            break;
        }
    }

    /* Check if any data was sent from the PC to the keyboard device.  Report
     * descriptor allows host to send 1 byte of data.  Bits 0-4 are LED states,
     * bits 5-7 are unused pad bits.  The host can potentially send this OUT
     * report data through the HID OUT endpoint (EP1 OUT), or, alternatively,
     * the host may try to send LED state information by sending a SET_REPORT
     * control transfer on EP0.  See the USBHIDCBSetReportHandler() function. */
    if(HIDRxHandleBusy(keyboard.lastOUTTransmission) == false)
    {
        keyboard.lastOUTTransmission = HIDRxPacket(HID_EP,(uint8_t*)&outputReport,sizeof(outputReport));
    }
    
    APP_KeyboardProcessOutputReport();
}

void APP_KeyboardProcessOutputReport(void)
{
    uint8_t led = controlLED(outputReport.value);
    if (led & LED_NUM_LOCK)
        LED_On(LED_USB_DEVICE_HID_KEYBOARD_NUM_LOCK);
    else
        LED_Off(LED_USB_DEVICE_HID_KEYBOARD_NUM_LOCK);

    if (led & LED_CAPS_LOCK)
        LED_On(LED_USB_DEVICE_HID_KEYBOARD_CAPS_LOCK);
    else
        LED_Off(LED_USB_DEVICE_HID_KEYBOARD_CAPS_LOCK);

    if (led & LED_SCROLL_LOCK)
        LED_On(LED_USB_DEVICE_HID_KEYBOARD_SCROLL_LOCK);
    else
        LED_Off(LED_USB_DEVICE_HID_KEYBOARD_SCROLL_LOCK);
}

void APP_Suspend()
{
    APP_LEDUpdateUSBStatus();

    PORTA &= 0xC0;
    PORTE &= 0xFC;
    TRISA |= 0x3F;
    TRISE |= 0x03;

    OSCCON = 0x13;	//Sleep on sleep, 125kHz selected as microcontroller clock source
}

void APP_WakeFromSuspend()
{
    OSCCON = 0x60;      //Primary clock source selected.

    //Adding a software start up delay will ensure
    //that the primary oscillator and PLL are running before executing any other
    //code.  If the PLL isn't being used, (ex: primary osc = 48MHz externally applied EC)
    //then this code adds a small unnecessary delay, but it is harmless to execute anyway.
    {
        unsigned int pll_startup_counter = 800;	//Long delay at 31kHz, but ~0.8ms at 48MHz
        while (pll_startup_counter--)
            ;                                   //Clock will switch over while executing this delay loop
    }

    APP_LEDUpdateUSBStatus();
}

static void USBHIDCBSetReportComplete(void)
{
    /* 1 byte of LED state data should now be in the CtrlTrfData buffer.  Copy
     * it to the OUTPUT report buffer for processing */
    outputReport.value = CtrlTrfData[0];

    /* Process the OUTPUT report. */
    APP_KeyboardProcessOutputReport();
}

void USBHIDCBSetReportHandler(void)
{
    /* Prepare to receive the keyboard LED state data through a SET_REPORT
     * control transfer on endpoint 0.  The host should only send 1 byte,
     * since this is all that the report descriptor allows it to send. */
    USBEP0Receive((uint8_t*)&CtrlTrfData, USB_EP0_BUFF_SIZE, USBHIDCBSetReportComplete);
}

/*******************************************************************************
 End of File
*/