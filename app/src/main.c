/* file: main.c | runtime: zephyr rtos | target: waveshare rp2350 usb-c (rpi_pico2 board) */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(os_main, LOG_LEVEL_INF);

static const struct gpio_dt_spec status_led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});

static void heartbeat_thread(void *a, void *b, void *c)
{
	ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);

	bool led_ready = gpio_is_ready_dt(&status_led);
	if (led_ready) {
		gpio_pin_configure_dt(&status_led, GPIO_OUTPUT_ACTIVE);
	} else {
		LOG_WRN("status led not ready — check boards/rpi_pico2_rp2350a_m33.overlay pin");
	}

	while (1) {
		if (led_ready) {
			gpio_pin_toggle_dt(&status_led);
		}
		k_msleep(500);
	}
}

K_THREAD_DEFINE(heartbeat_tid, 512, heartbeat_thread, NULL, NULL, NULL, 7, 0, 0);

static int cmd_uname(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc); ARG_UNUSED(argv);
	shell_print(sh, "Zephyr RTOS | rp2350 (cortex-m33) | waveshare rp2350 usb-c");
	return 0;
}
SHELL_CMD_REGISTER(uname, NULL, "print os/kernel identity", cmd_uname);

/* heap/thread stats available via stock shell: "kernel stacks", "kernel threads" */

int main(void)
{
	LOG_INF("boot: zephyr os on rp2350 (waveshare usb-c)");

	int usb_rc = usb_enable(NULL);
	if (usb_rc != 0 && usb_rc != -EALREADY) {
		LOG_ERR("usb_enable failed: %d", usb_rc);
	}

	LOG_INF("ready — connect via usb-c, open serial terminal, press enter for shell");
	return 0;
}
