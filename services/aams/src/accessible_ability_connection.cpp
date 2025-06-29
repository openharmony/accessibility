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

#include "accessible_ability_connection.h"

#ifdef OHOS_BUILD_ENABLE_HITRACE
#include <hitrace_meter.h>
#endif // OHOS_BUILD_ENABLE_HITRACE

#include "ability_manager_client.h"
#include "accessible_ability_manager_service.h"
#ifdef OHOS_BUILD_ENABLE_POWER_MANAGER
#include "accessibility_power_manager.h"
#endif
#include "hilog_wrapper.h"
#include "utils.h"

namespace OHOS {
namespace Accessibility {
AccessibleAbilityConnection::AccessibleAbilityConnection(int32_t accountId, int32_t connectionId,
    AccessibilityAbilityInfo &abilityInfo)
    : accountId_(accountId), connectionId_(connectionId), abilityInfo_(abilityInfo)
{
    eventHandler_ = std::make_shared<AppExecFwk::EventHandler>(
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetMainRunner());
}

AccessibleAbilityConnection::~AccessibleAbilityConnection()
{
    HILOG_DEBUG();
    if (abilityClient_ && abilityClient_->AsObject()) {
        abilityClient_->AsObject()->RemoveDeathRecipient(deathRecipient_);
        abilityClient_ = nullptr;
    }
}

void AccessibleAbilityConnection::HandleNoEventHandler(const AppExecFwk::ElementName &element)
{
    HILOG_ERROR("eventHandler_ is nullptr.");
    auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId_);
    if (accountData != nullptr) {
        accountData->RemoveConnectingA11yAbility(Utils::GetUri(element));
    }
}

void AccessibleAbilityConnection::OnAbilityConnectDone(const AppExecFwk::ElementName &element,
    const sptr<IRemoteObject> &remoteObject, int32_t resultCode)
{
    HILOG_INFO("ResultCode is %{public}d", resultCode);
    if (!eventHandler_) {
        HandleNoEventHandler(element);
        return;
    }

    int32_t accountId = accountId_;
    eventHandler_->PostTask([accountId, element, remoteObject, resultCode]() {
#ifdef OHOS_BUILD_ENABLE_HITRACE
        FinishAsyncTrace(HITRACE_TAG_ACCESSIBILITY_MANAGER, "AccessibleAbilityConnect",
            static_cast<int32_t>(TraceTaskId::ACCESSIBLE_ABILITY_CONNECT));
#endif // OHOS_BUILD_ENABLE_HITRACE
        std::string bundleName = element.GetBundleName();
        std::string abilityName = element.GetAbilityName();
        auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
        if (!accountData) {
            Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
                A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
            HILOG_ERROR("accountData is nullptr.");
            return;
        }

        if (resultCode != NO_ERROR) {
            accountData->RemoveEnabledAbility(Utils::GetUri(element));
            accountData->RemoveConnectingA11yAbility(Utils::GetUri(element));
            accountData->UpdateAbilities();
            Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
                A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
            return;
        }

        if (!remoteObject) {
            Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
                A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
            HILOG_ERROR("AccessibleAbilityConnection::OnAbilityConnectDone get remoteObject failed");
            return;
        }

        sptr<AccessibleAbilityConnection> connection = accountData->GetConnectingA11yAbility(Utils::GetUri(element));
        if (!connection) {
            HILOG_ERROR("connection is nullptr");
            return;
        }

        accountData->AddConnectedAbility(connection);
        accountData->RemoveConnectingA11yAbility(Utils::GetUri(element));
        Singleton<AccessibleAbilityManagerService>::GetInstance().UpdateAccessibilityManagerService();
        connection->InitAbilityClient(remoteObject);
        accountData->UpdateEnableAbilityListsState();
        }, "OnAbilityConnectDone");
}

void AccessibleAbilityConnection::OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int32_t resultCode)
{
    HILOG_INFO("resultCode[%{public}d]", resultCode);
    if (resultCode == NO_ERROR) {
        return;
    }
    // when calling ConnectAbility function, OnAbilityDisconnectDone could be called when ability crashed
    // and OnAbilityConnectDone won't be called. At this time, ConnectingA11yAbility should be cleaned up
    if (!eventHandler_) {
        HILOG_ERROR("eventHanlder_ is nullptr");
        auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId_);
        if (accountData != nullptr) {
            accountData->RemoveConnectingA11yAbility(Utils::GetUri(element));
        }
        return;
    }
    
    int32_t accountId = accountId_;
    eventHandler_->PostTask([accountId, element]() {
        auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
        if (accountData != nullptr) {
            accountData->RemoveConnectingA11yAbility(Utils::GetUri(element));
        }
        }, "OnAbilityDisconnectDone");
}

