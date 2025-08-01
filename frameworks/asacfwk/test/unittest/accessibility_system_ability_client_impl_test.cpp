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

#include <gtest/gtest.h>
#include <memory>
#include "accessibility_common_helper.h"
#include "accessibility_system_ability_client_impl.h"
#include "mock_accessibility_element_operator.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr int32_t WINDOW_ID = 1;
    constexpr int32_t REQUEST_ID = 1;
    constexpr int64_t COMPONENT_ID = 1;
    constexpr int64_t ELEMENT_ID = 1;
    constexpr uint32_t WINDOW_ID_MASK = 16;
    constexpr int32_t GET_SOURCE_MODE = 0;
    constexpr int32_t ROOT_TREE_ID = 0;
} // namespace

class AccessibilitySystemAbilityClientImplTest : public ::testing::Test {
public:
    AccessibilitySystemAbilityClientImplTest()
    {}
    ~AccessibilitySystemAbilityClientImplTest()
    {}
    std::shared_ptr<AccessibilitySystemAbilityClientImpl> impl_ = nullptr;

    static void SetUpTestCase()
    {
        GTEST_LOG_(INFO) << "AccessibilitySystemAbilityClientImplTest Start";
    }
    static void TearDownTestCase()
    {
        GTEST_LOG_(INFO) << "AccessibilitySystemAbilityClientImplTest End";
    }
    void SetUp()
    {
        GTEST_LOG_(INFO) << "AccessibilitySystemAbilityClientImplTest SetUp()";
    };
    void TearDown()
    {
        GTEST_LOG_(INFO) << "AccessibilitySystemAbilityClientImplTest TearDown()";
    }
};


