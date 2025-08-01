/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "mock_accessible_ability_connection.h"
#include <algorithm>
#include <map>
#include <vector>
#include "accessibility_account_data.h"
#include "accessibility_ut_helper.h"
#include "accessible_ability_manager_service.h"
#include "accessible_ability_channel.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
MockAccessibleAbilityConnection::MockAccessibleAbilityConnection(int32_t accountId, int32_t connectionId,
    AccessibilityAbilityInfo& abilityInfo)
    : AccessibleAbilityConnection(accountId, connectionId, abilityInfo)
{
    (void)accountId;
    (void)connectionId;
    (void)abilityInfo;
}
MockAccessibleAbilityConnection::~MockAccessibleAbilityConnection()
{}

AccessibleAbilityChannel::AccessibleAbilityChannel(const int32_t accountId, const std::string &clientName)
    : clientName_(clientName), accountId_(accountId)
{
}

RetError AccessibleAbilityChannel::SearchElementInfoByAccessibilityId(const ElementBasicInfo elementBasicInfo,
    const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback,
    const int32_t mode, bool isFilter, bool systemApi)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel SearchElementInfoByAccessibilityId";
    (void)elementBasicInfo;
    (void)requestId;
    (void)callback;
    (void)mode;
    (void)isFilter;
    (void)systemApi;
    return RET_OK;
}

RetError AccessibleAbilityChannel::SearchDefaultFocusedByWindowId(const ElementBasicInfo elementBasicInfo,
    const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback,
    const int32_t mode, bool isFilter)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel SearchDefaultFocusedByWindowId";
    (void)elementBasicInfo;
    (void)requestId;
    (void)callback;
    (void)mode;
    (void)isFilter;
    return RET_OK;
}

RetError AccessibleAbilityChannel::SearchElementInfosByText(const int32_t accessibilityWindowId,
    const int64_t elementId, const std::string& text, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback>& callback, bool systemApi)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel SearchElementInfosByText";
    (void)accessibilityWindowId;
    (void)elementId;
    (void)text;
    (void)requestId;
    (void)callback;
    (void)systemApi;
    return RET_OK;
}

RetError AccessibleAbilityChannel::FindFocusedElementInfo(const int32_t accessibilityWindowId,
    const int64_t elementId, const int32_t focusType, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback>& callback, bool systemApi)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel FindFocusedElementInfo";
    (void)accessibilityWindowId;
    (void)elementId;
    (void)focusType;
    (void)requestId;
    (void)callback;
    (void)systemApi;
    return RET_OK;
}

RetError AccessibleAbilityChannel::FocusMoveSearch(const int32_t accessibilityWindowId, const int64_t elementId,
    const int32_t direction, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback,
    bool systemApi)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel FocusMoveSearch";
    (void)accessibilityWindowId;
    (void)elementId;
    (void)direction;
    (void)requestId;
    (void)callback;
    (void)systemApi;
    return RET_OK;
}

RetError AccessibleAbilityChannel::ExecuteAction(const int32_t accessibilityWindowId, const int64_t elementId,
    const int32_t action, const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback>& callback)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel ExecuteAction";
    (void)accessibilityWindowId;
    (void)elementId;
    (void)action;
    (void)actionArguments;
    (void)requestId;
    (void)callback;
    return RET_OK;
}

RetError AccessibleAbilityChannel::GetWindows(uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows,
    bool systemApi) const
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel GetWindows";
    (void)displayId;
    (void)windows;
    (void)systemApi;
    sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId_, clientName_);
    if (!clientConnection) {
        HILOG_ERROR("There is no client connection");
        return RET_ERR_NO_CONNECTION;
    }

    if (!(clientConnection->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_RETRIEVE)) {
        HILOG_ERROR("AccessibleAbilityChannel::GetWindows failed: no capability");
        return RET_ERR_NO_CAPABILITY;
    }
    return RET_OK;
}

RetError AccessibleAbilityChannel::GetWindows(std::vector<AccessibilityWindowInfo> &windows, bool systemApi)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel GetWindows";
    uint64_t displayId = 0;
    return GetWindows(displayId, windows, systemApi);
}

RetError AccessibleAbilityChannel::GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel GetWindow";
    (void)windowId;
    (void)windowInfo;
    return RET_OK;
}

RetError AccessibleAbilityChannel::SetTargetBundleName(const std::vector<std::string> &targetBundleNames)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel SetTargetBundleName";
    (void)targetBundleNames;
    return RET_OK;
}

RetError AccessibleAbilityChannel::GetWindowsByDisplayId(const uint64_t displayId,
    std::vector<AccessibilityWindowInfo> &windows, bool systemApi)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel GetWindowsByDisplayId";
    return GetWindows(displayId, windows, systemApi);
}

void AccessibleAbilityChannel::SetOnKeyPressEventResult(const bool handled, const int32_t sequence)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel SetOnKeyPressEventResult";
    (void)handled;
    (void)sequence;
}

RetError AccessibleAbilityChannel::SendSimulateGesture(
    const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilityChannel SendSimulateGesture";
    (void)gesturePath;
    return RET_OK;
}

sptr<AccessibleAbilityConnection> AccessibleAbilityChannel::GetConnection(int32_t accountId,
    const std::string &clientName)
{
    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
    if (!accountData) {
        HILOG_ERROR("accountData is nullptr");
        return nullptr;
    }

    return accountData->GetAccessibleAbilityConnection(clientName);
}

