/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed On an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MOCK_MT_ACCESSIBILITY_TOUCHEVENT_INJECTOR_H
#define MOCK_MT_ACCESSIBILITY_TOUCHEVENT_INJECTOR_H

#include <gmock/gmock.h>
#include "accessibility_touchEvent_injector.h"

namespace OHOS {
namespace Accessibility {
class MockMtTouchInjectHandler : public TouchInjectHandler {
public:
    MockMtTouchInjectHandler(const std::shared_ptr<AppExecFwk::EventRunner>& runner, TouchEventInjector& server);
    virtual ~MockMtTouchInjectHandler() = default;

    MOCK_METHOD1(ProcessEvent, void(const AppExecFwk::InnerEvent::Pointer& event));
};

class MockTouchEventInjector : public TouchEventInjector {
public:
    MockTouchEventInjector();
    ~MockTouchEventInjector()
    {}

    MOCK_METHOD1(OnPointerEvent, void(MMI::PointerEvent& event));
    MOCK_METHOD0(DestroyEvents, void());
    MOCK_METHOD1(InjectEvents, void(const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath));
    MOCK_METHOD1(SendPointerEvent, void(MMI::PointerEvent& event));
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_MT_ACCESSIBILITY_TOUCHEVENT_INJECTOR_H