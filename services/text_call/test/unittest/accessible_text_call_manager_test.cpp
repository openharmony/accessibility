/*
 * Copyright (C) 2026-2026 Huawei Device Co., Ltd.
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
#define private public
#define protected public
#include "accessible_text_call_manager.h"
#undef private
#undef protected
#include "hilog_wrapper.h"
 
using namespace testing;
using namespace testing::ext;
 
namespace OHOS {
namespace Accessibility {
 
class AccessibleTextCallManagerUnitTest : public ::testing::Test {
public:
    AccessibleTextCallManagerUnitTest()
    {}
    ~AccessibleTextCallManagerUnitTest()
    {}
 
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
 
private:
    void ResetManagerState();
};
 
void AccessibleTextCallManagerUnitTest::SetUpTestCase()
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest SetUpTestCase";
}
 
void AccessibleTextCallManagerUnitTest::TearDownTestCase()
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest TearDownTestCase";
}
 
void AccessibleTextCallManagerUnitTest::SetUp()
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest SetUp";
}
 
void AccessibleTextCallManagerUnitTest::TearDown()
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest TearDown";
    ResetManagerState();
}
 
void AccessibleTextCallManagerUnitTest::ResetManagerState()
{
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceConn_ = nullptr;
    manager.serviceState_ = ServiceState::DISCONNECTED;
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_ConstructDestructor_001
 * @tc.name: ConstructDestructor
 * @tc.desc: Test initial state after TearDown cleanup
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, ConstructDestructor_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_ConstructDestructor_001 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    EXPECT_EQ(manager.serviceState_, ServiceState::DISCONNECTED);
    EXPECT_EQ(manager.serviceConn_, nullptr);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_ConstructDestructor_001 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_SetServiceState_001
 * @tc.name: SetServiceState
 * @tc.desc: Test SetServiceState with false when serviceConn_ is nullptr
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, SetServiceState_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_SetServiceState_001 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceConn_ = nullptr;
    manager.serviceState_ = ServiceState::DISCONNECTED;
    manager.SetServiceState(false);
    EXPECT_EQ(manager.serviceState_, ServiceState::DISCONNECTED);
    EXPECT_EQ(manager.serviceConn_, nullptr);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_SetServiceState_001 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_OnServiceConnectDone_001
 * @tc.name: OnServiceConnectDone
 * @tc.desc: Test OnServiceConnectDone changes state to CONNECTED
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, OnServiceConnectDone_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_OnServiceConnectDone_001 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::CONNECTING;
    manager.OnServiceConnectDone();
    EXPECT_EQ(manager.serviceState_, ServiceState::CONNECTED);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_OnServiceConnectDone_001 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_OnServiceDisconnectDone_001
 * @tc.name: OnServiceDisconnectDone
 * @tc.desc: Test OnServiceDisconnectDone resets state to DISCONNECTED
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, OnServiceDisconnectDone_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_OnServiceDisconnectDone_001 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::CONNECTED;
    manager.serviceConn_ = new AICallServiceConnection();
    manager.OnServiceDisconnectDone();
    EXPECT_EQ(manager.serviceState_, ServiceState::DISCONNECTED);
    EXPECT_EQ(manager.serviceConn_, nullptr);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_OnServiceDisconnectDone_001 end";
}

#ifdef OHOS_BUILD_TEXT_CALL
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_SendRequest_001
 * @tc.name: SendRequest
 * @tc.desc: Test SendRequest when DISCONNECTED and serviceConn_ is nullptr
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, SendRequest_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_SendRequest_001 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::DISCONNECTED;
    manager.serviceConn_ = nullptr;
    bool ret = manager.SendRequest(Telephony::TelCallState::CALL_STATUS_IDLE);
    // ConnectAICallService may succeed or fail depending on environment
    EXPECT_FALSE(ret);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_SendRequest_001 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_SendRequest_002
 * @tc.name: SendRequest
 * @tc.desc: Test SendRequest when CONNECTED but serviceConn_ is nullptr
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, SendRequest_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_SendRequest_002 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::CONNECTED;
    manager.serviceConn_ = nullptr;
    bool ret = manager.SendRequest(Telephony::TelCallState::CALL_STATUS_IDLE);
    EXPECT_FALSE(ret);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_SendRequest_002 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_CallServiceListener_001
 * @tc.name: CallServiceListener
 * @tc.desc: Test CallServiceListener::OnCallDetailsChange with non-VOIP call type
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, CallServiceListener_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_CallServiceListener_001 start";
    CallServiceListener listener;
    Telephony::CallAttributeInfo info;
    info.callType = Telephony::CallType::TYPE_CS;
    info.callState = Telephony::TelCallState::CALL_STATUS_ACTIVE;
    int32_t ret = listener.OnCallDetailsChange(info);
    EXPECT_EQ(ret, ERR_OK);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_CallServiceListener_001 end";
}
#endif // OHOS_BUILD_TEXT_CALL
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_AICallServiceConnection_001
 * @tc.name: AICallServiceConnection
 * @tc.desc: Test AICallServiceConnection GetRemoteObject when remoteObject_ is nullptr
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, AICallServiceConnection_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_001 start";
    sptr<AICallServiceConnection> conn = new AICallServiceConnection();
    sptr<IRemoteObject> remote = conn->GetRemoteObject();
    EXPECT_EQ(remote, nullptr);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_001 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_AICallServiceConnection_002
 * @tc.name: AICallServiceConnection
 * @tc.desc: Test AICallServiceConnection OnAbilityConnectDone with failed resultCode
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, AICallServiceConnection_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_002 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::CONNECTING;
    sptr<AICallServiceConnection> conn = new AICallServiceConnection();
    AppExecFwk::ElementName element;
    conn->OnAbilityConnectDone(element, nullptr, -1);
    EXPECT_EQ(manager.serviceState_, ServiceState::DISCONNECTED);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_002 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_AICallServiceConnection_003
 * @tc.name: AICallServiceConnection
 * @tc.desc: Test AICallServiceConnection OnAbilityConnectDone with null remoteObject
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, AICallServiceConnection_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_003 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::CONNECTING;
    sptr<AICallServiceConnection> conn = new AICallServiceConnection();
    AppExecFwk::ElementName element;
    conn->OnAbilityConnectDone(element, nullptr, ERR_OK);
    EXPECT_EQ(manager.serviceState_, ServiceState::DISCONNECTED);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_003 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_AICallServiceConnection_004
 * @tc.name: AICallServiceConnection
 * @tc.desc: Test AICallServiceConnection OnAbilityDisconnectDone
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, AICallServiceConnection_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_004 start";
    auto& manager = Singleton<AccessibleTextCallManager>::GetInstance();
    manager.serviceState_ = ServiceState::CONNECTED;
    sptr<AICallServiceConnection> conn = new AICallServiceConnection();
    AppExecFwk::ElementName element;
    conn->OnAbilityDisconnectDone(element, ERR_OK);
    EXPECT_EQ(manager.serviceState_, ServiceState::DISCONNECTED);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_AICallServiceConnection_004 end";
}
 
/**
 * @tc.number: AccessibleTextCallManagerUnitTest_ServiceState_001
 * @tc.name: ServiceState
 * @tc.desc: Test ServiceState enum values
 */
HWTEST_F(AccessibleTextCallManagerUnitTest, ServiceState_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_ServiceState_001 start";
    EXPECT_EQ(static_cast<int32_t>(ServiceState::DISCONNECTED), 0);
    EXPECT_EQ(static_cast<int32_t>(ServiceState::CONNECTING), 1);
    EXPECT_EQ(static_cast<int32_t>(ServiceState::CONNECTED), 2);
    GTEST_LOG_(INFO) << "AccessibleTextCallManagerUnitTest_ServiceState_001 end";
}
} // namespace Accessibility
} // namespace OHOS