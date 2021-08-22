include $(LVGL_DIR)/lv_drivers/display/display.mk
include $(LVGL_DIR)/lv_drivers/indev/indev.mk

CSRCS += $(wildcard $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/display/*.c)

CSRCS += win_drv.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_drivers
VPATH += :$(LVGL_DIR)/lv_drivers

CFLAGS += "-I$(LVGL_DIR)/lv_drivers"
