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

#include "accessibility_element_operator_callback_stub.h"
#include "accessibility_element_info_parcel.h"
#include "accessibility_ipc_interface_code.h"
#include "hilog_wrapper.h"
#include "parcel_util.h"
#include <securec.h>

#define SWITCH_BEGIN(code) switch (code) {
#define SWITCH_CASE(case_code, func) case case_code:\
    {\
        result_code = func(data, reply);\
        break;\
    }

#define SWITCH_END() default:\
    {\
        result_code = ERR_CODE_DEFAULT;\
        HILOG_WARN("AccessibilityElementOperatorCallbackStubTest::OnRemoteRequest, default case, need check."); \
        break;\
    }\
}

#define ACCESSIBILITY_ELEMENT_OPERATOR_CALLBACK_STUB_CASES() \
    SWITCH_CASE( \
        AccessibilityInterfaceCode::SET_RESULT_BY_ACCESSIBILITY_ID, HandleSetSearchElementInfoByAccessibilityIdResult)\
    SWITCH_CASE(AccessibilityInterfaceCode::SET_RESULT_BY_TEXT, HandleSetSearchElementInfoByTextResult)\
    SWITCH_CASE(AccessibilityInterfaceCode::SET_RESULT_FOCUSED_INFO, HandleSetFindFocusedElementInfoResult)\
    SWITCH_CASE(AccessibilityInterfaceCode::SET_RESULT_FOCUS_MOVE, HandleSetFocusMoveSearchResult)\
    SWITCH_CASE(AccessibilityInterfaceCode::SET_RESULT_PERFORM_ACTION, HandleSetExecuteActionResult)\
    SWITCH_CASE(AccessibilityInterfaceCode::SET_RESULT_CURSOR_RESULT, HandleSetCursorPositionResult)\
    SWITCH_CASE(AccessibilityInterfaceCode::SET_RESULT_BY_SPECIFIC_PROPERTY, \
        HandleSetSearchElementInfoBySpecificPropertyResult) \

namespace OHOS {
namespace Accessibility {

constexpr int32_t SINGLE_TRANSMIT = -2;
constexpr int32_t MULTI_TRANSMIT_FINISH = -1;
constexpr int32_t ERR_CODE_DEFAULT = -1000;
constexpr int32_t MAX_RAWDATA_SIZE = 128 * 1024 * 1024; // RawData limit is 128M, limited by IPC

static bool GetData(size_t size, const void *data, void *&buffer)
{
    if (data == nullptr || size == 0) {
        return false;
    }
    if (size > MAX_RAWDATA_SIZE) {
        return false;
    }
    buffer = malloc(size);
    if (buffer == nullptr) {
        return false;
    }
    if (memcpy_s(buffer, size, data, size) != EOK) {
        free(buffer);
        return false;
    }
    return true;
}

AccessibilityElementOperatorCallbackStubTest::AccessibilityElementOperatorCallbackStubTest()
{
}

AccessibilityElementOperatorCallbackStubTest::~AccessibilityElementOperatorCallbackStubTest()
{
}

int AccessibilityElementOperatorCallbackStubTest::OnRemoteRequest(uint32_t code, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    HILOG_DEBUG("cmd = %{public}u, flags= %{public}d", code, option.GetFlags());
    std::u16string descriptor = AccessibilityElementOperatorCallbackStubTest::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        HILOG_ERROR("AccessibilityElementOperatorCallbackStubTest::OnRemoteRequest"
            "local descriptor is not equal to remote");
        return ERR_INVALID_STATE;
    }

    ErrCode result_code = ERR_NONE;
    SWITCH_BEGIN(code)
    ACCESSIBILITY_ELEMENT_OPERATOR_CALLBACK_STUB_CASES()
    SWITCH_END()

