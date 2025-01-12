#include "common.h"
#include "device.h"
#include "flash_api.h"
#include "fmc_spi_aspeed.h"
#include "uart_aspeed.h"
#include "sgpiom_aspeed.h"
#include "gpio_aspeed.h"
#include "i2c_aspeed.h"
#include "i3c_aspeed.h"
#include "i2c_aspeed.h"
#include "espi_aspeed.h"
#include "peci_aspeed.h"
#include "kcs_aspeed.h"
#include "bt_aspeed.h"
#include "snoop_aspeed.h"
#include "pcc_aspeed.h"
#include "pwm_tach_aspeed.h"
#include "adc_aspeed.h"
#include "jtag_aspeed.h"
#include "mac_aspeed.h"

/* declare UART devices */
aspeed_uart_priv_t uart2_priv = {
	.dma_mode = false,
	.virt_mode = false,
};
DECLARE_DEV_CLK(uart2, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(18));
DECLARE_DEV_RESET(uart2, 0, 0, 0);
DECLARE_DEV(uart2, ASPEED_DEV_UART2, UART2_BASE, &uart2_priv);

aspeed_uart_priv_t uart4_priv = {
	.dma_mode = false,
	.virt_mode = false,
};
DECLARE_DEV_CLK(uart4, 0, 0, 0);
DECLARE_DEV_RESET(uart4, 0, 0, 0);
DECLARE_DEV(uart4, ASPEED_DEV_UART4, UART4_BASE, &uart4_priv);

aspeed_uart_priv_t uart6_priv = {
	.dma_mode = false,
	.virt_mode = false,
};
DECLARE_DEV_CLK(uart6, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(23));
DECLARE_DEV_RESET(uart6, 0, 0, 0);
DECLARE_DEV(uart6, ASPEED_DEV_UART6, UART6_BASE, &uart6_priv);

aspeed_uart_priv_t vuart0_priv = {
	.dma_mode = true,
	.virt_mode = true,
	.virt = {
		.port = 0x2f8,
		.sirq = 0x4,
		.sirq_pol = 0x0,
	},
	.dma = {
		.ch = 12,
	},
};
DECLARE_DEV_CLK(vuart0, 0, 0, 0);
DECLARE_DEV_RESET(vuart0, 0, 0, 0);
DECLARE_DEV(vuart0, ASPEED_DEV_VUART0, VUART0_BASE, &vuart0_priv);

/* declare GPIO device */
#if CONFIG_DEVICE_GPIO
GPIO_DEVICE_DECLARE(0, GPIO_BASE, Gpio_IRQn, 208, 0x3ffffff);
#endif /* end of "#if CONFIG_DEVICE_GPIO" */

/* declare SGPIOM device */
#if CONFIG_DEVICE_SGPIOM
SGPIOM_DEVICE_DECLARE(0, 200000, 128);
#endif /* end of "#if CONFIG_DEVICE_SGPIOM" */

#if CONFIG_DEVICE_TIMER
/* declare timer devices */
DECLARE_DEV_CLK(timer0, 0, 0, 0);
DECLARE_DEV_RESET(timer0, 0, 0, 0);
DECLARE_DEV(timer0, ASPEED_DEV_TMC0, TMC_BASE, NULL);
#endif

/* declare I2C devices */
#if CONFIG_DEVICE_I2C
DECLARE_DEV_CLK(i2c_global, 0, 0, 0);
DECLARE_DEV_RESET(i2c_global, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(2));
DECLARE_DEV(i2c_global, ASPEED_DEV_I2C_GLOBAL, I2C_GLOBAL_BASE, NULL);

aspeed_i2c_priv_t i2c0_priv = {.parent = &i2c_global, .bus_clk = 100000, .irq = I2c0_IRQn, .buff_addr = I2C0_BUFF_BASE};
DECLARE_DEV_CLK(i2c0, 0, 0, 0);
DECLARE_DEV_RESET(i2c0, 0, 0, 0);
DECLARE_DEV(i2c0, ASPEED_DEV_I2C0, I2C0_BASE, &i2c0_priv);

