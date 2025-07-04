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

#ifndef ACCESSIBILITY_ELEMENT_OPERATOR_CALLBACK_PROXY_H
#define ACCESSIBILITY_ELEMENT_OPERATOR_CALLBACK_PROXY_H

#include "accessibility_ipc_interface_code.h"
#include "iaccessibility_element_operator_callback.h"
#include "iremote_proxy.h"
namespace OHOS {
namespace Accessibility {
/*
* The class supply the callback to feedback the result from UI to AA.
*/
class AccessibilityElementOperatorCallbackProxy : public IRemoteProxy<IAccessibilityElementOperatorCallback> {
public:
    /**
     * @brief construct function
     * @param object The object of IPC
     */
    explicit AccessibilityElementOperatorCallbackProxy(const sptr<IRemoteObject> &object);

    /**
     * @brief destruct function
     */
    virtual ~AccessibilityElementOperatorCallbackProxy() override;

    /**
     * @brief Set the element information by accessibility id to AA.
     * @param infos The element info searched by accessibility id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchElementInfoByAccessibilityIdResult(const std::vector<AccessibilityElementInfo> &infos,
        const int32_t requestId) override;

    /**
     * @brief Set the Default Focus by window id to AA.
     * @param infos The element info searched by window id.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchDefaultFocusByWindowIdResult(const std::vector<AccessibilityElementInfo> &infos,
        const int32_t requestId) override;

    /**
     * @brief Set the element information matched with text to AA.
     * @param infos The element information searched matched with text.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchElementInfoByTextResult(const std::vector<AccessibilityElementInfo> &infos,
        const int32_t requestId) override;

    /**
     * @brief Set the element information matched with focus type to AA.
     * @param info The element information searched matched with focus type.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetFindFocusedElementInfoResult(const AccessibilityElementInfo &info, const int32_t requestId) override;

    /**
     * @brief Set the element information by focus direction to AA.
     * @param info The element information searched by focus direction.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetFocusMoveSearchResult(const AccessibilityElementInfo &info, const int32_t requestId) override;

    /**
     * @brief Set the result of action executed to AA.
     * @param succeeded True: The action is executed successfully; otherwise is false.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetExecuteActionResult(const bool succeeded, const int32_t requestId) override;

    /**
     * @brief Set the result of cursor position to AA.
     * @param cursorPosition The position of the cursor to return.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetCursorPositionResult(const int32_t cursorPosition, const int32_t requestId) override;

    /**
     * @brief Set the search element info by specific property result.
     * @param infos The element infos searched by specific property.
     * @param treeInfos The element infos searched by specific property.
     * @param requestId The request id from AA, it is used to match with request and response.
     */
    void SetSearchElementInfoBySpecificPropertyResult(const std::list<AccessibilityElementInfo> &infos,
         const std::list<AccessibilityElementInfo> &treeInfos, const int32_t requestId) override;

private:
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

    /**
     * @brief Write accessibility element infos to MessageParcel as raw data for
     *    SetSearchElementInfoBySpecificPropertyResult.
     * @param infos The accessibility element infos to write.
     * @param data The MessageParcel to write raw data to.
     * @return true: Write successfully; otherwise is false.
     */
    bool WriteElementInfosToRawData(const std::list<AccessibilityElementInfo> &infos, MessageParcel &data);

    static inline BrokerDelegator<AccessibilityElementOperatorCallbackProxy> delegator;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_ELEMENT_OPERATOR_CALLBACK_PROXY_H