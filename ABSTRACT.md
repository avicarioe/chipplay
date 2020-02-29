# Extended abstract for chipPLAY project

By **Alejandro Vicario** (950322-2516) and **Xiaoyu Wang** (991102-5881)

2020-02-29

## Objective and Requirements:

During this project, we made a music player named chipPLAY based on uC32.

We get a wav file from SD card, extract the sound samples from the file, show the information on an I<sup>2</sup>C screen and output stereo sound through a 3.5mm jack using PWM. Also, we use the 4 buttons in the IO shield or one infrared controller to control the player. We can connect with speakers or headphones to hear the songs.

Our music player contains the following features:

- Read the files stored in SD card.
- Connect with a standard speaker or headphone.
- Select songs by buttons or infrared controller.
- Change the volume by buttons or by the infrared controller.
- Operate **play**, **pause** and **stop** instructions by buttons or by infrared controller.
- Show the progress of the songs playing on a I<sup>2</sup>C OLED screen.
- Show the icons of the instructions on the screen.
- Show a greeting when we activate the player.
- Show the name of songs, if the name is over 16 characters, scroll to the left.

### Solution:
We developed the project using the chipKIT uC32 board. UART is used to connect with the computer to get logging information.
For SD card reading, we use SPI to communicate and **FatFS** library to read the file system.
We have implemented a **wave** decoding module to extract audio samples from a ```wav``` file.
For generating the sound, we use timers and two PWM peripherals.

For the infrared controller, we created the IRC module to decode the **NEC Protocol** and get the key pressed in the remote, using the external interrupt one. 

For the I<sup>2</sup>C screen, we create a module to communicate and draw arbitrary shapes, then, we use that module to present the player status.

We also create a module for logging Information through the UART peripheral connected to the computer, using ```stdio.h``` library.


## Verification:
To test the I/O, we tested the system with different speakers, headphones and different files in the SD card. 
We checked the system with different songs, including songs encoded with 8, 16, and 24 bit and different frequencies. We tested stereo (2 channels), mono (1 channel) and stereo with 8D sound songs.
We tested all possible situations in each state of the player, verifying that the system responds as expected for every controller input.

## Contributions:
All the code on the project was done by us. Each team member had different responsibilities and worked independently in some moments, but all the modules were discussed and finished together.

- **Alejandro**: Audio processing, communication with SD card, controller protocols.

- **Xiaoyu**: User interaction, user interface, screen control.

- **FatFS**: The [FatFs library R0.14](http://elm-chan.org/fsw/ff/00index_e.html) has been used to read the FAT filesystem in the SD card. We also used some examples in the website as an inspiration for the MMC interface.

## Reflections:
Generally, the player works well and can achieve all the expected features.

The sound quality could be better since we use PWM and it could be improved using additional hardware such as a DAC. 
Also, in rare cases, due to the poor wiring, some lecture errors happen in SD card causing abnormal audio glitches for a few milliseconds.
