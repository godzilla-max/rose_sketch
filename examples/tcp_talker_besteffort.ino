// Copyright 2018 eSOL Co.,Ltd.
// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <Arduino.h>
#include <WiFiEsp.h>
#include <Ethernet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "ros2_msg/Ros2String.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
}

// Micro-XRCE-DDS-Client key identifier of the client.
// If multiple clients connect to the same agent, each client must have a different key.
#define CLIENT_KEY      0xAAAABBBB

#define BUFFER_SIZE     UXR_CONFIG_TCP_TRANSPORT_MTU

char ssid[] = "SSID";                 // your network SSID (name)
char pass[] = "PASSWORD";             // your network password
int status = WL_IDLE_STATUS;          // the Wifi radio's status
byte mac[] = { 0x74, 0x90, 0x50, 0x00, 0x79, 0x03 };
IPAddress ip(192, 168, 2, 52);

uxrSession session;
uxrTCPTransport transport;
uxrTCPPlatform tcp_platform;
uxrObjectId datawriter_id;
uxrStreamId output_stream;
uxrStreamId input_stream;
uint32_t count = 0;
static char agent_ip[32] = {0};

uint8_t output_best_effort_stream_buffer[BUFFER_SIZE];
uint8_t input_best_effort_stream_buffer[BUFFER_SIZE];

static bool use_ethernet = false;

void on_agent_found(const uxrAgentAddress* address, void* args);
static void prvUXRManagerTask(void * pvParameters);

void setup() {
    // Initialize the LED pin
    pinMode(PIN_LED1, OUTPUT);

    if(!use_ethernet)
    {
        // initialize serial for debugging
        Serial.begin(115200);
        // initialize serial for ESP module
        Serial6.begin(115200);
        // initialize ESP module
        WiFi.init(&Serial6);
    }
    else
    {
        // Serial output to USB
        Serial.begin(9600);
    }

    if(!use_ethernet)
    {
        // check for the presence of the shield
        if (WiFi.status() == WL_NO_SHIELD) {
            Serial.println("WiFi shield not present");
            // don't continue
            while (true);
        }
        // attempt to connect to WiFi network
        while ( status != WL_CONNECTED) {
            Serial.print("Attempting to connect to WPA SSID: ");
            Serial.println(ssid);
            // Connect to WPA/WPA2 network
            status = WiFi.begin(ssid, pass);
        }
    }
    else
    {
        // Setting IP
        Ethernet.begin(mac, ip);
    }

    // Wait for network configuration
    vTaskDelay(5000);

    // Discovery Agent
    Serial.println("Discovery Agent...");
    uxrAgentAddress chosen;
    chosen.ip = agent_ip;


    // Choose Ethernet or WiFi
    uxr_discovery_choose_ethernet(use_ethernet);

    // Try forever until Agent is found
    uxr_discovery_agents_default(INT_MAX, 1000, on_agent_found, &chosen);

    Serial.print("Chosen agent => ip: ");
    Serial.print(chosen.ip);
    Serial.print(", port: ");
    Serial.println(chosen.port);

    // Transport
    tcp_platform.use_ethernet = use_ethernet;
    if (!uxr_init_tcp_transport(&transport, &tcp_platform, chosen.ip, chosen.port))
    {
        Serial.println("Error at create transport.");
        return;
    }

    // Session
    uxr_init_session(&session, &transport.comm, CLIENT_KEY);
    if (!uxr_create_session(&session))
    {
        Serial.println("Error at create session.");
        return;
    }

    // Streams
    output_stream = uxr_create_output_best_effort_stream(&session, output_best_effort_stream_buffer, BUFFER_SIZE);
    input_stream = uxr_create_input_best_effort_stream(&session);

    // Create entities
    uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char* participant_xml = "<dds>"
                                      "<participant>"
                                          "<rtps>"
                                              "<name>default_xrce_participant</name>"
                                          "</rtps>"
                                      "</participant>"
                                  "</dds>";
    uint16_t participant_req = uxr_buffer_create_participant_xml(&session, output_stream, participant_id, 0, participant_xml, UXR_REPLACE);

    // In order to avoid buffer overflow, uxr_flash_output_streams() has to be
    // called everytime entities message is created.
    (void) uxr_flash_output_streams(&session);

    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    const char* topic_xml = "<dds>"
                                "<topic>"
                                    "<name>rt/chatter</name>"
                                    "<dataType>std_msgs::msg::dds_::String_</dataType>"
                                "</topic>"
                            "</dds>";
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, output_stream, topic_id, participant_id, topic_xml, UXR_REPLACE);

    // In order to avoid buffer overflow, uxr_flash_output_streams() has to be
    // called everytime entities message is created.
    (void) uxr_flash_output_streams(&session);

    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char* publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, output_stream, publisher_id, participant_id, publisher_xml, UXR_REPLACE);

    // In order to avoid buffer overflow, uxr_flash_output_streams() has to be
    // called everytime entities message is created.
    (void) uxr_flash_output_streams(&session);

    datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    const char* datawriter_xml = "<dds>"
                                     "<data_writer>"
                                         "<topic>"
                                             "<kind>NO_KEY</kind>"
                                             "<name>rt/chatter</name>"
                                             "<dataType>std_msgs::msg::dds_::String_</dataType>"
                                         "</topic>"
                                     "</data_writer>"
                                 "</dds>";
    uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, output_stream, datawriter_id, publisher_id, datawriter_xml, UXR_REPLACE);

    // In order to avoid buffer overflow, uxr_flash_output_streams() has to be
    // called everytime entities message is created.
    (void) uxr_flash_output_streams(&session);

    xTaskCreate(prvUXRManagerTask, "TalkerDemo", configMINIMAL_STACK_SIZE * 5, NULL, 2, NULL);
}

void loop() {
    // Do nothing
    vTaskDelay(10000);
}

void on_agent_found(const uxrAgentAddress* address, void* args) {
    uxrAgentAddress* agent = (uxrAgentAddress*)args;

    Serial.print("Found agent => ip: ");
    Serial.print(address->ip);
    Serial.print(", port: ");
    Serial.println(address->port);

    memcpy((void*)(agent->ip), address->ip, 32);
    agent->port = address->port;
}

static void prvUXRManagerTask(void * pvParameters) {
    // Cannot confirm whether client is connected to agent or not.
    while (1)
    {
        // Topic serialization
        Ros2String topic;
        sprintf(topic.data, "Hello DDS world!: %lu", count++);

        ucdrBuffer mb;
        uint32_t topic_size = Ros2String_size_of_topic(&topic, 0);
        uxr_prepare_output_stream(&session, output_stream, datawriter_id, &mb, topic_size);
        Ros2String_serialize_topic(&mb, &topic);

        /* Set timeout period to 0s. */
        (void) uxr_run_session_time(&session, 0);
        Serial.print("Sent topic: ");
        Serial.println(topic.data);

        // Toggle the heartbeat LED
        digitalWrite(PIN_LED1, !digitalRead(PIN_LED1));

        /* Set timeout period to 1s. */
        vTaskDelay(1000);
    }

    // Delete resources
    (void) uxr_delete_session(&session);
    (void) uxr_close_tcp_transport(&transport);
    
    // Delete this task
    vTaskDelete(NULL);
}
