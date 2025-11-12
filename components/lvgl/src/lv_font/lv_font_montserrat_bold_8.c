/*******************************************************************************
 * Size: 8 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_MONTSERRAT_BOLD_8
#define LV_FONT_MONTSERRAT_BOLD_8 1
#endif

#if LV_FONT_MONTSERRAT_BOLD_8

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x8e, 0x7d, 0x7c, 0x6b, 0x59, 0x7c,

    /* U+0022 "\"" */
    0x97, 0xf1, 0x96, 0xe0, 0x53, 0x80,

    /* U+0023 "#" */
    0x5, 0x93, 0xa0, 0x9f, 0xff, 0xf9, 0x8, 0x57,
    0x60, 0xa, 0x39, 0x50, 0xdf, 0xff, 0xf5, 0xe,
    0xd, 0x10,

    /* U+0024 "$" */
    0x0, 0xc0, 0x2, 0xcf, 0xe5, 0x9c, 0xd5, 0x36,
    0xee, 0x40, 0x3, 0xee, 0xa5, 0x6d, 0xad, 0x6d,
    0xfd, 0x40, 0xc, 0x0,

    /* U+0025 "%" */
    0x6c, 0x90, 0x69, 0xc, 0x1d, 0x1d, 0x0, 0x5c,
    0x7a, 0xbc, 0x40, 0x4, 0xac, 0x1b, 0x0, 0xd1,
    0xc0, 0xb0, 0x86, 0x7, 0xc5,

    /* U+0026 "&" */
    0x9, 0xfc, 0x10, 0xf, 0x3d, 0x30, 0xb, 0xf9,
    0x0, 0x5d, 0xad, 0xb5, 0xab, 0x2c, 0xf3, 0x3d,
    0xfc, 0xa5,

    /* U+0027 "'" */
    0x97, 0x96, 0x53,

    /* U+0028 "(" */
    0xa, 0x90, 0xf3, 0x4f, 0x6, 0xd0, 0x6d, 0x6,
    0xd0, 0x3f, 0x0, 0xf3, 0xa, 0x90,

    /* U+0029 ")" */
    0xb8, 0x5, 0xe0, 0x2f, 0x10, 0xf3, 0xf, 0x40,
    0xf3, 0x2f, 0x15, 0xe0, 0xb8, 0x0,

    /* U+002A "*" */
    0x3a, 0x40, 0x8f, 0xd2, 0xad, 0xc2, 0x6, 0x10,

    /* U+002B "+" */
    0x0, 0x50, 0x0, 0x2e, 0x0, 0x7f, 0xff, 0x41,
    0x4e, 0x20, 0x1, 0x90, 0x0,

    /* U+002C "," */
    0x67, 0x9c, 0x67, 0x41,

    /* U+002D "-" */
    0x9f, 0xa1, 0x21,

    /* U+002E "." */
    0x67, 0x9a,

    /* U+002F "/" */
    0x0, 0x5, 0x30, 0x0, 0xe2, 0x0, 0x3d, 0x0,
    0x8, 0x80, 0x0, 0xd3, 0x0, 0x2e, 0x0, 0x7,
    0x90, 0x0, 0xc4, 0x0, 0x1f, 0x0, 0x0,

    /* U+0030 "0" */
    0x9, 0xfd, 0x30, 0x6f, 0x5b, 0xd0, 0xaa, 0x3,
    0xf1, 0xaa, 0x3, 0xf1, 0x6f, 0x5b, 0xd0, 0x9,
    0xfd, 0x30,

    /* U+0031 "1" */
    0xff, 0x72, 0xd7, 0xd, 0x70, 0xd7, 0xd, 0x70,
    0xd7,

    /* U+0032 "2" */
    0x5e, 0xfa, 0x7, 0x65, 0xf4, 0x0, 0x3f, 0x20,
    0x3e, 0x70, 0x3f, 0xa3, 0x1b, 0xff, 0xf8,

    /* U+0033 "3" */
    0xb, 0xff, 0xf3, 0x2, 0x3d, 0x90, 0x0, 0x7f,
    0xb0, 0x0, 0x13, 0xf6, 0x7, 0x55, 0xf5, 0x9,
    0xef, 0xa0,

    /* U+0034 "4" */
    0x0, 0x8c, 0x0, 0x5, 0xe4, 0x10, 0x3f, 0x3b,
    0x80, 0xbf, 0xff, 0xf7, 0x23, 0x3c, 0xa1, 0x0,
    0xb, 0x80,

    /* U+0035 "5" */
    0x4f, 0xff, 0x35, 0xd3, 0x30, 0x7f, 0xfb, 0x11,
    0x34, 0xe7, 0x65, 0x5e, 0x78, 0xef, 0xb0,

    /* U+0036 "6" */
    0x7, 0xef, 0x75, 0xf6, 0x32, 0xae, 0xdd, 0x4a,
    0xe3, 0x9d, 0x7e, 0x28, 0xd0, 0xaf, 0xe4,

    /* U+0037 "7" */
    0xcf, 0xff, 0xbc, 0x83, 0xf7, 0x31, 0x5f, 0x10,
    0xc, 0xa0, 0x2, 0xf3, 0x0, 0x9d, 0x0,

    /* U+0038 "8" */
    0x1c, 0xfd, 0x47, 0xe2, 0xac, 0x4f, 0xff, 0x8a,
    0xb0, 0x7e, 0x9c, 0x29, 0xe1, 0xcf, 0xd4,

    /* U+0039 "9" */
    0x4d, 0xfb, 0xc, 0xa2, 0xd8, 0x7f, 0xfe, 0xc0,
    0x11, 0x9b, 0x24, 0x5f, 0x66, 0xfe, 0x90,

    /* U+003A ":" */
    0x9a, 0x67, 0x0, 0x67, 0x9a,

    /* U+003B ";" */
    0x9a, 0x67, 0x0, 0x67, 0x9c, 0x67, 0x41,

    /* U+003C "<" */
    0x0, 0x1, 0x10, 0x3a, 0xf3, 0x7f, 0x71, 0x4,
    0xdd, 0x60, 0x0, 0x4b, 0x40, 0x0, 0x0,

    /* U+003D "=" */
    0x8f, 0xff, 0x40, 0x11, 0x10, 0x7f, 0xff, 0x41,
    0x22, 0x20,

    /* U+003E ">" */
    0x21, 0x0, 0x6, 0xf9, 0x20, 0x1, 0x8f, 0x41,
    0x8e, 0xb2, 0x79, 0x20, 0x0, 0x0, 0x0,

    /* U+003F "?" */
    0x5, 0xef, 0xb0, 0x7, 0x65, 0xf4, 0x0, 0x7,
    0xd0, 0x0, 0x2c, 0x10, 0x0, 0x29, 0x0, 0x0,
    0x4e, 0x0,

    /* U+0040 "@" */
    0x1, 0xae, 0xdc, 0x40, 0xd, 0x60, 0x2, 0xc4,
    0x68, 0x5e, 0xdf, 0x4c, 0xa4, 0xe4, 0x1f, 0x3d,
    0xa4, 0xe5, 0x2f, 0x4c, 0x68, 0x5e, 0xdb, 0xf5,
    0xd, 0x60, 0x2, 0x0, 0x1, 0xae, 0xeb, 0x0,

    /* U+0041 "A" */
    0x0, 0xc, 0xe0, 0x0, 0x0, 0x3e, 0xd5, 0x0,
    0x0, 0x99, 0x7b, 0x0, 0x1, 0xff, 0xff, 0x20,
    0x7, 0xd2, 0x2c, 0x90, 0xd, 0x70, 0x5, 0xf0,

    /* U+0042 "B" */
    0x5f, 0xff, 0xd2, 0x5f, 0x22, 0xe7, 0x5f, 0xff,
    0xf2, 0x5f, 0x22, 0xba, 0x5f, 0x12, 0xbb, 0x5f,
    0xff, 0xd3,

    /* U+0043 "C" */
    0x6, 0xdf, 0xc2, 0x4f, 0x84, 0xa4, 0xab, 0x0,
    0x0, 0xab, 0x0, 0x0, 0x4f, 0x84, 0xa4, 0x6,
    0xef, 0xc2,

    /* U+0044 "D" */
    0x5f, 0xff, 0xc2, 0x5, 0xf3, 0x4b, 0xe0, 0x5f,
    0x0, 0x1f, 0x45, 0xf0, 0x1, 0xf4, 0x5f, 0x34,
    0xbe, 0x5, 0xff, 0xfc, 0x20,

    /* U+0045 "E" */
    0x5f, 0xff, 0xe5, 0xf3, 0x32, 0x5f, 0xff, 0x85,
    0xf2, 0x21, 0x5f, 0x33, 0x35, 0xff, 0xff,

    /* U+0046 "F" */
    0x5f, 0xff, 0xe5, 0xf3, 0x32, 0x5f, 0xff, 0x85,
    0xf3, 0x31, 0x5f, 0x0, 0x5, 0xf0, 0x0,

    /* U+0047 "G" */
    0x6, 0xdf, 0xc2, 0x4f, 0x84, 0x95, 0xab, 0x0,
    0x0, 0xab, 0x0, 0x8a, 0x4f, 0x84, 0xba, 0x6,
    0xdf, 0xc4,

    /* U+0048 "H" */
    0x5f, 0x0, 0x8c, 0x5f, 0x0, 0x8c, 0x5f, 0xff,
    0xfc, 0x5f, 0x33, 0x9c, 0x5f, 0x0, 0x8c, 0x5f,
    0x0, 0x8c,

    /* U+0049 "I" */
    0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f,

    /* U+004A "J" */
    0x8, 0xff, 0xb0, 0x13, 0xab, 0x0, 0x9, 0xb0,
    0x0, 0x9b, 0x8, 0x4c, 0xa0, 0xaf, 0xd3,

    /* U+004B "K" */
    0x5f, 0x1, 0xe7, 0x5, 0xf1, 0xd9, 0x0, 0x5f,
    0xcc, 0x0, 0x5, 0xfe, 0xf3, 0x0, 0x5f, 0x1a,
    0xd0, 0x5, 0xf0, 0xd, 0xa0,

    /* U+004C "L" */
    0x5f, 0x0, 0x5, 0xf0, 0x0, 0x5f, 0x0, 0x5,
    0xf0, 0x0, 0x5f, 0x33, 0x25, 0xff, 0xfc,

    /* U+004D "M" */
    0x5f, 0x10, 0x6, 0xf5, 0xf9, 0x0, 0xef, 0x5e,
    0xe3, 0x8d, 0xf5, 0xe7, 0xde, 0x5f, 0x5e, 0xe,
    0x83, 0xf5, 0xe0, 0x31, 0x3f,

    /* U+004E "N" */
    0x5f, 0x20, 0x7c, 0x5f, 0xc0, 0x7c, 0x5f, 0xc8,
    0x7c, 0x5f, 0x2e, 0xbc, 0x5f, 0x5, 0xfc, 0x5f,
    0x0, 0xac,

    /* U+004F "O" */
    0x5, 0xdf, 0xc3, 0x4, 0xf8, 0x4a, 0xf1, 0xab,
    0x0, 0xf, 0x6a, 0xb0, 0x0, 0xf6, 0x4f, 0x84,
    0xaf, 0x10, 0x6d, 0xfc, 0x30,

    /* U+0050 "P" */
    0x5f, 0xff, 0xa0, 0x5f, 0x34, 0xf6, 0x5f, 0x1,
    0xe7, 0x5f, 0xff, 0xd1, 0x5f, 0x32, 0x0, 0x5f,
    0x0, 0x0,

    /* U+0051 "Q" */
    0x5, 0xdf, 0xc3, 0x4, 0xf8, 0x4a, 0xf1, 0xab,
    0x0, 0xf, 0x6a, 0xb0, 0x0, 0xf6, 0x4f, 0x84,
    0xaf, 0x10, 0x6e, 0xfe, 0x30, 0x0, 0x7, 0xff,
    0x70, 0x0, 0x0, 0x10,

    /* U+0052 "R" */
    0x5f, 0xff, 0xa0, 0x5f, 0x34, 0xf6, 0x5f, 0x1,
    0xe7, 0x5f, 0xff, 0xd1, 0x5f, 0x3b, 0xb0, 0x5f,
    0x1, 0xf5,

    /* U+0053 "S" */
    0x2c, 0xfe, 0x59, 0xc3, 0x53, 0x6e, 0x84, 0x0,
    0x38, 0xea, 0x56, 0x3a, 0xd6, 0xdf, 0xd4,

    /* U+0054 "T" */
    0xff, 0xff, 0xe3, 0x5f, 0x53, 0x2, 0xf2, 0x0,
    0x2f, 0x20, 0x2, 0xf2, 0x0, 0x2f, 0x20,

    /* U+0055 "U" */
    0x6e, 0x0, 0x9b, 0x6e, 0x0, 0x9b, 0x6e, 0x0,
    0x9b, 0x6f, 0x0, 0xaa, 0x2f, 0x75, 0xe7, 0x7,
    0xef, 0xa0,

    /* U+0056 "V" */
    0xd, 0x80, 0x7, 0xd0, 0x7, 0xe0, 0xd, 0x70,
    0x1, 0xf5, 0x4f, 0x10, 0x0, 0xab, 0xaa, 0x0,
    0x0, 0x4f, 0xf3, 0x0, 0x0, 0xd, 0xd0, 0x0,

    /* U+0057 "W" */
    0xaa, 0x1, 0xf7, 0x4, 0xf0, 0x5f, 0x5, 0xfc,
    0x9, 0xa0, 0x1f, 0x4a, 0x8f, 0x1e, 0x50, 0xc,
    0x9f, 0x2c, 0x9f, 0x10, 0x7, 0xfd, 0x8, 0xfc,
    0x0, 0x2, 0xf8, 0x3, 0xf7, 0x0,

    /* U+0058 "X" */
    0x8e, 0x3, 0xf3, 0xd, 0x9c, 0x80, 0x3, 0xfd,
    0x0, 0x4, 0xfe, 0x10, 0x1e, 0x7c, 0xa0, 0xad,
    0x2, 0xf5,

    /* U+0059 "Y" */
    0xd, 0x80, 0x1f, 0x40, 0x4f, 0x29, 0xb0, 0x0,
    0xbc, 0xf2, 0x0, 0x2, 0xf9, 0x0, 0x0, 0xf,
    0x50, 0x0, 0x0, 0xf5, 0x0,

    /* U+005A "Z" */
    0xaf, 0xff, 0xf1, 0x23, 0x4e, 0xa0, 0x0, 0xad,
    0x0, 0x7, 0xe2, 0x0, 0x4f, 0x73, 0x30, 0xbf,
    0xff, 0xf3,

    /* U+005B "[" */
    0x5f, 0xd5, 0xe1, 0x5e, 0x5, 0xe0, 0x5e, 0x5,
    0xe0, 0x5e, 0x5, 0xe1, 0x5f, 0xd0,

    /* U+005C "\\" */
    0x26, 0x0, 0x0, 0xf1, 0x0, 0xb, 0x60, 0x0,
    0x6b, 0x0, 0x1, 0xf0, 0x0, 0xc, 0x50, 0x0,
    0x7a, 0x0, 0x2, 0xe0, 0x0, 0xd, 0x40,

    /* U+005D "]" */
    0xef, 0x41, 0xf4, 0xf, 0x40, 0xf4, 0xf, 0x40,
    0xf4, 0xf, 0x41, 0xf4, 0xef, 0x40,

    /* U+005E "^" */
    0x4, 0xf1, 0x0, 0xac, 0x70, 0xe, 0x2c, 0x5,
    0x90, 0xc2,

    /* U+005F "_" */
    0xdd, 0xdd,

    /* U+0060 "`" */
    0x7, 0x10, 0x5, 0xb0,

    /* U+0061 "a" */
    0x4e, 0xfa, 0x2, 0x43, 0xf4, 0x4d, 0xdf, 0x6a,
    0x90, 0xe6, 0x4e, 0xde, 0x60,

    /* U+0062 "b" */
    0x6d, 0x0, 0x0, 0x6d, 0x0, 0x0, 0x6e, 0xdf,
    0x60, 0x6f, 0x47, 0xf1, 0x6d, 0x0, 0xf4, 0x6f,
    0x57, 0xf1, 0x6e, 0xdf, 0x60,

    /* U+0063 "c" */
    0x1b, 0xfd, 0x28, 0xd3, 0x83, 0xb8, 0x0, 0x8,
    0xd3, 0x83, 0x1b, 0xfd, 0x20,

    /* U+0064 "d" */
    0x0, 0x4, 0xf0, 0x0, 0x4f, 0x1c, 0xfc, 0xf9,
    0xd3, 0xaf, 0xb9, 0x4, 0xf8, 0xd3, 0xaf, 0x1b,
    0xfc, 0xf0,

    /* U+0065 "e" */
    0x1b, 0xfc, 0x19, 0xc2, 0xa9, 0xbe, 0xdd, 0xb8,
    0xd3, 0x51, 0x1b, 0xfd, 0x30,

    /* U+0066 "f" */
    0xb, 0xf3, 0x3f, 0x30, 0xff, 0xf0, 0x5f, 0x10,
    0x4f, 0x0, 0x4f, 0x0, 0x4f, 0x0,

    /* U+0067 "g" */
    0x1c, 0xfb, 0xf0, 0x8d, 0x29, 0xf0, 0xb8, 0x4,
    0xf0, 0x9c, 0x8, 0xf0, 0x2e, 0xfe, 0xf0, 0x15,
    0x49, 0xd0, 0x3d, 0xfd, 0x30,

    /* U+0068 "h" */
    0x6d, 0x0, 0x6, 0xd0, 0x0, 0x6e, 0xdf, 0x66,
    0xf5, 0x9e, 0x6d, 0x4, 0xf6, 0xd0, 0x4f, 0x6d,
    0x4, 0xf0,

    /* U+0069 "i" */
    0x7d, 0x36, 0x6d, 0x6d, 0x6d, 0x6d, 0x6d,

    /* U+006A "j" */
    0x6, 0xe0, 0x2, 0x70, 0x6, 0xe0, 0x6, 0xe0,
    0x6, 0xe0, 0x6, 0xe0, 0x6, 0xe0, 0x18, 0xd0,
    0x9f, 0x60,

    /* U+006B "k" */
    0x6d, 0x0, 0x0, 0x6d, 0x0, 0x0, 0x6d, 0xc,
    0xb0, 0x6d, 0xbd, 0x0, 0x6f, 0xf9, 0x0, 0x6f,
    0x6f, 0x40, 0x6d, 0x9, 0xe1,

    /* U+006C "l" */
    0x6d, 0x6d, 0x6d, 0x6d, 0x6d, 0x6d, 0x6d,

    /* U+006D "m" */
    0x6e, 0xde, 0x8e, 0xe4, 0x6f, 0x5b, 0xf5, 0xbc,
    0x6d, 0x6, 0xd0, 0x6d, 0x6d, 0x6, 0xd0, 0x6d,
    0x6d, 0x6, 0xd0, 0x6d,

    /* U+006E "n" */
    0x6d, 0xdf, 0x66, 0xf5, 0x9e, 0x6d, 0x4, 0xf6,
    0xd0, 0x4f, 0x6d, 0x4, 0xf0,

    /* U+006F "o" */
    0x1b, 0xfd, 0x28, 0xd3, 0xac, 0xb8, 0x5, 0xf8,
    0xd3, 0xac, 0x1b, 0xfd, 0x20,

    /* U+0070 "p" */
    0x6e, 0xdf, 0x60, 0x6f, 0x47, 0xf1, 0x6d, 0x0,
    0xf4, 0x6f, 0x57, 0xf1, 0x6e, 0xdf, 0x60, 0x6d,
    0x0, 0x0, 0x6d, 0x0, 0x0,

    /* U+0071 "q" */
    0x1c, 0xfc, 0xf9, 0xd3, 0xaf, 0xb8, 0x5, 0xf9,
    0xd3, 0xbf, 0x1c, 0xfc, 0xf0, 0x0, 0x4f, 0x0,
    0x4, 0xf0,

    /* U+0072 "r" */
    0x6e, 0xd3, 0x6f, 0x60, 0x6d, 0x0, 0x6d, 0x0,
    0x6d, 0x0,

    /* U+0073 "s" */
    0x4e, 0xfa, 0xb, 0xa1, 0x30, 0x5e, 0xf9, 0x3,
    0x26, 0xf0, 0x8f, 0xf8, 0x0,

    /* U+0074 "t" */
    0x2, 0x0, 0x4f, 0x0, 0xff, 0xf0, 0x5f, 0x10,
    0x4f, 0x0, 0x3f, 0x30, 0xc, 0xf3,

    /* U+0075 "u" */
    0x7c, 0x5, 0xe7, 0xc0, 0x5e, 0x7c, 0x5, 0xe5,
    0xf4, 0xbe, 0xb, 0xfc, 0xe0,

    /* U+0076 "v" */
    0xe, 0x60, 0x8a, 0x8, 0xb0, 0xe4, 0x2, 0xf5,
    0xe0, 0x0, 0xcf, 0x90, 0x0, 0x6f, 0x30,

    /* U+0077 "w" */
    0xd5, 0xe, 0x70, 0xc5, 0x8a, 0x3f, 0xc1, 0xf0,
    0x3e, 0x8a, 0xf7, 0xb0, 0xe, 0xf3, 0xbf, 0x60,
    0x9, 0xe0, 0x6f, 0x10,

    /* U+0078 "x" */
    0x9c, 0x1e, 0x50, 0xed, 0xa0, 0x7, 0xf3, 0x1,
    0xed, 0xc0, 0xab, 0x1e, 0x60,

    /* U+0079 "y" */
    0xe, 0x60, 0x8a, 0x8, 0xb0, 0xe4, 0x2, 0xf5,
    0xe0, 0x0, 0xce, 0x90, 0x0, 0x6f, 0x30, 0x3,
    0x7d, 0x0, 0xd, 0xe4, 0x0,

    /* U+007A "z" */
    0xaf, 0xff, 0x1, 0x1d, 0x90, 0x8, 0xd0, 0x4,
    0xf4, 0x10, 0xbf, 0xff, 0x20,

    /* U+007B "{" */
    0x8, 0xf0, 0xf, 0x70, 0xf, 0x40, 0xf, 0x40,
    0x8f, 0x10, 0x1f, 0x40, 0xf, 0x40, 0xf, 0x70,
    0x8, 0xf0,

    /* U+007C "|" */
    0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d,
    0x5d,

    /* U+007D "}" */
    0xda, 0x5, 0xf1, 0x2f, 0x12, 0xf1, 0xe, 0xb2,
    0xf3, 0x2f, 0x15, 0xf1, 0xda, 0x0,

    /* U+007E "~" */
    0x2d, 0x67, 0x58, 0x6b, 0xe1, 0x10, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 36, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 37, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 56, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 12, .adv_w = 92, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 30, .adv_w = 82, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 50, .adv_w = 112, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 71, .adv_w = 93, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 29, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 92, .adv_w = 46, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 106, .adv_w = 46, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 120, .adv_w = 56, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 128, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 141, .adv_w = 34, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 145, .adv_w = 49, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 148, .adv_w = 34, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 150, .adv_w = 50, .box_w = 5, .box_h = 9, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 173, .adv_w = 87, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 50, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 76, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 76, .box_w = 6, .box_h = 6, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 88, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 76, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 266, .adv_w = 82, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 79, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 84, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 82, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 34, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 34, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 338, .adv_w = 77, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 77, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 363, .adv_w = 77, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 75, .box_w = 6, .box_h = 6, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 396, .adv_w = 132, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 428, .adv_w = 98, .box_w = 8, .box_h = 6, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 98, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 470, .adv_w = 94, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 488, .adv_w = 106, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 509, .adv_w = 86, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 524, .adv_w = 82, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 539, .adv_w = 99, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 557, .adv_w = 103, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 42, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 581, .adv_w = 69, .box_w = 5, .box_h = 6, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 95, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 617, .adv_w = 77, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 632, .adv_w = 122, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 653, .adv_w = 103, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 671, .adv_w = 108, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 692, .adv_w = 94, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 710, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 738, .adv_w = 94, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 756, .adv_w = 82, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 771, .adv_w = 79, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 786, .adv_w = 101, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 804, .adv_w = 95, .box_w = 8, .box_h = 6, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 828, .adv_w = 149, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 858, .adv_w = 91, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 876, .adv_w = 87, .box_w = 7, .box_h = 6, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 897, .adv_w = 86, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 915, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 929, .adv_w = 50, .box_w = 5, .box_h = 9, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 952, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 966, .adv_w = 77, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 976, .adv_w = 64, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 978, .adv_w = 77, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 982, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 995, .adv_w = 88, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1016, .adv_w = 76, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1029, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1047, .adv_w = 81, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1060, .adv_w = 50, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1074, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1095, .adv_w = 88, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1113, .adv_w = 39, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1120, .adv_w = 39, .box_w = 4, .box_h = 9, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1138, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1159, .adv_w = 39, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1166, .adv_w = 134, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1186, .adv_w = 88, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1199, .adv_w = 84, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1212, .adv_w = 88, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1233, .adv_w = 88, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1251, .adv_w = 55, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1261, .adv_w = 68, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1274, .adv_w = 56, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1288, .adv_w = 88, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1301, .adv_w = 77, .box_w = 6, .box_h = 5, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 1316, .adv_w = 120, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1336, .adv_w = 76, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1349, .adv_w = 77, .box_w = 6, .box_h = 7, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1370, .adv_w = 70, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1383, .adv_w = 50, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1401, .adv_w = 40, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1410, .adv_w = 50, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1424, .adv_w = 77, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 0, 13, 14, 15, 16, 17,
    18, 19, 12, 20, 20, 0, 0, 0,
    21, 22, 23, 24, 25, 22, 26, 27,
    28, 29, 29, 30, 31, 32, 29, 29,
    22, 33, 34, 35, 3, 36, 30, 37,
    37, 38, 39, 40, 41, 42, 43, 0,
    44, 0, 45, 46, 47, 48, 49, 50,
    51, 45, 52, 52, 53, 48, 45, 45,
    46, 46, 54, 55, 56, 57, 51, 58,
    58, 59, 58, 60, 41, 0, 0, 9
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 12,
    18, 19, 20, 21, 21, 0, 0, 0,
    22, 23, 24, 25, 23, 25, 25, 25,
    23, 25, 25, 26, 25, 25, 25, 25,
    23, 25, 23, 25, 3, 27, 28, 29,
    29, 30, 31, 32, 33, 34, 35, 0,
    36, 0, 37, 38, 39, 39, 39, 40,
    39, 38, 41, 42, 38, 38, 43, 43,
    39, 43, 39, 43, 44, 45, 46, 47,
    47, 48, 49, 50, 0, 0, 35, 9
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 3, 3, 0, 1, 0, 3, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 6, 0, 3, -2, 0,
    0, 3, 0, -7, -8, 1, 6, 2,
    2, -5, 1, 5, 0, 5, 1, 3,
    0, -5, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 8, 2, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, 0, 0, -3, 2, 3, 0, 0,
    -1, 0, -1, 1, 0, -1, 0, -1,
    -1, -3, 0, 0, 0, 0, -1, 0,
    0, -2, -2, 0, 0, -1, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -1, -1, 0, 0, -3,
    0, -16, 0, 0, -3, -6, 3, 4,
    0, 0, -3, 1, 1, 4, 3, -2,
    3, 0, 0, -7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 1, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, -5, 0, -5, -1, 0, 0, -4,
    0, -1, 4, 0, -4, -2, -1, 1,
    0, -2, 0, 0, -1, -8, 0, 1,
    0, 3, -3, 0, -3, 0, -5, 1,
    0, -10, -2, 3, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 0, -2, -1,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 5, 0, 1, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 6, 2, 1, 0, 0,
    0, 0, 0, 13, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 3,
    1, 4, -1, 0, 0, 3, -1, -5,
    -17, 1, 3, 3, 0, -2, 0, 3,
    0, 3, 0, 3, 0, -12, 0, -2,
    4, 0, 4, -1, 3, 1, 0, 0,
    1, -1, 0, 0, -2, 0, 0, 0,
    10, 0, 4, 0, 5, 2, 5, 2,
    0, 0, -2, -4, 0, 0, 0, -1,
    1, -1, 0, 1, -3, -2, -3, 1,
    0, -1, 0, 0, 0, -5, 1, -3,
    0, -3, -5, 0, -3, -3, -4, 0,
    0, -8, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 1, 0, -2, -3,
    -2, -2, 1, -7, 1, -8, 0, 0,
    0, -5, -1, 0, 12, -2, -2, 1,
    1, 0, 0, -2, 1, 0, 0, -7,
    -3, 4, 0, 7, -4, -1, -5, 0,
    -5, 2, 0, -12, 0, 1, 1, 0,
    -1, 0, 0, 1, 0, 0, -1, -1,
    -4, 0, -4, 0, 0, 8, -3, 0,
    -4, 0, 5, 0, -9, -12, -9, -3,
    4, 0, 0, -8, 0, 1, -3, 0,
    -2, 0, -3, -5, 0, -1, 4, 0,
    4, 0, 4, 0, 0, 2, 4, -15,
    -8, 0, -8, 0, 2, 1, -8, -8,
    -3, -8, -4, -6, -4, -8, 0, 1,
    0, 0, 0, 0, 0, 1, 1, -2,
    -3, 0, -1, -1, -1, 0, 0, -1,
    0, 0, 0, -3, 0, -1, 0, -3,
    -3, 0, -3, -5, -5, -2, 0, -3,
    0, -3, 0, 0, 0, 0, 0, -1,
    0, 0, 1, 0, 1, -1, 1, 0,
    0, 0, 0, 1, -1, 0, 0, 0,
    -1, 1, 1, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 2, -1, 0, -2, 0, -2, 0,
    0, -1, 0, 4, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, 0, -1, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    -1, 0, -1, -1, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, -1, -1,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    -1, -2, -1, 0, 0, -4, -1, -4,
    3, 0, 0, -3, 1, 3, 3, 0,
    -3, 0, -1, 0, 0, -5, 1, -1,
    1, -7, 1, 0, 0, 1, -7, 0,
    -7, -1, -11, -1, 0, -6, 0, 3,
    3, 0, 2, 0, 0, 0, 0, 0,
    0, 0, -3, -2, -3, 0, 0, 0,
    0, -1, 0, 0, 0, -1, 0, 0,
    0, 0, 0, -1, -1, 0, -1, -2,
    0, 0, 0, 0, 0, 0, 0, -1,
    -1, 0, -1, -2, -1, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, -1, 0,
    0, -1, 0, -3, 1, 0, 0, -1,
    1, 1, 1, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 1,
    0, 0, -1, 0, -1, -1, -1, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    0, -1, 0, 0, 0, 0, -2, -2,
    -2, 0, 0, 4, -1, 1, -3, 0,
    0, 3, -6, -6, -5, -3, 1, 0,
    -1, -8, -2, 0, -2, 0, -3, 2,
    -2, -8, 0, -3, 0, 0, 1, 0,
    1, -1, 0, 1, 1, -4, -5, 0,
    -6, 0, 0, -3, -3, -4, -1, -3,
    1, -1, 1, -3, 0, 0, 0, -1,
    0, 0, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, -1, 0, 0, -1, 0,
    -2, -3, -3, 0, 0, -4, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    7, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, -1, 0, 1, 0, 4,
    -1, 0, -3, -1, -5, 0, 0, -3,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 1, -1, 1, 0,
    -1, 0, 0, 0, -1, 0, 0, 1,
    0, -6, -4, 0, 0, 0, -2, -6,
    0, 0, -1, 1, 0, -2, -1, -6,
    0, -4, 0, 0, -2, -3, -2, -1,
    -1, 0, 0, -2, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 0, -2, 0, 0, 0, 0,
    1, 0, 1, -3, -3, 0, -1, -1,
    -2, 0, 0, 0, 0, 0, 0, -4,
    0, -1, 0, -2, -1, 0, -3, -3,
    -4, -1, 0, -3, 1, -4, 0, 0,
    0, 0, 0, 10, 0, 0, 1, 0,
    0, -2, 0, 0, 0, -5, 0, 0,
    0, 0, 0, -12, -3, 4, 4, -1,
    -6, 0, 1, -2, 0, -6, -1, -2,
    1, -9, -1, 3, 0, 2, -4, -2,
    -5, -5, -6, 0, 0, -8, 0, 7,
    0, 0, -1, 0, 0, 0, 0, -1,
    -1, -1, -4, -5, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, -1, -1, -2, 0, 0, -3,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -3, 0, 0, 3,
    -1, 2, 0, -2, 1, -1, 0, -2,
    -1, 0, -2, -1, -1, 0, -2, -2,
    0, 0, -1, -1, -1, -2, -1, 0,
    0, -1, 0, 1, -1, 0, -2, 0,
    0, 0, 0, -3, 0, -2, 0, -2,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 1, 0, -1, 0, -1,
    -1, -2, -1, -1, -1, 0, -1, -1,
    0, 0, 0, 0, 0, 0, -1, -1,
    -1, 0, 0, 0, 0, 1, -1, 0,
    -1, 0, 0, 0, 0, -1, -1, -1,
    -1, -1, -1, -1, 2, 5, 0, 0,
    -3, 0, -1, 3, 0, -1, -5, -2,
    2, 1, 0, -5, -2, 1, -2, 1,
    0, 0, -1, -4, 0, -2, 1, 0,
    0, -2, 0, 0, 0, 1, 1, -3,
    -2, 0, -2, 0, 0, -2, -1, -1,
    0, -2, 1, -2, 1, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, -2, 0, 0, -1,
    -1, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, -2, 0,
    -3, 0, 0, 0, -5, 0, 1, -3,
    3, 0, -1, -6, 0, 0, -3, -1,
    0, -5, -3, -4, 0, 0, -6, -1,
    -5, -5, -7, 0, -2, 0, 2, 8,
    -2, 0, -3, 0, 0, 0, -1, -2,
    -3, -2, -5, -6, -5, -3, 0, 0,
    -1, 0, 1, 0, 0, -9, 0, 4,
    2, -2, -4, 0, 1, -2, 0, -6,
    -1, -1, 3, -11, -2, 1, 0, 0,
    -8, -1, -6, -1, -9, 0, 0, -9,
    0, 6, 1, 0, -1, 0, 0, 0,
    0, 0, -1, -1, -5, -1, -5, 0,
    0, 0, 0, 0, -4, 0, -1, 0,
    -1, -3, -6, 0, 0, -1, -2, -4,
    -1, 0, -1, 0, 0, 0, 0, -6,
    -1, -4, -4, -2, -2, -3, -1, -2,
    0, -3, -1, -5, -2, 0, -1, 0,
    0, -1, -1, 0, 1, 0, -1, -4,
    -1, 0, 0, -2, 0, 0, 0, 0,
    1, 0, 1, -3, 6, 0, -1, -1,
    -2, 0, 0, 0, 0, 0, 0, -4,
    0, -1, 0, -2, -1, 0, -3, -3,
    -4, -1, 0, -3, 2, 5, 0, 0,
    0, 0, 0, 10, 0, 0, 1, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -3, 0, 0, 0, 0,
    0, -1, 0, 0, 0, -1, -1, 0,
    0, -3, -1, 0, 0, -3, 0, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 3, 2,
    -1, 0, -3, -1, 0, 4, -5, -4,
    -3, -3, 5, 3, 1, -11, -1, 3,
    -1, 0, -1, 3, -1, -4, 0, -1,
    1, -2, -2, -4, -2, 0, 0, 4,
    3, 0, -3, 0, -7, 0, 0, 6,
    -2, -5, 1, -2, -4, -4, -4, -1,
    1, 0, -2, 0, -3, 0, 2, 4,
    -3, -5, -5, -3, 4, 0, 1, -10,
    -1, 1, -2, -1, -3, 0, -3, -5,
    -2, -2, -2, 0, 0, -3, -3, -1,
    0, 4, 3, -1, -7, 0, -7, 0,
    -1, -3, -4, -8, -1, -4, -2, -4,
    -2, -4, 0, 0, -2, 0, -3, -1,
    0, -1, -3, 0, 2, -5, 1, 0,
    0, -7, 0, -1, -3, -2, -1, -4,
    -3, -5, -3, 0, -4, -1, -3, -1,
    -4, -1, 0, 0, 1, 6, -2, 0,
    -4, 0, 0, 0, -1, -3, -3, -4,
    -4, -6, -4, -2, 3, 0, -2, 0,
    -6, -1, 1, 3, -4, -5, -3, -5,
    5, -1, 1, -12, -3, 3, -3, -2,
    -5, 0, -4, -6, -1, -1, -2, -1,
    -3, -4, -1, 0, 0, 4, 4, -1,
    -8, 0, -8, 0, -1, 5, -5, -9,
    -3, -4, -6, -6, -6, -5, 0, 0,
    0, 0, -1, 0, 0, 1, -1, 3,
    1, -2, 3, 0, 0, -2, 0, 0,
    0, 0, 1, 1, -1, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    2, 4, 0, 0, -1, 0, 0, 0,
    0, 0, -1, -1, -1, 0, -1, 0,
    1, 2, 0, 0, 0, 0, 2, 1,
    -1, 0, 6, 0, 3, 1, 1, -2,
    0, 3, 0, 0, 0, 2, 0, 0,
    0, 0, 3, 0, 3, 1, 4, 0,
    0, 4, 0, 4, -1, 0, 0, 0,
    0, 13, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -8, 0, -1, 2, 0,
    4, -17, 0, 12, 1, -3, -3, 1,
    1, -1, 1, -6, 0, 0, 7, -8,
    -3, 4, 0, 4, -3, -1, -5, 3,
    -3, 0, 0, -9, 6, 18, 0, 0,
    0, 0, 0, 15, 0, 0, 0, 0,
    3, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, -1, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    3, -3, 0, 0, 1, -1, 0, 1,
    15, -3, -1, 3, 3, -3, 1, 0,
    0, 1, 1, -2, -4, 7, 4, 10,
    0, -1, -1, 6, -1, 3, 0, -17,
    4, 0, -1, 0, -3, 0, 0, 14,
    0, 1, -3, -3, -2, 4, 3, 2,
    0, 0, 0, -3, 0, 0, 0, -3,
    0, 0, 0, 0, -2, -1, 0, 0,
    0, -2, 0, -1, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -1, 0, 0, -2, 0, -3, 0, 0,
    0, -2, 1, -1, 0, 0, -3, -1,
    -3, 0, 0, -3, 0, -1, 0, -6,
    0, -3, 0, 0, -8, -1, -5, -3,
    -5, 0, 0, -8, 0, -3, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, -2, -2, -1, 0, 0, 0, 0,
    -2, 0, -3, 2, -3, 3, 0, -1,
    -3, -1, -2, -1, 0, -1, -1, -1,
    1, -3, -1, 0, 0, 0, -9, -2,
    -3, 0, -5, 0, -1, -6, -1, 0,
    0, -1, -1, 0, -1, 0, 0, 0,
    1, 0, -1, -2, -1, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 5, 0, 0, 0, 0, 0,
    0, -2, 0, -1, 0, 0, 0, -3,
    1, 0, 0, 0, -3, -1, -3, 0,
    0, -3, 0, -1, 0, -6, 0, 0,
    0, 0, -12, 0, -3, -5, -6, 0,
    0, -8, 0, -1, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, -2,
    -1, -1, 1, 0, 0, 2, -2, 0,
    5, 5, -1, -1, -4, 1, 5, 2,
    2, -3, 1, 5, 1, 3, 2, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 8, 6, -3, -1, 1,
    -1, 1, 1, 6, 1, 0, 0, 0,
    1, 0, 1, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, -8, -1,
    -2, -4, -5, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 0, 0, 0, 0,
    -8, -1, -2, -4, -5, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    -3, 1, 0, -1, 2, 3, 1, -4,
    0, 1, -1, 1, 0, 2, 0, 0,
    0, 0, -1, 0, -1, -1, -3, 0,
    -1, -5, 0, 7, -1, 0, -3, 0,
    0, 0, -1, -2, 0, -1, -4, -3,
    -4, -2, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 3,
    0, 0, 0, 0, -8, -1, -2, -4,
    -5, 0, 0, -8, 0, 0, 0, 0,
    0, 0, 0, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -3, -1, -1, 4, -1, -1, -5, 1,
    1, 1, -1, -3, 0, 3, 0, 2,
    1, 2, -3, -6, -1, 0, -3, -1,
    -3, -5, -4, 0, -1, -3, -1, -2,
    -5, -1, -1, 0, -1, 0, -1, 0,
    2, 0, 2, -1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -1, -1, 0, 0, -3,
    0, -1, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, -1, 0,
    0, 0, 0, 0, -1, 0, 0, -2,
    -1, 1, 0, -2, -2, -1, 0, -3,
    -1, -2, -1, -1, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 3, 0, 0, -2, 0,
    0, 0, 0, 0, -2, 0, -1, 0,
    -1, 0, 0, 0, -1, 0, -3, 0,
    0, 5, -2, -4, -4, 1, 3, 3,
    1, -4, 1, 2, 1, 4, 1, 5,
    -1, -4, 0, 0, -2, 0, 0, -4,
    -3, 0, 0, -3, 0, -2, -2, 0,
    -2, 0, 0, -2, 0, -1, 2, 0,
    -2, -4, -2, -1, 0, 0, -1, 0,
    -3, 0, 0, 2, -3, 0, 1, -1,
    2, 1, 0, -5, 0, -1, -1, 0,
    -1, 3, -2, 0, 0, 0, -3, -1,
    -3, 0, -4, 0, 0, -6, 0, 4,
    -1, 0, -2, 0, 0, 3, 0, -1,
    0, -1, -4, 0, -4, -1, 0, 0,
    0, 0, -1, 0, 0, 1, -2, 1,
    0, 0, -1, -1, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -7,
    0, 2, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 0, -1, -1, -1, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 60,
    .right_class_cnt     = 50,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t lv_font_montserrat_bold_8 = {
#else
lv_font_t lv_font_montserrat_bold_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_MONTSERRAT_BOLD_8*/

