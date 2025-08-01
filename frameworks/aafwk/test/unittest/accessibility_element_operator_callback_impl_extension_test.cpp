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

#include <memory>
#include <gtest/gtest.h>
#include "accessibility_element_operator_callback_implementation.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr int32_t SEQUENCE_NUM_ZERO = 0;
    constexpr int32_t SEQUENCE_NUM = -1;
} // namespace

class AccessibilityElementOperatorCallbackImplExtensionTest : public ::testing::Test {
public:
    AccessibilityElementOperatorCallbackImplExtensionTest()
    {}
    ~AccessibilityElementOperatorCallbackImplExtensionTest()
    {}

    std::shared_ptr<AccessibilityElementOperatorCallbackImpl> instance_ = nullptr;

    static void SetUpTestCase()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplTest Start";
    }
    static void TearDownTestCase()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplTest End";
    }
    void SetUp()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplTest SetUp()";
        instance_ = std::make_shared<AccessibilityElementOperatorCallbackImpl>();
    };
    void TearDown()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplTest TearDown()";
        instance_ = nullptr;
    }
};

/**
 * @tc.number: SetSearchElementInfoByAccessibilityIdResult_001
 * @tc.name: SetSearchElementInfoByAccessibilityIdResult
 * @tc.desc: Test function SetSearchElementInfoByAccessibilityIdResult
 */
HWTEST_F(
    AccessibilityElementOperatorCallbackImplTest, SetSearchElementInfoByAccessibilityIdResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";
        return;
    }

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetSearchElementInfoByAccessibilityIdResult(infos, SEQUENCE_NUM);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoByTextResult_001
 * @tc.name: SetSearchElementInfoByTextResult
 * @tc.desc: Test function SetSearchElementInfoByTextResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetSearchElementInfoByTextResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_001 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetSearchElementInfoByTextResult(infos, SEQUENCE_NUM);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_001 end";
}

/**
 * @tc.number: SetFindFocusedElementInfoResult_001
 * @tc.name: SetFindFocusedElementInfoResult
 * @tc.desc: Test function SetFindFocusedElementInfoResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetFindFocusedElementInfoResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetFindFocusedElementInfoResult(info, SEQUENCE_NUM);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001 end";
}

/**
 * @tc.number: SetFocusMoveSearchResult_001
 * @tc.name: SetFocusMoveSearchResult
 * @tc.desc: Test function SetFocusMoveSearchResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetFocusMoveSearchResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetFocusMoveSearchResult(info, SEQUENCE_NUM);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 end";
}

/**
 * @tc.number: SetExecuteActionResult_001
 * @tc.name: SetExecuteActionResult
 * @tc.desc: Test function SetExecuteActionResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetExecuteActionResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetExecuteActionResult_001 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetExecuteActionResult(true, SEQUENCE_NUM);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetExecuteActionResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoByAccessibilityIdResult_0011
 * @tc.name: SetSearchElementInfoByAccessibilityIdResult
 * @tc.desc: Test function SetSearchElementInfoByAccessibilityIdResult
 */
HWTEST_F(
    AccessibilityElementOperatorCallbackImplTest, SetSearchElementInfoByAccessibilityIdResult_0011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_0011 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetSearchElementInfoByAccessibilityIdResult(infos, SEQUENCE_NUM_ZERO);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_0011 end";
}

/**
 * @tc.number: SetSearchElementInfoByTextResult_0011
 * @tc.name: SetSearchElementInfoByTextResult
 * @tc.desc: Test function SetSearchElementInfoByTextResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetSearchElementInfoByTextResult_0011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_0011 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetSearchElementInfoByTextResult(infos, SEQUENCE_NUM_ZERO);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_0011 end";
}

/**
 * @tc.number: SetFindFocusedElementInfoResult_0011
 * @tc.name: SetFindFocusedElementInfoResult
 * @tc.desc: Test function SetFindFocusedElementInfoResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetFindFocusedElementInfoResult_0011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_0011 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetFindFocusedElementInfoResult(info, SEQUENCE_NUM_ZERO);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_0011 end";
}

/**
 * @tc.number: SetFocusMoveSearchResult_0011
 * @tc.name: SetFocusMoveSearchResult
 * @tc.desc: Test function SetFocusMoveSearchResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetFocusMoveSearchResult_0011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_0011 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetFocusMoveSearchResult(info, SEQUENCE_NUM_ZERO);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_0011 end";
}

/**
 * @tc.number: SetExecuteActionResult_0011
 * @tc.name: SetExecuteActionResult
 * @tc.desc: Test function SetExecuteActionResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplTest, SetExecuteActionResult_0011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetExecuteActionResult_0011 start";
    EXPECT_TRUE(instance_);

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    instance_->SetExecuteActionResult(true, SEQUENCE_NUM_ZERO);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetExecuteActionResult_0011 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_001
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult
 */
HWTEST_F(
    AccessibilityElementOperatorCallbackImplTest, SetSearchElementInfoBySpecificPropertyResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_001 start";
    ASSERT_TRUE(instance_) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";

    std::list<AccessibilityElementInfo> infos;
    std::list<AccessibilityElementInfo> treeInfos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    treeInfos.push_back(info);
    instance_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, SEQUENCE_NUM);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoBySpecificPropertyResult_0011
 * @tc.name: SetSearchElementInfoBySpecificPropertyResult
 * @tc.desc: Test function SetSearchElementInfoBySpecificPropertyResult
 */
HWTEST_F(
    AccessibilityElementOperatorCallbackImplTest, SetSearchElementInfoBySpecificPropertyResult_0011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_0011 start";
    EXPECT_TRUE(instance_);

    std::list<AccessibilityElementInfo> infos;
    std::list<AccessibilityElementInfo> treeInfos;
    AccessibilityElementInfo info {};
    infos.push_back(info);
    treeInfos.push_back(info);
    instance_->SetSearchElementInfoBySpecificPropertyResult(infos, treeInfos, SEQUENCE_NUM_ZERO);
    EXPECT_NE(instance_.get(), nullptr);
    GTEST_LOG_(INFO) << "SetSearchElementInfoBySpecificPropertyResult_0011 end";
}
} // namespace Accessibility
} // namespace OHOS