/**
 * @tc.number: RegisterElementOperator_001
 * @tc.name: RegisterElementOperator
 * @tc.desc: Test function RegisterElementOperator(parameter is null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, RegisterElementOperator_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "RegisterElementOperator_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->RegisterElementOperator(WINDOW_ID, nullptr));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "RegisterElementOperator_001 end";
}

/**
 * @tc.number: RegisterElementOperator_002
 * @tc.name: RegisterElementOperator
 * @tc.desc: Test function RegisterElementOperator(parameter is not null, proxy is null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, RegisterElementOperator_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "RegisterElementOperator_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(RET_ERR_SAMGR, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "RegisterElementOperator_002 end";
}

/**
 * @tc.number: RegisterElementOperator_003
 * @tc.name: RegisterElementOperator
 * @tc.desc: Test function RegisterElementOperator(register success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, RegisterElementOperator_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "RegisterElementOperator_003 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(RET_OK, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "RegisterElementOperator_003 end";
}

/**
 * @tc.number: RegisterElementOperator_004
 * @tc.name: RegisterElementOperator
 * @tc.desc: Test function RegisterElementOperator(register fail)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, RegisterElementOperator_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "RegisterElementOperator_004 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(RET_OK, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        EXPECT_EQ(RET_ERR_CONNECTION_EXIST, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "RegisterElementOperator_004 end";
}

/**
 * @tc.number: DeregisterElementOperator_001
 * @tc.name: DeregisterElementOperator
 * @tc.desc: Test function DeregisterElementOperator
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, DeregisterElementOperator_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "DeregisterElementOperator_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_SAMGR, impl_->DeregisterElementOperator(WINDOW_ID));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "DeregisterElementOperator_001 end";
}

/**
 * @tc.number: DeregisterElementOperator_002
 * @tc.name: DeregisterElementOperator
 * @tc.desc: Test function DeregisterElementOperator
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, DeregisterElementOperator_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "DeregisterElementOperator_002 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(RET_OK, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        EXPECT_EQ(RET_OK, impl_->DeregisterElementOperator(WINDOW_ID));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "DeregisterElementOperator_002 end";
}

/**
 * @tc.number: GetAbilityList_001
 * @tc.name: GetAbilityList
 * @tc.desc: Test function GetAbilityList
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetAbilityList_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetAbilityList_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::vector<AccessibilityAbilityInfo> infos {};
        auto ret = impl_->GetAbilityList(ACCESSIBILITY_ABILITY_TYPE_ALL, ABILITY_STATE_INVALID, infos);
        EXPECT_EQ(RET_ERR_INVALID_PARAM, ret);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetAbilityList_001 end";
}

/**
 * @tc.number: GetAbilityList_002
 * @tc.name: GetAbilityList
 * @tc.desc: Test function GetAbilityList(proxy is null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetAbilityList_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetAbilityList_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::vector<AccessibilityAbilityInfo> infos {};
        auto ret = impl_->GetAbilityList(ACCESSIBILITY_ABILITY_TYPE_ALL, ABILITY_STATE_ENABLE, infos);
        EXPECT_EQ(RET_ERR_SAMGR, ret);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetAbilityList_002 end";
}

/**
 * @tc.number: GetAbilityList_003
 * @tc.name: GetAbilityList
 * @tc.desc: Test function GetAbilityList(proxy is not null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetAbilityList_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetAbilityList_003 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::vector<AccessibilityAbilityInfo> infos {};
        auto ret = impl_->GetAbilityList(ACCESSIBILITY_ABILITY_TYPE_ALL, ABILITY_STATE_ENABLE, infos);
        EXPECT_EQ(RET_OK, ret);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetAbilityList_003 end";
}

/**
 * @tc.number: IsEnabled_001
 * @tc.name: IsEnabled
 * @tc.desc: Test function IsEnabled
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, IsEnabled_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "IsEnabled_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        bool isEnable = false;
        impl_->IsEnabled(isEnable);
        EXPECT_FALSE(isEnable);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "IsEnabled_001 end";
}

/**
 * @tc.number: IsTouchExplorationEnabled_001
 * @tc.name: IsTouchExplorationEnabled
 * @tc.desc: Test function IsTouchExplorationEnabled
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, IsTouchExplorationEnabled_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "IsTouchExplorationEnabled_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        bool isEnable = false;
        impl_->IsTouchExplorationEnabled(isEnable);
        EXPECT_FALSE(isEnable);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "IsTouchExplorationEnabled_001 end";
}

/**
 * @tc.number: IsScreenReaderEnabled_001
 * @tc.name: IsScreenReaderEnabled
 * @tc.desc: Test function IsScreenReaderEnabled
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, IsScreenReaderEnabled_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "IsScreenReaderEnabled_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    ASSERT_TRUE(impl_);
    bool isEnable = false;
    impl_->IsScreenReaderEnabled(isEnable);
    EXPECT_FALSE(isEnable);
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "IsScreenReaderEnabled_001 end";
}

/**
 * @tc.number: SendEvent_001
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(proxy is null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_SAMGR, impl_->SendEvent(TYPE_VIEW_LONG_CLICKED_EVENT, COMPONENT_ID));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_001 end";
}

/**
 * @tc.number: SendEvent_002
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(invalid type)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->SendEvent(TYPE_VIEW_INVALID, COMPONENT_ID));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_002 end";
}

/**
 * @tc.number: SendEvent_003
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(proxy is not null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_003 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_OK, impl_->SendEvent(TYPE_VIEW_LONG_CLICKED_EVENT, COMPONENT_ID));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_003 end";
}

/**
 * @tc.number: SendEvent_004
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(proxy is null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_004 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        AccessibilityEventInfo event {};
        event.SetEventType(TYPE_VIEW_LONG_CLICKED_EVENT);
        EXPECT_EQ(RET_ERR_SAMGR, impl_->SendEvent(event));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_004 end";
}

/**
 * @tc.number: SendEvent_005
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(invalid type)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_005 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        AccessibilityEventInfo event {};
        event.SetEventType(TYPE_VIEW_INVALID);
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->SendEvent(event));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_005 end";
}

/**
 * @tc.number: SendEvent_006
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(proxy is not null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_006, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_006 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        AccessibilityEventInfo event {};
        event.SetEventType(TYPE_VIEW_LONG_CLICKED_EVENT);
        EXPECT_EQ(RET_OK, impl_->SendEvent(event));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_006 end";
}

/**
 * @tc.number: SendEvent_007
 * @tc.name: SendEvent
 * @tc.desc: Test function SendEvent(proxy is null)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SendEvent_007, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SendEvent_007 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        AccessibilityEventInfo event {};
        event.SetEventType(TYPE_VIEW_FOCUSED_EVENT);
        EXPECT_EQ(RET_ERR_SAMGR, impl_->SendEvent(event));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SendEvent_007 end";
}

/**
 * @tc.number: SubscribeStateObserver_001
 * @tc.name: SubscribeStateObserver
 * @tc.desc: Test function SubscribeStateObserver
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SubscribeStateObserver_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SubscribeStateObserver_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->SubscribeStateObserver(nullptr, EVENT_TOUCH_GUIDE_STATE_CHANGED));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SubscribeStateObserver_001 end";
}

/**
 * @tc.number: SubscribeStateObserver_002
 * @tc.name: SubscribeStateObserver
 * @tc.desc: Test function SubscribeStateObserver
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SubscribeStateObserver_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SubscribeStateObserver_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->SubscribeStateObserver(nullptr, EVENT_TYPE_MAX));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SubscribeStateObserver_002 end";
}

/**
 * @tc.number: UnsubscribeStateObserver_001
 * @tc.name: UnsubscribeStateObserver
 * @tc.desc: Test function UnsubscribeStateObserver(invalid parameter)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, UnsubscribeStateObserver_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "UnsubscribeStateObserver_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->UnsubscribeStateObserver(nullptr, EVENT_TYPE_MAX));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "UnsubscribeStateObserver_001 end";
}

/**
 * @tc.number: UnsubscribeStateObserver_002
 * @tc.name: UnsubscribeStateObserver
 * @tc.desc: Test function UnsubscribeStateObserver(invalid parameter)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, UnsubscribeStateObserver_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "UnsubscribeStateObserver_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->UnsubscribeStateObserver(nullptr, EVENT_KEVEVENT_STATE_CHANGED));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "UnsubscribeStateObserver_002 end";
}

/**
 * @tc.number: ResetService_001
 * @tc.name: ResetService
 * @tc.desc: Test function ResetService
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, ResetService_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "ResetService_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        impl_->ResetService(nullptr);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "ResetService_001 end";
}

/**
 * @tc.number: ResetService_002
 * @tc.name: ResetService
 * @tc.desc: Test function ResetService
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, ResetService_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "ResetService_002 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        wptr<IRemoteObject> object;
        impl_->ResetService(object);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "ResetService_002 end";
}

/**
 * @tc.number: OnAccessibleAbilityManagerStateChanged_001
 * @tc.name: OnAccessibleAbilityManagerStateChanged
 * @tc.desc: Test function OnAccessibleAbilityManagerStateChanged
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, OnAccessibleAbilityManagerStateChanged_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "OnAccessibleAbilityManagerStateChanged_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        uint32_t stateType = STATE_ACCESSIBILITY_ENABLED | STATE_EXPLORATION_ENABLED
            | STATE_KEYEVENT_ENABLED | STATE_GESTURE_ENABLED;
        impl_->OnAccessibleAbilityManagerStateChanged(stateType);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "OnAccessibleAbilityManagerStateChanged_001 end";
}

/**
 * @tc.number: OnAccessibleAbilityManagerStateChanged_002
 * @tc.name: OnAccessibleAbilityManagerStateChanged
 * @tc.desc: Test function OnAccessibleAbilityManagerStateChanged
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, OnAccessibleAbilityManagerStateChanged_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "OnAccessibleAbilityManagerStateChanged_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        impl_->OnAccessibleAbilityManagerStateChanged(0);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "OnAccessibleAbilityManagerStateChanged_002 end";
}

/**
 * @tc.number: GetEnabledAbilities_001
 * @tc.name: GetEnabledAbilities
 * @tc.desc: Test function GetEnabledAbilities
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetEnabledAbilities_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetEnabledAbilities_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::vector<std::string> enabledAbilities;
        EXPECT_EQ(RET_ERR_SAMGR, impl_->GetEnabledAbilities(enabledAbilities));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetEnabledAbilities_001 end";
}

/**
 * @tc.number: GetEnabledAbilities_002
 * @tc.name: GetEnabledAbilities
 * @tc.desc: Test function GetEnabledAbilities
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetEnabledAbilities_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetEnabledAbilities_002 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::vector<std::string> enabledAbilities;
        EXPECT_EQ(RET_OK, impl_->GetEnabledAbilities(enabledAbilities));
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetEnabledAbilities_002 end";
}

/**
 * @tc.number: SetSearchElementInfoByAccessibilityIdResult_001
 * @tc.name: SetSearchElementInfoByAccessibilityIdResult
 * @tc.desc: Test function SetSearchElementInfoByAccessibilityIdResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoByAccessibilityIdResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001"
            "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchElementInfoByAccessibilityIdResult(infos, requestId);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoByAccessibilityIdResult_002
 * @tc.name: SetSearchElementInfoByAccessibilityIdResult
 * @tc.desc: Test function SetSearchElementInfoByAccessibilityIdResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoByAccessibilityIdResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchElementInfoByAccessibilityIdResult(infos, requestId);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_002 end";
}

/**
 * @tc.number: SetSearchElementInfoByAccessibilityIdResult_003
 * @tc.name: SetSearchElementInfoByAccessibilityIdResult
 * @tc.desc: Test function SetSearchElementInfoByAccessibilityIdResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoByAccessibilityIdResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchElementInfoByAccessibilityIdResult(infos, -1);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_003 end";
}

/**
 * @tc.number: SetSearchDefaultFocusByWindowIdResult_001
 * @tc.name: SetSearchDefaultFocusByWindowIdResult
 * @tc.desc: Test function SetSearchDefaultFocusByWindowIdResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchDefaultFocusByWindowIdResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_001"
            "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchDefaultFocusByWindowIdResult(infos, requestId);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_001 end";
}

 /**
 * @tc.number: SetSearchDefaultFocusByWindowIdResult_002
 * @tc.name: SetSearchDefaultFocusByWindowIdResult
 * @tc.desc: Test function SetSearchDefaultFocusByWindowIdResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchDefaultFocusByWindowIdResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchDefaultFocusByWindowIdResult(infos, requestId);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_002 end";
}

/**
 * @tc.number: SetSearchDefaultFocusByWindowIdResult_003
 * @tc.name: SetSearchDefaultFocusByWindowIdResult
 * @tc.desc: Test function SetSearchDefaultFocusByWindowIdResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchDefaultFocusByWindowIdResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchDefaultFocusByWindowIdResult(infos, -1);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchDefaultFocusByWindowIdResult_003 end";
}

/**
 * @tc.number: SetSearchElementInfoByTextResult_001
 * @tc.name: SetSearchElementInfoByTextResult
 * @tc.desc: Test function SetSearchElementInfoByTextResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoByTextResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetSearchElementInfoByTextResult_001 impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchElementInfoByTextResult(infos, requestId);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoByTextResult_002
 * @tc.name: SetSearchElementInfoByTextResult
 * @tc.desc: Test function SetSearchElementInfoByTextResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoByTextResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchElementInfoByTextResult(infos, requestId);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_002 end";
}

/**
 * @tc.number: SetSearchElementInfoByTextResult_003
 * @tc.name: SetSearchElementInfoByTextResult
 * @tc.desc: Test function SetSearchElementInfoByTextResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoByTextResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::list<AccessibilityElementInfo> infos;
        impl_->SetSearchElementInfoByTextResult(infos, -1);
        EXPECT_EQ(0, infos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_003 end";
}

/**
 * @tc.number: SetFindFocusedElementInfoResult_001
 * @tc.name: SetFindFocusedElementInfoResult
 * @tc.desc: Test function SetFindFocusedElementInfoResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindFocusedElementInfoResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001"
            "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        AccessibilityElementInfo info;
        impl_->SetFindFocusedElementInfoResult(info, requestId);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001 end";
}

/**
 * @tc.number: SetFindFocusedElementInfoResult_002
 * @tc.name: SetFindFocusedElementInfoResult
 * @tc.desc: Test function SetFindFocusedElementInfoResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindFocusedElementInfoResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        AccessibilityElementInfo info;
        impl_->SetFindFocusedElementInfoResult(info, requestId);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_002 end";
}

/**
 * @tc.number: SetFindFocusedElementInfoResult_003
 * @tc.name: SetFindFocusedElementInfoResult
 * @tc.desc: Test function SetFindFocusedElementInfoResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindFocusedElementInfoResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        AccessibilityElementInfo info;
        impl_->SetFindFocusedElementInfoResult(info, -1);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_003 end";
}

/**
 * @tc.number: SetFocusMoveSearchResult_001
 * @tc.name: SetFocusMoveSearchResult
 * @tc.desc: Test function SetFocusMoveSearchResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFocusMoveSearchResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        AccessibilityElementInfo info;
        impl_->SetFocusMoveSearchResult(info, requestId);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 end";
}

/**
 * @tc.number: SetFocusMoveSearchResult_002
 * @tc.name: SetFocusMoveSearchResult
 * @tc.desc: Test function SetFocusMoveSearchResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFocusMoveSearchResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        AccessibilityElementInfo info;
        impl_->SetFocusMoveSearchResult(info, requestId);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_002 end";
}

/**
 * @tc.number: SetFocusMoveSearchResult_003
 * @tc.name: SetFocusMoveSearchResult
 * @tc.desc: Test function SetFocusMoveSearchResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFocusMoveSearchResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        AccessibilityElementInfo info;
        impl_->SetFocusMoveSearchResult(info, -1);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_003 end";
}

/**
 * @tc.number: SetExecuteActionResult_001
 * @tc.name: SetExecuteActionResult
 * @tc.desc: Test function SetExecuteActionResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetExecuteActionResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetExecuteActionResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "SetExecuteActionResult_001 Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        impl_->SetExecuteActionResult(true, requestId);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetExecuteActionResult_001 end";
}

/**
 * @tc.number: SetExecuteActionResult_002
 * @tc.name: SetExecuteActionResult
 * @tc.desc: Test function SetExecuteActionResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetExecuteActionResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetExecuteActionResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        impl_->SetExecuteActionResult(true, requestId);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetExecuteActionResult_002 end";
}

/**
 * @tc.number: SetExecuteActionResult_003
 * @tc.name: SetExecuteActionResult
 * @tc.desc: Test function SetExecuteActionResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetExecuteActionResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetExecuteActionResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        impl_->SetExecuteActionResult(true, -1);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetExecuteActionResult_003 end";
}

/**
 * @tc.number: SetCursorPositionResult_001
 * @tc.name: SetCursorPositionResult
 * @tc.desc: Test function SetCursorPositionResult(cursorPosition,requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetCursorPositionResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetCursorPositionResult_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t cursorPosition = 0;
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        impl_->SetCursorPositionResult(cursorPosition, requestId);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetCursorPositionResult_001 end";
}

/**
 * @tc.number: SetCursorPositionResult_002
 * @tc.name: SetCursorPositionResult
 * @tc.desc: Test function SetCursorPositionResult(cursorPosition,requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetCursorPositionResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetCursorPositionResult_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t cursorPosition = 0;
        impl_->SetCursorPositionResult(cursorPosition, -1);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetCursorPositionResult_001 end";
}

/**
 * @tc.number: SetAccessibilityState_001
 * @tc.name: SetAccessibilityState
 * @tc.desc: Test function SetAccessibilityState
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetAccessibilityState_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetAccessibilityState_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetAccessibilityState impl_";
    } else {
        impl_->SetAccessibilityState(0);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetAccessibilityState_001 end";
}

/**
 * @tc.number: GetAccessibilityState_001
 * @tc.name: GetAccessibilityState
 * @tc.desc: Test function GetAccessibilityState
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetAccessibilityState_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetAccessibilityState_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetAccessibilityState impl_";
    } else {
        impl_->SetAccessibilityState(0);
        EXPECT_EQ(0, impl_->GetAccessibilityState());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetAccessibilityState_001 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfoResult_001
 * @tc.name: SetFindAccessibilityNodeInfoResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfoResult(elementInfo,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfoResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetFindAccessibilityNodeInfoResult impl_";
    } else {
        AccessibilityElementInfo elementInfo;
        int32_t requestId = -1;
        int32_t requestCode = FIND_ACCESSIBILITY_NODE_BY_ACCESSIBILITY_ID;
        impl_->SetFindAccessibilityNodeInfoResult(elementInfo, requestId, requestCode);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_001 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfoResult_002
 * @tc.name: SetFindAccessibilityNodeInfoResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfoResult(elementInfo,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfoResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetFindAccessibilityNodeInfoResult impl_";
    } else {
        AccessibilityElementInfo elementInfo;
        int32_t requestId = -1;
        int32_t requestCode = FIND_FOCUS;
        impl_->SetFindAccessibilityNodeInfoResult(elementInfo, requestId, requestCode);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_002 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfoResult_003
 * @tc.name: SetFindAccessibilityNodeInfoResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfoResult(elementInfo,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfoResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetFindAccessibilityNodeInfoResult impl_";
    } else {
        AccessibilityElementInfo elementInfo;
        int32_t requestId = -1;
        int32_t requestCode = FIND_FOCUS_SEARCH;
        impl_->SetFindAccessibilityNodeInfoResult(elementInfo, requestId, requestCode);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_003 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfoResult_004
 * @tc.name: SetFindAccessibilityNodeInfoResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfoResult(elementInfo,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfoResult_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_004 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetFindAccessibilityNodeInfoResult impl_";
    } else {
        AccessibilityElementInfo elementInfo;
        int32_t requestId = -1;
        int32_t requestCode = FIND_ACCESSIBILITY_NODE_BY_TEXT;
        impl_->SetFindAccessibilityNodeInfoResult(elementInfo, requestId, requestCode);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfoResult_004 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfosResult_001
 * @tc.name: SetFindAccessibilityNodeInfosResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfosResult(elementInfos,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfosResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfosResult_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetFindAccessibilityNodeInfosResult_001 impl_";
    } else {
        std::list<AccessibilityElementInfo> elementInfos;
        int32_t requestId = -1;
        int32_t requestCode = FIND_ACCESSIBILITY_NODE_BY_ACCESSIBILITY_ID;
        impl_->SetFindAccessibilityNodeInfosResult(elementInfos, requestId, requestCode);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfosResult_001 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfosResult_002
 * @tc.name: SetFindAccessibilityNodeInfosResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfosResult(elementInfos,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfosResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfosResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get SetFindAccessibilityNodeInfosResult_002 impl_";
    } else {
        std::list<AccessibilityElementInfo> elementInfos;
        int32_t requestId = -1;
        int32_t requestCode = FIND_ACCESSIBILITY_NODE_BY_TEXT;
        impl_->SetFindAccessibilityNodeInfosResult(elementInfos, requestId, requestCode);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfosResult_002 end";
}

/**
 * @tc.number: SetFindAccessibilityNodeInfosResult_003
 * @tc.name: SetFindAccessibilityNodeInfosResult
 * @tc.desc: Test function SetFindAccessibilityNodeInfosResult(elementInfos,requestId,requestCode)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetFindAccessibilityNodeInfosResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfosResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    ASSERT_TRUE(impl_);
    std::list<AccessibilityElementInfo> elementInfos;
    int32_t requestId = -1;
    int32_t requestCode = FIND_FOCUS_SEARCH;
    impl_->SetFindAccessibilityNodeInfosResult(elementInfos, requestId, requestCode);
    EXPECT_NE(impl_.get(), nullptr);
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetFindAccessibilityNodeInfosResult_003 end";
}

/**
 * @tc.number: GetFocusedWindowId_001
 * @tc.name: GetFocusedWindowId
 * @tc.desc: Test function GetFocusedWindowId
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetFocusedWindowId_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetFocusedWindowId_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t windowId = 0;
        impl_->GetFocusedWindowId(windowId);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetFocusedWindowId_001 end";
}

/**
 * @tc.number: LoadAccessibilityService_001
 * @tc.name: LoadAccessibilityService
 * @tc.desc: Test function LoadAccessibilityService (false)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, LoadAccessibilityService_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "LoadAccessibilityService_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EXPECT_EQ(false, impl_->LoadAccessibilityService());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "LoadAccessibilityService_001 end";
}

/**
 * @tc.number: LoadSystemAbilitySuccess_001
 * @tc.name: LoadSystemAbilitySuccess
 * @tc.desc: Test function LoadSystemAbilitySuccess
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, LoadSystemAbilitySuccess_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "LoadSystemAbilitySuccess_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        sptr<IRemoteObject> object;
        impl_->LoadSystemAbilitySuccess(object);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "LoadSystemAbilitySuccess_001 end";
}

/**
 * @tc.number: LoadSystemAbilitySuccess_002
 * @tc.name: LoadSystemAbilitySuccess
 * @tc.desc: Test function LoadSystemAbilitySuccess
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, LoadSystemAbilitySuccess_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "LoadSystemAbilitySuccess_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        impl_->LoadSystemAbilitySuccess(nullptr);
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "LoadSystemAbilitySuccess_002 end";
}

/**
 * @tc.number: LoadSystemAbilityFail_001
 * @tc.name: LoadSystemAbilityFail
 * @tc.desc: Test function LoadSystemAbilityFail
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, LoadSystemAbilityFail_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "LoadSystemAbilityFail_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        impl_->LoadSystemAbilityFail();
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "LoadSystemAbilityFail_001 end";
}

/**
 * @tc.number: Init_001
 * @tc.name: Init
 * @tc.desc: Test function Init AccessibilitySystemAbilityClientImpl
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, Init_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Init_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        impl_->LoadSystemAbilityFail();
        impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
        EXPECT_NE(impl_.get(), nullptr);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "Init_001 end";
}

/**
 * @tc.number: CheckEventType_001
 * @tc.name: CheckEventType
 * @tc.desc: Test function CheckEventType SendEvent (false: TYPE_VIEW_INVALID)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, CheckEventType_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "CheckEventType_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EventType eventType = TYPE_VIEW_INVALID;
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->SendEvent(eventType, 0));
        impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "CheckEventType_001 end";
}

/**
 * @tc.number: CheckEventType_002
 * @tc.name: CheckEventType
 * @tc.desc: Test function CheckEventType SendEvent (false: TYPE_MAX_NUM)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, CheckEventType_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "CheckEventType_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EventType eventType = TYPE_MAX_NUM;
        EXPECT_EQ(RET_ERR_INVALID_PARAM, impl_->SendEvent(eventType, 0));
        impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "CheckEventType_002 end";
}

/**
 * @tc.number: CheckEventType_003
 * @tc.name: CheckEventType
 * @tc.desc: Test function CheckEventType SendEvent (false: TYPE_VIEW_FOCUSED_EVENT)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, CheckEventType_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "CheckEventType_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EventType eventType = TYPE_VIEW_FOCUSED_EVENT;
        EXPECT_EQ(RET_ERR_SAMGR, impl_->SendEvent(eventType, 0));
        impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "CheckEventType_003 end";
}

/**
 * @tc.number: CheckEventType_004
 * @tc.name: CheckEventType
 * @tc.desc: Test function CheckEventType SendEvent (false: TYPE_PAGE_STATE_UPDATE)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, CheckEventType_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "CheckEventType_004 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EventType eventType = TYPE_PAGE_STATE_UPDATE;
        EXPECT_EQ(RET_ERR_SAMGR, impl_->SendEvent(eventType, 0));
        impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "CheckEventType_004 end";
}

/**
 * @tc.number: CheckEventType_005
 * @tc.name: CheckEventType
 * @tc.desc: Test function CheckEventType SendEvent (false: TYPE_VIEW_SCROLLED_EVENT)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, CheckEventType_005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "CheckEventType_005 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        EventType eventType = TYPE_VIEW_SCROLLED_EVENT;
        EXPECT_EQ(RET_ERR_SAMGR, impl_->SendEvent(eventType, 0));
        impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "CheckEventType_005 end";
}

/**
 * @tc.number: SetSplicElementIdTreeId_001
 * @tc.name: SetSplicElementIdTreeId
 * @tc.desc: Test function SetSplicElementIdTreeId
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSplicElementIdTreeId_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSplicElementIdTreeId_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int64_t elementId = 0;
        impl_->SetSplicElementIdTreeId(1, elementId);
        EXPECT_NE(elementId, 0);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSplicElementIdTreeId_001 end";
}

/**
 * @tc.number: GetTreeIdAndElementIdBySplitElementId_001
 * @tc.name: GetTreeIdAndElementIdBySplitElementId
 * @tc.desc: Test function GetTreeIdAndElementIdBySplitElementId
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, GetTreeIdAndElementIdBySplitElementId_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetTreeIdAndElementIdBySplitElementId_001 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int64_t splitElementId;
        int32_t splitTreeId;
        impl_->GetTreeIdAndElementIdBySplitElementId(-1, splitElementId, splitTreeId);
        EXPECT_EQ(splitElementId, -1);
        EXPECT_EQ(splitTreeId, -1);
        impl_->GetTreeIdAndElementIdBySplitElementId(1, splitElementId, splitTreeId);
        EXPECT_EQ(splitElementId, 1);
        EXPECT_EQ(splitTreeId, 0);
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "GetTreeIdAndElementIdBySplitElementId_001 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_001
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult(success)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoBySpecificPropertyResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_001 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    if (!impl_) {
        GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_001"
            "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::shared_ptr<AccessibilityElementOperator> mockOperator
            = std::make_shared<MockAccessibilityElementOperator>();
        EXPECT_EQ(0, impl_->RegisterElementOperator(WINDOW_ID, mockOperator));
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        std::list<AccessibilityElementInfo> treeInfos;
        impl_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, requestId);
        EXPECT_EQ(0, infos.size());
        EXPECT_EQ(0, treeInfos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_002
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult(no operator)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoBySpecificPropertyResult_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_002 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        int32_t requestId = REQUEST_ID;
        requestId |= static_cast<uint32_t>(WINDOW_ID) << WINDOW_ID_MASK;
        std::list<AccessibilityElementInfo> infos;
        std::list<AccessibilityElementInfo> treeInfos;
        impl_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, requestId);
        EXPECT_EQ(0, infos.size());
        EXPECT_EQ(0, treeInfos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_002 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_003
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult(invalid requestId)
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoBySpecificPropertyResult_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_003 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    if (!impl_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilitySystemAbilityClientImpl impl_";
    } else {
        std::list<AccessibilityElementInfo> infos;
        std::list<AccessibilityElementInfo> treeInfos;
        impl_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, -1);
        EXPECT_EQ(0, infos.size());
        EXPECT_EQ(0, treeInfos.size());
    }
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_003 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_004
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult with serviceProxy_ is nullptr
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoBySpecificPropertyResult_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_004 start";
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    ASSERT_TRUE(impl_);
    std::list<AccessibilityElementInfo> infos;
    std::list<AccessibilityElementInfo> treeInfos;
    int32_t requestId = REQUEST_ID;
    impl_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, requestId);
    EXPECT_EQ(0, infos.size());
    EXPECT_EQ(0, treeInfos.size());
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_004 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_005
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult with callback nullptr
 */
HWTEST_F(AccessibilitySystemAbilityClientImplTest, SetSearchElementInfoBySpecificPropertyResult_005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_005 start";
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(true);
    impl_ = std::make_shared<AccessibilitySystemAbilityClientImpl>();
    AccessibilityCommonHelper::GetInstance().SetRemoteObjectNotNullFlag(false);
    ASSERT_TRUE(impl_);
    std::list<AccessibilityElementInfo> infos;
    std::list<AccessibilityElementInfo> treeInfos;
    AccessibilityElementInfo info;
    infos.push_back(info);
    int32_t requestId = 99999;
    impl_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, requestId);
    EXPECT_EQ(1, infos.size());
    EXPECT_EQ(0, treeInfos.size());
    impl_ = nullptr;
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_005 end";
}
} // namespace Accessibility
} // namespace OHOS