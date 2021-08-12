IoT Development Kit Simple SPI FLASH programmer 
===================================================

Usage Example
flash_programer <file_path> <spi_flash_offset_hex>
This program copy binary file <file_path> to on-board SPI FLASH memory at offset <spi_flash_offset_hex>.

Requirments
============================================
1)	Destination address must be in hexadecimal format.
2)	Destination address must be multiple of 4096.
3)	File size + destination address must be less then flash size (1MB).
4)	This program must be executed on board. 
	Example:   mdb -digilent -run flash_programmer.elf image_file.bin 0x00010000


Execution steps
===================================
	1) Check input parameters and flash availability.
	2) Erasing required flash area by blocks with size of 64kb, 32kb or 4kB.
	3) Binary writing input file in pre-erased flash memory area by pages with size of 256 bytes.

	
Usage example (writing boot loader and image in flash):
=================================================

elf2bin person_detection_test_int8.elf protobuf.bin -sn .protobuf 	# Extract protobug section from elf file into binary file.
mdb -run -digilent flash_programmer.elf ..\protobuf.bin 0x80000		# Write model (protobuf) into the second half of flash (starting from 512K)