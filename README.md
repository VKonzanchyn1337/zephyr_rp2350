# Zephyr OS — Waveshare RP2350 USB-C

bare-metal линукс сюда не встаёт физически (нет MMU у cortex-m33, 520КБ SRAM).
это ближайшее реальное к "ОС": Zephyr RTOS — preemptive scheduler, usb-cdc shell,
littlefs, драйверный стек.

## setup (один раз)

```bash
pip install west
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v3.7.0 zephyrproject
# ИЛИ если используешь этот репо как manifest:
#   west init -l app/
cd zephyrproject
west update
west zephyr-export
pip install -r zephyr/scripts/requirements.txt

# toolchain
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz
tar xf zephyr-sdk-0.16.8_linux-x86_64.tar.xz
cd zephyr-sdk-0.16.8 && ./setup.sh
```

## build

```bash
cd zephyrproject
west build -p always -b rpi_pico2 app -- -DEXTRA_DTC_OVERLAY_FILE=boards/rpi_pico2.overlay
```

выход: `build/zephyr/zephyr.uf2`

## flash (rp2350 bootsel)

1. зажми BOOTSEL на плате, воткни usb-c — появится диск `RP2350`
2. копируй `zephyr.uf2` на этот диск
3. плата ребутнется сама, появится новый CDC-ACM serial port

## подключение

```bash
# linux
screen /dev/ttyACM0 115200
# или
minicom -D /dev/ttyACM0

# windows — PuTTY/TeraTerm на COMx, 115200
```

жми Enter в терминале — появится shell prompt `uart:~$`

## команды shell

| команда | что делает |
|---|---|
| `uname` | кастомная — id ОС |
| `meminfo` | кастомная — heap stats |
| `kernel threads` | список всех тредов + стек usage |
| `kernel stacks` | детальная стек-статистика |
| `device list` | все драйверы/девайсы |
| `fs mount` / `ls /lfs` | работа с littlefs |
| `help` | полный список |

## FILL перед прошивкой

`boards/rpi_pico2.overlay` — пин status LED выставлен `GP25` предположительно
(стандартный pico-layout). сверь с pinout PDF именно твоей waveshare платы,
если светодиод на другом GPIO — поправь строку `gpios = <&gpio0 25 ...>`.

## если нужно реально POSIX-ish userspace (не просто shell)

добавь в prj.conf:
```
CONFIG_POSIX_API=y
CONFIG_PTHREAD_IPC=y
```
даёт pthread/mqueue/semaphore api поверх зефировских примитивов — ближайшее
к линуксовому API, что физически возможно на этом железе.
