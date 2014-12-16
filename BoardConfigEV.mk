# Disable Modules
TARGET_KERNEL_NO_MODULES := true

# Kernel
BUILD_KERNEL := true
BOARD_KERNEL_IMAGE_NAME := zImage-dtb
TARGET_KERNEL_SOURCE := kernel/moto/shamu
TARGET_KERNEL_CONFIG := evervolv_shamu_defconfig

# Enable workaround for slow rom flash
BOARD_SUPPRESS_SECURE_ERASE := true

# Krait optimizations
TARGET_USE_QCOM_BIONIC_OPTIMIZATION := true

# Time
BOARD_USES_QC_TIME_SERVICES := true

# TWRP
TW_THEME := portrait_hdpi
TW_INCLUDE_L_CRYPTO := true
BOARD_HAS_NO_REAL_SDCARD := true
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TARGET_RECOVERY_PIXEL_FORMAT := "RGB_565"
TW_SCREEN_BLANK_ON_BOOT := true