void AccessibleAbilityConnection::OnAccessibilityEvent(AccessibilityEventInfo &eventInfo)
{
    HILOG_DEBUG();
    if (!abilityClient_) {
        HILOG_ERROR("OnAccessibilityEvent​ failed");
        return;
    }

    std::vector<std::string> filterBundleNames = abilityInfo_.GetFilterBundleNames();
    if (IsWantedEvent(eventInfo.GetEventType()) && (filterBundleNames.empty() || find(filterBundleNames.begin(),
        filterBundleNames.end(), eventInfo.GetBundleName()) != filterBundleNames.end())) {
        abilityClient_->OnAccessibilityEvent(eventInfo);
        HILOG_DEBUG("windowId[%{public}d] evtType[%{public}d] windowChangeType[%{public}d] GestureId[%{public}d]",
            eventInfo.GetWindowId(), eventInfo.GetEventType(), eventInfo.GetWindowChangeTypes(),
            eventInfo.GetGestureType());
    }
}

bool AccessibleAbilityConnection::OnKeyPressEvent(const MMI::KeyEvent &keyEvent, const int32_t sequence)
{
    if (!abilityClient_) {
        HILOG_ERROR("OnKeyPressEvent failed");
        return false;
    }

    if (!(abilityInfo_.GetCapabilityValues() & Capability::CAPABILITY_KEY_EVENT_OBSERVER)) {
        HILOG_ERROR("Capability failed");
        return false;
    }

    abilityClient_->OnKeyPressEvent(keyEvent, sequence);
    return true;
}

void AccessibleAbilityConnection::SetAbilityInfoTargetBundleName(const std::vector<std::string> &targetBundleNames)
{
    HILOG_DEBUG();
    abilityInfo_.SetFilterBundleNames(targetBundleNames);
}

bool AccessibleAbilityConnection::IsWantedEvent(int32_t eventType)
{
    uint32_t type = static_cast<uint32_t>(eventType);
    if ((type & abilityInfo_.GetEventTypes()) != type) {
        HILOG_DEBUG("EventType(%{public}d) is not wanted", type);
        return false;
    }
    return true;
}

AAFwk::Want CreateWant(AppExecFwk::ElementName& element)
{
    AAFwk::Want want;
    want.SetElement(element);

    return want;
}

void AccessibleAbilityConnection::Disconnect()
{
    HILOG_DEBUG();

    if (accountId_ == Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountId()) {
        Singleton<AccessibleAbilityManagerService>::GetInstance().UpdateAccessibilityManagerService();
    }

    if (!abilityClient_) {
        HILOG_ERROR("abilityClient is nullptr");
        return;
    }
    abilityClient_->Disconnect(connectionId_);

    if (isRegisterDisconnectCallback_) {
        int32_t accountId = accountId_;
        std::string clientName = Utils::GetUri(elementName_);
        if (!eventHandler_) {
            HILOG_ERROR("eventHanlder_ is nullptr");
            auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId_);
            if (accountData != nullptr) {
                accountData->RemoveConnectingA11yAbility(clientName);
            }
            return;
        }
        eventHandler_->PostTask([this, accountId, clientName]() {
            DisconnectAbility();
            auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
            accountData->RemoveWaitDisconnectAbility(clientName);
            }, "DISCONNECT_" + elementName_.GetBundleName(), WAIT_NOTIFY_DISCONNECT_TIMEOUT);
    } else {
        DisconnectAbility();
    }
}

