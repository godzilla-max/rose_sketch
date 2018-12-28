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
#define CLIENT_KEY      0xCCCCDDDD

#define BUFFER_SIZE     UXR_CONFIG_TCP_TRANSPORT_MTU

uxrSession session;
uxrTCPTransport transport;
uxrTCPPlatform tcp_platform;
uxrStreamId output_stream;
uxrStreamId input_stream;

void on_topic(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer* mb, void* args);
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
    if (!uxr_init_tcp_transport(&transport, &tcp_platform, chosen.ip, chosen.port))
    {
        Serial.println("Error at create transport.");
        return;
    }

    // Session
    uxr_init_session(&session, &transport.comm, CLIENT_KEY);
    uxr_set_topic_callback(&session, on_topic, NULL);
    if (!uxr_create_session(&session))
    {
        Serial.println("Error at create session.");
        return;
    }

    // Streams
    uint8_t output_best_effort_stream_buffer[BUFFER_SIZE];
    output_stream = uxr_create_output_best_effort_stream(&session, output_best_effort_stream_buffer, BUFFER_SIZE);

    uint8_t input_best_effort_stream_buffer[BUFFER_SIZE];
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

    uxrObjectId subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
    const char* subscriber_xml = "";
    uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&session, output_stream, subscriber_id, participant_id, subscriber_xml, UXR_REPLACE);

    // In order to avoid buffer overflow, uxr_flash_output_streams() has to be
    // called everytime entities message is created.
    (void) uxr_flash_output_streams(&session);

    uxrObjectId datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
    const char* datareader_xml = "<dds>"
                                     "<data_reader>"
                                         "<topic>"
                                             "<kind>NO_KEY</kind>"
                                             "<name>rt/chatter</name>"
                                             "<dataType>std_msgs::msg::dds_::String_</dataType>"
                                         "</topic>"
                                     "</data_reader>"
                                 "</dds>";
    uint16_t datareader_req = uxr_buffer_create_datareader_xml(&session, output_stream, datareader_id, subscriber_id, datareader_xml, UXR_REPLACE);

    // In order to avoid buffer overflow, uxr_flash_output_streams() has to be
    // called everytime entities message is created.
    (void) uxr_flash_output_streams(&session);

    // Request topics
    uxrDeliveryControl delivery_control = {0};
    delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
    (void) uxr_buffer_request_data(&session, output_stream, datareader_id, input_stream, &delivery_control);

    xTaskCreate(prvUXRManagerTask, "ListenerDemo", configMINIMAL_STACK_SIZE * 5, NULL, 2, NULL);
}

void loop() {
    // Do nothing
    vTaskDelay(10000);
}

void on_topic(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer* mb, void* args) {
    (void) session; (void) object_id; (void) request_id; (void) stream_id;

    Ros2String topic;
    Ros2String_deserialize_topic(mb, &topic);

    Serial.print("Received topic: ");
    Serial.println(topic.data);

    // Toggle the heartbeat LED
    digitalWrite(PIN_LED1, !digitalRead(PIN_LED1));
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
    // Cannot confirm whether client is connected to agent or not.
    while (1)
    {
        // Receive topics
        (void) uxr_run_session_time(&session, 1);
        
        vTaskDelay(1);
    }

    // Delete resources
    (void) uxr_delete_session(&session);
    (void) uxr_close_tcp_transport(&transport);

    // Delete this task
    vTaskDelete(NULL);
}
