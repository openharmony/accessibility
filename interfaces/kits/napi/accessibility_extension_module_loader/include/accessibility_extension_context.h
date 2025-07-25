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

#ifndef ACCESSIBILITY_EXTENSION_CONTEXT_H
#define ACCESSIBILITY_EXTENSION_CONTEXT_H

#include <map>
#include <memory>
#include "accessibility_element_info.h"
#include "accessibility_event_info.h"
#include "accessibility_gesture_inject_path.h"
#include "accessibility_window_info.h"
#include "accessible_ability_client.h"
#include "extension_context.h"
#include "want.h"

namespace OHOS {
namespace Accessibility {
class AccessibilityExtensionContext : public AbilityRuntime::ExtensionContext {
public:
    AccessibilityExtensionContext() = default;
    virtual ~AccessibilityExtensionContext() = default;

    /**
     * @brief Obtains elementInfo of focus.
     * @param focusType The type of focus. It contains FOCUS_TYPE_INPUT and FOCUS_TYPE_ACCESSIBILITY.
     * @param elementInfo The accessibilityElementInfo of focus.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetFocus(const int32_t focusType, AccessibilityElementInfo &elementInfo, bool systemApi = false);

    /**
     * @brief Obtains elementInfo of focus.
     * @param sourceInfo
     * @param focusType The type of focus. It contains FOCUS_TYPE_INPUT and FOCUS_TYPE_ACCESSIBILITY.
     * @param elementInfo The accessibilityElementInfo of focus.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetFocusByElementInfo(const AccessibilityElementInfo &sourceInfo, const int32_t focusType,
        AccessibilityElementInfo &elementInfo);

    /**
     * @brief Sends simulate gestures to the screen.
     * @param gesturePath The gesture which need to send.
     * @return Return RET_OK if the gesture sends successfully, otherwise refer to the RetError for the failure.
     */
    RetError InjectGesture(const std::shared_ptr<AccessibilityGestureInjectPath> &gesturePath);

    /**
     * @brief Obtains elementInfo of the accessible root node.
     * @param elementInfo The elementInfo of the accessible root node.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetRoot(AccessibilityElementInfo &elementInfo, bool systemApi = false);

    /**
     * @brief Obtains elementInfo of the accessible root node.
     * @param windowInfo The source window info to get root.
     * @param elementInfo The elementInfo of the accessible root node.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetRootByWindow(const AccessibilityWindowInfo &windowInfo,
        AccessibilityElementInfo &elementInfo, bool systemApi = false);

    /**
     * @brief Obtains the list of interactive windows on the device, in the layers they are visible to users.
     * @param windows The information of windows.
     * @return Return RET_OK if obtains windowInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetWindows(std::vector<AccessibilityWindowInfo> &windows, bool systemApi = false);

    /**
     * @brief Obtains the list of interactive windows on the device, in the layers they are visible to users.
     * @param displayId the id of display
     * @param windows The information of windows.
     * @return Return RET_OK if obtains windowInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetWindows(
        const uint64_t displayId, std::vector<AccessibilityWindowInfo>& windows, bool systemApi = false);

    /**
     * @brief Gets the next focused node in the specified direction of the currently focused node.
     * @param elementInfo
     * @param direction Indicates the direction to obtain the next focused node. Refer to FocusMoveDirection
     * @param nextElementInfo The info of next element.
     * @return Return RET_OK if gets next elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetNext(const AccessibilityElementInfo &elementInfo, const FocusMoveDirection direction,
        AccessibilityElementInfo &nextElementInfo);

    /**
     * @brief Get the child node information by childId
     * @param index
     * @param parent
     * @param child The element info of child
     * @return Return RET_OK if gets child elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetChildElementInfo(const int32_t index, const AccessibilityElementInfo &parent,
        AccessibilityElementInfo &child);

    /**
     * @brief Searches for node information based on the specified content.
     * @param elementInfo
     * @param text specified content
     * @param elementInfos
     * @return Return RET_OK if gets elementInfos successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetByContent(const AccessibilityElementInfo &elementInfo, const std::string &text,
        std::vector<AccessibilityElementInfo> &elementInfos);

    /**
     * @brief Get Parent node information
     * @param child
     * @param parent
     * @return Return RET_OK if gets info successfully, otherwise refer to the RetError for the failure.
     */
    RetError GetParentElementInfo(const AccessibilityElementInfo &child, AccessibilityElementInfo &parent);

    /**
     * @brief Executes a specified action.
     * @param elementInfo
     * @param action: the action type
     * @param actionArguments: The parameter for action type. such as:
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
     * @return Return RET_OK if performs action succeed, otherwise refer to the RetError for the failure.
     */
    RetError ExecuteAction(const AccessibilityElementInfo &elementInfo, const ActionType action,
        const std::map<std::string, std::string> &actionArguments);

    /**
     * @brief Set target bundle names.
     * @param targetBundleNames The target bundle name
     * @return Return RET_OK if sets target bundle names successfully, otherwise refer to the RetError for the failure.
     */
    RetError SetTargetBundleName(const std::vector<std::string> &targetBundleNames);

    /**
    * @brief Starts a new ability.
    * An ability using the AbilityInfo.AbilityType.SERVICE or AbilityInfo.AbilityType.PAGE template uses this method
    * to start a specific ability. The system locates the target ability from installed abilities based on the value
    * of the want parameter and then starts it. You can specify the ability to start
    * using the want parameter.
    * @param want Indicates the Want containing information about the target ability to start.
    * @return RetError ERR_OK on success, others on failure.
    */
    RetError StartAbility(const AAFwk::Want &want);

    /**
     * @brief Set the screen curtain enable or disable.
     * @param isEnable Indicates whether the screen curtain is enabled.
     * @return Return RET_OK if set screen curtain successfully, otherwise refer to the RetError for the failure.
     */
    RetError EnableScreenCurtain(bool isEnable);

    RetError GetElements(const int32_t windowId, const int64_t elementId,
        std::vector<AccessibilityElementInfo> &elementInfos);

    RetError GetDefaultFocusedElementIds(const int32_t windowId,
        std::vector<AccessibilityElementInfo> &elementInfos);

    /**
    * @brief Hold running lock to prevent screen turning off automatically.
    * @param null.
    * @return RetError ERR_OK on success, others on failure.
    */
    RetError HoldRunningLock();

    /**
    * @brief Unhold running lock.
    * @param null.
    * @return RetError ERR_OK on success, others on failure.
    */
    RetError UnholdRunningLock();

    /**
    * @brief Register Disconnect Callback.
    * @param callback.
    * @return Return RET_OK if Register callback successfully, otherwise refer to the RetError for the failure.
    */
    RetError RegisterDisconnectCallback(std::shared_ptr<DisconnectCallback> &callback);

    /**
    * @brief UnRegister Disconnect Callback.
    * @return Return RET_OK if UnRegister callback successfully, otherwise refer to the RetError for the failure.
    */
    RetError UnRegisterDisconnectCallback(std::shared_ptr<DisconnectCallback> &callback);

    /**
    * @brief Notify Disconnect.
    * @return Return RET_OK if NotifyDisconnect successfully, otherwise refer to the RetError for the failure.
    */
    RetError NotifyDisconnect();
private:
    static int illegalRequestCode_;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_EXTENSION_CONTEXT_H