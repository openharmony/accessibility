/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_DATASHARE_HELPER
#define ACCESSIBILITY_DATASHARE_HELPER

#include "accessibility_setting_observer.h"
#include "accessibility_def.h"

#ifdef OHOS_BUILD_ENABLE_DATA_SHARE
#include "datashare_helper.h"
#endif
#include "ffrt.h"

namespace OHOS {
namespace Accessibility {

enum DATASHARE_TYPE : int32_t {
    GLOBAL,
    SYSTEM,
    SECURE,
};

class AccessibilityDatashareHelper {
public:
    AccessibilityDatashareHelper(DATASHARE_TYPE type, int32_t accountID);
    ~AccessibilityDatashareHelper();
    std::string GetStringValue(const std::string& key, const std::string& defaultValue,
        const bool readOnlyFlag = false);
    int32_t GetIntValue(const std::string& key, const int32_t& defaultValue, const bool readOnlyFlag = false);
    int64_t GetLongValue(const std::string& key, const int64_t& defaultValue, const bool readOnlyFlag = false);
    bool GetBoolValue(const std::string& key, const bool& defaultValue, const bool readOnlyFlag = false);
    float GetFloatValue(const std::string& key, const float& defaultValue, const bool readOnlyFlag = false);
    uint64_t GetUnsignedLongValue(const std::string& key, const uint64_t& defaultValue,
        const bool readOnlyFlag = false);

    RetError PutStringValue(const std::string& key, const std::string& value, bool needNotify = true);
    RetError PutIntValue(const std::string& key, int32_t value, bool needNotify = true);
    RetError PutLongValue(const std::string& key, int64_t value, bool needNotify = true);
    RetError PutBoolValue(const std::string& key, bool value, bool needNotify = true);
    RetError PutFloatValue(const std::string& key, float value, bool needNotify = true);
    RetError PutUnsignedLongValue(const std::string& key, uint64_t value, bool needNotify = true);

    RetError Initialize(int32_t systemAbilityId);

    sptr<AccessibilitySettingObserver> CreateObserver(const std::string& key,
        AccessibilitySettingObserver::UpdateFunc& func);
    RetError RegisterObserver(const sptr<AccessibilitySettingObserver>& observer);
    RetError UnregisterObserver(const sptr<AccessibilitySettingObserver>& observer);

    RetError RegisterObserver(const std::string& key, AccessibilitySettingObserver::UpdateFunc& func);
    RetError UnregisterObserver(const std::string& key);

private:
#ifdef OHOS_BUILD_ENABLE_DATA_SHARE
    std::shared_ptr<DataShare::DataShareHelper> CreateDatashareHelper();
    bool DestoryDatashareHelper(std::shared_ptr<DataShare::DataShareHelper>& helper);
#endif
    Uri AssembleUri(const std::string& key);

private:
    DATASHARE_TYPE type_;
    int32_t accountId_;
    std::string uriProxyStr_;
    sptr<IRemoteObject> remoteObj_ = nullptr;
#ifdef OHOS_BUILD_ENABLE_DATA_SHARE
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper_ = nullptr;
#endif
    static ffrt::mutex observerMutex_;
    std::map<std::string, sptr<AccessibilitySettingObserver>> settingObserverMap_;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_DATASHARE_HELPER
