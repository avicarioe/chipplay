# Extended abstract for chipPLAY project

By **Alejandro Vicario** (950322-2516) and **Xiaoyu Wang** (991102-5881)

2020-02-07

## Objective and Requirements:

In this project we will make a music player with support for multiple audio formats, graphic screen with sound visualization and some human interface. The basic and compulsory requirements for the music player should includes:

- Play music stored in SD card
- Connect with standard speaker
- Select songs by buttons
- Show the progress of the songs playing on the screen

### Optional requirements:

- MP3 support
- Infrared controller 
- LED matrix sound visualization
- Bigger screen
- Equalizer 

### Solution:
We plan to develop our project on the ChipKIT uC32 board together with the Basic I/O shield. We will use the small display at first but if we have extra time, we will add an external bigger screen. We will use the buttons and an infrared controller with receivers to control the songs. We will also use microcontroller interrupts to control the speed of reproduction precisely and to attend to user events.

## Verification:
Compare the quality of songs with other devices and check the reproduction speed is the same as original songs. The quality can not be perfect because of the limited capabilities of the board but it has to be close enough to the original. We will check if the system reacts correctly to the user inputs in any case. We will also check the system with different audio files to check if the sound processing works as expected.

## Contributions:
- **Alejandro**: Audio processing, communication with SD card.

- **Xiaoyu**: Controller interaction, user interface, screen control.

- **FatFS**: The [FatFs library R0.14](http://elm-chan.org/fsw/ff/00index_e.html) has been used to read the FAT filesystem in the SD card. We also use some examples in the website as an inspiration for the MMC interface.

## Reflections:
In the final abstract, we will discuss and reflect on what happened in the project.
