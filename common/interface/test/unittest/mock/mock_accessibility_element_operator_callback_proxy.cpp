/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#include "accessibility_element_operator_callback_proxy.h"
#include "accessibility_element_info_parcel.h"
#include "accessibility_def.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {

constexpr int32_t IPC_MEMORY_SIZE = 500 * 1024; // default size is 200 * 1024B, batch query need more memory
constexpr int32_t SINGLE_TRANSMIT = -2;
constexpr int32_t MULTI_TRANSMIT_FINISH = -1;
constexpr int32_t DATA_NUMBER_ONE_TIME = 400;

AccessibilityElementOperatorCallbackProxyTest::AccessibilityElementOperatorCallbackProxyTest(
    const sptr<IRemoteObject> &impl) : IRemoteProxy<IAccessibilityElementOperatorCallback>(impl)
{}

AccessibilityElementOperatorCallbackProxyTest::~AccessibilityElementOperatorCallbackProxyTest()
{}

bool AccessibilityElementOperatorCallbackProxyTest::WriteInterfaceToken(MessageParcel &data)
{
    HILOG_DEBUG();
    if (!data.WriteInterfaceToken(AccessibilityElementOperatorCallbackProxyTest::GetDescriptor())) {
        HILOG_ERROR("write interface token failed");
        return false;
    }
    return true;
}

bool AccessibilityElementOperatorCallbackProxyTest::SendTransactCmd(AccessibilityInterfaceCode code,
    MessageParcel &data, MessageParcel &reply,  MessageOption &option)
{
    HILOG_DEBUG();

    sptr<IRemoteObject> remoteObj = Remote();
    if (remoteObj == nullptr) {
        HILOG_ERROR("fail to send transact cmd %{public}d due to remote object", code);
        return false;
    }
    int32_t ret = remoteObj->SendRequest(static_cast<uint32_t>(code), data, reply, option);
    if (ret != NO_ERROR) {
        HILOG_ERROR("receive error transact code %{public}d in transact cmd %{public}d", ret, code);
        return false;
    }
    return true;
}

void AccessibilityElementOperatorCallbackProxy::SetSearchElementInfoByAccessibilityIdResult(
    const std::vector<AccessibilityElementInfo> &infos, const int32_t requestId)
{
    HILOG_DEBUG("infos size %{public}zu, resquestId %{public}d", infos.size(), requestId);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write request id failed");
        return;
    }

    if (!data.WriteInt32(infos.size())) {
        HILOG_ERROR("write infos's size failed");
        return;
    }

    MessageParcel tmpParcel;
    tmpParcel.SetMaxCapacity(MAX_RAWDATA_SIZE);
    // when set pracel's max capacity, it won't alloc memory immediately
    // MessageParcel will expand memory dynamiclly
    for (const auto &info : infos) {
        AccessibilityElementInfoParcel infoParcel(info);
        if (!tmpParcel.WriteParcelable(&infoParcel)) {
            HILOG_ERROR("write accessibilityElementInfoParcel failed");
            return;
        }
    }
    size_t tmpParcelSize = tmpParcel.GetDataSize();
    if (!data.WriteUint32(tmpParcelSize)) {
        HILOG_ERROR("write rawData size failed");
        return;
    }
    if (!data.WriteRawData(reinterpret_cast<uint8_t *>(tmpParcel.GetData()), tmpParcelSize)) {
        HILOG_ERROR("write rawData failed");
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_BY_ACCESSIBILITY_ID, data, reply, option)) {
        HILOG_ERROR("setSearchElementInfoByAccessibilityIdResult failed");
        return;
    }
    if (static_cast<RetError>(reply.ReadInt32() != RET_OK)) {
        HILOG_ERROR("read reply code failed");
    }
    return;
}

void AccessibilityElementOperatorCallbackProxyTest::SetSearchElementInfoByTextResult(
    const std::vector<AccessibilityElementInfo> &infos, const int32_t requestId)
{
    HILOG_DEBUG();
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }

    if (!data.WriteInt32(infos.size())) {
        HILOG_ERROR("write infos's size failed");
        return;
    }

    for (auto &info : infos) {
        AccessibilityElementInfoParcel infoParcel(info);
        if (!data.WriteParcelable(&infoParcel)) {
            HILOG_ERROR("write accessibility element info failed");
            return;
        }
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write request id failed");
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_BY_TEXT,
        data, reply, option)) {
        HILOG_ERROR("set search element info by text result failed");
        return;
    }
}

