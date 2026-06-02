# Armbian - ZC-328
Armbian patches for zc-328 board (third party &amp; hobby project, not offical)

This repository contains the board configuration, U-Boot patches, Linux DTS patch,
kernel configuration, and a small STC15 helper firmware used by this board.

# Verified baseline
This port was brought up and tested with:

- Armbian build tag: `v26.8.0-trunk.53`
- Armbian build commit: `9bd9bcd090a2e1ff385ec161ea4c4633273e2d19`
- Build branch used locally: `zc328-v26.8`
- U-Boot: `v2026.04` patches
- Linux kernel family: Rockchip current / Linux `6.18`
- Tested output kernel: `6.18.34-current-rockchip`
- Tested image name: `Armbian-unofficial_26.05.0-trunk_Zc-328_noble_current_6.18.34.img`
- Tested release: Ubuntu `noble`

# How to use
1. Clone this repo to your local machine (or download zip file)
2. Clone [armbian repo](https://github.com/armbian/build) to your local machine, check out the branch to `v26.8.0-trunk.53` tag (`9bd9bcd090a2e1ff385ec161ea4c4633273e2d19` commit)
3. Copy all files in this repo to armbian directory (e.g. /home/username/armbian/build)
4. Run armian build script to build your own image:
```bash
   ./compile.sh build \
     BOARD=zc-328 \
     BRANCH=current \
     BUILD_DESKTOP=no \
     BUILD_MINIMAL=no \
     KERNEL_CONFIGURE=no \
     KERNEL_GIT=full \
     NO_HOST_RELEASE_CHECK=yes \
     RELEASE=noble
```
5. After build process is done, you can find your image in output/images directory (e.g. /home/username/armbian/build/output/images)
6. Flash your image to SD card and enjoy it!

# Flashing to EMMC
For flashing to EMMC, you can use rkdeveloptool.   
rkdeveloptool is already included in armbian build script, so you can use it directly:  
```
# 0. Put your board into maskrom mode
#    - Connect your board to PC via USB OTG port
#    - Press and hold the "uboot" button near the EMMC chip (This is very important!)
#    - Then connect the 12V power supply

# 1. Download bootloader
/home/username/armbian/build/cache/sources/rkbin-tools/tools/rkdeveloptool db /home/username/armbian/build/cache/sources/rkbin-tools/rk32/rk3288_ubootloader_v1.01.06.bin

# 2. Flash image to EMMC
/home/username/armbian/build/cache/sources/rkbin-tools/tools/rkdeveloptool wl 0 /home/username/armbian/build/output/images/YOUR_IMAGE_FILE.img

# 3. Reboot your board.
/home/username/armbian/build/cache/sources/rkbin-tools/tools/rkdeveloptool rd

# 4. That's all. Enjoy it!
```

After installing to eMMC, it is **strongly recommended** to hold the board-specific
kernel/DTB/U-Boot packages,  
to avoid accidentally replacing this custom port with
an incompatible generic upgrade:

```bash
apt-mark hold \
  linux-image-current-rockchip \
  linux-dtb-current-rockchip \
  linux-u-boot-zc-328-current \
  linux-headers-current-rockchip \
  linux-libc-dev-current-rockchip \
  armbian-bsp-cli-zc-328-current
```

# About the STC15 firmware
A small STC15/8051 MCU is present on the ZC-328 advertising screen board, which is originally designed to control the reset-related network of RK808 PMIC, implementing watchdog, infrared, timed power on/off, etc., and communicating with RK3288 via RK3288 UART 3.  
There is currently no way to read out the original firmware of this MCU, so it is unknown how to feed the watchdog.  
If the watchdog is not fed properly, the STC15 MCU will reboot the RK808 PMIC approximately every 40 seconds, which severely interferes with normal use.  

Therefore, **it is necessary to re-flash the firmware of the STC15**.  

By measuring the connections on the board, we found that the `P1.3` pin of the STC15 MCU is connected to the `VPPOTP` pin of the RK808 PMIC.  
And by referring to the datasheet of RK808 PMIC, we found that `VPPOTP` is used as a high-level reset in the official reference design, roughly connected as follows:

```text
VCCRTC -- 10k -- RESET button -- VPPOTP
```

This repository implements a minimal STC15 firmware that configures `P1.3` as a high-impedance input and then puts the STC15 into power-down mode, simulating the state of the reset button not being pressed as much as possible.  
The code is located in the `mcu/*` directory:

- `main.c` - the minimal reset-release firmware source code
- `stc15.h` - the STC15 SFR definitions required by SDCC
- `Makefile` - the build rules for SDCC/packihx
- `firmware.hex` - the pre-built Intel HEX
- `README.md` - detailed instructions for the MCU firmware

The sha256 of the pre-built `firmware.hex` is:

```text
c584ee2426e4d6f454c70b36caa18cc9922d0cbd097158dcab76a6d963230167
```

To build this firmware:

```bash
cd mcu/stc15-rk808-reset-release
sudo apt install sdcc
make
```

It is recommended to use the official STC-ISP tool provided by STC to flash this firmware after flying wires.  
We tried open-source tools such as `stcgal`, but unfortunately we got a Protocol Error and flashing was unsuccessful.

# License
This project is licensed under the GPLv2 License - with ABSOLUTELY NO WARRANTY. See the [LICENSE](LICENSE) file for details. 

This project is a hobby project and has nothing to do with the original manufacturer of the zc-328 board.  
Please do not contact the original manufacturer for technical support.

------------

# Armbian - ZC-328
zc-328主板的armbian补丁（这是一个第三方爱好者项目，非官方作品）
本仓库包含 board config、U-Boot 补丁、Linux DTS 补丁、kernel config，以及该板子上使用的 STC15 辅助固件。

## 已验证基线
本次移植与实机验证基于：

- Armbian build tag：`v26.8.0-trunk.53`
- Armbian build commit：`9bd9bcd090a2e1ff385ec161ea4c4633273e2d19`
- 本地构建分支：`zc328-v26.8`
- U-Boot：`v2026.04` 补丁
- Linux kernel family：Rockchip current / Linux `6.18`
- 实测 kernel：`6.18.34-current-rockchip`
- 实测镜像：`Armbian-unofficial_26.05.0-trunk_Zc-328_noble_current_6.18.34.img`
- 发行版：Ubuntu `noble`

# 使用说明
1. 将本项目克隆到本地（或者下载zip文件）
2. 将[armbian项目](https://github.com/armbian/build)克隆到本地，切换到 `v26.8.0-trunk.53` tag （对应commit: `9bd9bcd090a2e1ff385ec161ea4c4633273e2d19`）
3. 将本项目中的所有文件复制粘贴到armbian目录下（例如/home/username/armbian/build）
4. 运行armbian的编译脚本来编译你自己的镜像：
```bash
   ./compile.sh build \
     BOARD=zc-328 \
     BRANCH=current \
     BUILD_DESKTOP=no \
     BUILD_MINIMAL=no \
     KERNEL_CONFIGURE=no \
     KERNEL_GIT=full \
     NO_HOST_RELEASE_CHECK=yes \
     RELEASE=noble
```
5. 编译完成后，你可以在output/images目录下找到你的镜像（例如/home/username/armbian/build/output/images）
6. 将镜像烧录到SD卡，大功告成！

# 烧录到EMMC
你可以使用rkdeveloptool来烧录镜像到EMMC。  
rkdeveloptool已经包含在armbian的编译脚本中，所以你可以直接使用：  
```
# 0. 将主板进入maskrom模式
#    - 通过USB OTG口将主板连接到电脑
#    - 然后按住EMMC芯片旁边的"uboot"按钮（这一步非常重要！）
#    - 最后连接12V电源

# 1. 下载uboot
/home/username/armbian/build/cache/sources/rkbin-tools/tools/rkdeveloptool db /home/username/armbian/build/cache/sources/rkbin-tools/rk32/rk3288_ubootloader_v1.01.06.bin

# 2. 烧录镜像到EMMC
/home/username/armbian/build/cache/sources/rkbin-tools/tools/rkdeveloptool wl 0 /home/username/armbian/build/output/images/你的镜像文件.img

# 3. 重启主板
/home/username/armbian/build/cache/sources/rkbin-tools/tools/rkdeveloptool rd

# 4. 搞定，大功告成！
```

写入 eMMC 后，**强烈建议** hold 住板级 kernel/DTB/U-Boot 包，避免普通升级覆盖自定义移植而导致无法启动：

```bash
apt-mark hold \
  linux-image-current-rockchip \
  linux-dtb-current-rockchip \
  linux-u-boot-zc-328-current \
  linux-headers-current-rockchip \
  linux-libc-dev-current-rockchip \
  armbian-bsp-cli-zc-328-current
```

## 关于 STC15 的固件

ZC-328 广告屏板子上有一个 STC15/8051 小 MCU，它的原设计作用是控制 RK808 PMIC 的 reset 相关网络, 实现了看门狗和红外、定时开关机等功能，并连接到了 RK3288 UART 3 与 RK3288 通讯.  
该项目暂时没有办法读出该 MCU 的原始固件, 因此无法得知如何进行看门狗喂狗.  
如果没有正确的喂狗, 该 STC15 MCU 将会大约40s重启一次 RK808 PMIC, 严重干扰正常使用.  

因此 **重新刷写 STC15 的固件是十分必要的**.   

通过测量板子上的连接, 我们发现 STC15 MCU 的 `P1.3` 引脚连接到了 RK808 PMIC 的 `VPPOTP` 引脚.  
而查阅 RK808 PMIC 的 datasheet, 我们发现 `VPPOTP` 在官方参考设计中被用作高电平复位, 大致连接如下:

```text
VCCRTC -- 10k -- RESET button -- VPPOTP
```

所以本仓库实现了一个最简化的 STC15 固件, 该固件会把 `P1.3` 配置成高阻输入, 然后让 STC15 进入 power-down 模式, 借此尽量模拟 reset 按钮未按下的状态.  
代码位于 `mcu/*` 目录下:

- `main.c` - 最小 reset-release 固件源码
- `stc15.h` - SDCC 所需 STC15 SFR 定义
- `Makefile` - SDCC/packihx 构建规则
- `firmware.hex` - 预构建 Intel HEX
- `README.md` - MCU 固件详细说明

预构建 `firmware.hex` 的 sha256:

```text
c584ee2426e4d6f454c70b36caa18cc9922d0cbd097158dcab76a6d963230167
```

构建该固件:

```bash
cd mcu/stc15-rk808-reset-release
sudo apt install sdcc
make
```

建议**飞线**后, 使用 STC 官方提供的 STC-ISP 工具刷写该固件.  
我们尝试了 `stcgal` 等开源工具, 但最终得到 Protocol Error, 未能成功刷写.

# 软件许可
本项目基于GPLv2协议发布，没有任何担保。更多详情请参阅[LICENSE](LICENSE)文件。  

本项目是一个爱好者项目，与zc-328主板的原始制造商无关。  
请不要联系原始制造商寻求技术支持。
