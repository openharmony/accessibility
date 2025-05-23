/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_SHORT_KEY_H
#define ACCESSIBILITY_SHORT_KEY_H

#include "accessibility_event_transmission.h"
#include "event_handler.h"

namespace OHOS {
namespace Accessibility {
class AccessibilityShortKey : public RefBase {
public:
    AccessibilityShortKey() = default;
    ~AccessibilityShortKey();

    void Register();
    void Unregister();

private:
    void SubscribeShortKey(std::set<int32_t> preKeys, int32_t finalKey, int32_t holdTime);
    void OnShortKey();

    int32_t subscribeId_ = -1;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_SHORT_KEY_H