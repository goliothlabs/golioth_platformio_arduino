
//file: main.cpp
#include "Arduino.h"
#include "WiFi.h"
#include "../credentials.h"
#include "golioth.h"

golioth_client_t client;

// Current firmware version
static const char* _current_version = "1.0.0";

#define TAG "golioth_example"

void setup(){
    Serial.begin(115200);
    while(!Serial){
        ; // wait for serial port to connect
    }
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Gather the Golioth credentials
    golioth_client_config_t config = {};
    config.credentials.auth_type = GOLIOTH_TLS_AUTH_TYPE_PSK;
    config.credentials.psk.psk_id = PSK_ID,
    config.credentials.psk.psk_id_len = strlen(PSK_ID);
    config.credentials.psk.psk = PSK,
    config.credentials.psk.psk_len = strlen(PSK);

    // Connect to Golioth
    client = golioth_client_create(&config);
 
    // For OTA, we will spawn a background task that will listen for firmware
    // updates from Golioth and automatically update firmware on the device
    golioth_fw_update_init(client, _current_version);
}

void loop(){
    static uint8_t counter = 0;

    // Use Arduino Serial to print messages to the serial terminal
    Serial.print("Hello, Golioth! #");
    Serial.println(counter);

    // Golioth log messages will be sent to the cloud in addition to printing
    // to the serial terminal.
    GLTH_LOGI(TAG, "Updating counter to: %d", counter);

    // Send the counter value to Golioth LightDB State
    golioth_lightdb_set_int_async(client, "counter", counter, NULL, NULL);
    ++counter;

    delay(5000);
}