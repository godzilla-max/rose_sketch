// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*! 
 * @file JointState.c
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#include "JointState.h"

#include <ucdr/microcdr.h>
#include <string.h>

bool JointState_serialize_topic(ucdrBuffer* writer, const JointState* topic)
{
    (void) Header_serialize_topic(writer, &topic->header);
    for(int i2 = 0; i2 < sizeof(topic->name) / sizeof(topic->name[0]); ++i2)
    {
        (void) ucdr_serialize_array_uint8_t(writer, topic->name[i2], sizeof(topic->name[i2]) / sizeof(uint8_t));
    }

    (void) ucdr_serialize_uint32_t(writer, topic->name_size);

    (void) ucdr_serialize_array_double(writer, topic->position, sizeof(topic->position) / sizeof(double));

    (void) ucdr_serialize_uint32_t(writer, topic->position_size);

    (void) ucdr_serialize_array_double(writer, topic->velocity, sizeof(topic->velocity) / sizeof(double));

    (void) ucdr_serialize_uint32_t(writer, topic->velocity_size);

    (void) ucdr_serialize_array_double(writer, topic->effort, sizeof(topic->effort) / sizeof(double));

    (void) ucdr_serialize_uint32_t(writer, topic->effort_size);

    return !writer->error;
}

bool JointState_deserialize_topic(ucdrBuffer* reader, JointState* topic)
{
    (void) Header_deserialize_topic(reader, &topic->header);
    for(int i2 = 0; i2 < sizeof(topic->name) / sizeof(topic->name[0]); ++i2)
    {
        (void) ucdr_deserialize_array_uint8_t(reader, topic->name[i2], sizeof(topic->name[i2]) / sizeof(uint8_t));
    }

    (void) ucdr_deserialize_uint32_t(reader, &topic->name_size);

    (void) ucdr_deserialize_array_double(reader, topic->position, sizeof(topic->position) / sizeof(double));

    (void) ucdr_deserialize_uint32_t(reader, &topic->position_size);

    (void) ucdr_deserialize_array_double(reader, topic->velocity, sizeof(topic->velocity) / sizeof(double));

    (void) ucdr_deserialize_uint32_t(reader, &topic->velocity_size);

    (void) ucdr_deserialize_array_double(reader, topic->effort, sizeof(topic->effort) / sizeof(double));

    (void) ucdr_deserialize_uint32_t(reader, &topic->effort_size);

    return !reader->error;
}

uint32_t JointState_size_of_topic(const JointState* topic, uint32_t size)
{
    uint32_t previousSize = size;
    size += Header_size_of_topic(&topic->header, size);
    for(int i2 = 0; i2 < sizeof(topic->name) / sizeof(topic->name[0]); ++i2)
    {
        size += ucdr_alignment(size, 1) + sizeof(topic->name[i2]);
    }

    size += ucdr_alignment(size, 4) + 4;

    size += ucdr_alignment(size, 8) + sizeof(topic->position);

    size += ucdr_alignment(size, 4) + 4;

    size += ucdr_alignment(size, 8) + sizeof(topic->velocity);

    size += ucdr_alignment(size, 4) + 4;

    size += ucdr_alignment(size, 8) + sizeof(topic->effort);

    size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}
