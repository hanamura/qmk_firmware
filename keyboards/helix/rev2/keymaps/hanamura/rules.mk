AUDIO_ENABLE = no
BACKLIGHT_ENABLE = no
BLUETOOTH_ENABLE = no
BOOTMAGIC_ENABLE = no
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
EXTRAKEY_ENABLE = no
MIDI_ENABLE = no
MOUSEKEY_ENABLE = no
NKRO_ENABLE = no
ONEHAND_ENABLE = no
RGBLIGHT_ENABLE = yes
SLEEP_LED_ENABLE = no
UNICODE_ENABLE = no

ifndef QUANTUM_DIR
  include ../../../../Makefile
endif
