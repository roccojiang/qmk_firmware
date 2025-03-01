/* Copyright 2021 Rocco Jiang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Bongo Cat WPM visualiser */
#define IDLE_FRAMES 5
#define TAP_FRAMES 2

#define IDLE_SPEED 30 // animation idles below this wpm value
#define TAP_SPEED 70 // animation triggers above this wpm value

#define FRAME_DURATION 200 // ms per frame

static uint32_t anim_timer = 0;
static uint32_t anim_sleep = 0;

// Run-length encoded idle frames
static unsigned char const idle0[] PROGMEM = {181,
    0x00, 0x33, 0x80, 0x02, 0x00, 0x05, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x04, 0x08, 0x01,
    0x04, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x01, 0x0c, 0x01, 0x30, 0x01, 0x40, 0x01, 0x80, 0x01,
    0x00, 0x2e, 0x80, 0x03, 0x00, 0x31, 0x1e, 0x01, 0xe1, 0x01, 0x00, 0x02, 0x01, 0x02, 0x02, 0x02,
    0x01, 0x01, 0x00, 0x04, 0x80, 0x02, 0x00, 0x09, 0x80, 0x01, 0x00, 0x01, 0x30, 0x02, 0x00, 0x01,
    0xc0, 0x01, 0xc1, 0x02, 0xc2, 0x01, 0x04, 0x01, 0x08, 0x01, 0x10, 0x01, 0x20, 0x01, 0x40, 0x01,
    0x80, 0x01, 0x00, 0x03, 0x80, 0x04, 0x40, 0x04, 0x20, 0x04, 0x10, 0x04, 0x08, 0x05, 0x04, 0x05,
    0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0x80, 0x01, 0x70, 0x01, 0x0c, 0x01, 0x03, 0x01, 0x00, 0x07,
    0x0c, 0x03, 0x0d, 0x01, 0x01, 0x01, 0x00, 0x01, 0x40, 0x01, 0xa0, 0x01, 0x21, 0x01, 0x22, 0x01,
    0x12, 0x01, 0x11, 0x03, 0x09, 0x01, 0x08, 0x04, 0x04, 0x02, 0x08, 0x02, 0x10, 0x05, 0x11, 0x01,
    0x0f, 0x01, 0x01, 0x02, 0x00, 0x36, 0x80, 0x05, 0x40, 0x05, 0x20, 0x05, 0x10, 0x05, 0x08, 0x05,
    0x04, 0x05, 0x02, 0x01, 0x03, 0x01, 0x02, 0x02, 0x01, 0x06, 0x02, 0x02, 0x04, 0x02, 0x08, 0x05,
    0x07, 0x01, 0x00, 0x3d};
static unsigned char const idle1[] PROGMEM = {165,
    0x00, 0x3b, 0x80, 0x02, 0x40, 0x04, 0x20, 0x04, 0x10, 0x01, 0x08, 0x01, 0x04, 0x01, 0x02, 0x02,
    0x04, 0x01, 0x18, 0x01, 0x60, 0x01, 0x80, 0x01, 0x00, 0x2f, 0x80, 0x03, 0x00, 0x31, 0x3c, 0x01,
    0xc2, 0x01, 0x01, 0x02, 0x02, 0x02, 0x04, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x10, 0x60, 0x02,
    0x00, 0x01, 0x81, 0x01, 0x82, 0x02, 0x84, 0x01, 0x08, 0x01, 0x10, 0x01, 0x20, 0x01, 0x40, 0x01,
    0x80, 0x01, 0x00, 0x04, 0x80, 0x04, 0x40, 0x04, 0x20, 0x04, 0x10, 0x04, 0x08, 0x05, 0x04, 0x05,
    0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0x80, 0x01, 0x70, 0x01, 0x19, 0x01, 0x06, 0x01, 0x00, 0x07,
    0x18, 0x03, 0x1b, 0x01, 0x03, 0x01, 0x00, 0x01, 0x40, 0x01, 0xa0, 0x01, 0x22, 0x01, 0x24, 0x01,
    0x14, 0x01, 0x12, 0x03, 0x0b, 0x01, 0x08, 0x04, 0x05, 0x02, 0x09, 0x02, 0x10, 0x05, 0x11, 0x01,
    0x0f, 0x01, 0x01, 0x02, 0x00, 0x36, 0x80, 0x05, 0x40, 0x05, 0x20, 0x05, 0x10, 0x05, 0x08, 0x05,
    0x04, 0x05, 0x02, 0x01, 0x03, 0x01, 0x02, 0x02, 0x01, 0x06, 0x02, 0x02, 0x04, 0x02, 0x08, 0x05,
    0x07, 0x01, 0x00, 0x3d};
