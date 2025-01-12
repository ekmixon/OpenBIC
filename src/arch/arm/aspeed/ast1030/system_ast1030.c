/*
 * Copyright (c) 2020-2021 Aspeed Technology Inc.
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"
#include CONFIG_SOC_INCLUDE_FILE

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#if CONFIG_TARGET_FPGA_AST1030
#define  SYSTEM_CLOCK		(48000000)
#else
#define  HPLL_FREQ			(1000000000UL)     /* Oscillator frequency */
#define  HCLK_FREQ			(200000000UL)
#define  SYSTEM_CLOCK		HCLK_FREQ
#endif


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[240];


/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;  /* System Core Clock Frequency */


/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
  SystemCoreClock = SYSTEM_CLOCK;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
	SCB->VTOR = (uint32_t) &(__VECTOR_TABLE[0]);
#endif

#if defined (__FPU_USED) && (__FPU_USED == 1U)
  SCB->CPACR |= ((3U << 10U*2U) |           /* enable CP10 Full Access */
                 (3U << 11U*2U)  );         /* enable CP11 Full Access */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
  SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif
  SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;

  SystemCoreClock = SYSTEM_CLOCK;
}
