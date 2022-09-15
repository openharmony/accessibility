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

#ifndef ACCESSIBILITY_SYSTEM_ABILITY_CLIENT_IMPL_H
#define ACCESSIBILITY_SYSTEM_ABILITY_CLIENT_IMPL_H

#include <array>
#include <mutex>
#include "accessibility_element_operator_impl.h"
#include "accessibility_system_ability_client.h"
#include "accessible_ability_manager_state_observer_stub.h"
#include "i_accessible_ability_manager_service.h"

namespace OHOS {
namespace Accessibility {
using StateArray = std::array<bool, AccessibilityStateEventType::EVENT_TYPE_MAX>;
using StateObserverVector = std::vector<std::shared_ptr<AccessibilityStateObserver>>;
using StateObserversArray = std::array<StateObserverVector, AccessibilityStateEventType::EVENT_TYPE_MAX>;
class AccessibilitySystemAbilityClientImpl : public AccessibilitySystemAbilityClient {
public:
    /**
     * @brief Construct.
     */
    AccessibilitySystemAbilityClientImpl();

    /**
     * @brief Deconstruct.
     */
    ~AccessibilitySystemAbilityClientImpl();

    /**
     * @brief Register the element operator, so the AA can get node info from ACE.
     * @param windowId Window ID
     * @param operation The callback object.
     * @return 0: Succeed ; otherwise is failed.
     */
    virtual int32_t RegisterElementOperator(const int32_t windowId,
        const std::shared_ptr<AccessibilityElementOperator> &operation) override;

    /**
     * @brief Deregister the element operator.
     * @param windowId Window ID
     */
    virtual void DeregisterElementOperator(const int32_t windowId) override;

    /**
     * @brief Checks whether accessibility ability is enabled.
     * @return true: enabled; false: disabled
     */
    virtual bool IsEnabled() override;

    /**
     * @brief Checks whether touch exploration ability is enabled.
     * @return true: enabled; false: disabled
     */
    virtual bool IsTouchExplorationEnabled() override;

    /**
     * @brief Queries the list of accessibility abilities.
     * @param accessibilityAbilityTypes Indicates the accessibility type specified by AccessibilityAbilityTypes.
     * @param stateType Indicates the accessibility ability status.
     *                  1 indicates that the ability is enabled;
     *                  2 indicates that the ability is disabled;
     *                  3 indicates that the ability has been installed.
     * @param infos accessibility ability infos by specified types.
     * @return Return true if get ability lists successfully, else return false.
     */
    virtual bool GetAbilityList(const uint32_t accessibilityAbilityTypes, const AbilityStateType stateType,
        std::vector<AccessibilityAbilityInfo> &infos) override;

    /**
     * @brief Sends an accessibility event.
     * @param eventType  Identifies the accessibility event specified by AccessibilityEventInfo.
     * @param componentId Indicates the ID of the component to be associated with the event.
     * @return true: send ok; otherwise is refused.
     */
    virtual bool SendEvent(const EventType eventType, const int32_t componentId) override;

    /**
     * @brief Sends information about an accessibility event.
     * @param event Indicates the accessibility event information specified by AccessibilityEventInfo.
     * @return true: send ok; otherwise is refused.
     */
    virtual bool SendEvent(const AccessibilityEventInfo &event) override;

    /**
     * @brief Subscribes to the specified type of accessibility status change events.
     * @param observer Indicates the observer for listening to status events, which is specified
     *              by AccessibilityStateObserver.
     * @param eventType Indicates the status type, which is specified by AccessibilityStateEventType.
     * @return true: send ok; otherwise is refused.
     */
    virtual bool SubscribeStateObserver(const std::shared_ptr<AccessibilityStateObserver> &observer,
        const uint32_t eventType) override;

    /**
     * @brief Unsubscribe the specified type of accessibility status change events.
     * @param observer Indicates the registered accessibility status event observer.
     * @param eventType Indicates the status type, which is specified by AccessibilityStateEventType.
     * @return true: send ok; otherwise is refused.
     */
    virtual bool UnsubscribeStateObserver(const std::shared_ptr<AccessibilityStateObserver> &observer,
        const uint32_t eventType) override;

    /**
     * @brief Get installed abilities.
     * @param installedAbilities The installed accessibility ability infos.
     * @return Return true if get installed abilities successfully, else return false.
     */
    virtual bool GetInstalledAbilities(std::vector<AccessibilityAbilityInfo> &installedAbilities) override;

    /**
     * @brief Get enabled abilities.
     * @param enabledAbilities The infos of enabled abilities.
     * @return Return true if get enabled abilities successfully, else return false.
     */
    virtual bool GetEnabledAbilities(std::vector<std::string> &enabledAbilities) override;

    /**
     * @brief Clean the AAMS object data.
     * @param remote The object access to AAMS.
     */
    void ResetService(const wptr<IRemoteObject> &remote);

    /**
     * @brief Receive the state notify from AAMS and send it to the observer registered.
     * @param stateType The state type and value.
     *                  state type: Refer to AccessibilityStateEventType.
     *                  value: STATE_ACCESSIBILITY_ENABLED/STATE_EXPLORATION_ENABLED/
     *                         STATE_ACCESSIBILITY_DISABLED/STATE_EXPLORATION_DISABLED
     */
    void OnAccessibleAbilityManagerStateChanged(const uint32_t stateType);
private:
    class AccessibleAbilityManagerStateObserverImpl : public AccessibleAbilityManagerStateObserverStub {
    public:
        explicit AccessibleAbilityManagerStateObserverImpl(AccessibilitySystemAbilityClientImpl &client)
            : client_(client) {}
        ~AccessibleAbilityManagerStateObserverImpl() = default;

        virtual void OnStateChanged(const uint32_t stateType) override
        {
            client_.OnAccessibleAbilityManagerStateChanged(stateType);
        }
    private:
        AccessibilitySystemAbilityClientImpl &client_;
    };

    class DeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        explicit DeathRecipient(AccessibilitySystemAbilityClientImpl &client) : client_(client) {}
        ~DeathRecipient() = default;
        DISALLOW_COPY_AND_MOVE(DeathRecipient);

        void OnRemoteDied(const wptr<IRemoteObject> &remote)
        {
            client_.ResetService(remote);
        }
    private:
        AccessibilitySystemAbilityClientImpl &client_;
    };

    /**
     * @brief Connect to AAMS Service.
     * @return success : true, failed : false.
     */
    bool ConnectToService();

    void Init();

    /**
     * @brief Notify the state is changed.
     * @param stateType The state type and value.
     *                  state type: Refer to AccessibilityStateEventType.
     * @param value The value be changed.
     */
    void NotifyStateChanged(uint32_t eventType, bool value);

    /**
     * @brief Check the event type is valid or not.
     * @param eventType The data of event type.
     * @return True: The data of event type is valid; otherwise is not.
     */
    bool CheckEventType(EventType eventType);

    std::mutex mutex_;
    StateArray stateArray_;
    StateObserversArray stateObserversArray_;

    std::map<int32_t, sptr<AccessibilityElementOperatorImpl>> interactionOperators_;

    std::vector<AccessibilityAbilityInfo> installedAbilities_;

    sptr<IRemoteObject::DeathRecipient> deathRecipient_ = nullptr;
    sptr<IAccessibleAbilityManagerService> serviceProxy_ = nullptr;
    sptr<AccessibleAbilityManagerStateObserverImpl> stateObserver_ = nullptr;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_SYSTEM_ABILITY_CLIENT_IMPL_H