static unsigned char const idle2[] PROGMEM = {169,
    0x00, 0x3a, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x05, 0x08, 0x02, 0x04, 0x03, 0x08, 0x01,
    0x30, 0x01, 0x40, 0x01, 0x80, 0x01, 0x00, 0x2e, 0x80, 0x03, 0x00, 0x31, 0x18, 0x01, 0x64, 0x01,
    0x82, 0x01, 0x02, 0x05, 0x01, 0x01, 0x00, 0x04, 0x80, 0x02, 0x00, 0x09, 0x80, 0x01, 0x00, 0x01,
    0x30, 0x02, 0x00, 0x01, 0xc0, 0x01, 0xc1, 0x02, 0xc2, 0x01, 0x04, 0x01, 0x08, 0x01, 0x10, 0x01,
    0x20, 0x01, 0x40, 0x01, 0x80, 0x01, 0x00, 0x03, 0x80, 0x04, 0x40, 0x04, 0x20, 0x04, 0x10, 0x04,
    0x08, 0x05, 0x04, 0x05, 0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0xc0, 0x01, 0x38, 0x01, 0x04, 0x01,
    0x03, 0x01, 0x00, 0x07, 0x0c, 0x03, 0x0d, 0x01, 0x01, 0x01, 0x00, 0x01, 0x40, 0x01, 0xa0, 0x01,
    0x21, 0x01, 0x22, 0x01, 0x12, 0x01, 0x11, 0x03, 0x09, 0x01, 0x08, 0x04, 0x04, 0x02, 0x08, 0x02,
    0x10, 0x05, 0x11, 0x01, 0x0f, 0x01, 0x01, 0x02, 0x00, 0x36, 0x80, 0x05, 0x40, 0x05, 0x20, 0x05,
    0x10, 0x05, 0x08, 0x05, 0x04, 0x05, 0x02, 0x01, 0x03, 0x01, 0x02, 0x02, 0x01, 0x06, 0x02, 0x02,
    0x04, 0x02, 0x08, 0x05, 0x07, 0x01, 0x00, 0x3d};
static unsigned char const idle3[] PROGMEM = {171,
    0x00, 0x3a, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x04, 0x08, 0x02, 0x04, 0x01, 0x02, 0x03,
    0x04, 0x01, 0x38, 0x01, 0x40, 0x01, 0x80, 0x01, 0x00, 0x2e, 0x80, 0x03, 0x00, 0x31, 0x1c, 0x01,
    0xe2, 0x01, 0x01, 0x02, 0x02, 0x04, 0x01, 0x01, 0x00, 0x04, 0x80, 0x02, 0x00, 0x09, 0x80, 0x01,
    0x00, 0x01, 0x30, 0x02, 0x00, 0x01, 0xc0, 0x01, 0xc1, 0x02, 0xc2, 0x01, 0x04, 0x01, 0x08, 0x01,
    0x10, 0x01, 0x20, 0x01, 0x40, 0x01, 0x80, 0x01, 0x00, 0x03, 0x80, 0x04, 0x40, 0x04, 0x20, 0x04,
    0x10, 0x04, 0x08, 0x05, 0x04, 0x05, 0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0x80, 0x01, 0x70, 0x01,
    0x0c, 0x01, 0x03, 0x01, 0x00, 0x07, 0x0c, 0x03, 0x0d, 0x01, 0x01, 0x01, 0x00, 0x01, 0x40, 0x01,
    0xa0, 0x01, 0x21, 0x01, 0x22, 0x01, 0x12, 0x01, 0x11, 0x03, 0x09, 0x01, 0x08, 0x04, 0x04, 0x02,
    0x08, 0x02, 0x10, 0x05, 0x11, 0x01, 0x0f, 0x01, 0x01, 0x02, 0x00, 0x36, 0x80, 0x05, 0x40, 0x05,
    0x20, 0x05, 0x10, 0x05, 0x08, 0x05, 0x04, 0x05, 0x02, 0x01, 0x03, 0x01, 0x02, 0x02, 0x01, 0x06,
    0x02, 0x02, 0x04, 0x02, 0x08, 0x05, 0x07, 0x01, 0x00, 0x3d};