aspeed_i2c_priv_t i2c1_priv = {.parent = &i2c_global, .bus_clk = 100000, .irq = I2c1_IRQn, .buff_addr = I2C1_BUFF_BASE};
DECLARE_DEV_CLK(i2c1, 0, 0, 0);
DECLARE_DEV_RESET(i2c1, 0, 0, 0);
DECLARE_DEV(i2c1, ASPEED_DEV_I2C1, I2C1_BASE, &i2c1_priv);

aspeed_i2c_priv_t i2c2_priv = {.parent = &i2c_global, .bus_clk = 100000, .irq = I2c2_IRQn, .buff_addr = I2C2_BUFF_BASE};
DECLARE_DEV_CLK(i2c2, 0, 0, 0);
DECLARE_DEV_RESET(i2c2, 0, 0, 0);
DECLARE_DEV(i2c2, ASPEED_DEV_I2C2, I2C2_BASE, &i2c2_priv);

aspeed_i2c_priv_t i2c3_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c3_IRQn, .buff_addr = I2C3_BUFF_BASE};
DECLARE_DEV_CLK(i2c3, 0, 0, 0);
DECLARE_DEV_RESET(i2c3, 0, 0, 0);
DECLARE_DEV(i2c3, ASPEED_DEV_I2C3, I2C3_BASE, &i2c3_priv);

aspeed_i2c_priv_t i2c4_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c4_IRQn, .buff_addr = I2C4_BUFF_BASE};
DECLARE_DEV_CLK(i2c4, 0, 0, 0);
DECLARE_DEV_RESET(i2c4, 0, 0, 0);
DECLARE_DEV(i2c4, ASPEED_DEV_I2C4, I2C4_BASE, &i2c4_priv);

aspeed_i2c_priv_t i2c5_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c5_IRQn, .buff_addr = I2C5_BUFF_BASE};
DECLARE_DEV_CLK(i2c5, 0, 0, 0);
DECLARE_DEV_RESET(i2c5, 0, 0, 0);
DECLARE_DEV(i2c5, ASPEED_DEV_I2C5, I2C5_BASE, &i2c5_priv);

aspeed_i2c_priv_t i2c6_priv = {.parent = &i2c_global, .bus_clk = 1000000, .irq = I2c6_IRQn, .buff_addr = I2C6_BUFF_BASE};
DECLARE_DEV_CLK(i2c6, 0, 0, 0);
DECLARE_DEV_RESET(i2c6, 0, 0, 0);
DECLARE_DEV(i2c6, ASPEED_DEV_I2C6, I2C6_BASE, &i2c6_priv);

aspeed_i2c_priv_t i2c7_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c7_IRQn, .buff_addr = I2C7_BUFF_BASE};
DECLARE_DEV_CLK(i2c7, 0, 0, 0);
DECLARE_DEV_RESET(i2c7, 0, 0, 0);
DECLARE_DEV(i2c7, ASPEED_DEV_I2C7, I2C7_BASE, &i2c7_priv);

aspeed_i2c_priv_t i2c8_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c8_IRQn, .buff_addr = I2C8_BUFF_BASE};
DECLARE_DEV_CLK(i2c8, 0, 0, 0);
DECLARE_DEV_RESET(i2c8, 0, 0, 0);
DECLARE_DEV(i2c8, ASPEED_DEV_I2C8, I2C8_BASE, &i2c8_priv);

aspeed_i2c_priv_t i2c9_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c9_IRQn, .buff_addr = I2C9_BUFF_BASE};
DECLARE_DEV_CLK(i2c9, 0, 0, 0);
DECLARE_DEV_RESET(i2c9, 0, 0, 0);
DECLARE_DEV(i2c9, ASPEED_DEV_I2C9, I2C9_BASE, &i2c9_priv);

aspeed_i2c_priv_t i2c10_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c10_IRQn, .buff_addr = I2C10_BUFF_BASE};
DECLARE_DEV_CLK(i2c10, 0, 0, 0);
DECLARE_DEV_RESET(i2c10, 0, 0, 0);
DECLARE_DEV(i2c10, ASPEED_DEV_I2C10, I2C10_BASE, &i2c10_priv);

