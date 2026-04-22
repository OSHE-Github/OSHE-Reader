# **![][/Media/OSHE-Logo.png]**

# **OSHE-Reader Spring 2026 Report**

Ian Mitchell, Riley Patterson, Owen Dodson, Ian Ranstadler, Alycia Greer  
---

# **![][/Media/OSHE-Reader-Teardown.png]![][/Media/OSHE-Reader-Assembled.png]**

# **Description**

OSHE-Reader is an open-source e-reader device capable of displaying text from the EPUB and PDF E-book file formats. This project’s goal is to create a cheaper alternative to products from companies like Amazon and Kobo. It will stay open to the public, using free and open source software like LVGL, Github, EEZ Studio and Arduino IDE, allowing more collaboration and easy uploading of books, compared to other e-readers that require workarounds to upload custom content.

# **Methodology**

### **Overview**

The design of the OSHE-Reader is loosely based on the previous work of Joey Castillo’s *The Open Book* \[1\] and atomic14’s *ESP32 Based ePub Reader* \[2\]. The former was designed as a very basic implementation of an e-reader, with the main goal of being easily manufacturable by an end-user. Unfortunately, this goal came at the expense of quality, with the design being stripped down to bare essentials, both on the hardware side, lacking a proper case, using AAA batteries, as well as the software side only supporting raw text file formats. Atomic14’s design incorporates much more complicated features, especially on the software side with .epub file parsing for standard e-book reading capabilities, however this design was not made for end users, is lacking a case, and has limited e-paper screen support. The OSHE-Reader attempts to solve problems in these prior open source e-readers by creating an open source e-reader with full functionality, principled design choices, and most importantly: a simple and consistent experience for an end user.

The OSHE-Reader’s design falls largely into three categories: electrical, firmware, and hardware. The design considerations of these three categories are discussed in further detail below.

### **Electrical**

#### **Battery**

In order to be a device capable of being used portably, the OSHE-Reader needed to use some form of battery, such that the device could store energy and be used while not directly plugged in. For this system, we chose lithium polymer (LiPO) batteries for their slim profile and ability to be recharged. The choice to use a LiPO battery to power the device provides the best features in energy storage density and internal recharging, though at the cost of increased complexity relative to consumer AAA batteries, which require less surrounding systems to function, though are bulkier, heavier, and need to be removed to be recharged.

In order to recharge our LiPO batteries, two systems are necessary: a method of power connection as a source of power from which the battery can be charged, and a charging integrated chip (IC) which can handle the complex recharging process.

#### **Battery Charging**

To handle the power connection system, we decided on using the existing USB-C port. This port provides a consistent, readily available connector that works with a variety of existing cables, such as those used for charging phones or plugging in peripheral devices like computer mice and keyboards. This USB-C port is currently only used for charging, and as such requires minimal components, while allowing the e-reader to consume up to 1.5A of current, well above the requirements of the device. This device’s USB-C port was designed with the USB Implementers Forum’s guidelines on USB-C devices in consideration, and should stand up to the requirements for certification \[3\], though that is not something this project will be doing.

Regarding the second system required for the chosen LiPO batteries, the BQ2057CSNTR from Texas Instruments was chosen as our battery charging IC. The BQ2057CSNTR is an older battery charging chip, but was chosen primarily for its package. Because the BQ2057CSNTR comes packaged as a SOIC-8, the IC can be hand-soldered, which greatly reduces the barrier of entry for end users attempting to create their own OSHE-Reader. This however poses additional challenges, as the BQ2057CSNTR is lacking in some features, most notably of which is the lack of a power path feature, which means that the BQ2057CSNTR cannot, on its own, support using a device while simultaneously charging. This issue can be circumvented with the addition of a load-sharing system \[4\]. With this load-sharing system, the OSHE-Reader can now be used while charging.

#### **Buck Converter**

The LiPO battery outputs a voltage of 3.7 volts, but our screen and microcontroller run on 3.3 volts. To make sure we would be able to run both devices safely, we decided to add a buck converter to the circuit. The buck converter we picked is the TPS5432. The primary factor behind this choice was its package size, similar to our choice with the battery charging IC. By allowing it to be easily hand-solderable, such as with the SOIC-8 packaging type, it keeps our device available to more at-home builders. This device also helps keep the other components safe, as when it's running, any voltage too high, around \~4.3 volts, will cause a break, and it will need to be reset, and any value less than \~3.3 volts also puts the device in the same state. This additional protection helps make sure the user will not accidentally fry any parts they are using.

