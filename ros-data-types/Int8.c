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
 * @file Int8.c
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#include "Int8.h"

#include <ucdr/microcdr.h>
#include <string.h>

bool Int8_serialize_topic(ucdrBuffer* writer, const Int8* topic)
{
    (void) ucdr_serialize_char(writer, topic->data);

    return !writer->error;
}

bool Int8_deserialize_topic(ucdrBuffer* reader, Int8* topic)
{
    (void) ucdr_deserialize_char(reader, &topic->data);

    return !reader->error;
}

uint32_t Int8_size_of_topic(const Int8* topic, uint32_t size)
{
    uint32_t previousSize = size;
    size += ucdr_alignment(size, 1) + 1;

    return size - previousSize;
}
