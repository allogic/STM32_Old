PROJECT = discovery
DEVICE = stm32f407vgt6

OOCD_FILE = board/stm32f4discovery.cfg

BUILD_DIR = ./bin
OPENCM3_DIR = ../../../vendor/libopencm3
SOURCE_DIR = .
VENDOR_DIR = ../../../vendor

INCLUDES += -I$(VENDOR_DIR)

CFILES += $(SOURCE_DIR)/main.c
CFILES += $(SOURCE_DIR)/terminal.c
CFILES += $(SOURCE_DIR)/jtag.c

CFILES += $(VENDOR_DIR)/printf/printf.c

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../../../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk