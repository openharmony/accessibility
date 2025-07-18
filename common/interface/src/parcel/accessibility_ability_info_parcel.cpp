/*
 * Copyright (C) 2022-2025 Huawei Device Co., Ltd.
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

#include "accessibility_ability_info_parcel.h"
#include "hilog_wrapper.h"
#include "parcel_util.h"

namespace OHOS {
namespace Accessibility {
AccessibilityAbilityInfoParcel::AccessibilityAbilityInfoParcel(
    const AccessibilityAbilityInfo &accessibilityAbilityInfo)
    : AccessibilityAbilityInfo(accessibilityAbilityInfo)
{
}

bool AccessibilityAbilityInfoParcel::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, bundleName_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, moduleName_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, name_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, description_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, label_);

    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, capabilities_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, staticCapabilities_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, rationale_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, abilityTypes_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, eventTypes_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, settingsAbility_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(StringVector, parcel, &targetBundleNames_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, needHide_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(UInt32Vector, parcel, &eventConfigure_);

    return true;
}

bool AccessibilityAbilityInfoParcel::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, bundleName_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, moduleName_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, name_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, description_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, label_);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, capabilities_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, staticCapabilities_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, rationale_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, abilityTypes_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, eventTypes_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, settingsAbility_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(StringVector, parcel, targetBundleNames_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, needHide_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(UInt32Vector, parcel, eventConfigure_);

    return true;
}

AccessibilityAbilityInfoParcel *AccessibilityAbilityInfoParcel::Unmarshalling(Parcel &parcel)
{
    AccessibilityAbilityInfoParcel *info = new(std::nothrow) AccessibilityAbilityInfoParcel();
    if (info == nullptr) {
        HILOG_ERROR("Failed to create info.");
        return nullptr;
    }
    if (!info->ReadFromParcel(parcel)) {
        HILOG_ERROR("ReadFromParcel AccessibilityAbilityInfo failed.");
        delete info;
        info = nullptr;
        return nullptr;
    }
    return info;
}
} // namespace Accessibility
} // namespace OHOS