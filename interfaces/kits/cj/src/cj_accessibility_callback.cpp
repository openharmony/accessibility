/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "cj_accessibility_callback.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
void cjAccessibilityStateObserver::SubscribeObserver(std::function<void(bool)> cbFunc)
{
    observers_.push_back(cbFunc);
    HILOG_INFO("observer size%{public}zu", observers_.size());
}

void cjAccessibilityStateObserver::OnStateChanged(const bool state)
{
    std::lock_guard<std::mutex> lock(cbMutex_);
    for (auto &func : observers_) {
        func(state);
    }
}
} // namespace Accessibility
} // namespace OHOS