aspeed_i2c_priv_t i2c11_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c11_IRQn, .buff_addr = I2C11_BUFF_BASE};
DECLARE_DEV_CLK(i2c11, 0, 0, 0);
DECLARE_DEV_RESET(i2c11, 0, 0, 0);
DECLARE_DEV(i2c11, ASPEED_DEV_I2C11, I2C11_BASE, &i2c11_priv);

aspeed_i2c_priv_t i2c12_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c12_IRQn, .buff_addr = I2C12_BUFF_BASE};
DECLARE_DEV_CLK(i2c12, 0, 0, 0);
DECLARE_DEV_RESET(i2c12, 0, 0, 0);
DECLARE_DEV(i2c12, ASPEED_DEV_I2C12, I2C12_BASE, &i2c12_priv);

aspeed_i2c_priv_t i2c13_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c13_IRQn, .buff_addr = I2C13_BUFF_BASE};
DECLARE_DEV_CLK(i2c13, 0, 0, 0);
DECLARE_DEV_RESET(i2c13, 0, 0, 0);
DECLARE_DEV(i2c13, ASPEED_DEV_I2C13, I2C13_BASE, &i2c13_priv);

aspeed_i2c_priv_t i2c14_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c14_IRQn, .buff_addr = I2C14_BUFF_BASE};
DECLARE_DEV_CLK(i2c14, 0, 0, 0);
DECLARE_DEV_RESET(i2c14, 0, 0, 0);
DECLARE_DEV(i2c14, ASPEED_DEV_I2C14, I2C14_BASE, &i2c14_priv);

aspeed_i2c_priv_t i2c15_priv = {.parent = &i2c_global, .bus_clk = 400000, .irq = I2c15_IRQn, .buff_addr = I2C15_BUFF_BASE};
DECLARE_DEV_CLK(i2c15, 0, 0, 0);
DECLARE_DEV_RESET(i2c15, 0, 0, 0);
DECLARE_DEV(i2c15, ASPEED_DEV_I2C15, I2C15_BASE, &i2c15_priv);
#endif /* end of "#if CONFIG_DEVICE_I2C" */

/* declare I3C devices */
#if CONFIG_DEVICE_I3C
DECLARE_DEV_CLK(i3c_global, 0, 0, 0);
DECLARE_DEV_RESET(i3c_global, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(7));
DECLARE_DEV(i3c_global, ASPEED_DEV_I3C_GLOBAL, I3C_GLOBAL_BASE, NULL);

aspeed_i3c_priv_t i3c0_priv = {.clk_period = 5, .max_addr_entry = 8, .irq = I3c0_IRQn};
DECLARE_DEV_CLK(i3c0, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(8));
DECLARE_DEV_RESET(i3c0, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(8));
DECLARE_DEV(i3c0, ASPEED_DEV_I3C0, I3C0_BASE, &i3c0_priv);

aspeed_i3c_priv_t i3c1_priv = {.clk_period = 5, .max_addr_entry = 8, .irq = I3c1_IRQn};
DECLARE_DEV_CLK(i3c1, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(9));
DECLARE_DEV_RESET(i3c1, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(9));
DECLARE_DEV(i3c1, ASPEED_DEV_I3C1, I3C1_BASE, &i3c1_priv);

aspeed_i3c_priv_t i3c2_priv = {.clk_period = 5, .max_addr_entry = 8, .irq = I3c2_IRQn};
DECLARE_DEV_CLK(i3c2, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(10));
DECLARE_DEV_RESET(i3c2, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(10));
DECLARE_DEV(i3c2, ASPEED_DEV_I3C2, I3C2_BASE, &i3c2_priv);

aspeed_i3c_priv_t i3c3_priv = {.clk_period = 5, .max_addr_entry = 8, .irq = I3c3_IRQn};
DECLARE_DEV_CLK(i3c3, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(11));
DECLARE_DEV_RESET(i3c3, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(11));
DECLARE_DEV(i3c3, ASPEED_DEV_I3C3, I3C3_BASE, &i3c3_priv);
#endif /* end of "#if CONFIG_DEVICE_I3C" */

