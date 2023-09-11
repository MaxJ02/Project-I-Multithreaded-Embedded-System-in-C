#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <gpiod.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif 

enum gpiod_line_direction { GPIOD_LINE_DIRECTION_IN, 
                            GPIOD_LINE_DIRECTION_OUT};

enum gpiod_line_edge { GPIOD_LINE_EDGE_RISING,
                       GPIOD_LINE_EDGE_FALLING,
                       GPIOD_LINE_EDGE_BOTH};

struct gpiod_line* gpiod_line_new(const uint8_t pin, const enum gpiod_line_direction direction);

void gpiod_line_toggle(struct gpiod_line* self);

void gpiod_line_blink(struct gpiod_line* self, const uint16_t blink_speed_ms);

bool gpiod_line_event_detected(struct gpiod_line* self, 
                               const enum gpiod_line_edge edge,
                               uint8_t* previous_input);

void delay_ms(const uint16_t delay_time_ms);                               


#ifdef __cplusplus
}
#endif