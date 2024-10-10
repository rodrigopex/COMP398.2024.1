target remote localhost:1234
dir $ZEPHYR_BASE
load ./build/zephyr/zephyr.elf
b main
b state_machine
tui enable
c
