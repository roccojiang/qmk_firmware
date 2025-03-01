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

#pragma once

#define XXX KC_NO

#include "quantum.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, \
    K10, K11, K12, K13, K14, K15, K16, K17, K18, K19,      K1B, \
    K20,      K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, \
    K30, K31,      K33, K34,      K36,      K38, K39, K3A, K3B  \
) { \
    { K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B }, \
    { K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, XXX, K1B }, \
    { K20, XXX, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B }, \
    { K30, K31, XXX, K33, K34, XXX, K36, XXX, K38, K39, K3A, K3B }  \
}

/* OLED display modes */
typedef enum oled_mode {
    OLED_INFO,
    OLED_WPM,
    _NUM_OLED_MODES
} oled_mode_t;

extern oled_mode_t oled_mode;

/* Custom keycode for OLED display mode toggling */
enum kb_custom_keycodes {
    OLED_TOGG = SAFE_RANGE,
    NEW_SAFE_RANGE
};

#undef SAFE_RANGE
#define SAFE_RANGE NEW_SAFE_RANGE