// Run-length encoded prep frame
static unsigned char const prep[] PROGMEM = {181,
    0x00, 0x33, 0x80, 0x02, 0x00, 0x05, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x04, 0x08, 0x01,
    0x04, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x01, 0x0c, 0x01, 0x30, 0x01, 0x40, 0x01, 0x80, 0x01,
    0x00, 0x2e, 0x80, 0x03, 0x00, 0x31, 0x1e, 0x01, 0xe1, 0x01, 0x00, 0x02, 0x01, 0x02, 0x02, 0x02,
    0x81, 0x01, 0x80, 0x02, 0x00, 0x02, 0x80, 0x02, 0x00, 0x09, 0x80, 0x01, 0x00, 0x01, 0x30, 0x02,
    0x00, 0x02, 0x01, 0x01, 0xe1, 0x01, 0x1a, 0x01, 0x06, 0x01, 0x09, 0x01, 0x31, 0x01, 0x35, 0x01,
    0x01, 0x01, 0x8a, 0x01, 0x7c, 0x01, 0x00, 0x02, 0x80, 0x04, 0x40, 0x04, 0x20, 0x04, 0x10, 0x04,
    0x08, 0x05, 0x04, 0x05, 0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0x80, 0x01, 0x70, 0x01, 0x0c, 0x01,
    0x03, 0x01, 0x00, 0x02, 0x18, 0x01, 0x06, 0x01, 0x05, 0x01, 0x98, 0x01, 0x99, 0x01, 0x84, 0x01,
    0xc3, 0x01, 0x7c, 0x01, 0x41, 0x02, 0x40, 0x02, 0x20, 0x01, 0x21, 0x01, 0x22, 0x01, 0x12, 0x01,
    0x11, 0x03, 0x09, 0x01, 0x08, 0x04, 0x04, 0x06, 0x02, 0x03, 0x01, 0x04, 0x00, 0x36, 0x80, 0x05,
    0x40, 0x05, 0x20, 0x05, 0x10, 0x05, 0x08, 0x05, 0x04, 0x05, 0x02, 0x01, 0x03, 0x01, 0x02, 0x02,
    0x01, 0x06, 0x00, 0x47};

// Run-length encoded tap frames
static unsigned char const tap0[] PROGMEM = {221,
    0x00, 0x33, 0x80, 0x02, 0x00, 0x05, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x04, 0x08, 0x01,
    0x04, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x01, 0x0c, 0x01, 0x30, 0x01, 0x40, 0x01, 0x80, 0x01,
    0x00, 0x07, 0xf8, 0x04, 0x00, 0x05, 0x80, 0x02, 0x00, 0x1c, 0x80, 0x03, 0x00, 0x31, 0x1e, 0x01,
    0xe1, 0x01, 0x00, 0x02, 0x01, 0x02, 0x02, 0x02, 0x81, 0x01, 0x80, 0x02, 0x00, 0x02, 0x80, 0x02,
    0x00, 0x09, 0x80, 0x01, 0x00, 0x01, 0x30, 0x02, 0x00, 0x02, 0x01, 0x02, 0x02, 0x01, 0x04, 0x01,
    0x08, 0x01, 0x10, 0x01, 0x20, 0x01, 0x43, 0x01, 0x87, 0x01, 0x07, 0x01, 0x01, 0x01, 0x00, 0x01,
    0xb8, 0x01, 0xbc, 0x01, 0xbe, 0x01, 0x9f, 0x01, 0x5f, 0x02, 0x4f, 0x01, 0x4c, 0x01, 0x20, 0x04,
    0x10, 0x04, 0x08, 0x05, 0x04, 0x05, 0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0x80, 0x01, 0x70, 0x01,
    0x0c, 0x01, 0x03, 0x01, 0x00, 0x02, 0x18, 0x01, 0x06, 0x01, 0x05, 0x01, 0x98, 0x01, 0x99, 0x01,
    0x84, 0x01, 0x43, 0x01, 0x7c, 0x01, 0x41, 0x02, 0x40, 0x02, 0x20, 0x01, 0x21, 0x01, 0x22, 0x01,
    0x12, 0x01, 0x11, 0x03, 0x09, 0x01, 0x08, 0x04, 0x04, 0x02, 0x08, 0x02, 0x10, 0x05, 0x11, 0x01,
    0x0f, 0x01, 0x01, 0x01, 0x3d, 0x01, 0x7c, 0x01, 0xfc, 0x05, 0x3c, 0x01, 0x0c, 0x01, 0x00, 0x2e,
    0x80, 0x05, 0x40, 0x05, 0x20, 0x05, 0x10, 0x05, 0x08, 0x05, 0x04, 0x05, 0x02, 0x01, 0x03, 0x01,
    0x02, 0x02, 0x01, 0x06, 0x00, 0x24, 0x01, 0x01, 0x03, 0x02, 0x00, 0x20};
