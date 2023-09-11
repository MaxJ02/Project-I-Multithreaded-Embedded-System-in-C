#include <stdio.h>
#include <pthread.h>
#include <gpiod_utils.h>
#include <semaphore.h>

#define SEMAPHORE_ID_CONSOLE 0

/***********************************************************
* @brief: A simple program that uses the gpiod_utils library
*         to blink four LEDs using two push buttons. The
*         program uses two threads to handle the push buttons
*         and the four LEDs.
************************************************************/

/********************************************************************************
 * @brief Structure containing thread arguments.
 * 
 * @param led 
 *        Reference to gpiod line connected to a LED.
 * @param button
 *        Reference to gpiod line connected to a button.
 * @param blink_speed_ms
 *        Blink speed of the LED at pressdown of the button.
 ********************************************************************************/
struct thread_args 
{
    struct gpiod_line* led_a;
    struct gpiod_line* led_b;
    struct gpiod_line* button;
    uint16_t blink_speed_ms;
    bool blink_enabled;
    uint8_t previous_input;
};


/********************************************************************************
 * @brief Thread function that handles a LED and a button.
 * 
 * @param arg 
 *        Reference to a thread_args struct.
 ********************************************************************************/

// printf("LED connected to pin %hu enabled!\n\n", gpiod_line_offset(self->led));
        
void* run_thread(void* arg) 
{
    struct thread_args* self = (struct thread_args*)(arg);

    while (1)
    {
        if (gpiod_line_event_detected(self->button, GPIOD_LINE_EDGE_RISING, &self->previous_input)) 
        {
            self->blink_enabled = !self->blink_enabled;
            semaphore_reserve(SEMAPHORE_ID_CONSOLE);

            if (self->blink_enabled == true) {
                printf("LEDs connected to pin %hu and %hu are now enabled!\n", 
                    gpiod_line_offset(self->led_a), gpiod_line_offset(self->led_b));
            } else {
                printf("LEDs connected to pin %hu and %hu are now disabled!\n", 
                    gpiod_line_offset(self->led_a), gpiod_line_offset(self->led_b));
            }
            delay_ms(10);
            semaphore_release(SEMAPHORE_ID_CONSOLE);
        }

        if (self->blink_enabled) 
        {
            gpiod_line_set_value(self->led_a, 1);
            gpiod_line_set_value(self->led_b, 0);
            delay_ms(self->blink_speed_ms);

            gpiod_line_set_value(self->led_a, 0);
            gpiod_line_set_value(self->led_b, 1);
            delay_ms(self->blink_speed_ms);
        } 
        else
        {
            gpiod_line_set_value(self->led_a, 0);
            gpiod_line_set_value(self->led_b, 0);
        }
    }
    return 0;
}

/********************************************************************************
 * @brief Creates two threads and connects two LEDs and a button in each thread.
 ********************************************************************************/
int main(void) 
{
    struct gpiod_line* led1 = gpiod_line_new(17, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* led2 = gpiod_line_new(22, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* led3 = gpiod_line_new(23, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* led4 = gpiod_line_new(24, GPIOD_LINE_DIRECTION_OUT);

    struct gpiod_line* button1 = gpiod_line_new(27, GPIOD_LINE_DIRECTION_IN);
    struct gpiod_line* button2 = gpiod_line_new(25, GPIOD_LINE_DIRECTION_IN);

    struct thread_args args1 = { led1, led2, button1, 100, false, 0 };
    struct thread_args args2 = { led3, led4, button2, 500, false, 0 };
    pthread_t t1, t2; 

    pthread_create(&t1, 0, run_thread, &args1);
    pthread_create(&t2, 0, run_thread, &args2);
    pthread_join(t1, 0);
    pthread_join(t2, 0);
    return 0;
}