#### **Microcontroller**

For the OSHE-Reader, a core processing unit is necessary to do the computation required for the parsing and displaying of e-books. Because our device is battery-powered, a lower power microcontroller greatly increases the lifespan of our device. We selected the ESP32-C6 to be a featureful, yet low-power device that is readily available for hobbyists. ESP-IDF and Arduino IDE support the ESP32-C6 so development would also be easier. Flash size was a factor in selecting a microcontroller and the C6’s 8MB flash memory would be able to hold screen buffers. Furthermore, the C6 has a lot of sleep modes to limit power consumption while idle. Deep sleep in particular will be useful for the device in an “off state” where it can still wake up or refresh the screen periodically.

#### **Micro SD Card**

In order to be able to upload files to the OSHE Reader, an SD card reader was needed to provide EPUB files to the device. For this purpose, an SD card reader slot was chosen for our design. We chose to use a Push-Push mechanism for our SD card reader, since that would allow easier removal of the SD card from the slot in our case design, allowing the user to simply push the card inwards again and have it pop out, clear of the case, rather than needing to grab the device and pull it outwards from the inside of the case.

#### **User Input**

As described below in the GUI and Case Design sections, the OSHE-Reader will have five buttons (4 directions, 1 select). Each of these buttons must be connected to a GPIO pin on the microcontroller. Each of the buttons is pulled down to ground, such that the buttons are active high.

#### **Display**

The most user-facing component of the electrical system of the OSHE-Reader is the display. There is an enormous number of choices for potential displays, however the unique use case of displaying black and white book pages, which change fairly infrequently, means e-readers are particularly suited to e-paper displays. E-paper display technology is a relatively new display technology that trades refresh speed and color density for extreme power efficiency. We chose a Waveshare 5.83” 648 x 480 pixel display with 1-bit color depth as our e-paper display. We chose this display primarily because of its general availability and documentation, additionally, Waveshare has an ESP32 e-paper display driver board, which we used to verify our screen. This e-paper display uses a 4-wire SPI connection which is easy to implement on the microcontroller, however the display requires high voltages to operate, and thus needs an external boost converter. For our implementation we looked at the Waveshare driver board’s open source schematics and implemented a similar option \[5\].

#### **Printed Circuit Board**

For the design of our Circuit board, we wanted the vast majority of components to be hand solderable on our board. For this reason, most of our components are surface mount with a size of 1206, which is on the larger side of surface mount components.

### **Firmware**

#### **Micro SD Card**

#### In order to read from and write to an SD card, we imported the arduino SD card library (SD.h), which provides low level functionalities. However, in order to extract compressed files from an epub (zip archive), we needed access to a posix-style file system. To solve this problem, we employed the arduino-posix-fs library \[6\], which provides stdio functions (notably fopen, fclose, fread, and fwrite) which emulate the stdio.h functions found in a unix operating system.

#### **User Input**

#### To give the microcontroller the ability to read user input we must first enter the Arduino IDE again. We will connect pins from the microcontroller to certain functions that we will use in the e-reader GUI. Using the library OneButton \[7\] by mathertel, we were able to successfully debounce the buttons for single-press applications. This library is promising for future long-press implementations, such as turning on/off the device. Explicit button labels in the GUI allow for the user input to move to a new screen directly as opposed to moving around the screen to select an option. The buttons are responsive for the user and LVGL.

#### **EPUB File Processing**

In order to process an EPUB file from a zip archive to formatted text, three main steps need to be taken. Firstly, the archives files need to be extracted and uncompressed, then the archive needs to be parsed using an xml parser to extract the structural information about the contents of the EPUB, then finally, a word wrapping algorithm can be applied to format the raw text into a single page worth of text.

##### **EPUB File Unzipping**

The first step in the processing of an EPUB file is the decompression and extraction of the contents of the EPUB. To do this, we needed a library that could handle not only the decompression, but also have the necessary extensions for the archive support.

Many libraries supported the simple compression and decompression algorithms of the zip format, but only the miniz library \[8\] had support for the archive extensions, which provide an API for viewing the individual files in an archive, extracting them individually, and decompressing them.

Unfortunately, this library’s archive functions required the inclusion of the stdio header, which typically provides file read and write functionality on an operating system, however, working within the Arduino ecosystem, we had no operating system or file system. Therefore, we had to use the previously mentioned arduino-posix-filesystem library. While this substitution appeared to be successful, overall the integration of miniz into an embedded platform was unsuccessful, with the library still failing to function as of the end of the semester.

