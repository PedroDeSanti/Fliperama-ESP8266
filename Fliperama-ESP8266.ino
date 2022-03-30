/**
 * @file Fliperama-ESP8266.ino
 * 
 * @author Pedro Henrique Martins de Santi <pedrodesanti@usp.br>
 * 
 * @brief Main ESP8266 code
 * 
 * @date 2022-03-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "button.h"
#include "led.h"
#include "config.h"

static Button button_1(GPIO_BUTTON_1, TOPIC_BUTTON_1);
static Button button_2(GPIO_BUTTON_2, TOPIC_BUTTON_2);
static Button button_3(GPIO_BUTTON_3, TOPIC_BUTTON_3);
static Button button_4(GPIO_BUTTON_4, TOPIC_BUTTON_4);

static Button buttons[] = {button_1, button_2, button_3, button_4};

static Led led_1(GPIO_LED_1, TOPIC_LED_1);
static Led led_2(GPIO_LED_2, TOPIC_LED_2);
static Led led_3(GPIO_LED_3, TOPIC_LED_3);
static Led led_4(GPIO_LED_4, TOPIC_LED_4);

static Led leds[] = {led_1, led_2, led_3, led_4};

static int active_buttons = 0;
static int active_leds = 0;

static bool leds_working = false;

String user = "usuario mqtt";
String passwd = "senha mqtt";

const char* ssid = "nome rede";
const char* password = "senha rede";
const char* mqtt_server = "ip mqtt";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        
        Serial.print("Attempting MQTT connection...");
        
        // Create a random client ID
        String clientId = user;
        clientId += String(random(0xffff), HEX);
        
        // Attempt to connect
        if (client.connect(clientId.c_str(), user.c_str(), passwd.c_str())) {
        
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish((user+"/homehello").c_str(), "hello world");

        client.subscribe((user+"/ledhome").c_str());

        client.subscribe((user+"/S0").c_str());
        client.subscribe((user+"/S1").c_str());
        client.subscribe((user+"/S2").c_str());
        client.subscribe((user+"/S3").c_str());
        
        } else {
        
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        
        // Wait 5 seconds before retrying
        delay(5000);
        
        }
    }
}

int buzzer_map(int active_points) {
    int note;
    switch (active_points) {
        case 1000: {note = NOTE_DO_1;  break;}        
        case  100: {note = NOTE_RE_1;  break;}
        case   10: {note = NOTE_MI_1;  break;}
        case    1: {note = NOTE_FA_1;  break;}
        case 1100: {note = NOTE_SOL_1; break;}
        case 1010: {note = NOTE_LA_1;  break;}
        case 1001: {note = NOTE_SI_1;  break;}
        case  110: {note = NOTE_DO_2;  break;}
        case  101: {note = NOTE_RE_2;  break;}
        case   11: {note = NOTE_MI_2;  break;}
        case 1110: {note = NOTE_FA_2;  break;}
        case 1101: {note = NOTE_SOL_2; break;}
        case 1011: {note = NOTE_LA_2;  break;}
        case  111: {note = NOTE_SI_2;  break;}
        case 1111: {note = NOTE_DO_3;  break;} 
        default: {note = NOTE_DO_1;}
    }
    return note;
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Checks every led topic, update state if it matches
    for (int i = 0; i <= 3; i++) {
        if (strcmp(topic, leds[i].get_topic()) == 0) {
            if ((char)payload[0] == '1') {
                leds[i].set_state(true);
            }
            else {
                leds[i].set_state(false);
            }
        }
    }

    // Activate buzzer based on active leds
    active_leds = 1000*leds[0].get_state() + 100*leds[1].get_state() + 10*leds[2].get_state() + 1*leds[3].get_state();
    if (active_leds > 0) leds_working = true;
    else leds_working = false;
    Serial.println(active_leds);
    // if (active_leds > 0) tone(GPIO_BUZZER, buzzer_map(active_leds), 250);
    // if (active_leds > 0) tone(GPIO_BUZZER, buzzer_map(active_leds), 200);
    // else noTone(GPIO_BUZZER);
    // delay(250);
}

void setup() {
    // initialize the LED pin as an output:
    pinMode(GPIO_LED_1, OUTPUT);
    pinMode(GPIO_LED_2, OUTPUT);
    pinMode(GPIO_LED_3, OUTPUT);
    pinMode(GPIO_LED_4, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(GPIO_BUTTON_1, INPUT);
    pinMode(GPIO_BUTTON_2, INPUT);
    pinMode(GPIO_BUTTON_3, INPUT);
    pinMode(GPIO_BUTTON_4, INPUT);
    
    pinMode(GPIO_BUZZER, OUTPUT); // configura o pino com o buzzer como saída

    Serial.begin(9600);
    setup_wifi();
    client.setServer(mqtt_server, 80);
    client.setCallback(callback);
}

void loop() {
    // Checks if client is connected
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Updates each button state, and publishes if it changed
    for (int i = 0; i <= 3; i++) {
        if (buttons[i].has_changed()) {
            client.publish(buttons[i].get_topic(), buttons[i].get_string_state());
            digitalWrite(leds[i].get_pin(), buttons[i].get_state()); // Led control
        }
    }

    // If leds are receiving infoemation
    if (leds_working) {
        tone(GPIO_BUZZER, buzzer_map(active_leds));
    }
    else {
        // Tone buzzer based on the active buttons
        active_buttons = 1000*buttons[0].get_state() + 100*buttons[1].get_state() + 10*buttons[2].get_state() + 1*buttons[3].get_state();
        if (active_buttons > 0) tone(GPIO_BUZZER, buzzer_map(active_buttons));
        else noTone(GPIO_BUZZER);
    }

    unsigned long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish((user+"/homehello").c_str(), msg);
    }
}
