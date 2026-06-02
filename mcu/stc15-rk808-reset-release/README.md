# STC15 RK808 reset-release helper firmware

This directory contains a small SDCC-buildable STC15/8051 firmware used on the
ZC-328 advertising-board variant that has an STC15 MCU connected to the RK808
PMIC `VPPOTP` / reset-related net.

## Why this exists

The original STC firmware/project appeared to drive `P1.3` / `RK808_VPP_OTP`
high and then enter power-down. On the RK808 reference reset circuit, `VPPOTP`
is also used as a high-active reset input:

```text
VCCRTC -- 10k -- RESET button -- VPPOTP
```

So holding this signal high is effectively like holding the RK808 reset button
pressed. This can prevent the RK3288 board from running normally.

The replacement firmware releases that signal instead:

- `P1.3` latch is released.
- `P1.3` is configured as high-impedance input via `P1M1/P1M0`.
- The MCU then enters STC power-down.

This tries to emulate the reset button being unpressed and avoids fighting the
board's reset circuit.

## Build

On Debian/Ubuntu:

```bash
sudo apt install sdcc
make
```

The checked-in `firmware.hex` was built from the sources in this directory.

## Flashing

Install `stcgal` if desired:

```bash
python3 -m pip install --user stcgal
```

Then flash with the correct serial adapter/port for the STC15 MCU:

```bash
make flash
# or:
stcgal -p /dev/ttyUSB0 firmware.hex
```

If ISP entry fails, start `stcgal` first and then power-cycle the MCU/board.

## Notes

This firmware is intentionally tiny and board-specific. Verify the actual PCB
net before flashing it to a different ZC-328 variant.