void AccessibleAbilityConnection::DisconnectAbility()
{
    HILOG_INFO();
    if (deathRecipient_ == nullptr || !abilityClient_->AsObject() ||
        !abilityClient_->AsObject()->RemoveDeathRecipient(deathRecipient_)) {
        HILOG_ERROR("Failed to add death recipient");
    }

    auto abilityManagerClient = AAFwk::AbilityManagerClient::GetInstance();
    if (!abilityManagerClient) {
        HILOG_ERROR("abilityManagerClient is nullptr");
        return;
    }
    if (abilityManagerClient->DisconnectAbility(this) != ERR_OK) {
        HILOG_ERROR("Disconnect failed!");
        return;
    }
}

void AccessibleAbilityConnection::NotifyDisconnect()
{
    HILOG_INFO();
    if (!eventHandler_) {
        HILOG_ERROR("eventHanlder_ is nullptr");
        auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId_);
        if (accountData != nullptr) {
            accountData->RemoveConnectingA11yAbility(Utils::GetUri(elementName_));
        }
        return;
    }
    eventHandler_->RemoveTask("DISCONNECT_" + elementName_.GetBundleName());
    DisconnectAbility();
}

bool AccessibleAbilityConnection::Connect(const AppExecFwk::ElementName &element)
{
    HILOG_DEBUG();
#ifdef OHOS_BUILD_ENABLE_HITRACE
    StartAsyncTrace(HITRACE_TAG_ACCESSIBILITY_MANAGER, "AccessibleAbilityConnect",
        static_cast<int32_t>(TraceTaskId::ACCESSIBLE_ABILITY_CONNECT));
#endif // OHOS_BUILD_ENABLE_HITRACE
    std::string bundleName = element.GetBundleName();
    std::string abilityName = element.GetAbilityName();

    elementName_ = element;
    HILOG_DEBUG("bundleName[%{public}s], abilityName [%{public}s], accountId [%{public}d]",
        bundleName.c_str(), abilityName.c_str(), accountId_);

    int uid = Singleton<AccessibilityResourceBundleManager>::GetInstance().GetUidByBundleName(
        bundleName, abilityName, accountId_);
    HILOG_DEBUG("uid is %{public}d ", uid);

    auto abilityManagerClient = AAFwk::AbilityManagerClient::GetInstance();
    if (!abilityManagerClient) {
        Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
            A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
        HILOG_ERROR("abilityManagerClient is nullptr");
        return false;
    }
    AAFwk::Want want = CreateWant(elementName_);
    if (abilityManagerClient->ConnectAbility(want, this, nullptr, uid / UID_MASK) != ERR_OK) {
        Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
            A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
        HILOG_ERROR("ConnectAbility failed!");
        return false;
    }
    return true;
}

int32_t AccessibleAbilityConnection::GetChannelId()
{
    HILOG_DEBUG("connectionId_ is %{public}d", connectionId_);
    return connectionId_;
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
    Singleton<AccessibleAbilityManagerService>::GetInstance().UpdateAccessibilityManagerService();
    InitAbilityClient(remoteObject);
}

void AccessibleAbilityConnection::OnAbilityDisconnectDoneSync(const AppExecFwk::ElementName &element)
{
    HILOG_DEBUG();

    if (accountId_ == Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountId()) {
        Singleton<AccessibleAbilityManagerService>::GetInstance().UpdateAccessibilityManagerService();
    }

    if (!abilityClient_) {
        HILOG_ERROR("abilityClient_ is nullptr");
        return;
    }
    abilityClient_->Disconnect(connectionId_);
}