void AccessibilityElementOperatorCallbackProxyTest::SetFindFocusedElementInfoResult(
    const AccessibilityElementInfo &info, const int32_t requestId)
{
    HILOG_DEBUG();
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    AccessibilityElementInfoParcel infoParcel(info);

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }

    if (!data.WriteParcelable(&infoParcel)) {
        HILOG_ERROR("connection write info failed");
        return;
    }
    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write request id failed");
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_FOCUSED_INFO,
        data, reply, option)) {
        HILOG_ERROR("set find focused element info result failed");
        return;
    }
}

void AccessibilityElementOperatorCallbackProxyTest::SetFocusMoveSearchResult(const AccessibilityElementInfo &info,
    const int32_t requestId)
{
    HILOG_DEBUG();
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    AccessibilityElementInfoParcel infoParcel(info);

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }

    if (!data.WriteParcelable(&infoParcel)) {
        HILOG_ERROR("connection write info failed");
        return;
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write requestId failed");
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_FOCUS_MOVE,
        data, reply, option)) {
        HILOG_ERROR("set focus move search result failed");
        return;
    }
}

void AccessibilityElementOperatorCallbackProxyTest::SetExecuteActionResult(const bool succeeded, const int32_t requestId)
{
    HILOG_DEBUG();
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }

    if (!data.WriteBool(succeeded)) {
        HILOG_ERROR("connection write failed");
        return;
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write request id failed");
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_PERFORM_ACTION,
        data, reply, option)) {
        HILOG_ERROR("set execute action result failed");
        return;
    }
}

void AccessibilityElementOperatorCallbackProxyTest::SetCursorPositionResult(const int32_t cursorPosition,
    const int32_t requestId)
{
    HILOG_DEBUG();
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }
    HILOG_INFO(" [cursorPosition:%{public}d]", cursorPosition);
    if (!data.WriteInt32(cursorPosition)) {
        HILOG_ERROR("connection write failed");
        return;
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write request id failed");
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_CURSOR_RESULT,
        data, reply, option)) {
        HILOG_ERROR("set cursor position result failed");
        return;
    }
}

void AccessibilityElementOperatorCallbackProxyTest::SetSearchElementInfoBySpecificPropertyResult(
    const std::list<AccessibilityElementInfo> &infos, const std::list<AccessibilityElementInfo> &treeInfos,
    const int32_t requestId)
{
    HILOG_DEBUG("infos size %{public}zu, treeInfos size %{public}zu, requestId %{public}d",
        infos.size(), treeInfos.size(), requestId);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("connection write token failed");
        return;
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("connection write request id failed");
        return;
    }

    if (!data.WriteUint32(infos.size())) {
        HILOG_ERROR("write infos's size failed");
        return;
    }

    if (!WriteElementInfosToRawData(infos, data)) {
        return;
    }

    if (!data.WriteUint32(treeInfos.size())) {
        HILOG_ERROR("write tree infos's size failed");
        return;
    }

    if (!WriteElementInfosToRawData(treeInfos, data)) {
        return;
    }

    if (!SendTransactCmd(AccessibilityInterfaceCode::SET_RESULT_BY_SPECIFIC_PROPERTY, data, reply, option)) {
        HILOG_ERROR("setSearchElementInfoBySpecificPropertyResult failed");
        return;
    }
    if (static_cast<RetError>(reply.ReadInt32() != RET_OK)) {
        HILOG_ERROR("read reply code failed");
    }
    return;
}

bool AccessibilityElementOperatorCallbackProxyTest::WriteElementInfosToRawData(
    const std::list<AccessibilityElementInfo> &infos, MessageParcel &data)
{
    if (infos.size() == 0) {
        HILOG_DEBUG("infos is empty, skip writing raw data");
        return true;
    }

    MessageParcel tmpParcel;
    tmpParcel.SetMaxCapacity(MAX_RAWDATA_SIZE);
    // when set pracel's max capacity, it won't alloc memory immediately
    // MessageParcel will expand memory dynamiclly
    for (const auto &info : infos) {
        AccessibilityElementInfoParcel infoParcel(info);
        if (!tmpParcel.WriteParcelable(&infoParcel)) {
            HILOG_ERROR("write accessibilityElementInfoParcel failed");
            return false;
        }
    }
    size_t tmpParcelSize = tmpParcel.GetDataSize();
    if (!data.WriteUint32(tmpParcelSize)) {
        HILOG_ERROR("write rawData size failed");
        return false;
    }
    if (!data.WriteRawData(reinterpret_cast<uint8_t *>(tmpParcel.GetData()), tmpParcelSize)) {
        HILOG_ERROR("write rawData failed");
        return false;
    }
    return true;
}
} // namespace Accessibility
} // namespace OHOS
