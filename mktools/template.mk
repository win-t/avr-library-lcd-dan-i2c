CPU_FREQ                ?= 12000000
MMCU                    ?= atmega32a

AVRFLAG_OPTIMIZATION    ?= -O1 -funsigned-char          \
                           -funsigned-bitfields         \
                           -ffunction-sections          \
                           -fdata-sections              \
                           -fpack-struct                \
                           -fshort-enums

AVRFLAG_DEBUG           ?= -g3 -gdwarf-2 -DDEBUG
AVRFLAG_WARNING         ?= -Wall
AVRFLAG_STD             ?= -std=gnu99
AVRFLAG_LINKER          ?= -Wl,--gc-sections
AVRFLAG_SYMBOL          := -DF_CPU=$(CPU_FREQ)UL
AVRFLAG_MCU             := -mmcu=$(MMCU)

AVRFLAGS                += $(AVRFLAG_OPTIMIZATION)      \
                           $(AVRFLAG_DEBUG)             \
                           $(AVRFLAG_WARNING)           \
                           $(AVRFLAG_STD)               \
                           $(AVRFLAG_MCU)               \
                           $(AVRFLAG_SYMBOL)            \
                           $(AVRFLAG_LINKER)            \
                           $(AVRFLAG_OTHER)

%.o: %.c
	@echo "Compile: $<  -->  $@"
	@avr-gcc $(AVRFLAGS) -c -o "$@" "$<"

%.hex: %.elf
	@echo "Object copy: $<  -->  $@"
	@avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex "$<" "$@"

%.eep: %.elf
	@echo "Object copy: $<  -->  $@"
	@avr-objcopy -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$<" "$@"

%.lss: %.elf
	@echo "Object dump: $<  -->  $@"
	@avr-objdump -h -S "$<" > "$@"

$(PROJECT).elf: $(SOURCE)
	@echo "Link: $(SOURCE)  -->  $(PROJECT).elf"
	@avr-gcc $(AVRFLAGS) -o $(PROJECT).elf $(SOURCE)
	@echo "AVR Size:"
	@avr-size "$@"
