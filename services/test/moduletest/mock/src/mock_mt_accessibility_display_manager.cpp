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

#include "accessibility_display_manager.h"
#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"
#include "accessibility_ut_helper.h"

namespace OHOS {
namespace Accessibility {

constexpr int32_t DEFAULT_DPI = 540;
constexpr int32_t DEFAULT_WIDTH = 1260;
constexpr int32_t DEFAULT_HEIGHT = 2720;
constexpr float DEFAULT_SCALE = 2.0f;
constexpr float NORMAL_SCALE = 1.0f;

MockAccessibilityDisplayManager::AccessibilityDisplayManager()
{
}

MockAccessibilityDisplayManager::~AccessibilityDisplayManager()
{
    UnregisterDisplayListener();
}

const sptr<Rosen::Display> MockAccessibilityDisplayManager::GetDisplay(uint64_t id)
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetDisplayById(id);
}

std::vector<sptr<Rosen::Display>> MockAccessibilityDisplayManager::GetDisplays()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetAllDisplays();
}

const sptr<Rosen::Display> MockAccessibilityDisplayManager::GetDefaultDisplay()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetDefaultDisplay();
}

uint64_t MockAccessibilityDisplayManager::GetDefaultDisplayId()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetDefaultDisplayId();
}

int32_t MockAccessibilityDisplayManager::GetWidth()
{
    HILOG_DEBUG();
    sptr<Rosen::Display> displaySync = GetDefaultDisplaySync();
    if (displaySync == nullptr) {
        HILOG_ERROR("default displaySync is null");
        return DEFAULT_WIDTH;
    }

    return displaySync->GetWidth();
}

int32_t MockAccessibilityDisplayManager::GetHeight()
{
    HILOG_DEBUG();
    sptr<Rosen::Display> displaySync = GetDefaultDisplaySync();
    if (displaySync == nullptr) {
        HILOG_ERROR("default displaySync is null");
        return DEFAULT_HEIGHT;
    }

    return displaySync->GetHeight();
}

OHOS::Rosen::DisplayOrientation MockAccessibilityDisplayManager::GetOrientation()
{
    HILOG_DEBUG();
    sptr<Rosen::Display> displaySync = GetDefaultDisplaySync();
    if (displaySync == nullptr) {
        HILOG_ERROR("default displaySync is null");
        return OHOS::Rosen::DisplayOrientation::PORTRAIT;
    }

    auto displayInfo = displaySync->GetDisplayInfo();
    if (displayInfo == nullptr) {
        HILOG_ERROR("default displayInfo is null");
        return OHOS::Rosen::DisplayOrientation::PORTRAIT;
    }

    return displayInfo->GetDisplayOrientation();
}

sptr<Rosen::Display> MockAccessibilityDisplayManager::GetDefaultDisplaySync()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetDefaultDisplaySync();
}

int32_t MockAccessibilityDisplayManager::GetDefaultDisplayDpi()
{
    HILOG_DEBUG();
    if (GetDefaultDisplay() == nullptr) {
        HILOG_ERROR("default display is null");
        return DEFAULT_DPI;
    }

    return GetDefaultDisplay()->GetDpi();
}

bool MockAccessibilityDisplayManager::IsFoldable()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().IsFoldable();
}

Rosen::FoldDisplayMode MockAccessibilityDisplayManager::GetFoldDisplayMode()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetFoldDisplayMode();
}

Rosen::FoldStatus MockAccessibilityDisplayManager::GetFoldStatus()
{
    HILOG_DEBUG();
    return Rosen::DisplayManager::GetInstance().GetFoldStatus();
}

void MockAccessibilityDisplayManager::SetDisplayScale(const uint64_t screenId,
    float scaleX, float scaleY, float pivotX, float pivotY)
{
    HILOG_DEBUG("scaleX = %{public}f, scaleY = %{public}f, pivotX = %{public}f, pivotY = %{public}f",
        scaleX, scaleY, pivotX, pivotY);
    if ((scaleX >= DEFAULT_SCALE) || (scaleY >= DEFAULT_SCALE)) {
        Accessibility::AccessibilityAbilityHelper::GetInstance().SetZoomState(true);
    }

    if ((scaleX == NORMAL_SCALE) || (scaleY == NORMAL_SCALE)) {
        Accessibility::AccessibilityAbilityHelper::GetInstance().SetZoomState(false);
    }
}

void MockAccessibilityDisplayManager::RegisterDisplayListener(
    const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    HILOG_DEBUG();
    if (listener_) {
        HILOG_DEBUG("Display listener is already registed!");
        return;
    }
    handler_ = handler;
    listener_ = new(std::nothrow) DisplayListener();
    if (!listener_) {
        HILOG_ERROR("Create display listener fail!");
        return;
    }
    Rosen::DisplayManager::GetInstance().RegisterDisplayListener(listener_);
}

void MockAccessibilityDisplayManager::UnregisterDisplayListener()
{
    HILOG_DEBUG();
    if (listener_) {
        Rosen::DisplayManager::GetInstance().UnregisterDisplayListener(listener_);
        listener_ = nullptr;
        handler_ = nullptr;
    }
}
} // namespace Accessibility
} // namespace OHOS