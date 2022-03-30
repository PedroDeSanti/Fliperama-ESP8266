/**
 * @file button.cpp
 *
 * @author Pedro Henrique Martins de Santi <pedrodesanti@usp.br>
 *
 * @brief Implementation of Button class
 *
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include "button.h"

/*****************************************
 * Public Functions
 *****************************************/

Button::Button(int GPIO_pin, const char* topic) {
    this->GPIO_pin = GPIO_pin;
    this->topic = (char*) topic;

    // this->topic = topic; // with std::string (must be converted with c_str() afterwards)
}

Button::~Button() {
}

bool Button::is_pressed() {
    return (!(this->previous_state) && this->current_state);
}

bool Button::is_released() {
    return (this->previous_state && !(this->current_state));
}

bool Button::has_changed() {
    this->previous_state = this->current_state;
    this->current_state = digitalRead(this->GPIO_pin);
    return (this->current_state != this->previous_state);
}

char* Button::get_topic() {
    return this->topic;
}

char* Button::get_string_state() {
    if (this->current_state) {
        return (char*) ("1");
    } else {
        return (char*) ("0");
    }
}

bool Button::get_state() {
    return this->current_state;
}
