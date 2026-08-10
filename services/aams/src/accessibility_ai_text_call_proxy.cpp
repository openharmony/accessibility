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

#include "accessibility_ai_text_call_proxy.h"

#include <dlfcn.h>
#include "hilog_wrapper.h"

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_TAG AAMS_LOG_TAG
#define LOG_DOMAIN AAMS_LOG_DOMAIN

namespace OHOS {
namespace Accessibility {
namespace {
    const std::string_view AI_TEXT_CALL_LIBRARY = "libaccessibleTextCall.z.so";
}

AccessibilityAITextCallProxy::AccessibilityAITextCallProxy()
{
}

AccessibilityAITextCallProxy::~AccessibilityAITextCallProxy()
{
}

void AccessibilityAITextCallProxy::SetServiceState(bool state)
{
    HILOG_INFO("AccessibilityAITextCallProxy::SetServiceState %{public}d", state);
    std::lock_guard<ffrt::mutex> lock(lock_);
    if (!loadFlag_) {
        LoadLibrary();
    }
    if (loadFlag_) {
        func_(state);
    }
    if (!state) {
        UnLoadLibrary();
    }
}

void AccessibilityAITextCallProxy::LoadLibrary()
{
    handle_ = dlopen(std::string(AI_TEXT_CALL_LIBRARY).c_str(), RTLD_LAZY);
    if (!handle_) {
        HILOG_ERROR("load libaccessibleTextCall.z.so failed %{public}s", dlerror());
        return;
    }
    func_ = (SetServiceStateFunc)dlsym(handle_, "SetServiceState");
    if (!func_) {
        char *error = dlerror();
        if (error) {
            HILOG_ERROR("get SetServiceState failed %{public}s", error);
        } else {
            HILOG_ERROR("get SetServiceState failed");
        }
        dlclose(handle_);
        handle_ = nullptr;
        return;
    }
    loadFlag_ = true;
}

void AccessibilityAITextCallProxy::UnLoadLibrary()
{
    if (handle_ != nullptr) {
        int ret = dlclose(handle_);
        if (ret != 0) {
            HILOG_ERROR("dlclose failed, ret = %{public}d", ret);
        }
        handle_ = nullptr;
    }
    func_ = nullptr;
    loadFlag_ = false;
}
} // namespace Accessibility
} // namespace OHOS
