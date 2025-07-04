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

#ifndef ACCESSIBILITY_ELEMENT_OPERATOR_PROXY_H
#define ACCESSIBILITY_ELEMENT_OPERATOR_PROXY_H

#include "accessibility_ipc_interface_code.h"
#include "iaccessibility_element_operator.h"
#include "iremote_proxy.h"

namespace OHOS {
namespace Accessibility {
/*
* The class define the interface for UI to implement.
* It triggered by ABMS when AA to request the accessibility information.
*/
class AccessibilityElementOperatorProxy : public IRemoteProxy<IAccessibilityElementOperator> {
public:
    /**
     * @brief construct function
     * @param object The object of IPC
     */
    explicit AccessibilityElementOperatorProxy(const sptr<IRemoteObject> &object);

    /**
     * @brief destruct function
     */
    virtual ~AccessibilityElementOperatorProxy() = default;

    /**
     * @brief Make the node information by accessibility ID and set the result by callback.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param mode PREFETCH_PREDECESSORS: Need to make the parent node info also.
     *              PREFETCH_SIBLINGS: Need to make the sister/brothers node info also.
     *              PREFETCH_CHILDREN: Need to make the child node info also.
     *              otherwise: Make the node information by elementId only.
     * @sysCap Accessibility
     */
    virtual RetError SearchElementInfoByAccessibilityId(const int64_t elementId, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback, const int32_t mode,
        bool isFilter = false) override;

    /**
     * @brief Make the node information by window ID and set the result by callback.
     * @param windowId: The window id.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param mode PREFETCH_PREDECESSORS: Need to make the parent node info also.
     *              PREFETCH_SIBLINGS: Need to make the sister/brothers node info also.
     *              PREFETCH_CHILDREN: Need to make the child node info also.
     *              otherwise: Make the node information by elementId only.
     * @sysCap Accessibility
     */
    virtual void SearchDefaultFocusedByWindowId(const int32_t windowId, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback, const int32_t mode,
        bool isFilter = false) override;

    /**
     * @brief Make the child node information by accessibility ID and filtered by text and set the result by callback.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param text  Filter for the child components to matched with the text
     */
    virtual void SearchElementInfosByText(const int64_t elementId, const std::string &text,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    /**
     * @brief Make the node information of the component focused by the focus type specified.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param focusType FOCUS_TYPE_ACCESSIBILITY: accessibility focus
     *                  FOCUS_TYPE_INPUT: text input focus
     */
    virtual void FindFocusedElementInfo(const int64_t elementId, const int32_t focusType, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    /**
     * @brief Make the node info by current focus move direction.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param direction Refer to AccessibilityElementInfo.FocusMoveDirection(UP/DOWN/LEFT/RIGHT/FORWARD/BACKWARD)
     */
    virtual void FocusMoveSearch(const int64_t elementId, const int32_t direction, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    /**
     * @brief To return the result of perform action.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param action Refer to [AccessibilityElementInfo.ActionType]
     * @param actionArguments The parameter for action type. such as:
     *      action: ACCESSIBILITY_ACTION_NEXT_HTML_ITEM,
     *                  actionArguments(ACTION_ARGU_HTML_ELEMENT,HtmlItemType)
     *      action: ACCESSIBILITY_ACTION_PREVIOUS_HTML_ITEM,
     *                  actionArguments(ACTION_ARGU_HTML_ELEMENT,HtmlItemType)
     *      action: ACCESSIBILITY_ACTION_NEXT_TEXT,
     *                  actionArguments(ACTION_ARGU_MOVE_UNIT,MOVE_UNIT_XXX)
     *      action: ACCESSIBILITY_ACTION_PREVIOUS_TEXT,
     *                  actionArguments(ACTION_ARGU_MOVE_UNIT,MOVE_UNIT_XXX)
     *      action: ACCESSIBILITY_ACTION_SET_SELECTION,
     *                  actionArguments({ACTION_ARGU_SELECT_TEXT_START,"1"(start location)},
     *                                  {ACTION_ARGU_SELECT_TEXT_END,"10"(end location)})
     *      action: ACCESSIBILITY_ACTION_SET_TEXT,
     *                  actionArguments(ACTION_ARGU_SET_TEXT,"the text of setted")
     */
    virtual void ExecuteAction(const int64_t elementId, const int32_t action,
        const std::map<std::string, std::string> &actionArguments,
        int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    /**
     * @brief To return the result of cursor position.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     */
    virtual void GetCursorPosition(const int64_t elementId,
        int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    /**
     * @brief The function is called while accessibility System check the id of window is not equal
     * to the id of active window when sendAccessibility.
     */
    virtual void ClearFocus() override;

    /**
     * @brief the low layer is notified by the function called while accessibility system execute
     * the function of executeAction from AS to check the all low windows cared the outside event.
     * Example: PopupWindow receive the OUTSIDE_EVENT to close itself.
     */
    virtual void OutsideTouch() override;

    /**
     * @brief Set isFilter.
     * @param enableFilter True : Perform filtering ;otherwise is false.
     */
    void SetIsFilter(bool enableFilter);

    /**
     * @brief Get isFilter.
     */
    bool GetFilter();

    /**
    * @brief To return the result of elementId, treeId, childWindowId.
    * @param elementId: The element Id.
    * @param treeId: The tree Id.
    * @param childWindowId The childwindow Id.
    */
    virtual void SetChildTreeIdAndWinId(const int64_t elementId, const int32_t treeId,
        const int32_t childWindowId) override;

    /**
    * @brief To return the result of treeId
    * @param treeId: The tree Id.
    */
    virtual void SetBelongTreeId(const int32_t treeId) override;

    /**
     * @brief Set the parent window Id to the element info.
     * @param iParentWindowId The parent window Id
     * @sysCap Accessibility
    */
    void SetParentWindowId(const int32_t iParentWindowId) override;

    /**
     * @brief Search element info by specific property.
     * @param elementId The unique id of the component ID.
     * @param param The specific property parameters.
     * @param requestId The request id from AA, it is used to match with request and response.
     * @param callback The callback to return the result.
     */
    virtual void SearchElementInfoBySpecificProperty(const int64_t elementId,
        const SpecificPropertyParam& param, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;
private:
    bool isFilter = false;

    /**
     * @brief Write the descriptor of IPC.
     * @param data It is include the descriptor of IPC.
     * @return true: Write the descriptor successfully; otherwise is not.
     */
    bool WriteInterfaceToken(MessageParcel &data);

    /**
     * @brief Send the command data from proxy to stub in IPC mechanism.
     * @param code The code matched the function called.
     * @param data Serializable data
     * @param reply The response of IPC
     * @param option The option parameter of IPC,such as: async,sync
     * @return true: Write the descriptor successfully; otherwise is not.
     */
    bool SendTransactCmd(AccessibilityInterfaceCode code, MessageParcel &data,
        MessageParcel &reply,  MessageOption &option);
    static inline BrokerDelegator<AccessibilityElementOperatorProxy> delegator;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_ELEMENT_OPERATOR_PROXY_H