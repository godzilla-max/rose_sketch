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

#define STREAM_HISTORY  8
#define BUFFER_SIZE     UXR_CONFIG_TCP_TRANSPORT_MTU * STREAM_HISTORY

// TODO
// Need to fix "bus error" using optimization -O2
#pragma GCC optimize ("O0")
uxrSession session;

uxrTCPTransport transport;
uxrObjectId datawriter_id;
uxrStreamId output_stream;
uxrStreamId input_stream;
bool connected = true;
uint32_t count = 0;

bool on_agent_found(const uxrAgentAddress* address, int64_t timestamp, void* args);
static void prvUXRManagerTask(void * pvParameters);

void setup() {
    // Initialize the LED pin
    pinMode(PIN_LED1, OUTPUT);

    // Serial output to USB
    Serial.begin(9600);

    // Wait for network configuration
    vTaskDelay(5000);

    // Discovery Agent
    Serial.println("Discovery Agent...");
    uxrAgentAddress chosen;

    // Try forever until Agent is found
    (void) uxr_discovery_agents_multicast(INT_MAX, 1000, on_agent_found, NULL, &chosen);

    Serial.print("Chosen agent => ip: ");
    Serial.print(chosen.ip);
    Serial.print(", port: ");
    Serial.println(chosen.port);

    // Transport
    uxrTCPPlatform tcp_platform;
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
    uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
    output_stream = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

    uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
    input_stream = uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

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

    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    const char* topic_xml = "<dds>"
                                "<topic>"
                                    "<name>rt/chatter</name>"
                                    "<dataType>std_msgs::msg::dds_::String_</dataType>"
                                "</topic>"
                            "</dds>";
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, output_stream, topic_id, participant_id, topic_xml, UXR_REPLACE);

    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char* publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, output_stream, publisher_id, participant_id, publisher_xml, UXR_REPLACE);

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

    // Send create entities message and wait its status
    uint8_t status[4];
    uint16_t requests[4] = {participant_req, topic_req, publisher_req, datawriter_req};
    if (!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
    {
        Serial.print("Error at create entities: participant: ");
        Serial.print(status[0]);
        Serial.print(" topic: ");
        Serial.print(status[1]);
        Serial.print(" publisher: ");
        Serial.print(status[2]);
        Serial.print(" datawriter: ");
        Serial.println(status[3]);
    }

    xTaskCreate(prvUXRManagerTask, "TalkerDemo", configMINIMAL_STACK_SIZE * 5, NULL, 2, NULL);
}

void loop() {
    // Do nothing
    vTaskDelay(10000);
}

bool on_agent_found(const uxrAgentAddress* address, int64_t timestamp, void* args) {
    (void) timestamp; (void) args;

    Serial.print("Found agent => ip: ");
    Serial.print(address->ip);
    Serial.print(", port: ");
    Serial.println(address->port);
    return true;
}

static void prvUXRManagerTask(void * pvParameters) {
    // Loop as long as client is connected to agent.
    while (connected)
    {
        // Topic serialization
        Ros2String topic;
        sprintf(topic.data, "Hello DDS world!: %lu", count++);

        ucdrBuffer mb;
        uint32_t topic_size = Ros2String_size_of_topic(&topic, 0);
        uxr_prepare_output_stream(&session, output_stream, datawriter_id, &mb, topic_size);
        Ros2String_serialize_topic(&mb, &topic);

        // Set timeout period to 1ms in order to send messages every 10ms
        connected = uxr_run_session_until_timeout(&session, 1);
        if (connected)
        {
            Serial.print(" Sent topic: ");
            Serial.println(topic.data);
        }
        else
        {
            Serial.println("connection error");
        }

        // Toggle the heartbeat LED
        digitalWrite(PIN_LED1, !digitalRead(PIN_LED1));

        vTaskDelay(1000);
    }

    // Delete resources
    (void) uxr_delete_session(&session);
    (void) uxr_close_tcp_transport(&transport);
    
    // Delete this task
    vTaskDelete(NULL);
}