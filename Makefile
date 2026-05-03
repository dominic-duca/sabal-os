
CC 			:= i686-elf-gcc
AS 			:= i686-elf-as

CFLAGS 		:= -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS 	:= -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

SRC_DIR		:= src
BUILD_DIR	:= build
ISO_DIR		:= iso

# Find every .c and .s source in SRC_DIR
C_SOURCES 	:= $(wildcard $(SRC_DIR)/*.c)
S_SOURCES 	:= $(wildcard $(SRC_DIR)/*.s)

# Infer .o filenames from source filenames
OBJECTS 	:= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES)) \
				$(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(S_SOURCES))

KERNEL 		:= sabal-os

TARGET		:= $(BUILD_DIR)/$(KERNEL)
ISO			:= $(BUILD_DIR)/$(KERNEL).iso

.PHONY: all iso run clean

all: $(TARGET)

# Compile .c sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Assemble .s sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) -o $@ $<

# Link objects into kernel ELF and copy into ISO staging dir
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	cp $(TARGET) $(ISO_DIR)/boot/$(KERNEL)

iso: $(TARGET)
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO) -serial stdio

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ISO_DIR)/boot/$(KERNEL)
