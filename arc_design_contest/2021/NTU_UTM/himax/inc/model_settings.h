/*
* Copyright 2019-2020, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/
#ifndef MODEL_SETTINGS_H_
#define MODEL_SETTINGS_H_

constexpr int kNumCols = 1;
constexpr int kNumRows = 4;

constexpr int kImageSize = kNumCols * kNumRows ;



constexpr int kCategoryCount = 4;
extern const char* kCategoryLabels[kCategoryCount];

#endif // MODEL_SETTINGS_H_
