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

#include "accessibility_common_event.h"
#include <unistd.h>
#include "accessible_ability_manager_service.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "hilog_wrapper.h"
#include "accessibility_notification_helper.h"

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr int32_t RETRY_SUBSCRIBER = 3;
    const std::string KEY_USER_ID = "userId";
} // namespace

AccessibilityCommonEvent::AccessibilityCommonEvent()
{
    HILOG_DEBUG();
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_USER_ADDED] =
        &AccessibilityCommonEvent::HandleUserAdded;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_USER_REMOVED] =
        &AccessibilityCommonEvent::HandleUserRemoved;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_USER_SWITCHED] =
        &AccessibilityCommonEvent::HandleUserSwitched;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_ADDED] =
        &AccessibilityCommonEvent::HandlePackageAdd;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_REMOVED] =
        &AccessibilityCommonEvent::HandlePackageRemoved;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_CHANGED] =
        &AccessibilityCommonEvent::HandlePackageChanged;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_DATA_SHARE_READY] =
        &AccessibilityCommonEvent::HandleDataShareReady;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_BOOT_COMPLETED] =
        &AccessibilityCommonEvent::HandlePowerOnEvent;
    handleEventFunc_[EventFwk::CommonEventSupport::COMMON_EVENT_LOCALE_CHANGED] =
        &AccessibilityCommonEvent::HandleLocalChangedEvent;

    for (auto it = handleEventFunc_.begin(); it != handleEventFunc_.end(); ++it) {
        HILOG_DEBUG("Add event: %{public}s", it->first.c_str());
        eventHandles_.emplace(it->first, [this, it](const OHOS::EventFwk::CommonEventData &eventId) {
            auto requestFunc = it->second;
            if (requestFunc != nullptr) {
                (this->*requestFunc)(eventId);
            }
        });
    }
}

AccessibilityCommonEvent::~AccessibilityCommonEvent()
{
    eventHandles_.clear();
    UnSubscriberEvent();
}

void AccessibilityCommonEvent::SubscriberEvent(const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    HILOG_DEBUG();

    if (subscriber_) {
        HILOG_DEBUG("Common Event is already subscribered!");
        return;
    }

    EventFwk::MatchingSkills matchingSkills;
    for (auto &event : handleEventFunc_) {
        HILOG_DEBUG("Add event: %{public}s", event.first.c_str());
        matchingSkills.AddEvent(event.first);
    }

    EventFwk::CommonEventSubscribeInfo subscribeInfo(matchingSkills);
    subscriber_ = std::make_shared<AccessibilityCommonEventSubscriber>(subscribeInfo, *this);
    eventHandler_ = handler;

    int32_t retry = RETRY_SUBSCRIBER;
    do {
        bool subscribeResult = EventFwk::CommonEventManager::SubscribeCommonEvent(subscriber_);
        if (subscribeResult) {
            HILOG_INFO("SubscriberEvent success.");
            return;
        } else {
            HILOG_DEBUG("SubscriberEvent failed, retry %{public}d", retry);
            retry--;
            sleep(1);
        }
    } while (retry);

    HILOG_ERROR("SubscriberEvent failed.");
}

void AccessibilityCommonEvent::UnSubscriberEvent()
{
    HILOG_INFO();
    if (subscriber_) {
        bool unSubscribeResult = EventFwk::CommonEventManager::UnSubscribeCommonEvent(subscriber_);
        HILOG_INFO("unSubscribeResult = %{public}d", unSubscribeResult);
        subscriber_ = nullptr;
        eventHandler_ = nullptr;
    }
}

void AccessibilityCommonEvent::OnReceiveEvent(const EventFwk::CommonEventData &data)
{
    HILOG_DEBUG();
    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return;
    }
    eventHandler_->PostTask([this, data]() {
        HILOG_DEBUG();
        std::string action = data.GetWant().GetAction();
        HILOG_INFO("Handle event:[%{public}s] eventHandles size[%{public}zu]", action.c_str(), eventHandles_.size());
        auto it = eventHandles_.find(action);
        if (it == eventHandles_.end()) {
            HILOG_ERROR("Ignore event: %{public}s", action.c_str());
            return;
        }
        it->second(data);
        }, "TASK_ON_RECEIVE_EVENT");
}

