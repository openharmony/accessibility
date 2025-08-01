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

#ifndef ACCESSIBLE_ABILITY_CHANNEL_H
#define ACCESSIBLE_ABILITY_CHANNEL_H

#include "accessible_ability_channel_stub.h"
#include "event_handler.h"
#include "ffrt_inner.h"
#include "iaccessibility_element_operator.h"
#include "key_event.h"

namespace OHOS {
namespace Accessibility {
class AccessibleAbilityConnection;
class AccessibleAbilityChannel : public AccessibleAbilityChannelStub {
public:
    AccessibleAbilityChannel(const int32_t accountId, const std::string &clientName);
    ~AccessibleAbilityChannel() = default;
    RetError SearchElementInfoByAccessibilityId(const ElementBasicInfo elementBasicInfo,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
        const int32_t mode, bool isFilter = false, bool systemApi = false) override;

    RetError SearchDefaultFocusedByWindowId(const ElementBasicInfo elementBasicInfo,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
        const int32_t mode, bool isFilter = false) override;

    RetError SearchElementInfosByText(const int32_t accessibilityWindowId, const int64_t elementId,
        const std::string &text, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback, bool systemApi = false) override;

    RetError FindFocusedElementInfo(const int32_t accessibilityWindowId, const int64_t elementId,
        const int32_t focusType, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback, bool systemApi = false) override;

    RetError FocusMoveSearch(const int32_t accessibilityWindowId, const int64_t elementId, const int32_t direction,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
        bool systemApi = false) override;

    RetError EnableScreenCurtain(bool isEnable) override;

    RetError HoldRunningLock() override;

    RetError UnholdRunningLock() override;

    RetError ExecuteAction(const int32_t accessibilityWindowId, const int64_t elementId, const int32_t action,
        const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    RetError GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo) override;

    RetError GetWindows(std::vector<AccessibilityWindowInfo> &windows, bool systemApi = false) override;

    RetError GetWindowsByDisplayId(const uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows,
        bool systemApi = false) override;

    void SetOnKeyPressEventResult(const bool handled, const int32_t sequence) override;

    RetError SendSimulateGesture(const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath) override;

    RetError SetTargetBundleName(const std::vector<std::string> &targetBundleNames) override;

    RetError GetCursorPosition(const int32_t accessibilityWindowId, const int64_t elementId, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    void SetFocusWindowIdAndElementId(const int32_t accessibilityWindowId,
    const int64_t elementId, const int32_t action);

    RetError SetIsRegisterDisconnectCallback(bool isRegister) override;

    RetError NotifyDisconnect() override;

    RetError ConfigureEvents(const std::vector<uint32_t> needEvents) override;

    void SearchElementInfoBySpecificProperty(const ElementBasicInfo elementBasicInfo,
        const SpecificPropertyParam& param, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

private:
    static sptr<AccessibleAbilityConnection> GetConnection(int32_t accountId, const std::string &clientName);
    static RetError GetElementOperator(int32_t accountId, int32_t windowId, int32_t focusType,
        const std::string &clientName, sptr<IAccessibilityElementOperator> &elementOperator, const int32_t treeId);
    static bool CheckWinFromAwm(const int32_t windowId);
    RetError GetWindows(
        uint64_t displayId, std::vector<AccessibilityWindowInfo>& windows, bool systemApi = false) const;
    RetError TransmitActionToMmi(const int32_t action);
    static void SetKeyCodeToMmi(std::shared_ptr<MMI::KeyEvent>& keyEvent, const bool isPress,
        const int32_t keyCode);
    RetError ExecuteActionAsync(const int32_t accessibilityWindowId, const int64_t elementId, const int32_t action,
        const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback);
    std::string clientName_ = "";
    int32_t accountId_ = -1;
    std::shared_ptr<AppExecFwk::EventHandler> eventHandler_ = nullptr;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBLE_ABILITY_CHANNEL_H