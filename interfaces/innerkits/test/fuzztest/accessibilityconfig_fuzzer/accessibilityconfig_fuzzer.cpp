/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "accessibilityconfig_fuzzer.h"
#include "accessibility_config.h"
#include "accesstoken_kit.h"
#include "nativetoken_kit.h"
#include "securec.h"
#include "token_setproc.h"

namespace OHOS {
namespace {
    constexpr size_t DATA_MIN_SIZE = 181;
    constexpr char END_CHAR = '\0';
    constexpr size_t LEN = 10;
    bool g_flag = true;
} // namespace
using namespace OHOS::Security::AccessToken;

class ConfigObserver : public OHOS::AccessibilityConfig::AccessibilityConfigObserver {
public:
    void OnConfigChanged(const OHOS::AccessibilityConfig::CONFIG_ID id,
        const OHOS::AccessibilityConfig::ConfigValue &value) override
    {
    }
};

class EnableAbilityListObserver : public OHOS::AccessibilityConfig::AccessibilityEnableAbilityListsObserver {
public:
    void OnEnableAbilityListsStateChanged() override
    {
    }
    void OnInstallAbilityListsStateChanged() override
    {
    }
};

template<class T>
size_t GetObject(T &object, const uint8_t *data, size_t size)
{
    size_t objectSize = sizeof(object);
    if (objectSize > size) {
        return 0;
    }
    return memcpy_s(&object, objectSize, data, objectSize) == EOK ? objectSize : 0;
}

void AddPermission()
{
    if (g_flag) {
        const char *perms[2];
        perms[0] = OHOS::Accessibility::OHOS_PERMISSION_READ_ACCESSIBILITY_CONFIG.c_str();
        perms[1] = OHOS::Accessibility::OHOS_PERMISSION_WRITE_ACCESSIBILITY_CONFIG.c_str();
        NativeTokenInfoParams infoInstance = {
            .dcapsNum = 0,
            .permsNum = 2,
            .aclsNum = 0,
            .dcaps = nullptr,
            .perms = perms,
            .acls = nullptr,
            .processName = "com.accessibility.config.fuzzer.test",
            .aplStr = "normal",
        };
        uint64_t tokenId = GetAccessTokenId(&infoInstance);
        SetSelfTokenID(tokenId);
        AccessTokenKit::ReloadNativeTokenInfo();
        g_flag = false;
    }
}

static size_t GenerateCaptionProperty(
    OHOS::AccessibilityConfig::CaptionProperty &property, const uint8_t* data, size_t size)
{
    size_t position = 0;
    uint32_t temp = 0;
    position += GetObject<uint32_t>(temp, &data[position], size - position);
    property.SetFontColor(temp);

    position += GetObject<uint32_t>(temp, &data[position], size - position);
    property.SetWindowColor(temp);

    position += GetObject<uint32_t>(temp, &data[position], size - position);
    property.SetBackgroundColor(temp);

    position += GetObject<uint32_t>(temp, &data[position], size - position);
    property.SetFontScale(static_cast<int32_t>(temp));

    char name[LEN + 1];
    name[LEN] = END_CHAR;
    for (size_t i = 0; i < LEN; i++) {
        position += GetObject<char>(name[i], &data[position], size - position);
    }
    std::string family(name);
    property.SetFontFamily(family);

    for (size_t i = 0; i < LEN; i++) {
        position += GetObject<char>(name[i], &data[position], size - position);
    }
    std::string type(name);
    property.SetFontEdgeType(type);

    return position;
}

void DoSomethingInterestingSetConfigFirstPart(OHOS::AccessibilityConfig::AccessibilityConfig& abConfig,
    const uint8_t* data, size_t size, size_t& startPos)
{
    abConfig.SetScreenMagnificationState(data[startPos++] & 0x01);
    abConfig.SetShortKeyState(data[startPos++] & 0x01);
    abConfig.SetMouseKeyState(data[startPos++] & 0x01);
    abConfig.SetCaptionsState(data[startPos++] & 0x01);
    abConfig.SetHighContrastTextState(data[startPos++] & 0x01);
    abConfig.SetDaltonizationState(data[startPos++] & 0x01);
    abConfig.SetInvertColorState(data[startPos++] & 0x01);
    abConfig.SetAnimationOffState(data[startPos++] & 0x01);
    abConfig.SetAudioMonoState(data[startPos++] & 0x01);

    uint32_t temp = 0;
    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.SetContentTimeout(temp);

    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.SetDaltonizationColorFilter(static_cast<OHOS::AccessibilityConfig::DALTONIZATION_TYPE>(temp));

    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.SetMouseAutoClick(static_cast<int32_t>(temp));

    float tempFloat = 0;
    startPos += GetObject<float>(tempFloat, &data[startPos], size - startPos);
    abConfig.SetBrightnessDiscount(tempFloat);

    startPos += GetObject<float>(tempFloat, &data[startPos], size - startPos);
    abConfig.SetAudioBalance(tempFloat);

    char name[LEN + 1];
    name[LEN] = END_CHAR;
    for (size_t i = 0; i < LEN; i++) {
        startPos += GetObject<char>(name[i], &data[startPos], size - startPos);
    }
    std::string nameStr(name);
    abConfig.SetShortkeyTarget(nameStr);

    std::vector<std::string> multiTargetName = {nameStr};
    abConfig.SetShortkeyMultiTarget(multiTargetName);

    OHOS::AccessibilityConfig::CaptionProperty property;
    startPos += GenerateCaptionProperty(property, &data[startPos], size - startPos);
    abConfig.SetCaptionsProperty(property);
}

void DoSomethingInterestingSetConfigSecondPart(OHOS::AccessibilityConfig::AccessibilityConfig& abConfig,
    const uint8_t* data, size_t size, size_t& startPos)
{
    uint32_t ResponseTime = 0;
    startPos += GetObject<uint32_t>(ResponseTime, &data[startPos], size - startPos);
    abConfig.SetClickResponseTime(static_cast<OHOS::AccessibilityConfig::CLICK_RESPONSE_TIME>(ResponseTime));

    bool state = false;
    startPos += GetObject<bool>(state, &data[startPos], size - startPos);
    abConfig.SetIgnoreRepeatClickState(state);

    uint32_t ignoreTime = 0;
    startPos += GetObject<uint32_t>(ignoreTime, &data[startPos], size - startPos);
    abConfig.SetIgnoreRepeatClickTime(static_cast<OHOS::AccessibilityConfig::IGNORE_REPEAT_CLICK_TIME>(ignoreTime));
}

void DoSomethingInterestingGetAbility(OHOS::AccessibilityConfig::AccessibilityConfig& abConfig,
    const uint8_t* data, size_t size, size_t& startPos)
{
    char name[LEN + 1];
    name[LEN] = END_CHAR;
    uint32_t temp = 0;
    std::shared_ptr<ConfigObserver> cObserver = std::make_shared<ConfigObserver>();
    std::shared_ptr<EnableAbilityListObserver> eObserver = std::make_shared<EnableAbilityListObserver>();

    for (size_t i = 0; i < LEN; i++) {
        startPos += GetObject<char>(name[i], &data[startPos], size - startPos);
    }
    std::string abilityName1(name);
    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.EnableAbility(abilityName1, temp);

    for (size_t i = 0; i < LEN; i++) {
        startPos += GetObject<char>(name[i], &data[startPos], size - startPos);
    }
    std::string abilityName2(name);
    abConfig.DisableAbility(abilityName2);

    bool flag = data[startPos++] & 0x01;
    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.SubscribeConfigObserver(static_cast<OHOS::AccessibilityConfig::CONFIG_ID>(temp), cObserver, flag);
    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.UnsubscribeConfigObserver(static_cast<OHOS::AccessibilityConfig::CONFIG_ID>(temp), cObserver);
    abConfig.SubscribeEnableAbilityListsObserver(eObserver);
    abConfig.UnsubscribeEnableAbilityListsObserver(eObserver);
}

static void CheckCaptionProperty(
    OHOS::AccessibilityConfig::CaptionProperty &captionProperty, const uint8_t* data, size_t size)
{
    size_t position = 0;
    char name[LEN + 1];
    name[LEN] = END_CHAR;
    for (size_t i = 0; i < LEN; i++) {
        position += GetObject<char>(name[i], &data[position], size - position);
    }
    std::string property(name);
    captionProperty.CheckProperty(property);

    captionProperty.GetBackgroundColor();
    captionProperty.GetFontScale();
    captionProperty.GetFontColor();
    captionProperty.GetFontEdgeType();
    captionProperty.GetWindowColor();
    captionProperty.GetFontFamily();
}

void DoSomethingInterestingGetConfigFirstPart(OHOS::AccessibilityConfig::AccessibilityConfig& abConfig,
    const uint8_t* data, size_t size, size_t& startPos)
{
    bool flag = data[startPos++] & 0x01;
    abConfig.GetScreenMagnificationState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetShortKeyState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetMouseKeyState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetCaptionsState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetHighContrastTextState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetDaltonizationState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetInvertColorState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetAnimationOffState(flag);
    flag = data[startPos++] & 0x01;
    abConfig.GetAudioMonoState(flag);

    uint32_t temp = 0;
    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    abConfig.GetContentTimeout(temp);

    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    OHOS::AccessibilityConfig::DALTONIZATION_TYPE types =
        static_cast<OHOS::AccessibilityConfig::DALTONIZATION_TYPE>(temp);
    abConfig.GetDaltonizationColorFilter(types);

    startPos += GetObject<uint32_t>(temp, &data[startPos], size - startPos);
    int32_t clicks = static_cast<int32_t>(temp);
    abConfig.GetMouseAutoClick(clicks);

    float tempFloat = 0;
    startPos += GetObject<float>(tempFloat, &data[startPos], size - startPos);
    abConfig.GetBrightnessDiscount(tempFloat);

    startPos += GetObject<float>(tempFloat, &data[startPos], size - startPos);
    abConfig.GetAudioBalance(tempFloat);

    OHOS::AccessibilityConfig::CaptionProperty propertyForGet;
    GenerateCaptionProperty(propertyForGet, &data[startPos], size - startPos);
    abConfig.GetCaptionsProperty(propertyForGet);
    CheckCaptionProperty(propertyForGet, data, size);
}

void DoSomethingInterestingGetConfigSecondPart(OHOS::AccessibilityConfig::AccessibilityConfig& abConfig,
    const uint8_t* data, size_t size, size_t& startPos)
{
    uint32_t tempUint32 = 0;
    startPos += GetObject<uint32_t>(tempUint32, &data[startPos], size - startPos);
    OHOS::AccessibilityConfig::CLICK_RESPONSE_TIME ResponseTime =
        static_cast<OHOS::AccessibilityConfig::CLICK_RESPONSE_TIME>(tempUint32);
    abConfig.GetClickResponseTime(ResponseTime);

    bool state = false;
    startPos += GetObject<bool>(state, &data[startPos], size - startPos);
    abConfig.GetIgnoreRepeatClickState(state);

    startPos += GetObject<uint32_t>(tempUint32, &data[startPos], size - startPos);
    OHOS::AccessibilityConfig::IGNORE_REPEAT_CLICK_TIME ignoreTime =
        static_cast<OHOS::AccessibilityConfig::IGNORE_REPEAT_CLICK_TIME>(tempUint32);
    abConfig.GetIgnoreRepeatClickTime(ignoreTime);

    char name[LEN + 1];
    name[LEN] = END_CHAR;
    for (size_t i = 0; i < LEN; i++) {
        startPos += GetObject<char>(name[i], &data[startPos], size - startPos);
    }
    std::string nameStrForGet(name);
    abConfig.GetShortkeyTarget(nameStrForGet);
    std::vector<std::string> multiTargetName = {nameStrForGet};
    abConfig.GetShortkeyMultiTarget(multiTargetName);
}

bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (data == nullptr || size < DATA_MIN_SIZE) {
        return false;
    }

    auto &abConfig = OHOS::AccessibilityConfig::AccessibilityConfig::GetInstance();
    (void)abConfig.InitializeContext();
    size_t startPos = 0;
    DoSomethingInterestingSetConfigFirstPart(abConfig, data, size, startPos);
    DoSomethingInterestingSetConfigSecondPart(abConfig, data, size, startPos);
    DoSomethingInterestingGetAbility(abConfig, data, size, startPos);
    DoSomethingInterestingGetConfigFirstPart(abConfig, data, size, startPos);
    DoSomethingInterestingGetConfigSecondPart(abConfig, data, size, startPos);
    abConfig.UnInitializeContext();

    return true;
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::AddPermission();
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
