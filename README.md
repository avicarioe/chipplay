# chipPLAY
A minimalist audio player based on ChipKit uC32 board for IS1200 coruse in KTH.

## Build
If all the dependencies below are satisfied, to compile the project and upload it to a board it is only necessary to execute:

```bash
make
make install
```

If the board is in a diferent port from `/dev/ttyUSB0`, execute instead:

```bash
make
make TTYDEV=/dev/port install
```

### mcp32tools
To compile the project it is required to have the mcp32tools development environment installed and activated.

The easiest way to perform this task is by following the instructions for the release v2.2 explained of [this repository](https://github.com/is1200-example-projects/mcb32tools/releases/tag/v2.2).

### FatFs

The application uses the [FatFs library R0.14](http://elm-chan.org/fsw/ff/00index_e.html) to read the filesystem in the SD card.

To include the library into the project the following commands have to be executed inside the project folder:

```bash
wget http://elm-chan.org/fsw/ff/arc/ff14.zip
unzip ff14.zip -d ff14
rm ff14/source/ffconf.h ff14.zip
```

## Hardware configuration

If the projects is to be used in a [chipKIT uC32 board](https://chipkit.net/wpcproduct/chipkit-uc32/) the following hardware connections need to be done.

### SD card

The SD card uses the SPI2 peripheral.


| SD Card (SPI Mode)  | chipKIT pin   |
| ------------------- |---------------|
| CS                  | 28(RE2)       |
| DI                  | 11 (SDO2)     |
| VSS                 | GND           |
| VDD                 | 3.3v          |
| SCLK                | 13 (SCK2)     |
| VSS2                | GND           |
| DO                  | 12 (SDI2)     |

### Headphone jack
For the audio signal generation the OC1 and OC2 peripherals are used.

| Jack      | chipKIT pin   |
| --------- |---------------|
| Right     | 5(OC2)        |
| Left      | 3 (OC1)       |
| Common    | GND           |

### I2C OLED Display
For  the OLED display the I2C1 peripheral is used.

| OLED Display  | chipKIT pin   |
| ------------- |---------------|
| VDD           | 3.3v          |
| SDA           | 36 (SDA1)     |
| SCL           | 37 (SCL1)     |
| GND           | GND           |

For this to work JP6 and JP8 must be configured in the I2C positions.
Two resistor to pull up SDA and SCL lines are also necesary, but they are usually included in displays modules.

### IR Receiver
The IR reciver uses the INT1.

| IR Receiver   | chipKIT pin   |
| ------------- |---------------|
| Vs            | 3.3v          |
| OUT           | 2 (INT1)      |
| GND           | GND           |

### Buttons
Optionally, the player can be controlled by four physical buttons, which are already included in [chipKIT Basic IO Shield](https://chipkit.net/wiki/index.php?title=Basic_IO_Shield).

| Buttons       | chipKIT pin   |
| ------------- |---------------|
| BUT0          | 37 (RD7)      |
| BUT1          | 36 (RD6)      |
| BUT2          | 34 (RD5)      |
| BUT3          | 4 (RF1)       |


## Documents

- [PIC32 Reference Manual](http://hades.mech.northwestern.edu/images/2/21/61132B_PIC32ReferenceManual.pdf)
- [chipKIT uC32 Manual](https://reference.digilentinc.com/_media/chipkit_uc32:chipkit_uc32_rm.pdf)
- [FatFs Reference](http://elm-chan.org/fsw/ff/00index_e.html)
- [PIC32MX3/4 Family Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf)
- [WAVE PCM soundfile format](http://soundfile.sapp.org/doc/WaveFormat/)
