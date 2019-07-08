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
 * @file JointTrajectoryPoint.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _JointTrajectoryPoint_H_
#define _JointTrajectoryPoint_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "Duration.h"

/*!
 * @brief This struct represents the structure JointTrajectoryPoint defined by the user in the IDL file.
 * @ingroup JOINTTRAJECTORYPOINT
 */
typedef struct JointTrajectoryPoint
{
    uint32_t positions_size;
    double positions[100];

    uint32_t velocities_size;
    double velocities[100];

    uint32_t accelerations_size;
    double accelerations[100];

    uint32_t effort_size;
    double effort[100];

    Duration time_from_start;
} JointTrajectoryPoint;

struct ucdrBuffer;

bool JointTrajectoryPoint_serialize_topic(struct ucdrBuffer* writer, const JointTrajectoryPoint* topic);
bool JointTrajectoryPoint_deserialize_topic(struct ucdrBuffer* reader, JointTrajectoryPoint* topic);
uint32_t JointTrajectoryPoint_size_of_topic(const JointTrajectoryPoint* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _JointTrajectoryPoint_H_