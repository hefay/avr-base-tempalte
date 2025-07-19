PRG         = sound
SRC         = sound.c
MCU_TARGET  = attiny13a
BUILD_DIR   ?= build

# Libs
DEFS        =
LIBS        =

# Compiler
CC          = avr-gcc

# Flags
CFLAGS      = -g -Wall -Os -mmcu=${MCU_TARGET} ${DEFS}
LDFLAGS     = -Wl,-Map,${BUILD_DIR}/${PRG}.map

# Tools
OBJCOPY     = avr-objcopy
OBJDUMP     = avr-objdump
AVRDUDE     = avrdude
AVRDUDE_FLAGS = -c usbasp -p t13a -B 1

# Targets
all: ${BUILD_DIR}/${PRG}.elf ${BUILD_DIR}/${PRG}.lst ${BUILD_DIR}/${PRG}.hex ${BUILD_DIR}/${PRG}.eep

# Ensure build directory exists
${BUILD_DIR}:
	mkdir -p $@

${BUILD_DIR}/${PRG}.elf: ${SRC} | ${BUILD_DIR}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ${LIBS}

${BUILD_DIR}/${PRG}.lst: ${BUILD_DIR}/${PRG}.elf
	${OBJDUMP} -h -S $< > $@

${BUILD_DIR}/${PRG}.hex: ${BUILD_DIR}/${PRG}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BUILD_DIR}/${PRG}.eep: ${BUILD_DIR}/${PRG}.elf
	${OBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" \
		--no-change-warnings --change-section-lma .eeprom=0 $< $@

upload: ${BUILD_DIR}/${PRG}.hex
	sudo ${AVRDUDE} ${AVRDUDE_FLAGS} -U flash:w:$<:i

clean:
	rm -rf ${BUILD_DIR}