    if (result_code != ERR_CODE_DEFAULT) {
        return result_code;
    }

    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetSearchElementInfoByAccessibilityIdResult(
    MessageParcel &data, MessageParcel &reply)
{
    HILOG_DEBUG();
    std::vector<AccessibilityElementInfo> storeData;
    int32_t requestId = data.ReadInt32();
    size_t infoSize = data.ReadUint32();
    size_t rawDataSize = data.ReadUint32();
    MessageParcel tmpParcel;
    void *buffer = nullptr;
    // memory alloced in GetData will be released when tmpParcel destruct
    if (!GetData(rawDataSize, data.ReadRawData(rawDataSize), buffer)) {
        reply.WriteInt32(RET_ERR_FAILED);
        return TRANSACTION_ERR;
    }

    if (!tmpParcel.ParseFrom(reinterpret_cast<uintptr_t>(buffer), rawDataSize)) {
        reply.WriteInt32(RET_ERR_FAILED);
        return TRANSACTION_ERR;
    }

    for (size_t i = 0; i < infoSize; i++) {
        sptr<AccessibilityElementInfoParcel> info = tmpParcel.ReadStrongParcelable<AccessibilityElementInfoParcel>();
        if (info == nullptr) {
            reply.WriteInt32(RET_ERR_FAILED);
            return TRANSACTION_ERR;
        }
        storeData.emplace_back(*info);
    }
    reply.WriteInt32(RET_OK);
    SetSearchElementInfoByAccessibilityIdResult(storeData, requestId);
    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetSearchElementInfoByTextResult(
    MessageParcel &data, MessageParcel &reply)
{
    HILOG_DEBUG();
    std::vector<AccessibilityElementInfo> infos {};
    int32_t accessibilityInfosize = data.ReadInt32();
    bool verifyResult = ContainerSecurityVerify(data, accessibilityInfosize, infos.max_size());
    if (!verifyResult || accessibilityInfosize < 0 || accessibilityInfosize > INT32_MAX) {
        return TRANSACTION_ERR;
    }
    for (int32_t i = 0; i < accessibilityInfosize; i++) {
        sptr<AccessibilityElementInfoParcel> accessibilityInfo =
            data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
        if (accessibilityInfo == nullptr) {
            HILOG_ERROR("ReadStrongParcelable<accessibilityInfo> failed");
            return TRANSACTION_ERR;
        }
        infos.emplace_back(*accessibilityInfo);
    }
    int32_t requestId = data.ReadInt32();

    SetSearchElementInfoByTextResult(infos, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetFindFocusedElementInfoResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();
    sptr<AccessibilityElementInfoParcel> info = data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
    if (info == nullptr) {
        HILOG_ERROR("ReadStrongParcelable<AccessibilityElementInfo> failed");
        return TRANSACTION_ERR;
    }

    int32_t requestId = data.ReadInt32();

    SetFindFocusedElementInfoResult(*info, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetFocusMoveSearchResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();
    sptr<AccessibilityElementInfoParcel> info = data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
    if (info == nullptr) {
        HILOG_ERROR("ReadStrongParcelable<AccessibilityElementInfo> failed");
        return TRANSACTION_ERR;
    }

    int32_t requestId = data.ReadInt32();

    SetFocusMoveSearchResult(*info, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetExecuteActionResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();

    bool succeeded = data.ReadBool();
    int32_t requestId = data.ReadInt32();

    SetExecuteActionResult(succeeded, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetCursorPositionResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();
    int32_t cursorPosition = data.ReadInt32();
    int32_t requestId = data.ReadInt32();
    HILOG_INFO("[cursorPosition:%{public}d]", cursorPosition);
    SetCursorPositionResult(cursorPosition, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStubTest::HandleSetSearchElementInfoBySpecificPropertyResult(
    MessageParcel &data, MessageParcel &reply)
{
    HILOG_DEBUG();
    int32_t requestId = data.ReadInt32();
    int32_t infoSize = data.ReadInt32();

    std::list<AccessibilityElementInfo> infos;
    bool verifyResult = ContainerSecurityVerify(data, infoSize, infos.max_size());
    if (!verifyResult || infoSize < 0 || infoSize > INT32_MAX) {
        return TRANSACTION_ERR;
    }
    for (int32_t i = 0; i < infoSize; i++) {
        sptr<AccessibilityElementInfoParcel> accessibilityInfo =
            data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
        if (accessibilityInfo == nullptr) {
            HILOG_ERROR("ReadStrongParcelable<accessibilityInfo> failed");
            return TRANSACTION_ERR;
        }
        infos.emplace_back(*accessibilityInfo);
    }

    int32_t treeInfoSize = data.ReadInt32();
    std::list<AccessibilityElementInfo> treeInfos;
    verifyResult = ContainerSecurityVerify(data, treeInfoSize, treeInfos.max_size());
    if (!verifyResult || treeInfoSize < 0 || treeInfoSize > INT32_MAX) {
        return TRANSACTION_ERR;
    }
    for (int32_t i = 0; i < treeInfoSize; i++) {
        sptr<AccessibilityElementInfoParcel> treeInfo =
            data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
        if (treeInfo == nullptr) {
            HILOG_ERROR("ReadStrongParcelable<treeInfo> failed");
            return TRANSACTION_ERR;
        }
        treeInfos.emplace_back(*treeInfo);
    }

    SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, requestId);

    return NO_ERROR;
}

void AccessibilityElementOperatorCallbackStubTest::SetSearchElementInfoBySpecificPropertyResult(
    const std::list<AccessibilityElementInfo> &infos, const std::list<AccessibilityElementInfo> &treeInfos,
    const int32_t requestId)
{
    HILOG_DEBUG("infos size %{public}zu, treeInfos size %{public}zu, requestId %{public}d",
        infos.size(), treeInfos.size(), requestId);
}
} // namespace Accessibility
} // namespace OHOS