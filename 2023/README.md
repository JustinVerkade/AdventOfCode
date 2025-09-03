# Embedded-AoC "Yuki"<br />
![Alt text](https://github.com/JustinVerkade/EmbeddedAoC-2023/blob/8d7cd2562cda0d604a7bd38779a287bdb10c7df2/board.jpg?raw=true "EmbeddedAoC")<br />

## Completed<br />
-> f_debug function to log FatFs problems to vcom interface.<br />
-> Added system check for SD card.<br />
-> Added system check for SRAM memory.<br />
-> Added system structure for executing AoC puzzles.<br />

## todo<br />
-> Add usb layer as calling the transmit function fails when bizzy without feedback.<br />
-> Add vcom enable variable so the system can respond to being opend as port.<br />
-> Add ESP32 bootloader function in vcom user interface for remote programming.<br />
-> Add system check for ESP32.<br />