static unsigned char const tap1[] PROGMEM = {181,
    0x00, 0x33, 0x80, 0x02, 0x00, 0x05, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x04, 0x08, 0x01,
    0x04, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x01, 0x0c, 0x01, 0x30, 0x01, 0x40, 0x01, 0x80, 0x01,
    0x00, 0x2e, 0x80, 0x03, 0x00, 0x31, 0x1e, 0x01, 0xe1, 0x01, 0x00, 0x02, 0x01, 0x02, 0x02, 0x02,
    0x01, 0x01, 0x00, 0x04, 0x80, 0x02, 0x00, 0x09, 0x80, 0x01, 0x00, 0x01, 0x30, 0x02, 0x00, 0x02,
    0x01, 0x01, 0xe1, 0x01, 0x1a, 0x01, 0x06, 0x01, 0x09, 0x01, 0x31, 0x01, 0x35, 0x01, 0x01, 0x01,
    0x8a, 0x01, 0x7c, 0x01, 0x00, 0x02, 0x80, 0x04, 0x40, 0x04, 0x20, 0x04, 0x10, 0x04, 0x08, 0x05,
    0x04, 0x05, 0x02, 0x04, 0x01, 0x04, 0x00, 0x33, 0x80, 0x01, 0x70, 0x01, 0x0c, 0x01, 0x03, 0x01,
    0x00, 0x0a, 0x01, 0x02, 0x00, 0x01, 0x40, 0x01, 0xa0, 0x01, 0x21, 0x01, 0x22, 0x01, 0x12, 0x01,
    0x11, 0x03, 0x09, 0x01, 0x08, 0x04, 0x04, 0x06, 0x02, 0x03, 0x01, 0x04, 0x00, 0x36, 0x80, 0x05,
    0x40, 0x05, 0x20, 0x05, 0x10, 0x05, 0x08, 0x05, 0x04, 0x05, 0x02, 0x01, 0x03, 0x01, 0x7a, 0x02,
    0x79, 0x04, 0x39, 0x01, 0x31, 0x01, 0x02, 0x02, 0x04, 0x02, 0x08, 0x03, 0x88, 0x02, 0x87, 0x01,
    0x80, 0x01, 0x00, 0x3c};

/* Keyboard information mode */
#define LAYERS_CURSOR_X 1
#define LAYERS_CURSOR_Y 0
#define LAYER_STR_LENGTH 5

#define SHIFT_CURSOR_X 0
#define CTRL_CURSOR_X 3
#define OPTN_CURSOR_X 6
#define CMD_CURSOR_X 9
#define MODS_CURSOR_Y 2
#define MODS_ICON_NUM_ROWS 2
#define MODS_ICON_NUM_COLS 3

// Modifier icons
static const char PROGMEM shift[2][4] = {
    {0x89, 0x8a, 0x20, 0x00},
    {0xa9, 0xaa, 0x20, 0x00}
};
static const char PROGMEM shift_pressed[2][4] = {
    {0x80, 0x81, 0x88, 0x00},
    {0xa0, 0xa1, 0xa8, 0x00}
};

static const char PROGMEM ctrl[2][4] = {
    {0x8b, 0x8c, 0x20, 0x00},
    {0xab, 0xac, 0x20, 0x00}
};
static const char PROGMEM ctrl_pressed[2][4] = {
    {0x82, 0x83, 0x88, 0x00},
    {0xa2, 0xa3, 0xa8, 0x00}
};

static const char PROGMEM optn[2][4] = {
    {0x8d, 0x8e, 0x20, 0x00},
    {0xad, 0xae, 0x20, 0x00}
};
static const char PROGMEM optn_pressed[2][4] = {
    {0x84, 0x85, 0x88, 0x00},
    {0xa4, 0xa5, 0xa8, 0x00}
};

static const char PROGMEM cmd[2][4] = {
    {0x8f, 0x90, 0x20, 0x00},
    {0xaf, 0xb0, 0x20, 0x00}
};
static const char PROGMEM cmd_pressed[2][4] = {
    {0x86, 0x87, 0x88, 0x00},
    {0xa6, 0xa7, 0xa8, 0x00}
};

// Layer indicator graphics
static const char PROGMEM layer_str[6] = {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0x00};
static const char PROGMEM layer_row_1[4][3] = {
    {0xc5, 0xc6, 0x00},
    {0xc7, 0xc8, 0x00},
    {0xc9, 0xca, 0x00},
    {0xcb, 0xcc, 0x00}
};
static const char PROGMEM layer_row_2[3] = {0xcd, 0xce, 0x00};

// Drawing graphics for matrix visualisation
#define MATRIX_KEY_WIDTH 3
#define MATRIX_CURSOR_X 82
#define MATRIX_CURSOR_Y 4
#define MATRIX_SIDE_PADDING 2
#define MATRIX_TOP_ROW_HORI_PADDING 3
#define MATRIX_OLED_WIDTH 8
#define MATRIX_OLED_HEIGHT 3

#define swap(a, b)     \
    {                  \
        uint8_t t = a; \
        a         = b; \
        b         = t; \
    }
