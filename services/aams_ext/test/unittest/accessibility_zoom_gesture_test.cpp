/*
 * Copyright (C) 2022-2026 Huawei Device Co., Ltd.
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
#include <map>
#include <memory>
#include "accessibility_ut_helper.h"
#include "accessibility_zoom_gesture.h"
#include "full_screen_magnification_manager.h"
#include "window_magnification_manager.h"
#include "magnification_menu_manager.h"
#include "magnification_def.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr uint32_t TRIPLE_TAP_COUNT = 3;
    constexpr uint32_t DOUBLE_TAP_COUNT = 2;
    constexpr int32_t SLEEP_MS_2 = 2;
    constexpr int32_t POINT_ID_0 = 0;
    constexpr int32_t POINT_ID_1 = 1;
    constexpr int32_t POINT_ID_2 = 2;
    constexpr int32_t INVALID_POINTER_ACTION = -1;
    constexpr int32_t READY_STATE = 0;
    constexpr int32_t ZOOM_STATE = 1;
} // namespace
class AccessibilityZoomGestureUnitTest : public ::testing::Test {
public:
    AccessibilityZoomGestureUnitTest()
    {}
    ~AccessibilityZoomGestureUnitTest()
    {}

    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
    std::shared_ptr<MMI::PointerEvent> CreatePointerEvent(int32_t sourceType, int32_t pointerAction);
    std::shared_ptr<MMI::PointerEvent> CreatePointerEvent(int32_t pointerAction,
        std::vector<MMI::PointerEvent::PointerItem> &points, int32_t pointerId);
    void SetPointerItem(MMI::PointerEvent::PointerItem &point, int32_t id, int32_t x, int32_t y);
    void TripleTaps();
    void DoubleTapsWithThreeFingers();

    std::shared_ptr<AccessibilityZoomGesture> zoomGesture_ = nullptr;
    std::shared_ptr<FullScreenMagnificationManager> fullScreenManager_ = nullptr;
    std::shared_ptr<WindowMagnificationManager> windowMagnificationManager_ = nullptr;
    std::shared_ptr<MagnificationMenuManager> menuManager_ = nullptr;
};

void AccessibilityZoomGestureUnitTest::SetUpTestCase()
{
    GTEST_LOG_(INFO) << "###################### AccessibilityZoomGestureUnitTest Start ######################";
}

void AccessibilityZoomGestureUnitTest::TearDownTestCase()
{
    GTEST_LOG_(INFO) << "###################### AccessibilityZoomGestureUnitTest End ######################";
}

void AccessibilityZoomGestureUnitTest::SetUp()
{
    GTEST_LOG_(INFO) << "SetUp";
    fullScreenManager_ = std::make_shared<FullScreenMagnificationManager>();
    windowMagnificationManager_ = std::make_shared<WindowMagnificationManager>();
    menuManager_ = std::make_shared<MagnificationMenuManager>();
    zoomGesture_ = std::make_shared<AccessibilityZoomGesture>(fullScreenManager_,
        windowMagnificationManager_, menuManager_);
    AccessibilityAbilityHelper::GetInstance().ClearZoom();
}

void AccessibilityZoomGestureUnitTest::TearDown()
{
    GTEST_LOG_(INFO) << "TearDown";
    AccessibilityAbilityHelper::GetInstance().ClearZoom();
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_ = nullptr;
    fullScreenManager_ = nullptr;
    windowMagnificationManager_ = nullptr;
    menuManager_ = nullptr;
}

std::shared_ptr<MMI::PointerEvent> AccessibilityZoomGestureUnitTest::CreatePointerEvent(int32_t sourceType,
    int32_t pointerAction)
{
    std::shared_ptr<MMI::PointerEvent> event = MMI::PointerEvent::Create();
    if (!event) {
        return nullptr;
    }
    event->SetSourceType(sourceType);
    event->SetPointerAction(pointerAction);
    return event;
}

std::shared_ptr<MMI::PointerEvent> AccessibilityZoomGestureUnitTest::CreatePointerEvent(int32_t pointerAction,
    std::vector<MMI::PointerEvent::PointerItem> &points, int32_t pointerId)
{
    std::shared_ptr<MMI::PointerEvent> event = MMI::PointerEvent::Create();
    if (!event) {
        return nullptr;
    }
    for (auto &point : points) {
        event->AddPointerItem(point);
    }
    event->SetPointerId(pointerId);
    event->SetSourceType(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    event->SetPointerAction(pointerAction);
    return event;
}

void AccessibilityZoomGestureUnitTest::SetPointerItem(MMI::PointerEvent::PointerItem &point,
    int32_t id, int32_t x, int32_t y)
{
    point.SetPointerId(id);
    point.SetDisplayX(x);
    point.SetDisplayY(y);
}

void AccessibilityZoomGestureUnitTest::TripleTaps()
{
    EXPECT_TRUE(zoomGesture_ != nullptr);
    for (uint32_t count = 0; count < TRIPLE_TAP_COUNT; count++) {
        std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
            MMI::PointerEvent::POINTER_ACTION_DOWN);
        EXPECT_TRUE(eventDown != nullptr);
        MMI::PointerEvent::PointerItem item;
        item.SetDisplayX(100);
        item.SetDisplayY(100);
        eventDown->AddPointerItem(item);
        zoomGesture_->OnPointerEvent(*eventDown);
        std::shared_ptr<MMI::PointerEvent> eventUp = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
            MMI::PointerEvent::POINTER_ACTION_UP);
        EXPECT_TRUE(eventUp != nullptr);
        MMI::PointerEvent::PointerItem item1;
        item1.SetDisplayX(100);
        item1.SetDisplayY(100);
        eventUp->AddPointerItem(item1);
        zoomGesture_->OnPointerEvent(*eventUp);
    }
}

void AccessibilityZoomGestureUnitTest::DoubleTapsWithThreeFingers()
{
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(THREE_FINGER_DOUBLE_TAP_MODE);
    for (uint32_t count = 0; count < DOUBLE_TAP_COUNT; count++) {
        std::vector<MMI::PointerEvent::PointerItem> points = {};
        MMI::PointerEvent::PointerItem point1 = {};
        SetPointerItem(point1, POINT_ID_0, 10, 10);
        MMI::PointerEvent::PointerItem point2 = {};
        SetPointerItem(point2, POINT_ID_1, 20, 20);
        MMI::PointerEvent::PointerItem point3 = {};
        SetPointerItem(point3, POINT_ID_2, 30, 30);
        points.emplace_back(point1);
        points.emplace_back(point2);
        points.emplace_back(point3);
        std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(
            MMI::PointerEvent::POINTER_ACTION_DOWN, points, POINT_ID_2);
        EXPECT_TRUE(eventDown != nullptr);
        zoomGesture_->OnPointerEvent(*eventDown);
        std::shared_ptr<MMI::PointerEvent> eventUp = CreatePointerEvent(
            MMI::PointerEvent::POINTER_ACTION_UP, points, POINT_ID_2);
        EXPECT_TRUE(eventUp != nullptr);
        zoomGesture_->OnPointerEvent(*eventUp);
    }
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_001
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_DOWN timeout)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    sleep(2);
    int32_t touchAction = AccessibilityAbilityHelper::GetInstance().GetTouchEventActionOfTargetIndex(0);
    EXPECT_EQ(touchAction, MMI::PointerEvent::POINTER_ACTION_DOWN);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_002
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(Tap once on the screen)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_002 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    // Pointer down
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item;
    eventDown->AddPointerItem(item);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*eventDown);
    // Pointer up
    std::shared_ptr<MMI::PointerEvent> eventUp = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_UP);
    EXPECT_TRUE(eventUp != nullptr);
    zoomGesture_->OnPointerEvent(*eventUp);
    sleep(2);

    int32_t touchAction = AccessibilityAbilityHelper::GetInstance().GetTouchEventActionOfTargetIndex(0);
    EXPECT_EQ(touchAction, MMI::PointerEvent::POINTER_ACTION_DOWN);
    touchAction = AccessibilityAbilityHelper::GetInstance().GetTouchEventActionOfTargetIndex(1);
    EXPECT_EQ(touchAction, MMI::PointerEvent::POINTER_ACTION_UP);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_002 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_003
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(3 Taps on the screen)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_003 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    // stop zoom
    TripleTaps();
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(!zoomState);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_003 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_004
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(transfer to sliding state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_004 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    // Pointer down
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    eventDown->AddPointerItem(item1);

    MMI::PointerEvent::PointerItem item2;
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    eventDown->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*eventDown);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_004 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_005
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_MOVE in sliding state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_005 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);

    // Pointer down
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    eventDown->AddPointerItem(item1);

    MMI::PointerEvent::PointerItem item2;
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    eventDown->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*eventDown);

    // Pointer move
    std::shared_ptr<MMI::PointerEvent> eventMove = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_MOVE);
    EXPECT_TRUE(eventMove != nullptr);
    MMI::PointerEvent::PointerItem item3;
    item3.SetDisplayX(0);
    item3.SetDisplayY(50);
    eventMove->AddPointerItem(item3);

    MMI::PointerEvent::PointerItem item4;
    item4.SetDisplayX(70);
    item4.SetDisplayY(50);
    eventMove->AddPointerItem(item4);
    zoomGesture_->OnPointerEvent(*eventMove);
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_005 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_006
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_DOWN in sliding state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_006, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_006 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);

    // Pointer down
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    eventDown->AddPointerItem(item1);

    MMI::PointerEvent::PointerItem item2;
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    eventDown->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*eventDown);

    MMI::PointerEvent::PointerItem item3;
    item3.SetDisplayX(50);
    item3.SetDisplayY(25);
    eventDown->AddPointerItem(item3);
    zoomGesture_->OnPointerEvent(*eventDown);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_006 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_007
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_UP in sliding state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_007, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_007 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);

    // Pointer down
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(event != nullptr);
    event->SetPointerId(1);
    MMI::PointerEvent::PointerItem item1;
    item1.SetPointerId(1);
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    event->AddPointerItem(item1);

    MMI::PointerEvent::PointerItem item2;
    item2.SetPointerId(2);
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    event->AddPointerItem(item2);

    MMI::PointerEvent::PointerItem item3;
    item3.SetPointerId(3);
    item3.SetDisplayX(50);
    item3.SetDisplayY(25);
    event->AddPointerItem(item3);
    zoomGesture_->OnPointerEvent(*event);

    // Pointer up
    event->SetPointerAction(MMI::PointerEvent::POINTER_ACTION_UP);
    zoomGesture_->OnPointerEvent(*event);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_007 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_008
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(sliding state->zoom state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_008, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_008 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);

    // Pointer down
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(event != nullptr);
    event->SetPointerId(1);
    MMI::PointerEvent::PointerItem item1;
    item1.SetPointerId(1);
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    event->AddPointerItem(item1);

    MMI::PointerEvent::PointerItem item2;
    item2.SetPointerId(2);
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    event->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*event);

    // Pointer up
    event->SetPointerAction(MMI::PointerEvent::POINTER_ACTION_UP);
    zoomGesture_->OnPointerEvent(*event);
    // Pointer up
    event->SetPointerAction(MMI::PointerEvent::POINTER_ACTION_UP);
    event->SetPointerId(2);
    event->RemovePointerItem(1);
    zoomGesture_->OnPointerEvent(*event);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_008 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_009
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_CANCEL in ready state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_009, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_009 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_CANCEL);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    int32_t touchAction = AccessibilityAbilityHelper::GetInstance().GetTouchEventActionOfTargetIndex(0);
    EXPECT_EQ(touchAction, MMI::PointerEvent::POINTER_ACTION_CANCEL);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_009 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_010
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_CANCEL in zoom state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_010, TestSize.Level1)
{
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_010 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    // send cancel event
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_CANCEL);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    int32_t touchAction = AccessibilityAbilityHelper::GetInstance().GetTouchEventActionOfTargetIndex(0);
    EXPECT_EQ(touchAction, MMI::PointerEvent::POINTER_ACTION_CANCEL);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_010 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_011
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent(POINTER_ACTION_CANCEL in sliding state)
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_011 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    // start zoom
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);

    // Pointer down
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(event != nullptr);
    event->SetPointerId(1);
    MMI::PointerEvent::PointerItem item1;
    item1.SetPointerId(1);
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    event->AddPointerItem(item1);

    MMI::PointerEvent::PointerItem item2;
    item2.SetPointerId(2);
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    event->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*event);

    // Pointer cancel
    event->SetPointerAction(MMI::PointerEvent::POINTER_ACTION_CANCEL);
    zoomGesture_->OnPointerEvent(*event);

    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_011 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_012
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with invalid pointer action
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_012, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_012 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        INVALID_POINTER_ACTION);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_012 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_013
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with levitate move action
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_013, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_013 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_LEVITATE_MOVE);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_013 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_014
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with scrollshot pointer id
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_014, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_014 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(event != nullptr);
    event->SetPointerId(SCROLL_SHOT_POINTER_ID);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_014 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_DestroyEvents_001
 * @tc.name: DestroyEvents
 * @tc.desc: Test function DestroyEvents
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_DestroyEvents_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_DestroyEvents_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    zoomGesture_->DestroyEvents();
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_FALSE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_DestroyEvents_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_ShieldZoomGesture_001
 * @tc.name: ShieldZoomGesture
 * @tc.desc: Test function ShieldZoomGesture with true state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_ShieldZoomGesture_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_ShieldZoomGesture_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    zoomGesture_->ShieldZoomGesture(true);
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_FALSE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_ShieldZoomGesture_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_ShieldZoomGesture_002
 * @tc.name: ShieldZoomGesture
 * @tc.desc: Test function ShieldZoomGesture with false state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_ShieldZoomGesture_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_ShieldZoomGesture_002 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->ShieldZoomGesture(false);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_ShieldZoomGesture_002 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_ShieldZoomGesture_003
 * @tc.name: ShieldZoomGesture
 * @tc.desc: Test function ShieldZoomGesture called twice with same state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_ShieldZoomGesture_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_ShieldZoomGesture_003 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->ShieldZoomGesture(true);
    zoomGesture_->ShieldZoomGesture(true);
    int32_t zoomState = zoomGesture_->GetZoomState();
    EXPECT_EQ(zoomState, READY_STATE);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_ShieldZoomGesture_003 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_GetWindowParam_001
 * @tc.name: GetWindowParam
 * @tc.desc: Test function GetWindowParam with needRefresh true
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_GetWindowParam_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetWindowParam_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->GetWindowParam(true);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetWindowParam_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_GetWindowParam_002
 * @tc.name: GetWindowParam
 * @tc.desc: Test function GetWindowParam with needRefresh false
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_GetWindowParam_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetWindowParam_002 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->GetWindowParam(false);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetWindowParam_002 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_StartMagnificationInteract_001
 * @tc.name: StartMagnificationInteract
 * @tc.desc: Test function StartMagnificationInteract
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_StartMagnificationInteract_001,
    TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_StartMagnificationInteract_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->StartMagnificationInteract();
    int32_t zoomState = zoomGesture_->GetZoomState();
    EXPECT_EQ(zoomState, ZOOM_STATE);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_StartMagnificationInteract_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_SetGestureMode_001
 * @tc.name: SetGestureMode
 * @tc.desc: Test function SetGestureMode with SINGLE_FINGER_TRIPLE_TAP_MODE
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_SetGestureMode_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetGestureMode_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetGestureMode_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_SetGestureMode_002
 * @tc.name: SetGestureMode
 * @tc.desc: Test function SetGestureMode with THREE_FINGER_DOUBLE_TAP_MODE
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_SetGestureMode_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetGestureMode_002 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(THREE_FINGER_DOUBLE_TAP_MODE);
    DoubleTapsWithThreeFingers();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(!zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetGestureMode_002 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_SetMagnificationMode_001
 * @tc.name: SetMagnificationMode
 * @tc.desc: Test function SetMagnificationMode with FULL_SCREEN_MAGNIFICATION
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_SetMagnificationMode_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetMagnificationMode_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetMagnificationMode(FULL_SCREEN_MAGNIFICATION);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetMagnificationMode_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_SetMagnificationMode_002
 * @tc.name: SetMagnificationMode
 * @tc.desc: Test function SetMagnificationMode with WINDOW_MAGNIFICATION
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_SetMagnificationMode_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetMagnificationMode_002 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetMagnificationMode(WINDOW_MAGNIFICATION);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_SetMagnificationMode_002 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_DisableGesture_001
 * @tc.name: DisableGesture
 * @tc.desc: Test function DisableGesture
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_DisableGesture_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_DisableGesture_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    zoomGesture_->DisableGesture();
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_FALSE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_DisableGesture_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_GetZoomState_001
 * @tc.name: GetZoomState
 * @tc.desc: Test function GetZoomState in ready state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_GetZoomState_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetZoomState_001 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    int32_t zoomState = zoomGesture_->GetZoomState();
    EXPECT_EQ(zoomState, READY_STATE);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetZoomState_001 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_GetZoomState_002
 * @tc.name: GetZoomState
 * @tc.desc: Test function GetZoomState in zoom state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_GetZoomState_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetZoomState_002 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    int32_t zoomState = zoomGesture_->GetZoomState();
    EXPECT_EQ(zoomState, ZOOM_STATE);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_GetZoomState_002 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_015
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with non-touchscreen source type in ready state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_015, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_015 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_MOUSE,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_015 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_016
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with three finger double tap to start zoom
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_016, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_016 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    zoomGesture_->SetGestureMode(THREE_FINGER_DOUBLE_TAP_MODE);
    DoubleTapsWithThreeFingers();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(!zoomState);
    DoubleTapsWithThreeFingers();
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(!zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_016 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_017
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with two finger move in sliding state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_017, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_017 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    item1.SetPointerId(POINT_ID_0);
    eventDown->AddPointerItem(item1);
    MMI::PointerEvent::PointerItem item2;
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    item2.SetPointerId(POINT_ID_1);
    eventDown->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*eventDown);
    std::shared_ptr<MMI::PointerEvent> eventMove = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_MOVE);
    EXPECT_TRUE(eventMove != nullptr);
    MMI::PointerEvent::PointerItem item3;
    item3.SetDisplayX(0);
    item3.SetDisplayY(50);
    item3.SetPointerId(POINT_ID_0);
    eventMove->AddPointerItem(item3);
    MMI::PointerEvent::PointerItem item4;
    item4.SetDisplayX(70);
    item4.SetDisplayY(50);
    item4.SetPointerId(POINT_ID_1);
    eventMove->AddPointerItem(item4);
    zoomGesture_->OnPointerEvent(*eventMove);
    zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_017 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_018
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with hold state down and move
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_018, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_018 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item;
    item.SetDisplayX(100);
    item.SetDisplayY(100);
    eventDown->AddPointerItem(item);
    zoomGesture_->OnPointerEvent(*eventDown);
    std::shared_ptr<MMI::PointerEvent> eventMove = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_MOVE);
    EXPECT_TRUE(eventMove != nullptr);
    MMI::PointerEvent::PointerItem itemMove;
    itemMove.SetDisplayX(110);
    itemMove.SetDisplayY(110);
    eventMove->AddPointerItem(itemMove);
    zoomGesture_->OnPointerEvent(*eventMove);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_018 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_020
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with passing through state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_020, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_020 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    eventDown->AddPointerItem(item1);
    MMI::PointerEvent::PointerItem item2;
    item2.SetDisplayX(40);
    item2.SetDisplayY(20);
    eventDown->AddPointerItem(item2);
    zoomGesture_->OnPointerEvent(*eventDown);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    std::shared_ptr<MMI::PointerEvent> eventUp = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_UP);
    EXPECT_TRUE(eventUp != nullptr);
    zoomGesture_->OnPointerEvent(*eventUp);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_020 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_021
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with hot area sliding state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_021, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_021 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    AccessibilityAbilityHelper::GetInstance().SetTapOnHotArea(true);
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    eventDown->AddPointerItem(item1);
    zoomGesture_->OnPointerEvent(*eventDown);
    std::shared_ptr<MMI::PointerEvent> eventMove = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_MOVE);
    EXPECT_TRUE(eventMove != nullptr);
    MMI::PointerEvent::PointerItem itemMove;
    itemMove.SetDisplayX(200);
    itemMove.SetDisplayY(200);
    eventMove->AddPointerItem(itemMove);
    zoomGesture_->OnPointerEvent(*eventMove);
    std::shared_ptr<MMI::PointerEvent> eventUp = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_UP);
    EXPECT_TRUE(eventUp != nullptr);
    MMI::PointerEvent::PointerItem itemUp;
    itemUp.SetDisplayX(200);
    itemUp.SetDisplayY(200);
    eventUp->AddPointerItem(itemUp);
    zoomGesture_->OnPointerEvent(*eventUp);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_021 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_022
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with menu sliding state
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_022, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_022 start";
    zoomGesture_->SetGestureMode(SINGLE_FINGER_TRIPLE_TAP_MODE);
    TripleTaps();
    bool zoomState = AccessibilityAbilityHelper::GetInstance().GetZoomState();
    EXPECT_TRUE(zoomState);
    AccessibilityAbilityHelper::GetInstance().SetTapOnMenu(true);
    std::shared_ptr<MMI::PointerEvent> eventDown = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_DOWN);
    EXPECT_TRUE(eventDown != nullptr);
    MMI::PointerEvent::PointerItem item1;
    item1.SetDisplayX(10);
    item1.SetDisplayY(20);
    eventDown->AddPointerItem(item1);
    zoomGesture_->OnPointerEvent(*eventDown);
    std::shared_ptr<MMI::PointerEvent> eventMove = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_MOVE);
    EXPECT_TRUE(eventMove != nullptr);
    MMI::PointerEvent::PointerItem itemMove;
    itemMove.SetDisplayX(200);
    itemMove.SetDisplayY(200);
    eventMove->AddPointerItem(itemMove);
    zoomGesture_->OnPointerEvent(*eventMove);
    std::shared_ptr<MMI::PointerEvent> eventUp = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_UP);
    EXPECT_TRUE(eventUp != nullptr);
    MMI::PointerEvent::PointerItem itemUp;
    itemUp.SetDisplayX(200);
    itemUp.SetDisplayY(200);
    eventUp->AddPointerItem(itemUp);
    zoomGesture_->OnPointerEvent(*eventUp);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_022 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_023
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with levitate in window action
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_023, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_023 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_LEVITATE_IN_WINDOW);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_023 end";
}

/**
 * @tc.number: AccessibilityZoomGesture_Unittest_OnPointerEvent_024
 * @tc.name: OnPointerEvent
 * @tc.desc: Test function OnPointerEvent with levitate out window action
 */
HWTEST_F(AccessibilityZoomGestureUnitTest, AccessibilityZoomGesture_Unittest_OnPointerEvent_024, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_024 start";
    EXPECT_TRUE(zoomGesture_ != nullptr);
    std::shared_ptr<MMI::PointerEvent> event = CreatePointerEvent(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN,
        MMI::PointerEvent::POINTER_ACTION_LEVITATE_OUT_WINDOW);
    EXPECT_TRUE(event != nullptr);
    AccessibilityAbilityHelper::GetInstance().ClearTouchEventActionVector();
    zoomGesture_->OnPointerEvent(*event);
    GTEST_LOG_(INFO) << "AccessibilityZoomGesture_Unittest_OnPointerEvent_024 end";
}
} // namespace Accessibility
} // namespace OHOS