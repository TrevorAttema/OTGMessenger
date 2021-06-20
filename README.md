# OTGMessenger
Off The Grid (OTG) Messenger is an easy way for people to communicate through text messages when in remote areas.
With a theoretical transmission range of 10 miles (16kms), OTG messenger can be used by groups of people to stay connected when they are in areas not serviced by mobile connectivity.

For portability and low power purposes, the device was created by re-purposing an old Nokia e63 phone I had laying aroud. The enclosure, LCD, keypad, backlighting and speaker have been re-used however the motherboard was re-designed from the ground up with low power components, a modern STM32 H7 microcontroller, an ISM LoRA radio and expanded flash memory. 

Common use cases
- Sports enthusiasts - people who participate in remote area sporting activities, out of mobile service
- Farmers - people who work in remote areas without mobile phone coverage
- Encrypted communication - for private point-to-point communication

Why was this device created? It was developed as a science project to learn/teach everything about the STM32 ecosystem of products, hardware engineering, embedded systems engineering, and encrypted communications.

This is considered an 'advanced' project so if you're just starting out with embedded systems or have only ever used the arduino platform then the concepts shown here could be challenging. You should be familiar with C/C++, SWD debugging, ARM core knowledge along with bare metal device driver development.

Hardware breakdown
* STM32H742VGT6 - ARM STM32 H7 - https://www.st.com/resource/en/datasheet/stm32h742vg.pdf
* MP2667 - Battery Management chip - https://www.monolithicpower.com/en/documentview/productdocument/index/version/2/document_type/Datasheet/lang/en/sku/MP2667
* ncp5007 - Backlight drivers - https://www.onsemi.com/pdf/datasheet/ncp5007-d.pdf
* Nokia e63 LCD display - https://kbiva.wordpress.com/2015/02/15/nokia-e63-lcd/ and https://spritesmods.com/mcportal/MC2PA8201.pdf
* Nokia e63 Keypad - connector pinouts - http://www.altehandys.de/downloads/ser-no-e63-schematics.pdf
* SX1276 Semtech ISM LoRA radio - https://www.semtech.com/products/wireless-rf/lora-core/sx1276
* Secure element - ATECC608A-MAHCZ-T - https://ww1.microchip.com/downloads/en/DeviceDoc/ATECC608A-CryptoAuthentication-Device-Summary-Data-Sheet-DS40001977B.pdf
* TPS62088YWCR 3A buck regulator - amazing low noise device - https://www.ti.com/lit/ds/symlink/tps62088.pdf
* And many more devices....

Software breakdown
* Touch GFX- the incredible ST GUI technology has been adapted to function with a Nokia e63 keypad
* STM32 ChromeART for graphics
* STM32CubeIDE - software development and debug environment. C/C++ coding
* STM32CubeMX - hardware configuration platform by STM
* Self healing mesh networking - enables group communication over vast distances. Devices (nodes) act as relay stations.