/* declare eSPI device */
#if CONFIG_DEVICE_ESPI
aspeed_espi_priv_t espi_priv = {
	.delay_timing = 0x7,
	.perif = { CONFIG_DEVICE_ESPI_HOST_MAP_ADDR, CONFIG_DEVICE_ESPI_HOST_MAP_SIZE, true },
	.oob = { true },
	.flash = { ESPI_SAFS_SW, true },
};
DECLARE_DEV_CLK(espi_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(2));
DECLARE_DEV_RESET(espi_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(25));
DECLARE_DEV(espi_dev, ASPEED_DEV_ESPI, ESPI_BASE, &espi_priv);
#endif /* end of "$if CONFIG_DEVICE_ESPI */

/* declare PECI device */
#if CONFIG_DEVICE_PECI

#define PECI_DEVICE_DECLARE(_clk_freq_, _rd_sampling_point_, _byte_mode_64_) \
aspeed_peci_priv_t peci_priv = {  \
    .clk_freq = _clk_freq_, \
    .rd_sampling_point = _rd_sampling_point_, \
    .byte_mode_64 = _byte_mode_64_ \
    }; \
DECLARE_DEV_CLK(peci, 0, 0, 0); \
DECLARE_DEV_RESET(peci, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(4)); \
DECLARE_DEV(peci, ASPEED_DEV_PECI, PECI_BASE, &peci_priv)

PECI_DEVICE_DECLARE(1000000, 8, false);
#endif /* end of "#if CONFIG_DEVICE_IPI" */

/* declare PWM TACH device */
#if CONFIG_DEVICE_PWM_TACH
aspeed_g_pwm_tach_priv_t g_pwm_tach_priv;
DECLARE_DEV_CLK(g_pwm_tach, 0, 0, 0);
DECLARE_DEV_RESET(g_pwm_tach, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(5));
DECLARE_DEV(g_pwm_tach, ASPEED_DEV_G_PWM_TACH, PWM_TACH_BASE, &g_pwm_tach_priv);
/* default_duty: range from 0 to 100 */
#define PWM_DEVICE_DECLARE(channel, freq, reload_enable, reload_duty) \
aspeed_pwm_priv_t pwm ## channel ## _priv = { \
	.parent = &g_pwm_tach, \
	.pwm_channel = channel, \
	.pwm_freq = freq, \
	.wdt_reload_enable = reload_enable, \
	.wdt_reload_duty = reload_duty \
    }; \
