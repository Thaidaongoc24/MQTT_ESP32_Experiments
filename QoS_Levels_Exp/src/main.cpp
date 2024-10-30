#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "ca_cert.h"
#include <WiFiClientSecure.h>
#include "secrets/mqtt.h"
#include "secrets/wifi.h"
// #include <mqtt_client.h> // native esp-idf mqtt client for more control, e.g. QoS 2.

// WiFi: they are in secrets/wifi.h; if not please uncomment and provide them below
// const char *ssid = "xxxxx"; // Enter your Wi-Fi name
// const char *password = "xxxxx";  // Enter Wi-Fi password

// MQTT Broker
const char *echo_topic = "esp32/echo_test";
const char *lwt_topic = "esp32/status";
const char *lwt_message = "offline";
const char *retained_topic = "esp32/retained";
const char *non_retained_topic = "esp32/non_retained";

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("From %s:  ", topic);
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}
// MQTT crendentials: can be found in secrets/mqtt.h, if not found please uncomment and provide them below
// const char *mqtt_broker = "broker.emqx.io";
// const char *topic = "emqx/esp32";
// const int mqtt_port = 1883;
// const char *mqtt_username = "xxxx";
// const char *mqtt_password = "xxxxxx";

WiFiClientSecure tlsClient;
PubSubClient mqttClient(tlsClient);

inline void setup_wifi(const char *SSID, const char *PASS)
{
    delay(10);
    Serial.printf("Connecting to %s ", SSID);
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\r\nConnected to SSID: '%s'\r\n", WiFi.SSID().c_str());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

inline void connect_mqtt(PubSubClient &client, const char *address, const int port)
{
    client.setCallback(mqttCallback);
    client.setServer(address, port);
    client.setKeepAlive(5); // default 15s, set to 5s for testing --> expect to see LWT message after ~7.5
    while (!client.connected())
    {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\r\n", client_id.c_str());
        // Connect to the MQTT broker with LWT (Last Will and Testament):
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password, lwt_topic, 2, true, "offline"))
        {
            Serial.println("Public MQTT broker connected");
            client.publish(lwt_topic, "online", true);
        }
        else
        {
            Serial.print("Failed with state ");
            Serial.println(client.state());
            delay(1000);
        }
    }
    // Connect with Clean Session = true: the broker will remove all information about the client
    // client.connect(client_id.c_str(), mqtt_username, mqtt_password);
    // Connect with Clean Session = false: the broker will keep the information about the client
    // client.connect(client_id.c_str(), mqtt_username, mqtt_password, lwt_topic, 2, true, "offline");
}

void setup()
{
    Serial.begin(115200);
    setup_wifi(ssid, password);

    tlsClient.setCACert(ca_cert);
    connect_mqtt(mqttClient, mqtt_broker, mqtt_port);
    /**
     * Echo test:
     * - Pub to echo_topic with QoS = 0 and retained = false
     * - Sub echo_topic with QoS 0: can be QoS 1.
     * - Scenario: Pub numbers from 0 to 99 to the echo_topic, then listen (subscribe) to this topic to see any message loss.
     */
    mqttClient.subscribe(echo_topic);
    Serial.println("Publishing 100 messages to the echo topic ...");
    for (int i = 0; i < 100; i++) // 100 messages
    {
        mqttClient.publish(echo_topic, String(i).c_str(), true);
        delay(1); // 1 ms one message
    }
    mqttClient.loop();
    delay(1000); // wait for the last message to be received
    /**
     * Test Retained message:
     * - Pub to retained_topic with QoS = 0 and retained = true
     * - Sub retained_topic with QoS 0: can be QoS 1.
     * - Scenario: subcribe to the retained_topic AFTER publishing to see the retained message.
     * - Publish a non-retained message to non_retained_topic, THEN subsribe to non_retained_topic --> see NO message.
     */
    // mqttClient.publish(retained_topic, "This is a retained message", true);
    // mqttClient.subscribe(retained_topic);
    // mqttClient.loop();
    // // Publish a non-retained message
    // Serial.println("Publish a non-retained message");
    // mqttClient.publish(non_retained_topic, "This is a non-retained message", false);
    // mqttClient.subscribe(non_retained_topic);
    // Serial.println("Subscribed to non-retained topic ...");
    // mqttClient.loop();
    // Serial.println("Expected: No message from the non-retained topic");

    /**
     * Test LWT - Last Will and Testament:
     * - Connect this client to the broker with LWT setting as in connect_mqtt()
     * - Using a PC client (e.g. EMPX Client) to connect to the broker and subscribe to the LWT topic
     * - Disconnect the wifi Client (wating for 1.5 x KeepAlive time)
     * - Expect to see the LWT message on the PC client
     */
    // tlsClient.stop(); // disconnect the wifi client
    // Serial.println("The wifi client is disconnected");
    // // Count down 8s to see the LWT message
    // for (int i = 8; i > 0; i--)
    // {
    //     Serial.printf("Waiting for %d seconds ...\n", i);
    //     delay(1000);
    // }
    // tlsClient.connect(mqtt_broker, mqtt_port);
}

void loop()
{
    mqttClient.loop();
}
