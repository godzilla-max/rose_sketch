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
 * @file Odometry.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _Odometry_H_
#define _Odometry_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "Header.h"
#include "PoseWithCovariance.h"
#include "TwistWithCovariance.h"

/*!
 * @brief This struct represents the structure Odometry defined by the user in the IDL file.
 * @ingroup ODOMETRY
 */
typedef struct Odometry
{
    Header header;
    char child_frame_id[255];

    PoseWithCovariance pose;
    TwistWithCovariance twist;
} Odometry;

struct ucdrBuffer;

bool Odometry_serialize_topic(struct ucdrBuffer* writer, const Odometry* topic);
bool Odometry_deserialize_topic(struct ucdrBuffer* reader, Odometry* topic);
uint32_t Odometry_size_of_topic(const Odometry* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _Odometry_H_