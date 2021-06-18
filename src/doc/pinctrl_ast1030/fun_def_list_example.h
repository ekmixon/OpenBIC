/*
 * Copyright (c) 2020-2021 Aspeed Technology Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * this file lists all the possible signal-function combinations so the the 
 * system developers can make fun_def_list.h for their own board easily by 
 * copy-and-paste.
 *
 * Noticed that this file is just for reference, please don't include it
 * directly since these objects may result in huge amoungs of pin conflicts.
*/
#error "fun_def_list_example.h is just for reference, please don't include it directly."

#include "config.h"

#ifdef CONFIG_STDIO_UART
#if (CONFIG_STDIO_UART == 10)
FUN_DEFINE(UART10, TXD11, RXD11)
#endif
#endif

#ifdef CONFIG_DEVICE_ADC
FUN_DEFINE(ADC0, ADC0)
FUN_DEFINE(ADC1, ADC1)
FUN_DEFINE(ADC2, ADC2)
FUN_DEFINE(ADC3, ADC3)
FUN_DEFINE(ADC4, ADC4)
FUN_DEFINE(ADC5, ADC5)
FUN_DEFINE(ADC6, ADC6)
FUN_DEFINE(ADC7, ADC7)
FUN_DEFINE(ADC8, ADC8)
FUN_DEFINE(ADC9, ADC9)
FUN_DEFINE(ADC10, ADC10)
FUN_DEFINE(ADC11, ADC11)
FUN_DEFINE(ADC12, ADC12)
FUN_DEFINE(ADC13, ADC13)
FUN_DEFINE(ADC14, ADC14)
FUN_DEFINE(ADC15, ADC15)
#endif /* end of "#if CONFIG_DEVICE_ADC" */

#ifdef CONFIG_DEVICE_PWM_TACH
FUN_DEFINE(PWM0, PWM0)
FUN_DEFINE(PWM1, PWM1)
FUN_DEFINE(PWM2, PWM2)
FUN_DEFINE(PWM3, PWM3)
FUN_DEFINE(PWM4, PWM4)
FUN_DEFINE(PWM5, PWM5)
FUN_DEFINE(PWM6, PWM6)
FUN_DEFINE(PWM7, PWM7)
FUN_DEFINE(PWM8, PWM8)
FUN_DEFINE(PWM9, PWM9)
FUN_DEFINE(PWM10, PWM10)
FUN_DEFINE(PWM11, PWM11)
FUN_DEFINE(PWM12, PWM12)
FUN_DEFINE(PWM13, PWM13)
FUN_DEFINE(PWM14, PWM14)
FUN_DEFINE(PWM15, PWM15)
FUN_DEFINE(TACH0, TACH0)
FUN_DEFINE(TACH1, TACH1)
FUN_DEFINE(TACH2, TACH2)
FUN_DEFINE(TACH3, TACH3)
FUN_DEFINE(TACH4, TACH4)
FUN_DEFINE(TACH5, TACH5)
FUN_DEFINE(TACH6, TACH6)
FUN_DEFINE(TACH7, TACH7)
FUN_DEFINE(TACH8, TACH8)
FUN_DEFINE(TACH9, TACH9)
FUN_DEFINE(TACH10, TACH10)
FUN_DEFINE(TACH11, TACH11)
FUN_DEFINE(TACH12, TACH12)
FUN_DEFINE(TACH13, TACH13)
FUN_DEFINE(TACH14, TACH14)
FUN_DEFINE(TACH15, TACH15)
#endif

#ifdef CONFIG_DEVICE_JTAG
FUN_DEFINE(JTAGM0, MTRSTN1, MTDI1, MTCK1, MTMS1, MTDO1)
FUN_DEFINE(JTAGM1, MTRSTN2, MTDI2, MTCK2, MTMS2, MTDO2)
#endif

#ifdef CONFIG_DEVICE_GPIO
#endif

#ifdef CONFIG_DEVICE_I3C
/* low voltage I3C function declaration */
FUN_DEFINE(I3C0, I3C1SCL, I3C1SDA)
FUN_DEFINE(I3C1, I3C2SCL, I3C2SDA)
FUN_DEFINE(I3C2, I3C3SCL, I3C3SDA)
FUN_DEFINE(I3C3, I3C4SCL, I3C4SDA)

/* high voltage I3C function declaration */
FUN_DEFINE(HVI3C0, HVI3C1SCL, HVI3C1SDA)
FUN_DEFINE(HVI3C1, HVI3C2SCL, HVI3C2SDA)
FUN_DEFINE(HVI3C2, HVI3C3SCL, HVI3C3SDA)
FUN_DEFINE(HVI3C3, HVI3C4SCL, HVI3C4SDA)
#endif

#ifdef CONFIG_DEVICE_I2C
FUN_DEFINE(I2C0, SCL1, SDA1)
FUN_DEFINE(I2C1, SCL2, SDA2)
FUN_DEFINE(I2C2, SCL3, SDA3)
FUN_DEFINE(I2C3, SCL4, SDA4)
FUN_DEFINE(I2C4, SCL5, SDA5)
FUN_DEFINE(I2C5, SCL6, SDA6)
FUN_DEFINE(I2C6, SCL7, SDA7)
FUN_DEFINE(I2C7, SCL8, SDA8)
FUN_DEFINE(I2C8, SCL9, SDA9)
FUN_DEFINE(I2C9, SCL10, SDA10)
FUN_DEFINE(I2C10, SCL11, SDA11)
FUN_DEFINE(I2C11, SCL12, SDA12)
FUN_DEFINE(I2C12, SCL13, SDA13)
FUN_DEFINE(I2C13, SCL14, SDA14)
#endif