void AccessibilityCommonEvent::HandleUserAdded(const EventFwk::CommonEventData &data) const
{
    int32_t accountId = data.GetCode();
    HILOG_INFO();
    if (accountId == -1) {
        HILOG_ERROR("account id is wrong");
        return;
    }
    Singleton<AccessibleAbilityManagerService>::GetInstance().AddedUser(accountId);
}

void AccessibilityCommonEvent::HandleUserRemoved(const EventFwk::CommonEventData &data) const
{
    int32_t accountId = data.GetCode();
    HILOG_INFO();
    if (accountId == -1) {
        HILOG_ERROR("account id is wrong");
        return;
    }
    Singleton<AccessibleAbilityManagerService>::GetInstance().RemovedUser(accountId);
}

void AccessibilityCommonEvent::HandleUserSwitched(const EventFwk::CommonEventData &data) const
{
    int32_t accountId = data.GetCode();
    HILOG_INFO();
    if (accountId == -1) {
        HILOG_ERROR("account id is wrong");
        return;
    }
    Singleton<AccessibleAbilityManagerService>::GetInstance().SwitchedUser(accountId);
}

void AccessibilityCommonEvent::HandlePackageRemoved(const EventFwk::CommonEventData &data) const
{
    std::string bundleName = data.GetWant().GetBundle();
    int userId = data.GetWant().GetIntParam(KEY_USER_ID, 0);
    int32_t accountId = Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountId();
    HILOG_INFO("bundleName is %{public}s", bundleName.c_str());
    if (userId != accountId) {
        HILOG_ERROR("not same user.");
        return;
    }
    Singleton<AccessibleAbilityManagerService>::GetInstance().PackageRemoved(bundleName);
}

void AccessibilityCommonEvent::HandlePackageAdd(const EventFwk::CommonEventData &data) const
{
    std::string bundleName = data.GetWant().GetBundle();
    int userId = data.GetWant().GetIntParam(KEY_USER_ID, 0);
    int32_t accountId = Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountId();
    HILOG_INFO("bundleName is %{public}s", bundleName.c_str());
    if (userId != accountId) {
        HILOG_ERROR("not same user.");
        return;
    }
    Singleton<AccessibleAbilityManagerService>::GetInstance().PackageAdd(bundleName);
}

void AccessibilityCommonEvent::HandlePackageChanged(const EventFwk::CommonEventData &data) const
{
    std::string bundleName = data.GetWant().GetBundle();
    int userId = data.GetWant().GetIntParam(KEY_USER_ID, 0);
    int32_t accountId = Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountId();
    HILOG_INFO("bundleName is %{public}s", bundleName.c_str());
    if (userId != accountId) {
        HILOG_ERROR("not same user.");
        return;
    }
    Singleton<AccessibleAbilityManagerService>::GetInstance().PackageChanged(bundleName);
}

void AccessibilityCommonEvent::HandleDataShareReady(const EventFwk::CommonEventData &data) const
{
    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountData();
    if (accountData == nullptr) {
        HILOG_ERROR("get accountData failed");
        return;
    }

    auto config = accountData->GetConfig();
    if (config == nullptr) {
        HILOG_WARN("config is nullptr");
        return;
    }

    if (config->GetInitializeState()) {
        HILOG_INFO("datashare no need reInit.");
        return;
    }

    HILOG_INFO("reInit datashare.");
    config->Init();
}

void AccessibilityCommonEvent::HandlePowerOnEvent(const EventFwk::CommonEventData &data) const
{
    IgnoreRepeatClickNotification::PublishIgnoreRepeatClickReminder();
    HILOG_ERROR("HandlePowerOnEvent");
}

void AccessibilityCommonEvent::HandleLocalChangedEvent(const EventFwk::CommonEventData &data) const
{
    HILOG_DEBUG("reInit Resource.");
    Singleton<AccessibleAbilityManagerService>::GetInstance().InitResource(true);
}
} // namespace Accessibility
} // namespace OHOS