##### **EPUB File Parsing**

#### Parsing an EPUB requires extracting information from a few key files. The container file gives the location of the contents file. The contents file gives metadata (author, title, book language), locations of pages as xhtml files, the table of contents, and the spine. The table of contents includes all the chapters and a unique id attached to each.

Using Expat, the XML parser chosen last semester for its functionality with limited memory/processing power, we can parse this chain of files to gather all the relevant information on a book. Using the chapter ids from the table of contents and header tags in XML, we can locate the start of new chapters, and begin parsing them based on the paragraph tags. 

As we extract paragraphs, they will be sent to the pagination algorithm in chunks to be formatted on the screen. As the paragraphs are sent, we can keep track of where we left off in a chapter to maintain a kind of bookmark, as well as making the pages smoother if you close and reopen a book.

##### **Pagination and Word Wrapping**

For displaying the book for the user to view, we need a way to format the text while keeping count of pages. By taking the paragraphs from the file and formatting them to break the section of text into lines and not cut off words, we used EEZ Studio to have textboxes that will automatically wrap the text and display the page the user is on.

#### **Display**

In order to display images to our e-paper display, we needed an appropriate driver to send commands and data to the e-paper controller board located on the display. Previously we had been successful in using our display’s provided firmware, however that code was poorly written and documented and was both missing features we needed, while having a lot of core functionality like SPI being written using basic GPIO pins rather than the onboard SPI hardware. In order to solve this problem the library was re-written, keeping only the core needed functionality such as: turning on, clearing, updating the image displayed, entering sleep mode, and turning off. This way, the library could be both smaller, saving space for the rest of the firmware, and also faster, by using the hardware SPI functions of our microcontroller rather than the GPIO pins.

#### **GUI Design**

The GUI design pivoted to using direct button to screen navigation. Due to the slow refresh rate of the screen, it was preferable to make one button press change a screen, instead of using a button press to navigate through moving menus. We wanted a simplistic and easy to navigate GUI to make the experience as smooth as possible. Explicit buttons help the user understand where a button press will go, eliminating ambiguity. Settings allow for more customization for the screen, such as dark mode. The library screen will be full of book selections and clicking a book will take you to the text in the file. The center button will have an explicit back button when applicable, taking the user to the previous screen they were at. We included marmot imagery to set up a recognizable, user-friendly “mascot” of sorts. 

#### **LVGL**

The Light and Versatile Graphics Library (LVGL) is an open source program/library that allows for easy GUI development. After discussing possible routes of GUI libraries we could use, we eventually settled on LVGL due to its documentation, compatibility with ESP32, and UI editor applications. EEZ studio was used to generate LVGL assets. EEZ studio allowed for easy implementation of groups, buttons, labels, and other custom actions. The design was created in a GUI that generates C code once settings were adjusted.

### **Hardware**

#### **Case Design**

To design the case we wanted to make sure it was optimized for as many people as possible. This can be difficult due to different hand sizes, but we tried to standardize it. To get feedback we created an initial case out of cardboard, and showed it to people asking them to fill out a google form. This allowed us to get feedback before we invested hours into an incorrectly designed case. Due to feedback, we slightly increased the size of the case, and added buttons to both sides of the case. This should mean that it is accessible for people both left and right handed, and that it sits comfortably in most people's hands. We kept all the buttons on the front of the screen (vs the side) because it keeps the electronic and case design simple, and from our feedback it is still easy to push with the thumb when the device is in standard operating condition. PLA and PETG were tested when printing the case design, both are suitable for the application \[11\].

#### **3D Model**

To house the device we decided to create a shell in Onshape, as it is an open source and relatively simple to use modeling software; for the situations that the user wanted to update the case. The model is split into 5 key parts, the front shell, back plate, battery holder, rocker buttons, and screen holder. The goal of the design is to create a case that has covered tactile buttons, ensuring a seamless look. The main center button will be a flush spring button built into the front panel. This design was partially inspired by an older nook button design. Due to ribbon cable location issues, the side buttons were changed to rocker buttons. The original plan of designing side buttons similarly to the center button proved impossible as there was no location where the buttons could exist without compromising the ribbon cable on the screen. The outcome was good, the separate print is not time or resource intensive and they feel great. All parts of the shell will be held together with M3 screws. By using screws instead of a snapping system, if the device gets dropped it will not split apart. Some feedback suggested a screw hole that does not travel through the entire back panel for structure so that change was made to the original design.

