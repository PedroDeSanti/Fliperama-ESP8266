/**
 * @file button.h
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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

// #include <string>

class Button {
    public:
        /**
         * @brief Construct a new Button object
         *
         * @param GPIO_pin Pin where the button is connected to the board
         * @param topic Topic string (std::string can be used)
         */
        Button(int GPIO_pin, const char* topic);

        /**
         * @brief Destroy the Button object
         *
         */
        ~Button();

        /**
         * @brief Identify the button's rising edge, when it is clicked
         *
         * @return true
         * @return false
         */
        bool is_pressed();

        /**
         * @brief Identify the button's falling edge, when it is released
         *
         * @return true
         * @return false
         */
        bool is_released();

        /**
         * @brief Updates Button state and checks if it changed
         *
         * @return true if Button state changed
         * @return false if Button state did not change
         */
        bool has_changed();

        /**
         * @brief Get the Button's topic string
         *
         * @return char* topic string
         */
        char* get_topic();

        /**
         * @brief Get the Button's state in string format
         *
         * @return char* "1" when it is clicked, "0" when it is not
         */
        char* get_string_state();

        /**
         * @brief Get the Button's state
         *
         * @return true if it is clicked
         * @return false if it is not clicked
         */
        bool get_state();

    private:
        int GPIO_pin;
        bool previous_state;
        bool current_state;

        // std::string topic;
        char* topic;
};

#endif // __BUTTON_H__
