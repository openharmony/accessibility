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

#ifndef MOCK_ACCESSIBLE_ABILITY_CHANNEL_STUB_H
#define MOCK_ACCESSIBLE_ABILITY_CHANNEL_STUB_H

#include "accessible_ability_channel_stub.h"
#include "gmock/gmock.h"

namespace OHOS {
namespace Accessibility {
class MockAccessibleAbilityChannelStub : public AccessibleAbilityChannelStub {
public:
    MockAccessibleAbilityChannelStub();
    virtual ~MockAccessibleAbilityChannelStub();

    MOCK_METHOD4(OnRemoteRequest, int(uint32_t code, MessageParcel& data,
        MessageParcel& reply, MessageOption& option));
    MOCK_METHOD6(
        SearchElementInfoByAccessibilityId, RetError(const ElementBasicInfo elementBasicInfo, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback>& callback,
        const int32_t mode, bool isFilter, bool systemApi));
    MOCK_METHOD5(SearchDefaultFocusedByWindowId,
        RetError(const ElementBasicInfo elementBasicInfo, const int32_t requestId,
            const sptr<IAccessibilityElementOperatorCallback>& callback, const int32_t mode, bool isFilter));
    MOCK_METHOD6(SearchElementInfosByText,
        RetError(const int32_t accessibilityWindowId, const int64_t elementId, const std::string& text,
            const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback, bool systemApi));
    MOCK_METHOD6(FindFocusedElementInfo,
        RetError(const int32_t accessibilityWindowId, const int64_t elementId, const int32_t focusType,
            const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback, bool systemApi));
    MOCK_METHOD6(FocusMoveSearch, RetError(const int32_t accessibilityWindowId, const int64_t elementId,
        const int32_t direction, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback>& callback, bool systemApi));
    MOCK_METHOD6(ExecuteAction, RetError(const int32_t accessibilityWindowId, const int64_t elementId,
        const int32_t action, const std::map<std::string, std::string>& actionArguments, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD1(EnableScreenCurtain, RetError(bool isEnable));
    MOCK_METHOD0(HoldRunningLock, RetError());
    MOCK_METHOD0(UnholdRunningLock, RetError());
    MOCK_METHOD4(GetCursorPosition, RetError(const int32_t accessibilityWindowId, const int64_t elementId,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD2(GetWindow, RetError(const int32_t windowId, AccessibilityWindowInfo &windowInfo));
    MOCK_METHOD2(GetWindows, RetError(std::vector<AccessibilityWindowInfo> &windows, bool systemApi));
    MOCK_METHOD3(GetWindowsByDisplayId, RetError(const uint64_t displayId,
        std::vector<AccessibilityWindowInfo> &windows, bool systemApi));
    MOCK_METHOD2(SetOnKeyPressEventResult, void(const bool handled, const int32_t sequence));
    MOCK_METHOD1(SendSimulateGesture, RetError(const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath));
    MOCK_METHOD1(SetTargetBundleName, RetError(const std::vector<std::string> &targetBundleNames));
    MOCK_METHOD1(SetIsRegisterDisconnectCallback, RetError(bool isRegister));
    MOCK_METHOD0(NotifyDisconnect, RetError());
    MOCK_METHOD1(ConfigureEvents, RetError(const std::vector<uint32_t> needEvents));
    MOCK_METHOD4(SearchElementInfoBySpecificProperty, void(const ElementBasicInfo elementBasicInfo,
        const SpecificPropertyParam& param, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback>& callback));
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_ACCESSIBLE_ABILITY_CHANNEL_STUB_H