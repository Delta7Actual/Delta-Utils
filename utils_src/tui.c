/* =====================================================================
 *
 * This module is a part of:
 * "deltautils.h" - General-purpose utility library for C
 *
 * Source Code: https://github.com/Delta7Actual/Delta-Utils
 * Created and maintained by Dror Sheffer
 *
 * Licensed under the MIT License.
 * See the accompanying LICENSE file for full terms.
 *
 * =====================================================================
 */


#include <stdio.h>
#include <stdint.h>


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


#define DU_TUI_RESET       "\x1b[0m"   // Reset all attributes
#define DU_TUI_BOLD        "\x1b[1m"   // Bold text
#define DU_TUI_DIM         "\x1b[2m"   // Dim text
#define DU_TUI_UNDERLINE   "\x1b[4m"   // Underlined text
#define DU_TUI_BLINK       "\x1b[5m"   // Blinking text
#define DU_TUI_REVERSE     "\x1b[7m"   // Reverse foreground/background
#define DU_TUI_HIDDEN      "\x1b[8m"   // Hidden text

#define DU_TUI_FG_BLACK    "\x1b[30m"
#define DU_TUI_FG_RED      "\x1b[31m"
#define DU_TUI_FG_GREEN    "\x1b[32m"
#define DU_TUI_FG_YELLOW   "\x1b[33m"
#define DU_TUI_FG_BLUE     "\x1b[34m"
#define DU_TUI_FG_MAGENTA  "\x1b[35m"
#define DU_TUI_FG_CYAN     "\x1b[36m"
#define DU_TUI_FG_WHITE    "\x1b[37m"

#define DU_TUI_FG_BBLACK   "\x1b[90m"
#define DU_TUI_FG_BRED     "\x1b[91m"
#define DU_TUI_FG_BGREEN   "\x1b[92m"
#define DU_TUI_FG_BYELLOW  "\x1b[93m"
#define DU_TUI_FG_BBLUE    "\x1b[94m"
#define DU_TUI_FG_BMAGENTA "\x1b[95m"
#define DU_TUI_FG_BCYAN    "\x1b[96m"
#define DU_TUI_FG_BWHITE   "\x1b[97m"

#define DU_TUI_BG_BLACK    "\x1b[40m"
#define DU_TUI_BG_RED      "\x1b[41m"
#define DU_TUI_BG_GREEN    "\x1b[42m"
#define DU_TUI_BG_YELLOW   "\x1b[43m"
#define DU_TUI_BG_BLUE     "\x1b[44m"
#define DU_TUI_BG_MAGENTA  "\x1b[45m"
#define DU_TUI_BG_CYAN     "\x1b[46m"
#define DU_TUI_BG_WHITE    "\x1b[47m"

#define DU_TUI_BG_BBLACK   "\x1b[100m"
#define DU_TUI_BG_BRED     "\x1b[101m"
#define DU_TUI_BG_BGREEN   "\x1b[102m"
#define DU_TUI_BG_BYELLOW  "\x1b[103m"
#define DU_TUI_BG_BBLUE    "\x1b[104m"
#define DU_TUI_BG_BMAGENTA "\x1b[105m"
#define DU_TUI_BG_BCYAN    "\x1b[106m"
#define DU_TUI_BG_BWHITE   "\x1b[107m"


/**
 * tuiSetColor:
 *   Sets the foreground and/or background color for text output.
 *
 * Parameters:
 *   foreground - ANSI escape code string for foreground color (e.g., DU_TUI_RED)
 *   background - ANSI escape code string for background color
 */
void tuiSetColor(const char *foreground, const char *background);

/**
 * tuiReset:
 *   Resets text formatting and colors to defaults.
 */
void tuiReset(void);

/**
 * tuiCursorUp:
 *   Moves the cursor up by `n` lines.
 *
 * Parameters:
 *   n - number of lines to move up
 */
void tuiCursorUp(uint8_t n);

/**
 * tuiCursorDown:
 *   Moves the cursor down by `n` lines.
 *
 * Parameters:
 *   n - number of lines to move down
 */
void tuiCursorDown(uint8_t n);

/**
 * tuiCursorRight:
 *   Moves the cursor right by `n` columns.
 *
 * Parameters:
 *   n - number of columns to move right
 */
void tuiCursorRight(uint8_t n);

/**
 * tuiCursorLeft:
 *   Moves the cursor left by `n` columns.
 *
 * Parameters:
 *   n - number of columns to move left
 */
void tuiCursorLeft(uint8_t n);

/**
 * tuiCursorPos:
 *   Moves the cursor to absolute position (x, y).
 *
 * Parameters:
 *   x - column number (1-based)
 *   y - row number (1-based)
 */
void tuiCursorPos(uint8_t x, uint8_t y);

/**
 * tuiHideCursor:
 *   Hides the terminal cursor.
 */
void tuiHideCursor(void);

/**
 * tuiShowCursor:
 *   Shows the terminal cursor if it was hidden.
 */
void tuiShowCursor(void);

/**
 * tuiClearScreen:
 *   Clears the terminal screen and moves cursor to top-left.
 */
void tuiClearScreen(void);


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


void tuiSetColor(const char *foreground, const char *background) {
    if (foreground) printf("%s", foreground);
    if (background) printf("%s", background);
}

void tuiReset(void) {
    printf("%s", DU_TUI_RESET);
}

void tuiCursorUp(uint8_t n) {
    if (n > 0) printf("\x1b[%uA", n);
}

void tuiCursorDown(uint8_t n) {
    if (n > 0) printf("\x1b[%uB", n);
}

void tuiCursorRight(uint8_t n) {
    if (n > 0) printf("\x1b[%uC", n);
}

void tuiCursorLeft(uint8_t n) {
    if (n > 0) printf("\x1b[%uD", n);
}

void tuiCursorPos(uint8_t x, uint8_t y) {
    printf("\x1b[%u;%uH", y, x);
}

void tuiHideCursor(void) {
    printf("\x1b[?25l");
}

void tuiShowCursor(void) {
    printf("\x1b[?25h");
}

void tuiClearScreen(void) {
    printf("\x1b[2J\x1b[H");
}