/**
 * @file led.h
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

#ifndef __LED_H__
#define __LED_H__

#include <Arduino.h>

// #include <string>

class Led {
    public:
        /**
         * @brief Construct a new Led object
         *
         * @param GPIO_pin Pin where the button is connected to the board
         * @param topic Topic string (std::string can be used)
         */
        Led(int GPIO_pin, const char* topic);

        /**
         * @brief Destroy the Led object
         *
         */
        ~Led();

        /**
         * @brief Get Led's topic string
         *
         * @return char* topic string
         */
        char* get_topic();

        /**
         * @brief Get Led pin
         *
         * @return int GPIO_pin number
         */
        int get_pin();

        /**
         * @brief Get Led's state
         *
         * @return true if it is on
         * @return false if it is off
         */
        bool get_state();

        /**
         * @brief Set Led state
         *
         * @param state true if it is on, false if it is off
         */
        void set_state(bool state);

    private:
        int GPIO_pin;
        bool state;

        // std::string topic;
        char* topic;
};

#endif // __LED_H__
