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

#ifndef ACCESSIBILITY_ELEMENT_OPERATOR_IMPL_H
#define ACCESSIBILITY_ELEMENT_OPERATOR_IMPL_H

#include <memory>
#include <unordered_map>
#include "accessibility_element_operator_callback.h"
#include "accessibility_element_operator_stub.h"
#include "accessibility_element_operator.h"
#include "nocopyable.h"
#include "ffrt.h"

namespace OHOS {
namespace Accessibility {
/*
* The class define the interface for UI to implement.
* It is triggered by ABMS when AA to request the accessibility information.
*/
class AccessibilityElementOperatorImpl : public AccessibilityElementOperatorStub {
public:
    /**
     * @brief construct function
     * @param windowId The window id.
     * @param operation The object implemented by ACE.
     * @param callback The callback which is to transfer the result from ACE.
     */
    explicit AccessibilityElementOperatorImpl(int32_t windowId,
        const std::shared_ptr<AccessibilityElementOperator> &operation,
        AccessibilityElementOperatorCallback &callback);

    /**
     * @brief destruct function
     */
    ~AccessibilityElementOperatorImpl();

    /**
     * @brief Make the node information by accessibility ID and set the result by callback.
     * @param elementId: The unique id of the component ID.
     * @param requestId Matched the request and response. It needn't cared by ACE, transfer it by callback only.
     * @param callback  To transfer the node info to ASAC and it defined by ASAC.
     * @param mode PREFETCH_PREDECESSORS: Need to make the parent node info also.
     *              PREFETCH_SIBLINGS: Need to make the sister/brothers node info also.
     *              PREFETCH_CHILDREN: Need to make the child node info also.
     *              otherwise: Make the node information by elementId only.
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
    virtual void GetCursorPosition(const int64_t elementId, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

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
     * @brief Get the window id related with operator object
     * @return The unique id of the window related with operator object.
     */
    int32_t GetWindowId();

    /**
     * @brief Set the elementId, childWindowId, childtreeId to AA
     * @param elementId The element ID.
     * @param childTreeId The childTree ID.
     * @param childWindowId The childwindow ID.
     */
    virtual void SetChildTreeIdAndWinId(const int64_t elementId, const int32_t childTreeId,
        const int32_t childWindowId) override;

    /**
     * @brief Set the belongTree id to AA
     * @param  treeId The tree ID.
     */
    virtual void SetBelongTreeId(const int32_t treeId) override;

    /**
     * @brief Set the parent window id to AA
     * @param  parentWindowId The parentWindowId ID.
     */
    virtual void SetParentWindowId(const int32_t parentWindowId) override;

    /**
     * @brief Set the element information by accessibility id to AA.
     * @param infos The element info searched by accessibility id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchElementInfoByAccessibilityIdResult(const std::list<AccessibilityElementInfo> &infos,
        const int32_t requestId);

    /**
     * @brief Set the element information by window id to AA.
     * @param infos The element info searched by window id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchDefaultFocusByWindowIdResult(const std::list<AccessibilityElementInfo> &infos,
        const int32_t requestId);

    /**
     * @brief Set whether to perform filtering.
     */
    static void SetFiltering(std::vector<AccessibilityElementInfo> &filterInfos);

    /**
     * @brief Set the element information matched with text to AA.
     * @param infos The element information searched matched with text.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchElementInfoByTextResult(const std::list<AccessibilityElementInfo> &infos,
        const int32_t requestId);

    /**
     * @brief Set the element information matched with focus type to AA.
     * @param info The element information searched matched with focus type.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetFindFocusedElementInfoResult(const AccessibilityElementInfo &info, const int32_t requestId);

    /**
     * @brief Set the element information by focus direction to AA.
     * @param info The element information searched by focus direction.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetFocusMoveSearchResult(const AccessibilityElementInfo &info, const int32_t requestId);

    /**
     * @brief Set the result of action executed to AA.
     * @param succeeded True: The action is executed successfully; otherwise is false.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetExecuteActionResult(const bool succeeded, const int32_t requestId);

    /**
     * @brief Set the result of cursor position to AA.
     * @param cursorPosition The cursorPosition to be returned.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetCursorPositionResult(const int32_t cursorPosition, const int32_t requestId);

    /**
     * @brief Search element infos by specific property.
     * @param elementId The accessibility id of the component.
     * @param param The specific property parameters.
     * @param requestId The request id from AA, it is used to match with request and response.
     * @param callback The callback to return the result.
     */
    virtual void SearchElementInfoBySpecificProperty(const int64_t elementId,
        const SpecificPropertyParam& param, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    /**
     * @brief Set the search element info by specific property result.
     * @param infos The element infos searched by specific property.
     * @param treeInfos The element infos searched by specific property.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchElementInfoBySpecificPropertyResult(const std::list<AccessibilityElementInfo> &infos,
        const std::list<AccessibilityElementInfo> &treeInfos, const int32_t requestId);

    static sptr<IAccessibilityElementOperatorCallback> GetCallbackByRequestId(const int32_t requestId);
    static void EraseCallback(const int32_t requestId);

private:
    int32_t AddRequest(int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback);

    static ffrt::mutex requestsMutex_;
    int32_t windowId_ = 0;
    AccessibilityElementOperatorCallback &operatorCallback_;
    std::shared_ptr<AccessibilityElementOperator> operator_ = nullptr;
    static std::unordered_map<int32_t, sptr<IAccessibilityElementOperatorCallback>> requests_;
    DISALLOW_COPY_AND_MOVE(AccessibilityElementOperatorImpl);
};

template<class T>
std::vector<T> TranslateListToVector(const std::list<T> &originList)
{
    std::size_t len = originList.size();
    std::vector<T> destVector(len);
    std::copy(originList.begin(), originList.end(), destVector.begin());
    return destVector;
}

template<class T>
std::list<T> TranslateVectorToList(const std::vector<T> &originVector)
{
    size_t len = originVector.length();
    std::list<T> destList(len);
    std::copy(originVector.begin(), originVector.end(), destList.begin());
    return destList;
}
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_ELEMENT_OPERATOR_IMPL_H