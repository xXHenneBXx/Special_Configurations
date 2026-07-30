/**
 * Custom G-code implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.1.0
 * Date: 2023/10/27
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfigPre.h"

#if ALL(PROUI_EX, HAS_CGCODE)

#include "../MarlinCore.h" // for wait_for_user
#include "../core/types.h"
#include "../gcode/gcode.h"
#include "../libs/buzzer.h"
#include "../lcd/marlinui.h"

#if HAS_MEDIA
  #include "file_header.h"
#endif

#ifdef DWIN_LCD_PROUI
  #include "../lcd/e3v2/proui/dwin.h"
  #if ENABLED(LCD_BED_TRAMMING)
    #include "../lcd/e3v2/proui/bedlevel_tools.h"
  #endif
#endif

#include "proui_ex.h"
#include "custom_gcodes.h"

#if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
  static bool gcode_preview_enabled = false;
#endif

//=============================================================================
// Extended G-CODES
//=============================================================================

void cError() {
  parser.unknown_command_warning();
}

#if HAS_CUSTOM_COLORS
// C11 Set color for UI element E
  void C11() {
    const int16_t E = parser.seenval('E') ? parser.value_byte() : 0; // UI element
    if (E) {
      HMI_value.Color[0] = parser.seenval('R') ? parser.value_byte() : 0;
      HMI_value.Color[1] = parser.seenval('G') ? parser.value_byte() : 0;
      HMI_value.Color[2] = parser.seenval('B') ? parser.value_byte() : 0;
      uint16_t *target = nullptr;
      switch (E) {
        case 1: target = &HMI_data.Background_Color; break;
        case 2: target = &HMI_data.Cursor_Color; break;
        case 3: target = &HMI_data.TitleBg_Color; break;
        case 4: target = &HMI_data.TitleTxt_Color; break;
        case 5: target = &HMI_data.Text_Color; break;
        case 6: target = &HMI_data.Selected_Color; break;
        case 7: target = &HMI_data.SplitLine_Color; break;
        case 8: target = &HMI_data.Highlight_Color; break;
        case 9: target = &HMI_data.StatusBg_Color; break;
        case 10: target = &HMI_data.StatusTxt_Color; break;
        case 11: target = &HMI_data.PopupBg_Color; break;
        case 12: target = &HMI_data.PopupTxt_Color; break;
        case 13: target = &HMI_data.AlertBg_Color; break;
        case 14: target = &HMI_data.AlertTxt_Color; break;
        case 15: target = &HMI_data.PercentTxt_Color; break;
        case 16: target = &HMI_data.Barfill_Color; break;
        case 17: target = &HMI_data.Indicator_Color; break;
        case 18: target = &HMI_data.Coordinate_Color; break;
        default: target = nullptr; break;
      }
      if (target) {
        *target = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
        DWIN_RedrawScreen();
      } else DWIN_RedrawScreen();
    } else DWIN_RedrawScreen();
  }
#endif

#if ALL(HAS_BED_PROBE, HAS_TRAMMING_WIZARD)
  // Bed tramming
  void C35() {
    if (parser.seenval('T')) {
      const int8_t i = parser.value_byte();
      if (WITHIN(i, 0, 4)) Tram(i);
    }
    TERN_(HAS_TRAMMING_WIZARD, else Trammingwizard());
  }
#endif

// Cancel a Wait for User without an Emergecy Parser
void C108() {
  #if DEBUG_DWIN
    SERIAL_ECHOLNPGM(F("wait_for_user was "), marlin.wait_for_user);
    SERIAL_ECHOLNPGM(F("checkkey was "), checkkey);
  #endif
  TERN_(HAS_BACKLIGHT_TIMEOUT, ui.refresh_backlight_timeout());
  #if HAS_LCD_BRIGHTNESS
    if (!ui.backlight) ui.refresh_brightness();
  #endif
  wait_for_user = false;
  DONE_BUZZ(true);
}

// Enable or disable preview screen
#if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
void C250() {
  if (parser.seenval('P')) {
    gcode_preview_enabled = !!parser.value_byte();
  }
  SERIAL_ECHOLNPGM(F("PREVIEW:"), (int)gcode_preview_enabled);
}
#endif

// lock/unlock screen
#if HAS_LOCKSCREEN
  void C510() {
    if (parser.seenval('U') && parser.value_int()) DWIN_UnLockScreen();
    else DWIN_LockScreen();
  }
#endif

#if ENABLED(DEBUG_DWIN)
  void C997() {
    SERIAL_ECHOLNPGM("Simulating a printer freeze");
    TERN_(DWIN_LCD_PROUI, dwinRebootScreen());
    while (1) {};
  }
#elif NONE(PLATFORM_M997_SUPPORT, DWIN_LCD_PROUI)
  void C997() {
    SERIAL_ECHOLNPGM("Reboot printer");
    hal.reboot();
  }
#endif

// Special Creality DWIN GCodes
void customGcode(const int16_t codenum) {
  switch(codenum) {
    #if HAS_CUSTOM_COLORS
      case 11: C11(); break;              // Set color for UI element E
    #endif
    #if ALL(HAS_BED_PROBE, HAS_TRAMMING_WIZARD)
      case 35: C35(); break;              // Launch bed tramming wizard
    #endif
    case 108: C108(); break;              // Cancel a Wait for User without an Emergecy Parser
    #if HAS_LOCKSCREEN
      case 510: C510(); break;            // lock screen
    #endif
    #if ENABLED(DEBUG_DWIN) || NONE(PLATFORM_M997_SUPPORT, DWIN_LCD_PROUI)
      case 997: C997(); break;            // Simulate a printer freeze (or reboot)
    #endif
    #if HAS_MEDIA
      case 10: proUIEx.C10(); break;    // Mark the G-code file as a Configuration file
    #endif
    #if HAS_MESH
      case 29: proUIEx.C29(); break;    // Set probing area and mesh leveling settings
    #endif
    case 100: proUIEx.C100(); break;    // Change Physical minimums
    case 101: proUIEx.C101(); break;    // Change Physical maximums
    case 102: proUIEx.C102(); break;    // Change Bed size
    #if HAS_PROUI_MAXTEMP
      case 104: proUIEx.C104(); break;    // Set extruder max temperature (limited by maxtemp in thermistor table)
    #endif
    case 115: proUIEx.C115(); break;    // ProUI Info
    #if ENABLED(NOZZLE_PARK_FEATURE)
      case 125: proUIEx.C125(); break;  // Set park position
    #endif
    #if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
      case 250: C250(); break;          // Enable or disable preview screen
    #endif
    #if HAS_PROUI_RUNOUT_SENSOR
      case 412: proUIEx.C412(); break;  // Set runout sensor active mode
    #endif
    case 562: proUIEx.C562(); break;    // Invert Extruder
    case 851: proUIEx.C851(); break;    // If has a probe set z feed rate and multiprobe, if not, set manual z-offset
    #if HAS_TOOLBAR
      case 810: proUIEx.C810(); break;  // Config toolbar
    #endif
    default: cError(); break;
  }
}

void customGcodeReport(const bool forReplay/*=true*/) {
  proUIEx.C100_report(forReplay);
  proUIEx.C101_report(forReplay);
  proUIEx.C102_report(forReplay);
  #if HAS_MESH
    proUIEx.C29_report(forReplay);
  #endif
  #if HAS_PROUI_MAXTEMP
    proUIEx.C104_report(forReplay);
  #endif
  #if ENABLED(NOZZLE_PARK_FEATURE)
    proUIEx.C125_report(forReplay);
  #endif
  #if HAS_PROUI_RUNOUT_SENSOR
    proUIEx.C412_report(forReplay);
  #endif
    proUIEx.C562_report(forReplay);
  #if HAS_BED_PROBE
    proUIEx.C851_report(forReplay);
  #endif
}

#endif // PROUI_EX && HAS_CGCODE