DECLARE_DEV_CLK(pwm ## channel, 0, 0, 0); \
DECLARE_DEV_RESET(pwm ## channel, 0, 0, 0); \
DECLARE_DEV(pwm ## channel, ASPEED_DEV_PWM ## channel, PWM_TACH_BASE, &pwm ## channel ## _priv);

#define TACH_DEVICE_DECLARE(channel, div, pulse_pr, min_rpm) \
aspeed_tach_priv_t tach ## channel ## _priv = { \
	.parent = &g_pwm_tach, \
	.tach_channel = channel, \
	.tach_div = div, \
	.fan_pulse_pr = pulse_pr, \
	.fan_min_rpm = min_rpm, \
    }; \
DECLARE_DEV_CLK(tach ## channel, 0, 0, 0); \
DECLARE_DEV_RESET(tach ## channel, 0, 0, 0); \
DECLARE_DEV(tach ## channel, ASPEED_DEV_TACH ## channel, PWM_TACH_BASE, &tach ## channel ## _priv);
/* Don't change above */

PWM_DEVICE_DECLARE(0, 25000, 1, 70);
PWM_DEVICE_DECLARE(1, 25000, 0, 0);
PWM_DEVICE_DECLARE(2, 25000, 0, 0);
PWM_DEVICE_DECLARE(3, 25000, 0, 0);
PWM_DEVICE_DECLARE(4, 25000, 0, 0);
PWM_DEVICE_DECLARE(5, 25000, 0, 0);
PWM_DEVICE_DECLARE(6, 25000, 0, 0);
PWM_DEVICE_DECLARE(7, 25000, 0, 0);
PWM_DEVICE_DECLARE(8, 25000, 0, 0);
PWM_DEVICE_DECLARE(9, 25000, 0, 0);
PWM_DEVICE_DECLARE(10, 25000, 0, 0);
PWM_DEVICE_DECLARE(11, 25000, 0, 0);
PWM_DEVICE_DECLARE(12, 25000, 0, 0);
PWM_DEVICE_DECLARE(13, 25000, 0, 0);
PWM_DEVICE_DECLARE(14, 25000, 0, 0);
PWM_DEVICE_DECLARE(15, 25000, 0, 0);

TACH_DEVICE_DECLARE(0, 6, 2, 1000);
TACH_DEVICE_DECLARE(1, 6, 2, 1000);
TACH_DEVICE_DECLARE(2, 6, 2, 1000);
TACH_DEVICE_DECLARE(3, 6, 2, 1000);
TACH_DEVICE_DECLARE(4, 6, 2, 1000);
TACH_DEVICE_DECLARE(5, 6, 2, 1000);
TACH_DEVICE_DECLARE(6, 6, 2, 1000);
TACH_DEVICE_DECLARE(7, 6, 2, 1000);
TACH_DEVICE_DECLARE(8, 6, 2, 1000);
TACH_DEVICE_DECLARE(9, 6, 2, 1000);
TACH_DEVICE_DECLARE(10, 6, 2, 1000);
TACH_DEVICE_DECLARE(11, 6, 2, 1000);
TACH_DEVICE_DECLARE(12, 6, 2, 1000);
TACH_DEVICE_DECLARE(13, 6, 2, 1000);
TACH_DEVICE_DECLARE(14, 6, 2, 1000);
TACH_DEVICE_DECLARE(15, 6, 2, 1000);

#undef PWM_DEVICE_DECLARE
#undef TACH_DEVICE_DECLARE
#endif /* end of "#if CONFIG_DEVICE_PWM_TACH" */

/* declare ADC device */
#if CONFIG_DEVICE_ADC
DECLARE_DEV_CLK(g_adc, 0, 0, 0);
DECLARE_DEV_RESET(g_adc, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(23));
DECLARE_DEV(g_adc, ASPEED_DEV_G_ADC, 0, NULL);

/*
 * idx: Select adc index: 0 or 1
 * mv: reference voltage of adc
 * battery_ctrl: 1:Enable/0: Disable battery sensing mode (ch7)
 * cal_ctrl: 1:Enable/0:Disable calibration flow
 */
#define ADC_DEVICE_DECLARE(idx, mv, battery_ctrl, cal_ctrl) \
aspeed_adc_priv_t adc ## idx ## _priv = { \
	.ref_voltage_mv = mv, \
	.battery_en = battery_ctrl, \
	.do_calibration = cal_ctrl, \
	.index = idx, \
	.parent = &g_adc, \
    }; \
DECLARE_DEV_CLK(adc ## idx, 0, 0, 0); \
DECLARE_DEV_RESET(adc ## idx, 0, 0, 0); \
DECLARE_DEV(adc ## idx, ASPEED_DEV_ADC ## idx, ADC ## idx ##_BASE, \
	&adc ## idx ## _priv);
/* Don't change above */

ADC_DEVICE_DECLARE(0, 1200, 1, 1);
ADC_DEVICE_DECLARE(1, 2500, 1, 1);

#undef ADC_DEVICE_DECLARE
#endif /* end of "#if CONFIG_DEVICE_ADC" */

/* declare JTAG device */
#if CONFIG_DEVICE_JTAG
aspeed_jtag_priv_t jtag0_priv = {
	.fifo_length = 512,
};
DECLARE_DEV_CLK(jtag0, 0, 0, 0);
DECLARE_DEV_RESET(jtag0, SCU_BASE + 0x40, SCU_BASE + 0x44, BIT(22));
DECLARE_DEV(jtag0, ASPEED_DEV_JTAG0, JTAG0_BASE, &jtag0_priv);

aspeed_jtag_priv_t jtag1_priv = {
	.fifo_length = 512,
};
DECLARE_DEV_CLK(jtag1, 0, 0, 0);
DECLARE_DEV_RESET(jtag1, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(22));
DECLARE_DEV(jtag1, ASPEED_DEV_JTAG1, JTAG1_BASE, &jtag1_priv);
#endif /* end of "#if CONFIG_DEVICE_JTAG" */

/* declare KCS device */
#if CONFIG_DEVICE_KCS
aspeed_kcs_priv_t kcs1_priv = {
	.chan = KCS_CH1,
	.addr = 0xca0,
};
aspeed_kcs_priv_t kcs2_priv = {
	.chan = KCS_CH2,
	.addr = 0xca8,
};
aspeed_kcs_priv_t kcs3_priv = {
	.chan = KCS_CH3,
	.addr = 0xca2,
};
aspeed_kcs_priv_t kcs4_priv = {
	.chan = KCS_CH4,
	.addr = 0xca4,
};
DECLARE_DEV_CLK(kcs1_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(kcs1_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(kcs1_dev, ASPEED_DEV_KCS1, LPC_BASE, &kcs1_priv);
DECLARE_DEV_CLK(kcs2_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(kcs2_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(kcs2_dev, ASPEED_DEV_KCS2, LPC_BASE, &kcs2_priv);
DECLARE_DEV_CLK(kcs3_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(kcs3_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(kcs3_dev, ASPEED_DEV_KCS3, LPC_BASE, &kcs3_priv);
DECLARE_DEV_CLK(kcs4_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(kcs4_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(kcs4_dev, ASPEED_DEV_KCS4, LPC_BASE, &kcs4_priv);
#endif

/* declaure Snoop device */
#if CONFIG_DEVICE_SNOOP
aspeed_snoop_priv_t snoop_priv = {
	.chan[0] = { 0x80, true },
	.chan[1] = { 0x81, true },
};
DECLARE_DEV_CLK(snoop_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(snoop_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(snoop_dev, ASPEED_DEV_SNOOP, LPC_BASE, &snoop_priv);
#endif /* end of "#if CONFIG_DEVICE_SNOOP */

/* declare BT device */
#if CONFIG_DEVICE_BT
aspeed_bt_priv_t bt_priv = {
	.addr = 0xe4,
	.sirq = 0x0a,
};
DECLARE_DEV_CLK(bt_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(bt_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(bt_dev, ASPEED_DEV_BT, LPC_BASE, &bt_priv);
#endif

#if CONFIG_DEVICE_PCC
aspeed_pcc_priv_t pcc_priv = {
	.addr = 0x80,
	.addr_xbit = 0x3,
	.addr_hbit_sel = 0x1,
	.rec_mode = 0x1,
	.dma_mode = false,
};
DECLARE_DEV_CLK(pcc_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(0));
DECLARE_DEV_RESET(pcc_dev, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(0));
DECLARE_DEV(pcc_dev, ASPEED_DEV_PCC, LPC_BASE, &pcc_priv);
#endif

/* declaure usb device */
#if CONFIG_DEVICE_USB
DECLARE_DEV_CLK(usb_dev, SCU_BASE + 0x80, SCU_BASE + 0x84, BIT(7));
DECLARE_DEV_RESET(usb_dev, SCU_BASE + 0x40, SCU_BASE + 0x44, BIT(3));
DECLARE_DEV(usb_dev, ASPEED_DEV_USB, USB_BASE, NULL);
#endif /* end of "#if CONFIG_DEVICE_USB */

#if CONFIG_DEVICE_MDIO
DECLARE_DEV_CLK(mdio0, 0, 0, 0);
DECLARE_DEV_RESET(mdio0, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(3));
DECLARE_DEV(mdio0, ASPEED_DEV_MDIO0, MDIO0_BASE, NULL);
#endif /* end of "#if DEVICE_MDIO" */

#if CONFIG_DEVICE_MAC
aspeed_mac_priv_t mac_priv = { .cfg_rgmii.reg_set = SCU_BASE + 0x510, .cfg_rgmii.reg_clr = SCU_BASE + 0x514, .cfg_rgmii.bits = BIT(0) };

DECLARE_DEV_CLK(mac0, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(20));
DECLARE_DEV_RESET(mac0, SCU_BASE + 0x50, SCU_BASE + 0x54, BIT(20));
DECLARE_DEV(mac0, ASPEED_DEV_MAC0, MAC_BASE, &mac_priv);
#endif	/* end of "#if DEVICE_MAC" */

#if CONFIG_DEVICE_FMC_SPI
fmc_spi_priv_t fmc_priv = {
	.name = "fmc",
	.ahb_base = 0x80000000,
	.max_cs = 2,
	.chipes = {
		{
			.max_freq = 50000000,
			.tx_bus_width = 4,
			.rx_bus_width = 4,
			.enable = true,
			.flash_component = true,
			.probed = false,
		},
		{
			.max_freq = 50000000,
			.tx_bus_width = 4,
			.rx_bus_width = 4,
			.enable = true,
			.flash_component = true,
			.probed = false,
		}
	},

	.segment_start = ast1030_fmc_segment_addr_start,
	.segment_end = ast1030_fmc_segment_addr_end,
	.segment_value = ast1030_fmc_segment_addr_val,
};

fmc_spi_priv_t spi1_priv = {
	.name = "spi1",
	.ahb_base = 0x90000000,
	.max_cs = 2,
	.chipes = {
		{
			.max_freq = 50000000,
			.tx_bus_width = 4,
			.rx_bus_width = 4,
			.enable = true,
			.flash_component = true,
			.probed = false,
		},
		{
			.max_freq = 50000000,
			.tx_bus_width = 4,
			.rx_bus_width = 4,
			.enable = true,
			.flash_component = true,
			.probed = false,
		},
	},

	.segment_start = ast1030_spi_segment_addr_start,
	.segment_end = ast1030_spi_segment_addr_end,
	.segment_value = ast1030_spi_segment_addr_val,
};

fmc_spi_priv_t spi2_priv = {
	.name = "spi2",
	.ahb_base = 0xB0000000,
	.max_cs = 2,
	.chipes = {
		{
			.max_freq = 50000000,
			.tx_bus_width = 4,
			.rx_bus_width = 4,
			.enable = true,
			.flash_component = true,
			.probed = false,
		},
		{
			.max_freq = 50000000,
			.tx_bus_width = 4,
			.rx_bus_width = 4,
			.enable = true,
			.flash_component = true,
			.probed = false,
		}
	},

	.segment_start = ast1030_spi_segment_addr_start,
	.segment_end = ast1030_spi_segment_addr_end,
	.segment_value = ast1030_spi_segment_addr_val,
};

DECLARE_DEV_CLK(fmc_dev, 0, 0, 0);
DECLARE_DEV_RESET(fmc_dev, 0, 0, 0);
DECLARE_DEV(fmc_dev, ASPEED_DEV_FMC, FMC_BASE, &fmc_priv);

DECLARE_DEV_CLK(spi1_dev, 0, 0, 0);
DECLARE_DEV_RESET(spi1_dev, 0, 0, 0);
DECLARE_DEV(spi1_dev, ASPEED_DEV_SPI1, SPI1_BASE, &spi1_priv);

DECLARE_DEV_CLK(spi2_dev, 0, 0, 0);
DECLARE_DEV_RESET(spi2_dev, 0, 0, 0);
DECLARE_DEV(spi2_dev, ASPEED_DEV_SPI2, SPI2_BASE, &spi2_priv);

/* declaure hac engine */
DECLARE_DEV_CLK(hace_dev, SCU_BASE + 0x84, SCU_BASE + 0x80, BIT(13));
DECLARE_DEV_RESET(hace_dev, SCU_BASE + 0x40, SCU_BASE + 0x44, BIT(4));
DECLARE_DEV(hace_dev, ASPEED_DEV_HACE, HACE_BASE, NULL);

/* declaure rsa controller */
DECLARE_DEV_CLK(rsa_dev, SCU_BASE + 0x94, SCU_BASE + 0x90, BIT(6));
DECLARE_DEV_RESET(rsa_dev, 0, 0, 0);
DECLARE_DEV(rsa_dev, ASPEED_DEV_RSA, SEC_BASE, NULL);

#endif
