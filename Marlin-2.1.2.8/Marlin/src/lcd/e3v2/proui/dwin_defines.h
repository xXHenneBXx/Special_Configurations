/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * DWIN general defines and data structs for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.12.2
 * Date: 2022/08/08
 */

//#define DEBUG_DWIN 1
//#define NEED_HEX_PRINT 1

#if defined(__STM32F1__) || defined(STM32F1)
  #define DASH_REDRAW 1
#endif

#if DISABLED(PROBE_MANUALLY) && ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_UBL)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#define Def_Background_Color  Color_Bg_Black;      // deep carbon background
#define Def_Cursor_Color      RGB( 0, 50, 63);     // dark cyan cursor bar
#define Def_TitleBg_Color     RGB( 0, 40, 53);     // slightly darker cyan header
#define Def_TitleTxt_Color    Color_White
#define Def_Text_Color        RGB(0,210,255);      // cyan body text
#define Def_Selected_Color    RGB( 0,173,204);     // bright cyan selected highlight
#define Def_SplitLine_Color   RGB( 28, 38, 45);    // subtle slate divider
#define Def_Highlight_Color   RGB( 0,210,255);     // vivid cyan icon frame
#define Def_StatusBg_Color    RGB( 0, 40, 53);     // cyan-tinted status bar
#define Def_StatusTxt_Color   RGB(180,240,255);    // light cyan status text
#define Def_PopupBg_Color     RGB( 16, 22, 28);    // dark popup background
#define Def_PopupTxt_Color    Color_White;         // white popup text
#define Def_AlertBg_Color     RGB( 60,  8, 12);    // deep red alert background
#define Def_AlertTxt_Color    RGB(255,200,200);    // soft red alert text
#define Def_PercentTxt_Color  RGB( 0,210,255);     // cyan percent readout
#define Def_Barfill_Color     RGB( 0,173,204);     // cyan progress bar fill
#define Def_Indicator_Color   RGB(0,210,255);      // cyan dashboard values
#define Def_Coordinate_Color  RGB( 0,210,255);     // cyan XYZ readouts
#define Def_Button_Color      RGB( 0, 23, 16)
#if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  #define Def_Leds_Color      LEDColorWhite()
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  #define Def_CaseLight_Brightness 255
#endif
#ifdef Z_AFTER_HOMING
  #define DEF_Z_AFTER_HOMING Z_AFTER_HOMING
#else
  #define DEF_Z_AFTER_HOMING 0
#endif
#define DEF_HOTENDPIDT TERN(PREHEAT_1_TEMP_BED, PREHEAT_1_TEMP_HOTEND, 195)
#define DEF_BEDPIDT TERN(PREHEAT_1_TEMP_BED, PREHEAT_1_TEMP_HOTEND, 60)
#define DEF_PIDCYCLES 5
