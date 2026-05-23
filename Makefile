
CC 			:= i686-elf-gcc
AS 			:= i686-elf-as

CFLAGS 		:= -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS 	:= -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

SRC_DIR		:= src
BUILD_DIR	:= build
ISO_DIR		:= iso

# Recursively find every .c and .s source in SRC_DIR/
C_SOURCES 	:= $(shell find $(SRC_DIR) -name "*.c")
S_SOURCES 	:= $(shell find $(SRC_DIR) -name "*.s")

# Infer .o filenames from source filenames (tag .s sources as .s.o to avoid collisions)
OBJECTS 	:= $(patsubst %.c, $(BUILD_DIR)/%.o, $(patsubst $(SRC_DIR)/%, %, $(C_SOURCES))) \
			   $(patsubst %.s, $(BUILD_DIR)/%.s.o, $(patsubst $(SRC_DIR)/%, %, $(S_SOURCES)))

KERNEL 		:= sabal-os

TARGET		:= $(BUILD_DIR)/$(KERNEL)
ISO			:= $(BUILD_DIR)/$(KERNEL).iso

.PHONY: all iso run clean

all: $(TARGET)

# Compile .c sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Assemble .s sources
$(BUILD_DIR)/%.s.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $<

# Link objects into kernel ELF and copy into ISO staging dir
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	cp $(TARGET) $(ISO_DIR)/boot/$(KERNEL)

iso: $(TARGET)
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO) -serial stdio

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ISO_DIR)/boot/$(KERNEL)
