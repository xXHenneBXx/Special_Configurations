# TJC DWIN_SET Template for Marlin Ender 3 V2

This document explains how to build a DWIN icon archive for the Ender 3 V2 / TJC DWIN screen using Marlin's existing DWIN tools.

## What Marlin expects

Marlin's DWIN `PROUI` support uses a DWIN icon container file (`*.ICO`) and JPEG icons. For the Ender 3 V2 stock/Creality DWIN set, the correct container is usually `9.ICO`.

There is no Marlin-side `.HMI` authoring tool in this repository. The runtime assets are packed into `*.ICO` and `*.JPG` files under `DWIN_SET`.

## Recommended workflow

1. Put your custom icons in a folder, for example:
   - `buildroot/share/dwin/icons-9/`

2. Name each file with a leading 3-digit index and symbol name:
   - `000-ICON_LOGO_Creality.jpg`
   - `001-ICON_Print_0.jpg`
   - `002-ICON_Print_1.jpg`
   - ...
   - `091-ICON_Info_1.jpg`

3. Do not create an entry for index `039` unless you intentionally want a blank slot. In the stock DWIN archive, icon `39` is reserved/empty.

4. Create the archive using the existing tool:
   - `cd buildroot/share/dwin`
   - `C:/Python314/python.exe ./bin/makeIco.py icons-9 9.ICO`

5. Place `9.ICO` into `DWIN_SET` on the screen's SD card, alongside any other required JPEG files.

## Icon indexes and names

Use these exact indexes to preserve the stock DWIN_SET layout and Marlin icon mapping.

- `000` - `ICON_LOGO_Creality`
- `001` - `ICON_Print_0`
- `002` - `ICON_Print_1`
- `003` - `ICON_Prepare_0`
- `004` - `ICON_Prepare_1`
- `005` - `ICON_Control_0`
- `006` - `ICON_Control_1`
- `007` - `ICON_Leveling_0`
- `008` - `ICON_Leveling_1`
- `009` - `ICON_HotendTemp`
- `010` - `ICON_BedTemp`
- `011` - `ICON_Speed`
- `012` - `ICON_Zoffset`
- `013` - `ICON_Back`
- `014` - `ICON_File`
- `015` - `ICON_PrintTime`
- `016` - `ICON_RemainTime`
- `017` - `ICON_Setup_0`
- `018` - `ICON_Setup_1`
- `019` - `ICON_Pause_0`
- `020` - `ICON_Pause_1`
- `021` - `ICON_Continue_0`
- `022` - `ICON_Continue_1`
- `023` - `ICON_Stop_0`
- `024` - `ICON_Stop_1`
- `025` - `ICON_Bar`
- `026` - `ICON_More`
- `027` - `ICON_Axis`
- `028` - `ICON_CloseMotor`
- `029` - `ICON_Homing`
- `030` - `ICON_SetHome`
- `031` - `ICON_PLAPreheat`
- `032` - `ICON_ABSPreheat`
- `033` - `ICON_Cool`
- `034` - `ICON_Language`
- `035` - `ICON_MoveX`
- `036` - `ICON_MoveY`
- `037` - `ICON_MoveZ`
- `038` - `ICON_Extruder`
- `039` - (blank/reserved)
- `040` - `ICON_Temperature`
- `041` - `ICON_Motion`
- `042` - `ICON_WriteEEPROM`
- `043` - `ICON_ReadEEPROM`
- `044` - `ICON_ResumeEEPROM`
- `045` - `ICON_Info`
- `046` - `ICON_SetEndTemp`
- `047` - `ICON_SetBedTemp`
- `048` - `ICON_FanSpeed`
- `049` - `ICON_SetPLAPreheat`
- `050` - `ICON_SetABSPreheat`
- `051` - `ICON_MaxSpeed`
- `052` - `ICON_MaxAccelerated`
- `053` - `ICON_MaxJerk`
- `054` - `ICON_Step`
- `055` - `ICON_PrintSize`
- `056` - `ICON_Version`
- `057` - `ICON_Contact`
- `058` - `ICON_StockConfiguration`
- `059` - `ICON_MaxSpeedX`
- `060` - `ICON_MaxSpeedY`
- `061` - `ICON_MaxSpeedZ`
- `062` - `ICON_MaxSpeedE`
- `063` - `ICON_MaxAccX`
- `064` - `ICON_MaxAccY`
- `065` - `ICON_MaxAccZ`
- `066` - `ICON_MaxAccE`
- `067` - `ICON_MaxSpeedJerkX`
- `068` - `ICON_MaxSpeedJerkY`
- `069` - `ICON_MaxSpeedJerkZ`
- `070` - `ICON_MaxSpeedJerkE`
- `071` - `ICON_StepX`
- `072` - `ICON_StepY`
- `073` - `ICON_StepZ`
- `074` - `ICON_StepE`
- `075` - `ICON_Setspeed`
- `076` - `ICON_SetZOffset`
- `077` - `ICON_Rectangle`
- `078` - `ICON_BLTouch`
- `079` - `ICON_TempTooLow`
- `080` - `ICON_AutoLeveling`
- `081` - `ICON_TempTooHigh`
- `082` - `ICON_NoTips_C`
- `083` - `ICON_NoTips_E`
- `084` - `ICON_Continue_C`
- `085` - `ICON_Continue_E`
- `086` - `ICON_Cancel_C`
- `087` - `ICON_Cancel_E`
- `088` - `ICON_Confirm_C`
- `089` - `ICON_Confirm_E`
- `090` - `ICON_Info_0`
- `091` - `ICON_Info_1`

## Stock Ender 3 V2 layout hints

The default main screen is drawn from `Marlin/src/lcd/e3v2/proui/dwin.cpp`:

- `ICON_LOGO` at `x=71, y=52`
- `ICON_Print_0` at `x=17, y=110`
- `ICON_Prepare_0` at `x=145, y=110`
- `ICON_Control_0` at `x=17, y=226`
- `ICON_Info_0` (Advanced Settings) at `x=145, y=226`

Print process buttons are drawn as:

- `ICON_Setup_0` at `x=8, y=232`
- `ICON_Pause_0` / `ICON_Continue_0` at `x=96, y=232`
- `ICON_Stop_0` at `x=184, y=232`

## Notes for TJC firmware

- If you want the stock Creality DWIN set, enable `USE_STOCK_DWIN_SET` in `Marlin/src/lcd/e3v2/common/dwin_set.h`.
- Otherwise use the default unified DWIN set (`ICON 7`) with the current Marlin layout.
- Your custom icons can replace the JPEGs in the folder and be packed with `makeIco.py`.

## Useful existing repository assets

- `buildroot/share/dwin/bin/makeIco.py` — create an `.ICO` from JPEG files.
- `buildroot/share/dwin/bin/splitIco.py` — extract JPEGs from an existing `.ICO`.
- `buildroot/share/dwin/icons-7/` — existing icon folder with stock names/indexes.
- `buildroot/share/dwin/make_jpgs.sh` — example script for generating JPEG icons from SVGs.

## Example

If you want to build a new `9.ICO` from your own icon set:

```powershell
cd "c:\Users\henne\Desktop\Ender3FW_Project\xXHenneBXx_Configurator(Working)\Marlin-2.1.2.8\buildroot\share\dwin"
mkdir icons-9
# copy your custom index-named JPEGs into icons-9
C:/Python314/python.exe .\bin\makeIco.py icons-9 9.ICO
```

Then copy `9.ICO` into `DWIN_SET` on the SD card for the TJC/Ender 3 V2 screen.
