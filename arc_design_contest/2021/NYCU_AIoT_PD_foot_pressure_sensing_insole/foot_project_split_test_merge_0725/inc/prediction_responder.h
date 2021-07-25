/* SPDX-License-Identifier: BSD-3-Clause*/
/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 *==============================================================================
 *
 *
 * Provides an interface to take an action based on the output from the person
 * detection model.
 */


#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_EXPERIMENTAL_DETECTION_RESPONDER_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_EXPERIMENTAL_DETECTION_RESPONDER_H_

/*
 *Called every time the results of a person detection run are available. The
 * `person_score` has the numerical confidence that the captured image contains
 * a person, and `no_person_score` has the numerical confidence that the image
 * does not contain a person. Typically if person_score > no person score, the
 * image is considered to contain a person.  This threshold may be adjusted for
 * particular applications.
 */

void Prediction(float pt_score_l, float co_score_l, float pt_score_r, float co_score_r);
/*
 *void Prediction(tflite::ErrorReporter* error_reporter, float pt_score_l, float co_score_l, float pt_score_r, float co_score_r);
 */
#endif
/*
 *TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_EXPERIMENTAL_DETECTION_RESPONDER_H_
 */
