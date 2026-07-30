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

/**
 * Mesh Viewer for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 3.14.1
 * Date: 2022/04/11
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_MESH)

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin.h"
#include "dwin_popup.h"
#include "../../../feature/bedlevel/bedlevel.h"
#include "meshviewer.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "bedlevel_tools.h"
#endif

MeshViewerClass MeshViewer;

void MeshViewerClass::DrawMesh(bed_mesh_t zval, const uint8_t sizex, const uint8_t sizey) {
  const int8_t mx = 25;  // Margin
  const int16_t width = DWIN_WIDTH - 2 * mx; // square drawing area, tied to DWIN_WIDTH for both axes
  const int8_t rmin = 5;
  const int8_t rmax = _MIN(mx - 2, (int16_t)(0.5 * width / (sizex - 1)));
  #define zmin -20
  #define zmax  20
  int16_t zmesh[sizex][sizey];
  #define px(xp) (mx + (xp) * (width) / (sizex - 1))
  #define py(yp) (30 + DWIN_WIDTH - mx - (yp) * (width) / (sizey - 1))
  #define rm(z) ((_MAX(_MIN((z), zmax), zmin) - zmin) * (rmax - rmin) / (zmax - zmin) + rmin)
    #if ENABLED(TJC_DISPLAY)
      #define meshfont font8x16
    #else
      #define meshfont font6x12
    #endif
    const uint8_t fs = DWINUI::fontWidth(meshfont);
    #define DrawMeshValue(xp, yp, zv) DWINUI::Draw_Signed_Float(meshfont, 1, 2, px(xp) - 18, py(yp) - 6, zv)
  #define DrawMeshHLine(yp) DWIN_Draw_HLine(HMI_data.SplitLine_Color, px(0), py(yp), px(sizex - 1) - px(0))
  #define DrawMeshVLine(xp) DWIN_Draw_VLine(HMI_data.SplitLine_Color, px(xp), py(sizey - 1), py(0) - py(sizey - 1))
  int16_t maxz =-32000; int16_t minz = 32000;
  for (uint8_t y = 0; y < sizey; ++y) for (uint8_t x = 0; x < sizex; ++x) {
    const float v = isnan(zval[x][y]) ? 0 : round(zval[x][y] * 100);
    zmesh[x][y] = v;
    NOLESS(maxz, v);
    NOMORE(minz, v);
  }
  max = (float)maxz / 100;
  min = (float)minz / 100;
  DWINUI::ClearMainArea();
  DWIN_Draw_Rectangle(0, HMI_data.SplitLine_Color, px(0), py(0), px(sizex - 1), py(sizey - 1));
  for (uint8_t x = 1; x < sizex - 1; ++x) DrawMeshVLine(x);
  for (uint8_t y = 1; y < sizey - 1; ++y) DrawMeshHLine(y);
  for (uint8_t y = 0; y < sizey; ++y) {
    hal.watchdog_refresh();
      for (uint8_t x = 0; x < sizex; ++x) {
      uint16_t color = DWINUI::RainbowInt(zmesh[x][y], zmin, zmax);
      uint8_t radius = rm(zmesh[x][y]);
      const int16_t xpix = px(x);
      const int16_t ypix = py(y);
      DWINUI::Draw_FillCircle(color, xpix, ypix, radius);
      if (sizex < (ENABLED(TJC_DISPLAY) ? 8 : 9)) {
        if (zmesh[x][y] == 0) DWIN_Draw_String(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, xpix - 2*fs - 1, ypix - fs, "0.00");
        else DWINUI::Draw_Signed_Float(meshfont, 1, 2, xpix - 3*fs, ypix - fs, zval[x][y]);
      }
      else {
        char str_1[9];
        str_1[0] = 0;
        switch (zmesh[x][y]) {
          case -999 ... -100:
            DWINUI::Draw_Signed_Float(meshfont, 1, 1, xpix - 18, ypix - 6, zval[x][y]);
            break;
          case -99 ... -1:
            sprintf_P(str_1, PSTR("-.%02i"), -zmesh[x][y]);
            break;
          case 0:
            DWIN_Draw_String(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, xpix - 4, ypix - 6, "0");
            break;
          case 1 ... 99:
            sprintf_P(str_1, PSTR(".%02i"), zmesh[x][y]);
            break;
          case 100 ... 999:
            DWINUI::Draw_Signed_Float(meshfont, 1, 1, xpix - 18, ypix - 6, zval[x][y]);
            break;
        }
        if (str_1[0])
          DWIN_Draw_String(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, xpix - 2*fs, ypix - fs, str_1);
      }
      SERIAL_FLUSH();
      TERN_(TJC_DISPLAY, delay(100));
    }
  }
}

void MeshViewerClass::Draw(bool withsave /*= false*/) {
  Title.ShowCaption(GET_TEXT_F(MSG_MESH_VIEWER));
  #if USE_UBL_VIEWER
    DWINUI::ClearMainArea();
    bedLevelTools.viewer_print_value = true;
    bedLevelTools.Draw_Bed_Mesh(-1, 1, 8, 10 + TITLE_HEIGHT);
  #else
    DrawMesh(bedlevel.z_values, GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y);
  #endif
  if (withsave) {
    DWINUI::Draw_Button(BTN_Save, 26, 305);
    DWINUI::Draw_Button(BTN_Continue, 146, 305);
    Draw_Select_Highlight(HMI_flag.select_flag, 305);
  }
  else
    DWINUI::Draw_Button(BTN_Continue, 86, 305);

  #if USE_UBL_VIEWER
    bedLevelTools.Set_Mesh_Viewer_Status();
  #else
    char str_1[6], str_2[6] = "";
    ui.status_printf(0, F("Mesh minZ: %s, maxZ: %s"),
      dtostrf(min, 1, 2, str_1),
      dtostrf(max, 1, 2, str_2)
    );
  #endif
}

void Draw_MeshViewer() { MeshViewer.Draw(true); }
void onClick_MeshViewer() { if (HMI_flag.select_flag) SaveMesh(); HMI_ReturnScreen(); DWIN_RedrawScreen(); }
void Goto_MeshViewer() { if (leveling_is_valid()) Goto_Popup(Draw_MeshViewer, onClick_MeshViewer); else HMI_ReturnScreen(); }

#endif // DWIN_LCD_PROUI && HAS_MESH
