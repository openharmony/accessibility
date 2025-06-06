/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include "accessibility_window_manager.h"
#include "accessibility_ut_helper.h"
#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr int32_t TOP_X  = 0;
    constexpr int32_t TOP_Y = 100;
    constexpr int32_t BOTTOM_X = 800;
    constexpr int32_t BOTTOM_Y = 400;
} // namespace

AccessibilityWindowManager::AccessibilityWindowManager()
{
}

AccessibilityWindowManager::~AccessibilityWindowManager()
{
}

bool AccessibilityWindowManager::Init()
{
    return true;
}

void AccessibilityWindowManager::DeInit()
{
    a11yWindows_.clear();
    activeWindowId_ = INVALID_WINDOW_ID;
    a11yFocusedWindowId_ = INVALID_WINDOW_ID;
}

void AccessibilityWindowManager::RegisterWindowListener(const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    (void)handler;
}

void AccessibilityWindowManager::DeregisterWindowListener()
{
}

void AccessibilityWindowManager::OnWindowUpdate(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos,
    Rosen::WindowUpdateType type)
{
    HILOG_DEBUG("start");
    (void)infos;
    (void)type;
}

int32_t AccessibilityWindowManager::ConvertToRealWindowId(int32_t windowId, int32_t focusType)
{
    (void)windowId;
    (void)focusType;
    int32_t realId = AccessibilityAbilityHelper::GetInstance().GetRealId();
    return realId;
}

AccessibilityWindowType ConvertWindowType(Rosen::WindowType type)
{
    (void)type;
    AccessibilityWindowType winType = TYPE_WINDOW_INVALID;

    if (type < Rosen::WindowType::SYSTEM_WINDOW_BASE) {
        winType = TYPE_APPLICATION;
    } else if ((type >= Rosen::WindowType::SYSTEM_WINDOW_BASE) && (type <= Rosen::WindowType::SYSTEM_WINDOW_END)) {
        winType = TYPE_SYSTEM;
    } else {
        HILOG_ERROR("ConvertWindowType Unknown windowType[%{public}d]", type);
    }
    return winType;
}

AccessibilityWindowInfo AccessibilityWindowManager::CreateAccessibilityWindowInfo(
    const sptr<Rosen::AccessibilityWindowInfo> windowInfo)
{
    (void)windowInfo;
    AccessibilityWindowInfo info;
    info.SetWindowId(0);
    info.SetAccessibilityWindowType(ConvertWindowType(Rosen::WindowType::SYSTEM_WINDOW_BASE));
    info.SetFocused(true);
    Rect bound(TOP_X, TOP_Y, BOTTOM_X, BOTTOM_Y);
    info.SetRectInScreen(bound);
    return info;
}

void AccessibilityWindowManager::SetActiveWindow(int32_t windowId, bool isSendEvent)
{
    activeWindowId_ = windowId;
    (void)isSendEvent;
}

void AccessibilityWindowManager::SetAccessibilityFocusedWindow(int32_t windowId)
{
    (void)windowId;
}

std::vector<AccessibilityWindowInfo> AccessibilityWindowManager::GetAccessibilityWindows()
{
    std::vector<AccessibilityWindowInfo> windows;
    return windows;
}

bool AccessibilityWindowManager::GetAccessibilityWindow(int32_t windowId, AccessibilityWindowInfo& window)
{
    HILOG_DEBUG("start windowId(%{public}d)", windowId);
    if (a11yWindows_.count(windowId)) {
        window = a11yWindows_[windowId];
        return true;
    }
    return false;
}

void AccessibilityWindowManager::SetEventInfoBundleName(const AccessibilityEventInfo& uiEvent)
{
    std::string windowsBundleNameCache = "";
    const_cast<AccessibilityEventInfo&>(uiEvent).SetBundleName(windowsBundleNameCache);
}

bool AccessibilityWindowManager::IsValidWindow(int32_t windowId)
{
    (void)windowId;
    return true;
}

void AccessibilityWindowManager::SetWindowSize(int32_t windowId, Rect rect)
{
    HILOG_DEBUG("SetWindowSize windowId(%{public}d)", windowId);

    auto it = std::find_if(a11yWindows_.begin(), a11yWindows_.end(),
        [windowId](const std::map<int32_t, AccessibilityWindowInfo>::value_type &window) {
            return window.first == windowId;
        });
    if (it != a11yWindows_.end()) {
        it->second.SetRectInScreen(rect);
    }
}

void AccessibilityWindowManager::ClearAccessibilityFocused()
{
}

int64_t AccessibilityWindowManager::GetSceneBoardElementId(const int32_t windowId, const int64_t elementId)
{
    return elementId;
}

void AccessibilityWindowManager::GetRealWindowAndElementId(int32_t& windowId, int64_t& elementId)
{
}

void AccessibilityWindowManager::GetSceneBoardInnerWinId(int32_t windowId, int64_t elementId,
    int32_t& innerWid)
{
}

void AccessibilityWindowManager::SceneBoardElementIdMap::InsertPair(const int32_t windowId, const int64_t elementId)
{
}

void AccessibilityWindowManager::SceneBoardElementIdMap::RemovePair(const int32_t windowId)
{
}

void AccessibilityWindowManager::SceneBoardElementIdMap::Clear()
{
}

std::map<int32_t, int64_t> AccessibilityWindowManager::SceneBoardElementIdMap::GetAllPairs()
{
    return std::map<int32_t, int64_t>();
}

RetError AccessibilityWindowManager::GetFocusedWindowId(int32_t &focusedWindowId)
{
    focusedWindowId = 1;
    return RET_OK;
}

bool AccessibilityWindowManager::IsInnerWindowRootElement(int64_t elementId)
{
    (void)elementId;
    return true;
}

int32_t AccessibilityWindowManager::GetActiveWindowId()
{
    return activeWindowId_;
}

void AccessibilityWindowManager::InsertTreeIdWindowIdPair(int32_t treeId, int32_t windowId)
{
    (void)treeId;
    (void)windowId;
}

void AccessibilityWindowManager::RemoveTreeIdWindowIdPair(int32_t treeId)
{
    (void)treeId;
}

int32_t AccessibilityWindowManager::FindTreeIdWindowIdPair(int32_t treeId)
{
    (void)treeId;
    return 0;
}
} // namespace Accessibility
} // namespace OHOS