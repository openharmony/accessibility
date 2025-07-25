/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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
#include "accessibility_element_operator_impl.h"
#include "accessibility_system_ability_client.h"
#include "accessible_ability_manager_state_observer_stub.h"
#include "ffrt.h"
#include "ffrt_inner.h"
#include "iaccessible_ability_manager_service.h"
#include "refbase.h"
#include "system_ability_load_callback_stub.h"
#include "system_ability_status_change_stub.h"
#include "rwlock.h"

namespace OHOS {
namespace Accessibility {
using StateArray = std::array<bool, AccessibilityStateEventType::EVENT_TYPE_MAX>;
using StateObserverVector = std::vector<std::shared_ptr<AccessibilityStateObserver>>;
using StateObserversArray = std::array<StateObserverVector, AccessibilityStateEventType::EVENT_TYPE_MAX>;
class AccessibilitySystemAbilityClientImpl
    : public AccessibilitySystemAbilityClient, public AccessibilityElementOperatorCallback {
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
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError RegisterElementOperator(const int32_t windowId,
        const std::shared_ptr<AccessibilityElementOperator> &operation) override;

    /**
     * @brief Register the element operator, so the AA can get node info from ACE.
     * @param parameter The Register parameters.
     * @param operation The callback object.
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError RegisterElementOperator(Registration parameter,
        const std::shared_ptr<AccessibilityElementOperator> &operation) override;

    /**
     * @brief Deregister the element operator.
     * @param windowId Window ID
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError DeregisterElementOperator(const int32_t windowId) override;

    /**
     * @brief Deregister the element operator.
     * @param windowId Window ID
     * @param treeId Tree ID
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError DeregisterElementOperator(const int32_t windowId, const int32_t treeId) override;

    /**
     * @brief Checks whether screenreader ability is enabled.
     * @param isEnabled true: enabled; false: disabled
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError IsScreenReaderEnabled(bool &isEnabled) override;

    /**
     * @brief Checks whether accessibility ability is enabled.
     * @param isEnabled true: enabled; false: disabled
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError IsEnabled(bool &isEnabled) override;

    /**
     * @brief Checks whether touch exploration ability is enabled.
     * @param isEnabled true: enabled; false: disabled
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError IsTouchExplorationEnabled(bool &isEnabled) override;

    /**
     * @brief Get touch exploration mode.
     * @param touchMode 'singleTouchMode': single click mode; 'doubleTouchMode': double click mode;
     *                  'none": touch exploration is not enabled.
     */
    virtual void GetTouchMode(std::string &touchMode) override;

    /**
     * @brief Queries the list of accessibility abilities.
     * @param accessibilityAbilityTypes Indicates the accessibility type specified by AccessibilityAbilityTypes.
     * @param stateType Indicates the accessibility ability status.
     *                  1 indicates that the ability is enabled;
     *                  2 indicates that the ability is disabled;
     *                  3 indicates that the ability has been installed.
     * @param infos accessibility ability infos by specified types.
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetAbilityList(const uint32_t accessibilityAbilityTypes, const AbilityStateType stateType,
        std::vector<AccessibilityAbilityInfo> &infos) override;

    /**
     * @brief Sends an accessibility event.
     * @param eventType  Identifies the accessibility event specified by AccessibilityEventInfo.
     * @param componentId Indicates the ID of the component to be associated with the event.
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError SendEvent(const EventType eventType, const int64_t componentId) override;

    /**
     * @brief Sends information about an accessibility event.
     * @param event Indicates the accessibility event information specified by AccessibilityEventInfo.
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError SendEvent(const AccessibilityEventInfo &event) override;

    /**
     * @brief Subscribes to the specified type of accessibility status change events.
     * @param observer Indicates the observer for listening to status events, which is specified
     *              by AccessibilityStateObserver.
     * @param eventType Indicates the status type, which is specified by AccessibilityStateEventType.
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError SubscribeStateObserver(const std::shared_ptr<AccessibilityStateObserver> &observer,
        const uint32_t eventType) override;

    /**
     * @brief Unsubscribe the specified type of accessibility status change events.
     * @param observer Indicates the registered accessibility status event observer.
     * @param eventType Indicates the status type, which is specified by AccessibilityStateEventType.
     * @return Returns RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError UnsubscribeStateObserver(const std::shared_ptr<AccessibilityStateObserver> &observer,
        const uint32_t eventType) override;

    /**
     * @brief Get enabled abilities.
     * @param enabledAbilities The infos of enabled abilities.
     * @return Return RET_OK if successful, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetEnabledAbilities(std::vector<std::string> &enabledAbilities) override;

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

    /**
     * @brief Set the accessibility state.
     * @param stateType The state type and value.
     *                  state type: Refer to AccessibilityStateEventType.
     *                  value: STATE_ACCESSIBILITY_ENABLED/STATE_EXPLORATION_ENABLED/
     *                         STATE_ACCESSIBILITY_DISABLED/STATE_EXPLORATION_DISABLED
     */
    void SetAccessibilityState(const uint32_t stateType);

    /**
     * @brief Set the element information by accessibility id to AA.
     * @param infos The element info searched by accessibility id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetSearchElementInfoByAccessibilityIdResult(const std::list<AccessibilityElementInfo> &infos,
        const int32_t requestId) override;

    /**
     * @brief Set the element information by window id to AA.
     * @param infos The element info searched by window id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetSearchDefaultFocusByWindowIdResult(const std::list<AccessibilityElementInfo> &infos,
        const int32_t requestId) override;

    /**
     * @brief Set the element information matched with text to AA.
     * @param infos The element information searched matched with text.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetSearchElementInfoByTextResult(const std::list<AccessibilityElementInfo> &infos,
        const int32_t requestId) override;

    /**
     * @brief Set the element information matched with focus type to AA.
     * @param info The element information searched matched with focus type.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetFindFocusedElementInfoResult(const AccessibilityElementInfo &info,
        const int32_t requestId) override;

    /**
     * @brief Set the element information by focus direction to AA.
     * @param info The element information searched by focus direction.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetFocusMoveSearchResult(const AccessibilityElementInfo &info, const int32_t requestId) override;

    /**
     * @brief Set the result of action executed to AA.
     * @param succeeded True: The action is executed successfully; otherwise is false.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetExecuteActionResult(const bool succeeded, const int32_t requestId) override;

    /**
     * @brief Set the result of cursor position to AA.
     * @param cursorPosition The cursorPosition to be returned.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetCursorPositionResult(const int32_t cursorPosition, const int32_t requestId) override;

    /**
     * @brief Set the element information by specific property to AA.
     * @param infos The element info searched by accessibility id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    virtual void SetSearchElementInfoBySpecificPropertyResult(const std::list<AccessibilityElementInfo> &infos,
        const std::list<AccessibilityElementInfo> &treeInfos, const int32_t requestId) override;

    virtual uint32_t GetAccessibilityState() override;
    virtual void SetFindAccessibilityNodeInfoResult(const AccessibilityElementInfo elementInfo,
        const int32_t requestId, const int32_t requestCode) override;
    virtual void SetFindAccessibilityNodeInfosResult(const std::list<AccessibilityElementInfo> elementInfos,
        const int32_t requestId, const int32_t requestCode) override;
    virtual void SetPerformActionResult(const bool succeeded, const int32_t requestId) override;
    virtual RetError GetFocusedWindowId(int32_t &focusedWindowId) override;
    virtual RetError SearchNeedEvents(std::vector<uint32_t> &needEvents) override;

    bool LoadAccessibilityService();
    void LoadSystemAbilitySuccess(const sptr<IRemoteObject> &remoteObject);
    void LoadSystemAbilityFail();

private:
    class AccessibleAbilityManagerStateObserverImpl : public AccessibleAbilityManagerStateObserverStub {
    public:
        explicit AccessibleAbilityManagerStateObserverImpl(AccessibilitySystemAbilityClientImpl &client)
            : client_(client) {}
        ~AccessibleAbilityManagerStateObserverImpl() = default;

        virtual void OnStateChanged(const uint32_t stateType) override
        {
            if (clientDeleted_ == false) {
                client_.OnAccessibleAbilityManagerStateChanged(stateType);
            }
        }
        void OnClientDeleted()
        {
            clientDeleted_ = true;
        }
    private:
        AccessibilitySystemAbilityClientImpl &client_;
        std::atomic<bool> clientDeleted_ = false;
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

    class AccessibilityLoadCallback : public SystemAbilityLoadCallbackStub {
    public:
        void OnLoadSystemAbilitySuccess(int32_t systemAbilityId,
            const sptr<IRemoteObject> &remoteObject) override;
        void OnLoadSystemAbilityFail(int32_t systemAbilityId) override;
    };

    class StateArrayHandler {
    public:
        StateArrayHandler();
        ~StateArrayHandler() = default;
        void SetState(AccessibilityStateEventType type, bool state);
        bool GetState(AccessibilityStateEventType);
        void Reset();

    private:
        Utils::RWLock rwLock_;
        StateArray stateArray_;
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

    void NotifyTouchModeChanged(bool touchExplorationState, bool isSingleTouchMode);

    /**
     * @brief Check the event type is valid or not.
     * @param eventType The data of event type.
     * @return True: The data of event type is valid; otherwise is not.
     */
    bool CheckEventType(EventType eventType);
    static void OnParameterChanged(const char *key, const char *value, void *context);
    void ReregisterElementOperator();

    uint32_t state_{0};
    ffrt::mutex mutex_;
    StateArrayHandler stateHandler_;
    StateObserversArray stateObserversArray_;

    std::map<int32_t, sptr<AccessibilityElementOperatorImpl>> elementOperators_;
    sptr<IRemoteObject::DeathRecipient> deathRecipient_ = nullptr;
    sptr<IAccessibleAbilityManagerService> serviceProxy_ = nullptr;
    sptr<AccessibleAbilityManagerStateObserverImpl> stateObserver_ = nullptr;

    ffrt::condition_variable proxyConVar_;
    ffrt::mutex conVarMutex_; // mutex for proxyConVar
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_SYSTEM_ABILITY_CLIENT_IMPL_H