# **Progress in Previous Semester**

The past semester we started the schematic on the components for the PCB including USB-C Connector, Battery Charging, Buck Converter, Button Input, Display Connector, Micro SD Card Slot, and the Microcontroller. Testing the Buck Converter, Battery Charging, and the Microcontroller Input on breadboard to test the components to make sure the parts will work. On the software side we used the Arduino libraries to display the screen and LVGL to develop the GUI with an easy navigated menu. Looking into parsing and displaying an EPUB file on the e-reader. We also started to design a 3D printed case keeping in mind the button placement and how everything will be held in place.

# **Bill of Materials**

| Description | Part Number | Source | Unit Price | Qty | Cost |
| :---: | :---: | :---: | :---: | :---: | :---: |
| ESP32-C6 Devboard | 25723 | Waveshare | $7.29 | 1 | $7.29 |
| PCB | Rev 1 | JLCPCB | $35.40 | 1 | $35.40 |
| 4.22k Ohm Resistor | AC1206FR-074K22L | Digikey | $0.10 | 1 | $0.10 |
| 45.3k Ohm Resistor | RC1206FR-0745K3L | Digikey | $0.10 | 1 | $0.10 |
| 66.5k Ohm Resistor | RC1206FR-0766K5L | Digikey | $0.10 | 1 | $0.10 |
| 10k Ohm Resistor | RMCF1206FT10K0 | Digikey | $0.10 | 2 | $0.20 |
| 49.9 Ohm Resistor | TNPW120649R9BEEA | Digikey | $0.41 | 1 | $0.41 |
| 3.24k Ohm Resistor | RC1206FR-073K24L | Digikey | $0.10 | 1 | $0.10 |
| 5.1k Ohm Resistor | AC1206FR-075K1L | Digikey | $0.10 | 2 | $0.20 |
| 100k Resistor | RC1206FR-07100KL | Digikey | $0.10 | 8 | $0.80 |
| 1k Resistor | RMCF1206FT1K00 | Digikey | $0.10 | 1 | $0.10 |
| 8.2 nFarad Capacitor | CC1206JKNPOZBN822 | Digikey | $0.47 | 1 | $0.47 |
| 82 pFarad Capacitor | CC1206JRNPOBBN820 | Digikey | $0.19 | 1 | $0.19 |
| 10 nFarad Capacitor | C1206C103K2RECTU | Digikey | $0.18 | 1 | $0.18 |
| 470 pFarad Capacitor | VJ1206A471JXGTW1HV | Digikey | $0.14 | 1 | $0.14 |
| 22 uFarad Capacitor | CL31A226KAHNNNE | Digikey | $0.08 | 2 | $0.16 |
| 0.1 uFarad Capacitor | C1206C104K5RACTU | Digikey | $0.08 | 6 | $0.48 |
| 10 uFarad Capacitor | CL31A106KBHNNNE | Digikey | $0.08 | 3 | $0.24 |
| 4.7 uFarad Capacitor | CL31B475KAHNNNE | Digikey | $0.16 | 2 | $0.32 |
| 1u/50V Farad Capacitor | C3216X7R1H105K160DB | Digikey | $0.24 | 7 | $1.68 |
| PNP BJT | PZT751T1G | DigiKey | $0.63 | 1 | $0.63 |
| BQ2057CSNTR | BQ2057CSNTR | Digikey | $3.87 | 1 | $3.87 |
| IRF7326D2TRPBF | IRF7326D2TRPBF | Digikey | $0.63 | 1 | $0.63 |
| 2.2 uHenries Inductor | AIML-1206-2R2K-T | Digikey | $0.21 | 1 | $0.21 |
| TPS5432DDA | TPS5432DDA | Digikey | $0.96 | 1 | $0.96 |
| Push Buttons | PTS636SM43SMTR LFS | Digikey | $0.20 | 5 | $1.00 |
| MBR0530 | MBR0530 | Digikey | $0.31 | 3 | $0.93 |
| Sil308EDL | SI1308EDL-T1-GE3 | Digikey | $0.58 | 1 | $0.58 |
| 10 u/1A Henries Inductor | CC453232A-100KL | Digikey | $0.40 | 1 | $0.40 |
| 112L-TDA0 | 112L-TDA0 | Digikey | $3.44 | 1 | $3.44 |
| CUHZ5V6\_H3F Diode | CUHZ5V6,H3F | Digikey | $0.44 | 1 | $0.44 |
| PPTC Polyfuse | 0ZCJ0050FF2G | Digikey | $0.20 | 2 | $0.40 |
| Thermistor | B57550G1103F000 | Digikey | $3.11 | 1 | $3.11 |
| LED | HSMY-C110 | Digikey | $0.29 | 1 | $0.29 |
| 2k Ohm Resistor | RC1206FR-132KL | Digikey | $0.10 | 1 | $0.10 |
| JST Connector | S2B-PH-K-S | Digikey | $0.10 | 1 | $0.10 |
| USB-C Connector | 2171750001 | Digikey | $0.75 | 1 | $0.75 |
| Test Point | S1751-46R | Digikey | $0.24 | 20 | $4.72 |
| Ribbon Cable Connector | FH12A-24S-0.5SH(55) | Digikey | $1.65 | 2 | $3.30 |
| 6.8k Resistor | ERA-8AEB682V | Digikey | $0.24 | 2 | $0.48 |
| 0.47 Resistor | KDV12DR470ET | Digikey | $0.28 | 1 | $0.28 |
| 0.2 Ohm Shunt Resistor | WSL2512R2000FEA | Digikey | $0.65 | 2 | $1.30 |

