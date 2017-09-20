/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006-2017, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-09-19     Quintin.Z    the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include "sysinit.h"
#include "board.h"
#include "drv_uart.h"
#include "nv32.h"

/* RT_USING_COMPONENTS_INIT */
#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif


#define portNVIC_SYSTICK_CTRL           ( ( volatile uint32_t *) 0xe000e010 )
#define portNVIC_SYSTICK_LOAD           ( ( volatile uint32_t *) 0xe000e014 )
#define portNVIC_INT_CTRL               ( ( volatile uint32_t *) 0xe000ed04 )
#define portNVIC_SYSPRI2                ( ( volatile uint32_t *) 0xe000ed20 )
#define portNVIC_SYSTICK_CLK            0x00000004
#define portNVIC_SYSTICK_INT            0x00000002
#define portNVIC_SYSTICK_ENABLE         0x00000001
#define portNVIC_PENDSVSET              0x10000000
#define portMIN_INTERRUPT_PRIORITY      ( 255UL )
#define portNVIC_PENDSV_PRI             ( portMIN_INTERRUPT_PRIORITY << 16UL )
#define portNVIC_SYSTICK_PRI            ( portMIN_INTERRUPT_PRIORITY << 24UL )


/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}


/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
    /* Configure the SysTick */
    *(portNVIC_SYSTICK_LOAD) = ( 40000000 / RT_TICK_PER_SECOND ) - 1UL;
    *(portNVIC_SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;

    rt_hw_uart_init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif


#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

}

long cmd_reset(int argc, char** argv)
{
    NVIC_SystemReset();

    return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(cmd_reset, __cmd_reset, Reset Board.);



/*@}*/
