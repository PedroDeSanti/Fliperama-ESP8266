/**
 * @file led.cpp
 *
 * @author Pedro Henrique Martins de Santi <pedrodesanti@usp.br>
 *
 * @brief Implementation of Led class
 *
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include "led.h"

/*****************************************
 * Public Functions
 *****************************************/

Led::Led(int GPIO_pin, const char* topic) {
    this->GPIO_pin = GPIO_pin;
    this->topic = (char*) topic;

    // this->topic = topic; // with std::string (must be converted with c_str() afterwards)
}

Led::~Led() {
}

char* Led::get_topic() {
    return this->topic;
}

int Led::get_pin() {
    return this->GPIO_pin;
}

bool Led::get_state() {
    return this->state;
}

void Led::set_state(bool state) {
    digitalWrite(this->GPIO_pin, state);
    this->state = state;
}