## **E-Reader Cost Breakdown![][Media/OSHE-Reader-Cost-Breakdown.png]**

# **Tools Used**

| Category  | Item  | Part Number | Source | Cost |
| :---- | :---- | :---- | :---- | :---- |
| Hardware | Breadboard  | PRT-12615 | SparkFun Electronics | $7.75 |
|  | Multimeter  | 167 | Fluke | $260.00 |
|  | Oscilloscope  | DSOX2004A | Newark | $3306.90 |
|  | Solder Iron | Hakko FX888Dx | Equipment | $121.47 |
| Software | Google Slides | N/A | Google | Free |
|  | LVGL | N/A | LVGL LLC | Free |
|  | Arduino IDE | N/A | Qualcomm Technologies, Inc. | Free |
|  | EEZ Studio | N/A | Envox Experimental Zone | Free  |
|  | KiCAD | N/A | KiCAD | Free |
|  | OnShape | N/A | PTC Inc. | Free |

# **Part Assembly**

### **PCB**

The circuit schematic and Circuit Board were both designed using KiCAD, a free open source software for circuit and circuit board design. The PCB was designed to be hand solderable while also not taking up a lot of space within the case to allow for the battery to take up the rest of the available space. Doing it this way would allow consumers to hand solder boards without the need of solder paste or reflow oven, while the size would allow the device to keep a slimmer profile making it more comfortable to hold. To achieve this, most of the components were surface mounted with a package size of 1206\. These components were easily soldered with a standard soldering iron heated to 470 degrees centigrade. Some components were not available in the 1206 package size that we wanted so these parts were soldered using reflow techniques. 

Future revisions of the Circuit Board should focus on a redesign of the device to allow for a smaller board to be created. There was also a need to add a schottky diode to our BJT to prevent backfeeding through Resistor 5 to allow for our battery charger to receive the voltage it needs to function properly.

#### **Display Connector**

The Display connector needs to adapt a fine pitch FPC cable with 0.5mm pitch, thus the connector component also has a 0.5mm pin pitch. For this reason, the display connector was not hand soldered and was instead soldered using reflow techniques in the interest of saving time. This had varying levels of success as this caused the contacts to bridge. Attempts to remove the display connector were made, but resulted in the removal of pads and traces connected to it. On a second attempt, the display connector was soldered to the board with a mixture of reflow and hand soldering techniques. This attempt did not result in bridged contacts and the display connector could be used.

#### **Buck Converter**

The design for the buck converter was borrowed from a design made by Texas Instruments as it was a proven design that utilized most of the parts already implemented in the schematic. The buck converter was then isolated from other components on the board due to the noise it creates, with all other components being routed around it.

#### **SD Card Slot**

The SD card reader was also only available in a package size smaller than 1206, however it was bigger than the package size for the display connector. This does mean that the SD Card reader is hand solderable given enough time and patience. However, it was reflow soldered for the sake of time.

### **Case**