void AccessibleAbilityConnection::InitAbilityClient(const sptr<IRemoteObject> &remoteObject)
{
    std::string bundleName = elementName_.GetBundleName();
    std::string abilityName = elementName_.GetAbilityName();
    abilityClient_ = new(std::nothrow) AccessibleAbilityClientProxy(remoteObject);
    if (!abilityClient_) {
        Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
            A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
        HILOG_ERROR("abilityClient_ is null");
        return;
    }

    if (!deathRecipient_) {
        deathRecipient_ = new(std::nothrow) AccessibleAbilityConnectionDeathRecipient(
            accountId_, elementName_, eventHandler_);
        if (!deathRecipient_) {
            Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
                A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
            HILOG_ERROR("deathRecipient_ is null");
            return;
        }
    }

    if (!abilityClient_->AsObject() || !abilityClient_->AsObject()->AddDeathRecipient(deathRecipient_)) {
        HILOG_ERROR("Failed to add death recipient");
    }

    channel_ = new(std::nothrow) AccessibleAbilityChannel(accountId_, abilityInfo_.GetId());
    if (!channel_) {
        Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
            A11yError::ERROR_CONNECT_A11Y_APPLICATION_FAILED, bundleName, abilityName);
        HILOG_ERROR("channel_ is null");
        return;
    }
    abilityClient_->Init(channel_, connectionId_);
}

void AccessibleAbilityConnection::AccessibleAbilityConnectionDeathRecipient::OnRemoteDied(
    const wptr<IRemoteObject> &remote)
{
    Utils::RecordUnavailableEvent(A11yUnavailableEvent::CONNECT_EVENT,
        A11yError::ERROR_A11Y_APPLICATION_DISCONNECT_ABNORMALLY,
        recipientElementName_.GetBundleName(), recipientElementName_.GetAbilityName());
    if (!handler_) {
        HILOG_ERROR("handler_ is nullptr");
        return;
    }
    std::shared_ptr<AppExecFwk::ElementName> sharedElementName =
        std::make_shared<AppExecFwk::ElementName>(recipientElementName_);
    
    if (sharedElementName == nullptr) {
        HILOG_ERROR("sharedElementName is nullptr");
        return;
    }
    int32_t accountId = accountId_;
    handler_->PostTask([accountId, sharedElementName]() {
        HILOG_DEBUG();

        auto &aams = Singleton<AccessibleAbilityManagerService>::GetInstance();
        auto accountData = aams.GetAccountData(accountId);
        if (!accountData) {
            HILOG_ERROR("accountData is null.");
            return;
        }

        std::string uri = Utils::GetUri(*sharedElementName);
        sptr<AccessibleAbilityConnection> connection = accountData->GetAccessibleAbilityConnection(uri);
        if (!connection) {
            HILOG_ERROR("There is no connection for %{public}s.", uri.c_str());
            return;
        }
        accountData->RemoveConnectedAbility(*sharedElementName);
        accountData->RemoveEnabledAbility(uri);

        // the extension ability connected to aams by inner api
        if (sharedElementName->GetBundleName() == sharedElementName->GetAbilityName()) {
            accountData->RemoveInstalledAbility(sharedElementName->GetBundleName());
        }

#ifdef OHOS_BUILD_ENABLE_POWER_MANAGER
    std::string bundleName = sharedElementName->GetBundleName();
    auto &powerManager = Singleton<AccessibilityPowerManager>::GetInstance();
    if (!powerManager.UnholdRunningLock(bundleName)) {
        HILOG_DEBUG("OnRemoteDied UnholdRunningLock failed.");
    }
#endif
        accountData->UpdateAbilities();
        aams.UpdateAccessibilityManagerService();
        }, "OnRemoteDied");
}

bool AccessibleAbilityConnection::GetIsRegisterDisconnectCallback() const
{
    return isRegisterDisconnectCallback_;
}

void AccessibleAbilityConnection::SetIsRegisterDisconnectCallback(bool isRegister)
{
    isRegisterDisconnectCallback_ = isRegister;
}
} // namespace Accessibility
} // namespace OHOS