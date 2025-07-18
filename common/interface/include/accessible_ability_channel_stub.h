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

#ifndef ACCESSIBLE_ABILITY_CHANNEL_STUB_H
#define ACCESSIBLE_ABILITY_CHANNEL_STUB_H

#include "iaccessible_ability_channel.h"
#include "iremote_stub.h"

namespace OHOS {
namespace Accessibility {
class AccessibleAbilityChannelStub : public IRemoteStub<IAccessibleAbilityChannel> {
public:
    /**
     * @brief construct function
     * @param object The object of IPC
     */
    AccessibleAbilityChannelStub();

    /**
     * @brief destruct function
     */
    virtual ~AccessibleAbilityChannelStub();

    /**
     * @brief Receive the event from proxy by IPC mechanism.
     * @param code The code is matched with the process function.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @param option The option parameter of IPC,such as: async,sync
     */
    virtual int OnRemoteRequest(uint32_t code, MessageParcel &data,
        MessageParcel &reply, MessageOption &option) override;
private:
    /**
     * @brief Handle the IPC request for the function:HandleSearchElementInfoByAccessibilityId.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSearchElementInfoByAccessibilityId(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:SearchDefaultFocusedByWindowId.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSearchDefaultFocusedByWindowId(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSearchElementInfosByText.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSearchElementInfosByText(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleFindFocusedElementInfo.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleFindFocusedElementInfo(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleFocusMoveSearch.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleFocusMoveSearch(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleExecuteAction.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleExecuteAction(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleEnableScreenCurtain.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleEnableScreenCurtain(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleHoldRunningLock.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleHoldRunningLock(MessageParcel &data, MessageParcel &reply);
 
    /**
     * @brief Handle the IPC request for the function:HandleUnholdRunningLock.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleUnholdRunningLock(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetWindow.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetWindow(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetWindows.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetWindows(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetWindows.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetWindowsByDisplayId(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSetOnKeyPressEventResult.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSetOnKeyPressEventResult(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSendSimulateGesturePath.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSendSimulateGesturePath(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSetTargetBundleName.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSetTargetBundleName(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetCursorPosition.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetCursorPosition(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSetIsRegisterDisconnectCallback.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSetIsRegisterDisconnectCallback(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleNotifyDisconnect.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleNotifyDisconnect(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleConfigureEvents.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleConfigureEvents(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSearchElementInfoBySpecificProperty.
     * @param data The data of process communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSearchElementInfoBySpecificProperty(MessageParcel &data, MessageParcel &reply);

    using AccessibleAbilityConnectionFunc =
        ErrCode (AccessibleAbilityChannelStub::*)(MessageParcel &data, MessageParcel &reply);
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBLE_ABILITY_CHANNEL_STUB_H