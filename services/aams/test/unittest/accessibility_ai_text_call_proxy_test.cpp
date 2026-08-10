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
#include <dlfcn.h>
#define private public
#define protected public
#include "accessibility_ai_text_call_proxy.h"
#undef private
#undef protected
#include "hilog_wrapper.h"
 
using namespace testing;
using namespace testing::ext;
 
namespace OHOS {
namespace Accessibility {
namespace {
    const std::string_view AI_TEXT_CALL_LIBRARY = "libaccessibleTextCall.z.so";
}
 
class AccessibilityAITextCallProxyUnitTest : public ::testing::Test {
public:
    AccessibilityAITextCallProxyUnitTest()
    {}
    ~AccessibilityAITextCallProxyUnitTest()
    {}
 
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
 
private:
    void ResetProxyState();
};
 
void AccessibilityAITextCallProxyUnitTest::SetUpTestCase()
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest SetUpTestCase";
}
 
void AccessibilityAITextCallProxyUnitTest::TearDownTestCase()
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest TearDownTestCase";
}
 
void AccessibilityAITextCallProxyUnitTest::SetUp()
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest SetUp";
}
 
void AccessibilityAITextCallProxyUnitTest::TearDown()
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest TearDown";
    ResetProxyState();
}
 
void AccessibilityAITextCallProxyUnitTest::ResetProxyState()
{
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    if (proxy.handle_ != nullptr) {
        dlclose(proxy.handle_);
        proxy.handle_ = nullptr;
    }
    proxy.func_ = nullptr;
    proxy.loadFlag_ = false;
}
 
/**
 * @tc.number: AccessibilityAITextCallProxyUnitTest_SetServiceState_001
 * @tc.name: SetServiceState
 * @tc.desc: Test SetServiceState with true, verify state after call
 */
HWTEST_F(AccessibilityAITextCallProxyUnitTest, SetServiceState_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_SetServiceState_001 start";
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    proxy.SetServiceState(true);
    // If library exists, loadFlag_ will be true; otherwise false. Both are valid outcomes.
    if (proxy.loadFlag_) {
        EXPECT_NE(proxy.handle_, nullptr);
        EXPECT_NE(proxy.func_, nullptr);
    } else {
        EXPECT_EQ(proxy.handle_, nullptr);
    }
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_SetServiceState_001 end";
}
 
/**
 * @tc.number: AccessibilityAITextCallProxyUnitTest_SetServiceState_002
 * @tc.name: SetServiceState
 * @tc.desc: Test SetServiceState with false, should unload library and reset state
 */
HWTEST_F(AccessibilityAITextCallProxyUnitTest, SetServiceState_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_SetServiceState_002 start";
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    // First enable to potentially load the library
    proxy.SetServiceState(true);
    // Then disable, which should unload and reset
    proxy.SetServiceState(false);
    EXPECT_FALSE(proxy.loadFlag_);
    EXPECT_EQ(proxy.handle_, nullptr);
    EXPECT_EQ(proxy.func_, nullptr);
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_SetServiceState_002 end";
}
 
/**
 * @tc.number: AccessibilityAITextCallProxyUnitTest_LoadLibrary_001
 * @tc.name: LoadLibrary
 * @tc.desc: Test LoadLibrary, verify state consistency after call
 */
HWTEST_F(AccessibilityAITextCallProxyUnitTest, LoadLibrary_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_LoadLibrary_001 start";
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    proxy.LoadLibrary();
    // If library exists, loadFlag_ will be true with valid handle and func
    if (proxy.loadFlag_) {
        EXPECT_NE(proxy.handle_, nullptr);
        EXPECT_NE(proxy.func_, nullptr);
    } else {
        EXPECT_EQ(proxy.handle_, nullptr);
    }
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_LoadLibrary_001 end";
}
 
/**
 * @tc.number: AccessibilityAITextCallProxyUnitTest_UnLoadLibrary_001
 * @tc.name: UnLoadLibrary
 * @tc.desc: Test UnLoadLibrary when handle is nullptr, should not crash
 */
HWTEST_F(AccessibilityAITextCallProxyUnitTest, UnLoadLibrary_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_UnLoadLibrary_001 start";
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    proxy.handle_ = nullptr;
    proxy.func_ = nullptr;
    proxy.loadFlag_ = false;
    proxy.UnLoadLibrary();
    EXPECT_EQ(proxy.handle_, nullptr);
    EXPECT_EQ(proxy.func_, nullptr);
    EXPECT_FALSE(proxy.loadFlag_);
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_UnLoadLibrary_001 end";
}
 
/**
 * @tc.number: AccessibilityAITextCallProxyUnitTest_UnLoadLibrary_002
 * @tc.name: UnLoadLibrary
 * @tc.desc: Test UnLoadLibrary after LoadLibrary succeeds, should reset all state
 */
HWTEST_F(AccessibilityAITextCallProxyUnitTest, UnLoadLibrary_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_UnLoadLibrary_002 start";
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    proxy.LoadLibrary();
    if (proxy.loadFlag_) {
        proxy.UnLoadLibrary();
        EXPECT_FALSE(proxy.loadFlag_);
        EXPECT_EQ(proxy.handle_, nullptr);
        EXPECT_EQ(proxy.func_, nullptr);
    }
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_UnLoadLibrary_002 end";
}
 
/**
 * @tc.number: AccessibilityAITextCallProxyUnitTest_ConstructDestructor_001
 * @tc.name: ConstructDestructor
 * @tc.desc: Test constructor and destructor, initial state should be clean
 */
HWTEST_F(AccessibilityAITextCallProxyUnitTest, ConstructDestructor_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_ConstructDestructor_001 start";
    auto& proxy = Singleton<AccessibilityAITextCallProxy>::GetInstance();
    // After TearDown cleanup, state should be clean
    EXPECT_EQ(proxy.func_, nullptr);
    EXPECT_EQ(proxy.handle_, nullptr);
    EXPECT_FALSE(proxy.loadFlag_);
    GTEST_LOG_(INFO) << "AccessibilityAITextCallProxyUnitTest_ConstructDestructor_001 end";
}
} // namespace Accessibility
} // namespace OHOS