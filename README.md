# PET Pultrusion Configurator

A configuration tool for generating Marlin firmware files for **PET/PETE plastic bottle-to-filament pultrusion systems**, based on the **Recreator MK*** by Josh Taylor and derived from the Professional Firmware Configurator by MRiscoC, adapted by xXHenneBXx.

This tool strips out 3D-printer-specific features that a pultrusion system does not need (bed leveling, probes, homing, CoreXY, laser modules) while keeping all beneficial enhancements (Input Shaping, Linear Advance, MPC temperature control, NeoPixel, Speaker, SD features, and more). New pultrusion-specific features have been added for PET/PETE processing.

## Features

### What was removed (not needed for pultrusion)
- Bed leveling (BLTouch, CR Touch, Manual Mesh, UBL, all probes)
- CoreXY kinematics
- Laser module support (CVLM)
- Home offset adjustments
- All 3D printer-specific configs (Ender, Aquila, etc.)

### What was kept (beneficial for pultrusion)
- Input Shaping (IS)
- Linear Advance (LA)
- Model Predictive Temperature Control (MPC)
- NeoPixel LED support (NP)
- Speaker tones
- SD card read-only mode
- Stepper lock / timeout control
- G-code repeat and depth markers
- All board support (4.2.2, 4.2.7, SKR Mini E3, etc.)
- All display support (DWIN, TJC, DACAI, SYNWIT, CR10)
- All thermistor options (T1, T5, T13, T20)

### What was added (pultrusion-specific)
- **PETPreset**: PET/PETE optimized temperature and PID settings (240-290C range)
- **PullTune**: Pultrusion motion tuning for slow, steady filament pull
- **Spooler**: Filament runout detection and auto-pause for spooler integration
- **DualDrive**: Dual drive gear extruder settings (BMG-style 3:1 ratio)

### Printer profiles
- **RecreatorMK3**: Higher E-steps (130), 290C max, direct-drive style retract
- **RecreatorMK2**: Standard E-steps (93), 275C max, Bowden retract
- **CustomPull**: Blank canvas for custom pultrusion builds

## Running the Configurator

Execute the Python GUI:

```bash
python Configurator.pyw
```

> On Windows, `.pyw` files should be opened by pythonw.exe by default.

Select the printer, board, display, thermistor, and features, then press **Set Config** to preview the generation command. Enter a name (or press **Auto Name**), then press **Generate** to create the configuration files.

## Creating configurations programmatically

```python
import CreateConfigs
CreateConfigs.Generate('MyPultrusion', ['RecreatorMK3', '422', 'PETPreset', 'PullTune', 'Spooler'])
```

## Custom features

Advanced users can create custom JSON configs in the `_features` folder:

```json
{
"Configuration.h" : [
  {
    "op": "CustomVal",
    "searchfor": "TEMP_SENSOR_0",
    "value": "13",
    "comment": "PET thermistor"
  }
],
"Configuration_adv.h" : [],
"Version.h" : []
}
```

Supported operations:
- **InsertAfter**: Insert text after matching a pattern
- **Custom**: Replace text after matching a mask (regex)
- **CustomVal**: Replace simple values (numbers, booleans)
- **Enable**: Enable a feature
- **Disable**: Disable a feature
- **Replace**: Replace a pattern with other text

## Compiling the firmware

Move the generated `Version.h`, `Configuration.h`, `Configuration_adv.h` to the Marlin folder, and `platformio.ini` to the project root. Compile using VSCode with PlatformIO and Auto Build Marlin.

## Board nomenclature

- **422** - Creality board v4.2.2
- **427** - Creality board v4.2.7
- **301F1** - Creality board V2.4.S1.301 with STM32F103
- **301F4** - Creality board V2.4.S1.301 with STM32F401
- **SKRME3V3** - BIGTREETECH SKR Mini E3 V3

## Thermistor nomenclature

- **T1** - Creality stock temperature sensor
- **T5** - Marlin #5 (Volcano-style)
- **T13** - Marlin #13 (Sprite Pro, 100k Hisens 3950, up to 300C)
- **T20** - Marlin #20

## Credits

- Based on **Recreator MK*** by Josh Taylor
- Derived from **Professional Firmware** by MRiscoC, adapted by xXHenneBXx
- Marlin is licensed under GPL

## Disclaimer

THIS FIRMWARE AND ALL OTHER FILES ARE PROVIDED FREE OF CHARGE WITH NO WARRANTY OR GUARANTEE. WE ARE NOT LIABLE FOR ANY DAMAGE TO YOUR EQUIPMENT, PERSON, OR PROPERTY DUE TO USE OF THIS FIRMWARE.
