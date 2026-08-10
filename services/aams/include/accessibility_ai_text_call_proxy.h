/*
 * Copyright (C) 2026-2026 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_AI_TEXT_CALL_PROXY_H
#define ACCESSIBILITY_AI_TEXT_CALL_PROXY_H

#include "singleton.h"
#include "ffrt.h"

namespace OHOS {
namespace Accessibility {
class AccessibilityAITextCallProxy {
    DECLARE_SINGLETON(AccessibilityAITextCallProxy);
public:
    void SetServiceState(bool state);

private:
    void LoadLibrary();
    void UnLoadLibrary();

private:
    using SetServiceStateFunc = void(*)(bool);
    SetServiceStateFunc func_ = nullptr;
    void* handle_ = nullptr;
    bool loadFlag_ = false;
    ffrt::mutex lock_;
};
} // namespace Accessibility
} // namespace OHOS

#endif // ACCESSIBILITY_AI_TEXT_CALL_PROXY_H