RetError AccessibleAbilityChannel::ConfigureEvents(const std::vector<uint32_t> needEvents)
{
    return RET_OK;
}

AccessibleAbilityConnection::AccessibleAbilityConnection(
    int32_t accountId, int32_t connectionId, AccessibilityAbilityInfo& abilityInfo)
{
    accountId_ = accountId;
    connectionId_ = connectionId;
    abilityInfo_ = abilityInfo;
}

AccessibleAbilityConnection::~AccessibleAbilityConnection()
{
    HILOG_DEBUG("start");
}

void AccessibleAbilityConnection::OnAbilityConnectDone(
    const AppExecFwk::ElementName& element, const sptr<IRemoteObject>& remoteObject, int32_t resultCode)
{
    (void)element;
    (void)remoteObject;
    (void)resultCode;
    auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId_);
    sptr<AccessibleAbilityConnection> pointer = this;
    if (accountData) {
        accountData->AddConnectedAbility(pointer);
    }
}

void AccessibleAbilityConnection::OnAbilityDisconnectDone(const AppExecFwk::ElementName& element, int32_t resultCode)
{
    (void)element;
    (void)resultCode;
    return;
}

bool AccessibleAbilityConnection::OnKeyPressEvent(const MMI::KeyEvent& keyEvent, const int32_t sequence)
{
    (void)sequence;
    if (keyEvent.GetKeyAction() == MMI::KeyEvent::KEY_ACTION_UP) {
        return false;
    }
    return true;
}

bool AccessibleAbilityConnection::IsWantedEvent(int32_t eventType)
{
    (void)eventType;
    return true;
}

AAFwk::Want CreateWant(AppExecFwk::ElementName& element)
{
    (void)element;
    AAFwk::Want want;
    return want;
}

void AccessibleAbilityConnection::Disconnect()
{
    HILOG_DEBUG("start");
    AccessibilityAbilityHelper::GetInstance().SetTestChannelId(-1);
}

bool AccessibleAbilityConnection::Connect(const AppExecFwk::ElementName& element)
{
    HILOG_DEBUG("start");
    elementName_ = element;
    return true;
}

int32_t AccessibleAbilityConnection::GetChannelId()
{
    HILOG_DEBUG("connectionId_ is %{public}d", connectionId_);
    return connectionId_;
}

void AccessibleAbilityConnection::AccessibleAbilityConnectionDeathRecipient::OnRemoteDied(
    const wptr<IRemoteObject>& remote)
{
    (void)remote;
    // Temp deal: notify setting
}

void AccessibleAbilityConnection::OnAccessibilityEvent(AccessibilityEventInfo &eventInfo)
{
    (void)eventInfo;
}

void AccessibleAbilityConnection::OnAbilityConnectDoneSync(const AppExecFwk::ElementName &element,
    const sptr<IRemoteObject> &remoteObject)
{
    HILOG_DEBUG();
    auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId_);
    if (!accountData) {
        HILOG_ERROR("accountData is nullptr.");
        return;
    }
    if (!remoteObject) {
        HILOG_ERROR("AccessibleAbilityConnection::OnAbilityConnectDone get remoteObject failed");
        return;
    }
    elementName_ = element;

    sptr<AccessibleAbilityConnection> pointer = this;
    accountData->AddConnectedAbility(pointer);
}

void AccessibleAbilityConnection::OnAbilityDisconnectDoneSync(const AppExecFwk::ElementName &element)
{
    HILOG_DEBUG("start");
    (void)element;
}

RetError AccessibleAbilityChannel::EnableScreenCurtain(bool isEnable)
{
    (void)isEnable;
    return RET_OK;
}

RetError AccessibleAbilityChannel::GetCursorPosition(const int32_t accessibilityWindowId, const int64_t elementId,
    const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilitychannel GetCursorPosition";
    (void)accessibilityWindowId;
    (void)elementId;
    (void)requestId;
    (void)callback;
    return RET_OK;
}

RetError AccessibleAbilityChannel::SetIsRegisterDisconnectCallback(bool isRegister)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilitychannel SetIsRegisterDisconnectCallback";
    (void)isRegister;
    return RET_OK;
}

RetError AccessibleAbilityChannel::NotifyDisconnect()
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilitychannel NotifyDisconnect";
    return RET_OK;
}

void AccessibleAbilityChannel::SearchElementInfoBySpecificProperty(const ElementBasicInfo elementBasicInfo,
    const SpecificPropertyParam& param, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    GTEST_LOG_(INFO) << "MOCK AccessibleAbilitychannel SearchElementInfoBySpecificProperty";
    (void)elementBasicInfo;
    (void)param;
    (void)requestId;
    (void)callback;
}

bool AccessibleAbilityConnection::GetIsRegisterDisconnectCallback() const
{
    return false;
}

void AccessibleAbilityConnection::SetIsRegisterDisconnectCallback(bool isRegister)
{
    (void)isRegister;
    return;
}

void AccessibleAbilityConnection::NotifyDisconnect()
{
    return;
}

void AccessibleAbilityConnection::DisconnectAbility()
{
    return;
}

RetError AccessibleAbilityChannel::HoldRunningLock()
{
    return RET_OK;
}

RetError AccessibleAbilityChannel::UnholdRunningLock()
{
    return RET_OK;
}
} // namespace Accessibility
} // namespace OHOS