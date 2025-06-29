/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_STATE_EVENT_H
#define ACCESSIBILITY_STATE_EVENT_H

#include <cstdint>

namespace OHOS {
namespace Accessibility {
enum AccessibilityStateEventType : uint32_t {
    EVENT_ACCESSIBILITY_STATE_CHANGED = 0x00000000,
    EVENT_TOUCH_GUIDE_STATE_CHANGED,
    EVENT_KEVEVENT_STATE_CHANGED,
    EVENT_GESTURE_STATE_CHANGED,
    EVENT_SCREEN_MAGNIFIER_CHANGED,
    EVENT_AUTO_CLICK_CHANGED,
    EVENT_SHORT_KEY_CHANGED,
    EVENT_SCREEN_READER_STATE_CHANGED,
    EVENT_TOUCH_MODE_CHANGED,
    EVENT_CONFIG_EVENT_CHANGED,
    EVENT_TYPE_MAX,
};

class AccessibilityStateObserver {
public:
    /**
     * @brief Destruct
     * @since 3
     * @sysCap Accessibility
     */
    virtual ~AccessibilityStateObserver() = default;

    /**
     * @brief Receives notifications on accessibility status changes.
     * @param state Indicates the status change.
     * @since 3
     * @sysCap Accessibility
     */
    virtual void OnStateChanged(const bool state) = 0;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_STATE_EVENT_H