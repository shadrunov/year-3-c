/*

    Copyright 2008-2013 Michel Pollet <buserror@gmail.com>


    simavr is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simavr is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __AVR_CR_H__
#define __AVR_CR_H__
/*
 * Smallest coroutine implementation for AVR. Takes
 * 23 + (24 * tasks) bytes of SRAM to run.
 *
 * Use it like:
 *
 * AVR_TASK(mytask1, 32);
 * AVR_TASK(mytask2, 48);
 * ...
 * void my_task_function() {
 *    do {
 *        AVR_YIELD(mytask1, 1);
 *    } while (1);
 * }
 * ...
 * main() {
 *     AVR_TASK_START(mytask1, my_task_function);
 *     AVR_TASK_START(mytask2, my_other_task_function);
 *     do {
 *          AVR_RESUME(mytask1);
 *          AVR_RESUME(mytask2);
 *     } while (1);
 * }
 * NOTE: Do *not* use "static" on the function prototype, otherwise it
 * will fail to link (compiler doesn't realize the "jmp" is referencing)
 */
#include <setjmp.h>
#include <stdint.h>
static inline void _set_stack(register void *stack)
{
    asm volatile(
        "in r0, __SREG__"
        "\n\t"
        "cli"
        "\n\t"
        "out __SP_H__, %B0"
        "\n\t"
        "out __SREG__, r0"
        "\n\t"
        "out __SP_L__, %A0"
        "\n\t"
        :
        : "e"(stack) /* : */
    );
}

jmp_buf g_caller;
#define AVR_TASK(_name, _stack_size) \
    struct                           \
    {                                \
        jmp_buf jmp;                 \
        uint8_t running : 1;         \
        uint8_t stack[_stack_size];  \
    } _name
#define AVR_TASK_START(_name, _entry)                  \
    if (!setjmp(g_caller))                             \
    {                                                  \
        _set_stack(_name.stack + sizeof(_name.stack)); \
        asm volatile("rjmp " #_entry);                 \
    }
#define AVR_YIELD(_name, _sleep) \
    _name.running = !_sleep;     \
    if (!setjmp(_name.jmp))      \
    longjmp(g_caller, 1)
#define AVR_RESUME(_name)  \
    if (!setjmp(g_caller)) \
    longjmp(_name.jmp, 1)
#endif /* __AVR_CR_H__ */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,
                                         _FDEV_SETUP_WRITE);

char input_file[250] = "R+fF?s?o}aXw72(0)!JO@2ll9mgEbw*Kv=4cMc&&CD2tEX3t?L#_348#qFn@RO)e8@>z}#KF7f9W[E2Q0q0-3DeZv+dkD1332956MH!i5/_Lqzr>&^I9E8*={Y36/7--I(INKDwC0]nORRfb8g.$U)Z3P-33/12wU>#+9kbmSUYDi(5S014c-%s6S)aU55P$2m1rgeSM4d&977]a75fu+N?XBp>v(LboDB{-t5Mc#*vToW67R}g3y46b2V";
uint8_t count(char *buf, uint8_t start, uint8_t end, char to_find)
{
    uint8_t count = 0;
    for (uint8_t j = start; j < end; j++)
    {
        if (buf[j] == to_find)
        {
            count++;
        }
    }
    return count;
}

AVR_TASK(mytask1, 32);
AVR_TASK(mytask2, 32);
AVR_TASK(mytask3, 32);

uint8_t task_stat_1 = 0;
uint8_t task_stat_2 = 0;

uint8_t task_res_1 = 0;
uint8_t task_res_2 = 0;

uint16_t t = 0;

void smth()
{
    uint16_t c = 0;
    do
    {
        c++;
        if (c == 20000)
        {
            printf("running\n");
        }
        AVR_YIELD(mytask3, 1);
    } while (1);
}

void count_function1()
{
    uint16_t c = 0;
    uint8_t start = 0;
    uint8_t end = 50;
    uint8_t step = 5;
    static uint8_t i = 0;
    do
    {
        c++;
        if (c == 10000)
        {
            c = 0;
            i++;
            uint8_t curc = count(input_file, start + step * (i - 1), start + step * i, '3');
            task_res_1 += curc;
            printf("%d from proc1 read %d - %d\n", curc, start + step * (i - 1), start + step * i);

            if (start + step * i == end)
            {
                printf("Goodbye from proc1, subtotal %d\n", task_res_1 + task_res_2);
                task_stat_1 = (uint8_t)1;
            }
        }

        AVR_YIELD(mytask1, 1);
    } while (task_stat_1 == 0);
}

void count_function2()
{
    uint16_t c = 0;
    uint8_t start = 50;
    uint8_t end = 100;
    uint8_t step = 5;
    static uint8_t i = 0;

    do
    {
        c++;
        if (c == 10000)
        {
            c = 0;
            i++;
            uint8_t curc = count(input_file, start + step * (i - 1), start + step * i, '3');
            task_res_2 += curc;
            printf("%d from proc2 read %d - %d\n", curc, start + step * (i - 1), start + step * i);
            if (start + step * i == end)
            {
                task_stat_2 = (uint8_t)1;
                printf("Goodbye from proc2, subtotal %d\n", task_res_1 + task_res_2);
            }
        }
        AVR_YIELD(mytask2, 1);
    } while (task_stat_2 == 0);
}

void dispatch()
{
    AVR_TASK_START(mytask1, count_function1);
    AVR_TASK_START(mytask2, count_function2);
    AVR_TASK_START(mytask3, smth);
    static uint8_t total = 0;
    do
    {
        if (task_stat_1 == 0)
        {
            AVR_RESUME(mytask1);
        }
        if (task_stat_2 == 0)
        {
            AVR_RESUME(mytask2);
        }
        AVR_RESUME(mytask3);
    } while (1);
}

int main()
{
    stdout = &mystdout;
    sei(); // Enable global interrupts
    printf("Starting\n");
    dispatch();
}
