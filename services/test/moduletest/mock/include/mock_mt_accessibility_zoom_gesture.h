/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MOCK_MT_ACCESSIBILITY_ZOOM_GESTURE_H
#define MOCK_MT_ACCESSIBILITY_ZOOM_GESTURE_H

#include <gmock/gmock.h>
#include "accessibility_zoom_gesture.h"

namespace OHOS {
namespace Accessibility {
class MockMtAccessibilityZoomGesture : public AccessibilityZoomGesture {
public:
    MockMtAccessibilityZoomGesture();
    ~MockMtAccessibilityZoomGesture();

    MOCK_METHOD1(Triple, bool(MMI::PointerEvent& event));
    MOCK_METHOD0(Up, void());
    MOCK_METHOD0(Clear, void());
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_MT_ACCESSIBILITY_ZOOM_GESTURE_H