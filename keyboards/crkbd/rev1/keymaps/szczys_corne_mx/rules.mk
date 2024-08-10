LAYOUTS = split_3x6_3

# Build for the rp2040
CONVERT_TO=promicro_rp2040

# markstos: enable media keys
EXTRAKEY_ENABLE = yes

# markstos: smaller file size, little down-side
LTO_ENABLE = yes

COMBO_ENABLE = yes
TAP_DANCE_ENABLE = yes

# This is for RGB *underglow*
# https://github.com/qmk/qmk_firmware/blob/master/docs/feature_rgblight.md
RGBLIGHT_ENABLE = yes
