#pragma once
#include <vector>
#include <config.h>
typedef struct {
    String name;
    uint16_t tempo;
    int16_t data_length;
    // NOTE: Could reduce amount of memory used by introducing 
    // note selection via note index in enum and providing
    // a function to retrieve note value.
    // This would allow to use int8_t for data as amounts of 
    // different notes isn't that big as their value
    int16_t data[]; // data includes NOTES and Durations together;
}Melody;

extern std::vector<Melody *> melodies;