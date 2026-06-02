TARGET := firmware
MCU_CODE_SIZE := 2048
IRAM_SIZE := 128

CC := sdcc
PACKIHX := packihx

CFLAGS := -mmcs51 --model-small --iram-size $(IRAM_SIZE) --xram-size 0 --code-size $(MCU_CODE_SIZE) --out-fmt-ihx

all: $(TARGET).hex

$(TARGET).ihx: main.c stc15.h
	$(CC) $(CFLAGS) -o $(TARGET).ihx main.c

$(TARGET).hex: $(TARGET).ihx
	$(PACKIHX) $(TARGET).ihx > $(TARGET).hex

clean:
	rm -f *.asm *.ihx *.lk *.lst *.map *.mem *.rel *.rst *.sym *.hex

flash: $(TARGET).hex
	stcgal -p /dev/ttyUSB0 $(TARGET).hex
