/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "accessible_ability_client_stub.h"
#include "parcel.h"

using namespace OHOS::Accessibility;

AccessibleAbilityClientStub::AccessibleAbilityClientStub()
{}

AccessibleAbilityClientStub::~AccessibleAbilityClientStub()
{}

int AccessibleAbilityClientStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
                                                        MessageOption &option)
{
    return 0;
}

ErrCode AccessibleAbilityClientStub::HandleInit(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibleAbilityClientStub::HandleDisconnect(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibleAbilityClientStub::HandleOnAccessibilityEvent(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibleAbilityClientStub::HandleOnKeyPressEvent(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibleAbilityClientStub::HandleOnDisplayResized(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibleAbilityClientStub::HandleOnGestureSimulateResult(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}