OPT = -O0

PROJECT = discovery
DEVICE = stm32f407vgt6

OOCD_FILE = board/stm32f4discovery.cfg

PROJECT_DIR = .
BUILD_DIR = $(PROJECT)
OPENCM3_DIR = ../vendor/libopencm3
LIBRARY_DIR = ../library
VENDOR_DIR = ../vendor

INCLUDES += -I$(LIBRARY_DIR)
INCLUDES += -I$(VENDOR_DIR)

CFILES += $(PROJECT_DIR)/main.c
CFILES += $(PROJECT_DIR)/terminal.c
CFILES += $(PROJECT_DIR)/jtag.c

CFILES += $(LIBRARY_DIR)/queue.c
CFILES += $(LIBRARY_DIR)/utility.c

CFILES += $(VENDOR_DIR)/printf/printf.c

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk