PRGORAMMER_TTY     ?= /dev/ttyUSB0
MMCU               ?= atmega32

ifeq ($(MMCU), atmega32a)
    MMCU           := atmega32
endif

AVRDUDE            := avrdude -cavr910 -p$(MMCU) -P$(PRGORAMMER_TTY) -b115200