All case pieces must be 3D-printed, which includes the front panel, back panel, screen bracket, two side buttons, and the battery bracket. The case has been tested in both PLA and PETG, both are suitable \[11\]. To assemble, start by placing the PCB on the bottom standoffs of the rear panel, aligning them with the screw holes. Plug in the battery to the PCB and fit inside the battery bracket. Align the battery bracket with the top standoffs of the rear panel, facing the battery downwards. The PCB and battery bracket should lay somewhat flush with each other. Attach the screen to the PCB via the ribbon cable and put the screen in the screen bracket. Set the screen bracket on top of the four standoffs, aligning them with the screw holes. For the front panel, align the side buttons with the holes on the front case. Flip over the front case with your figures holding the buttons in place or set the buttons on top of the PCB, facing you. Finally, place the top panel on top of the rest of the assembly and use four M3x14mm screws to close the case. Screw in the screws until there is a resistance of it hitting the back of the rear panel. The back of the case should be flush with no screw protrusions.

# **Characteristic Data**

![][/Media/OSHE-Reader-Battery-Charge-Data.png]  
The above graph shows an example of our safe battery charging circuit. On the horizontal axis the duration of the charging cycle is displayed in minutes, and on the vertical axis, the battery’s voltage level. During a charging cycle, the battery charging chip will start the battery in current regulation mode, where a set amount of current (\~100mA for our design) is driven into the battery, once a voltage threshold is reached, the battery charger initiates the voltage regulation mode, where the charging voltage is instead held steady and the charging current gradually decreases (not pictured). Finally, after the charging current reaches a minimum threshold, all charging stops automatically to prevent overcharging, at which point the battery is fully charged.

# **References**

| \[1\] | GitHub \- atomic14/diy-esp32-epub-reader: ESP32 Based ePub Reader [https://github.com/atomic14/diy-esp32-epub-reader](https://github.com/atomic14/diy-esp32-epub-reader) |
| :---- | :---- |
| \[2\] | GitHub \- joeycastillo/The-Open-Book: The Open Book: Project Reboot [https://github.com/joeycastillo/The-Open-Book](https://github.com/joeycastillo/The-Open-Book)  |
| \[3\] | USB Type-C® Cable and Connector Specification Release 2.4 [https://usb.org/document-library/usb-type-cr-cable-and-connector-specification-release-24](https://usb.org/document-library/usb-type-cr-cable-and-connector-specification-release-24) |
| \[4\] | Designing A Li-Ion Battery Charger and Load Sharing System With Microchip’s Stand-Alone Li-Ion Battery Charge Management Controller [https://ww1.microchip.com/downloads/en/AppNotes/01149c.pdf](https://ww1.microchip.com/downloads/en/AppNotes/01149c.pdf) |
| \[5\] | E-Paper ESP32 Driver Board V3 Schematic [https://files.waveshare.com/wiki/E-Paper-ESP32-Driver-Board/E-Paper\_ESP32\_Driver\_Board\_V3.pdf](https://files.waveshare.com/wiki/E-Paper-ESP32-Driver-Board/E-Paper_ESP32_Driver_Board_V3.pdf) |
| \[6\] | Arduino POSIX Filesystem Library [https://github.com/pschatzmann/arduino-posix-fs](https://github.com/pschatzmann/arduino-posix-fs) |
| \[7\] | OneButton Button Press Library [https://github.com/mathertel/OneButton](https://github.com/mathertel/OneButton) |
| \[8\] | Miniz (z-lib replacement) Library [https://github.com/richgel999/miniz](https://github.com/richgel999/miniz) |
| \[9\] | ESP32: Guide for MicroSD Card Module using Arduino IDE [https://randomnerdtutorials.com/esp32-microsd-card-arduino/](https://randomnerdtutorials.com/esp32-microsd-card-arduino/) |
| \[10\] | \[ESP32 & LVGL\] How to Use a Button as an Input for UI Control\!: [https://www.youtube.com/watch?v=Lp2\_MLgs7d0](https://www.youtube.com/watch?v=Lp2_MLgs7d0) |
| \[11\] | PETG vs PLA vs ABS: 3D Printing Strength Comparison [https://ultimaker.com/learn/petg-vs-pla-vs-abs-3d-printing-strength-comparison/](https://ultimaker.com/learn/petg-vs-pla-vs-abs-3d-printing-strength-comparison/) |
| \[12\] | Texas Instruments Buck Converter design [https://www.ti.com/lit/ds/symlink/tps5432.pdf](https://www.ti.com/lit/ds/symlink/tps5432.pdf) |
