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
 * @file MagneticField.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _MagneticField_H_
#define _MagneticField_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "Header.h"
#include "Vector3.h"

/*!
 * @brief This struct represents the structure MagneticField defined by the user in the IDL file.
 * @ingroup MAGNETICFIELD
 */
typedef struct MagneticField
{
    Header header;
    Vector3 magnetic_field;
    double magnetic_field_covariance[9];
} MagneticField;

struct ucdrBuffer;

bool MagneticField_serialize_topic(struct ucdrBuffer* writer, const MagneticField* topic);
bool MagneticField_deserialize_topic(struct ucdrBuffer* reader, MagneticField* topic);
uint32_t MagneticField_size_of_topic(const MagneticField* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _MagneticField_H_