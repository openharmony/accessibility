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

#ifndef ACCESSIBILITY_TOUCH_EXPLORATION_H
#define ACCESSIBILITY_TOUCH_EXPLORATION_H

#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <functional>
#include "accessibility_element_info.h"
#include "accessibility_element_operator_callback_stub.h"
#include "accessibility_event_transmission.h"
#include "accessible_ability_manager_service.h"
#include "accessibility_def.h"
#include "event_handler.h"

namespace OHOS {
namespace Accessibility {
class TouchExploration;

const int32_t POINTER_COUNT_1 = 1;
const int32_t POINTER_COUNT_2 = 2;
const int32_t POINTER_COUNT_3 = 3;
const int32_t POINTER_COUNT_4 = 4;
const uint32_t MAX_MULTI_FINGER_TYPE = 3;
const int32_t LIMIT_SIZE_TWO = 2;
const int32_t LIMIT_SIZE_THREE = 3;
const int32_t DIRECTION_NUM = 4;
const int32_t TAP_COUNT_MAXIMUM = 3;
const int64_t LONG_PRESS_TIMEOUT = 200000; // microsecond
const int64_t DOUBLE_TAP_TIMEOUT = 300000; // microsecond
const int64_t MULTI_FINGER_TAP_INTERVAL_TIMEOUT = 100000; // microsecond
const int64_t SWIPE_COMPLETE_TIMEOUT = 300000; // microsecond
const int32_t MINI_POINTER_DISTANCE_DIP = 200;
const float DEGREES_THRESHOLD = 0.0f;
const uint32_t MIN_MULTI_FINGER_SWIPE_POINTER_NUM = 2;
const float TOUCH_SLOP = 8.0f;
const float MULTI_TAP_SLOP = 100.0f;
const float MULTI_TAP_SLOP_DELTA = 0.5f;
const int32_t SCREEN_AXIS_NUM = 2;
const double MAX_DRAG_GESTURE_COSINE = 0.525321989;
const int32_t MM_PER_CM = 10;
const double EPSINON = 0.01;
const float PIXEL_MULTIPLIER = 0.1f;
const int32_t DIVIDE_NUM = 2;
#define BIND(func) [this](MMI::PointerEvent& event) { (func(event)); }

/**
 * @brief touch exploration state define
 */
enum class TouchExplorationState : int32_t {
    TOUCH_INIT,
    PASSING_THROUGH,
    INVALID,

    ONE_FINGER_DOWN,
    ONE_FINGER_LONG_PRESS,
    ONE_FINGER_SWIPE,
    ONE_FINGER_SINGLE_TAP,
    ONE_FINGER_SINGLE_TAP_THEN_DOWN,

    TWO_FINGERS_DOWN,
    TWO_FINGERS_DRAG,
    TWO_FINGERS_TAP,
    TWO_FINGERS_CONTINUE_DOWN,
    TWO_FINGERS_UNKNOWN,

    THREE_FINGERS_DOWN,
    THREE_FINGERS_SWIPE,
    THREE_FINGERS_TAP,
    THREE_FINGERS_CONTINUE_DOWN,

    FOUR_FINGERS_DOWN,
    FOUR_FINGERS_SWIPE,
    FOUR_FINGERS_TAP,
    FOUR_FINGERS_CONTINUE_DOWN
};

enum class ChangeAction : int32_t {
    NO_CHANGE,
    HOVER_MOVE,
    POINTER_DOWN,
    POINTER_UP,
    HOVER_ENTER,
    HOVER_EXIT,
    HOVER_CANCEL
};

struct Pointer {
    float px_;
    float py_;
};

class TouchExplorationEventHandler : public AppExecFwk::EventHandler {
public:
    TouchExplorationEventHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner,
                 TouchExploration &tgServer);
    virtual ~TouchExplorationEventHandler() = default;
    /**
     * @brief Process the event of install system bundles.
     * @param event Indicates the event to be processed.
     */
    virtual void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;

private:
    TouchExploration &server_;
};

class TouchExploration : public EventTransmission {
public:
    static constexpr uint32_t SEND_HOVER_MSG = 0;
    static constexpr uint32_t LONG_PRESS_MSG = 1;
    static constexpr uint32_t DOUBLE_TAP_MSG = 2;
    static constexpr uint32_t SWIPE_COMPLETE_TIMEOUT_MSG = 3;
    static constexpr uint32_t TWO_FINGER_SINGLE_TAP_MSG = 4;
    static constexpr uint32_t TWO_FINGER_LONG_PRESS_MSG = 5;
    static constexpr uint32_t TWO_FINGER_DOUBLE_TAP_MSG = 6;
    static constexpr uint32_t TWO_FINGER_DOUBLE_TAP_AND_HOLD_MSG = 7;
    static constexpr uint32_t TWO_FINGER_TRIPLE_TAP_MSG = 8;
    static constexpr uint32_t TWO_FINGER_TRIPLE_TAP_AND_HOLD_MSG = 9;
    static constexpr uint32_t THREE_FINGER_SINGLE_TAP_MSG = 10;
    static constexpr uint32_t THREE_FINGER_LONG_PRESS_MSG = 11;
    static constexpr uint32_t THREE_FINGER_DOUBLE_TAP_MSG = 12;
    static constexpr uint32_t THREE_FINGER_DOUBLE_TAP_AND_HOLD_MSG = 13;
    static constexpr uint32_t THREE_FINGER_TRIPLE_TAP_MSG = 14;
    static constexpr uint32_t THREE_FINGER_TRIPLE_TAP_AND_HOLD_MSG = 15;
    static constexpr uint32_t FOUR_FINGER_SINGLE_TAP_MSG = 16;
    static constexpr uint32_t FOUR_FINGER_LONG_PRESS_MSG = 17;
    static constexpr uint32_t FOUR_FINGER_DOUBLE_TAP_MSG = 18;
    static constexpr uint32_t FOUR_FINGER_DOUBLE_TAP_AND_HOLD_MSG = 19;
    static constexpr uint32_t FOUR_FINGER_TRIPLE_TAP_MSG = 20;
    static constexpr uint32_t FOUR_FINGER_TRIPLE_TAP_AND_HOLD_MSG = 21;
    static constexpr uint32_t WAIT_ANOTHER_FINGER_DOWN_MSG = 22;

    static constexpr int32_t SWIPE_UP = 0;
    static constexpr int32_t SWIPE_DOWN = 1;
    static constexpr int32_t SWIPE_LEFT = 2;
    static constexpr int32_t SWIPE_RIGHT = 3;

    static constexpr GestureType GESTURE_DIRECTION[DIRECTION_NUM] = {
        GestureType::GESTURE_SWIPE_UP,
        GestureType::GESTURE_SWIPE_DOWN,
        GestureType::GESTURE_SWIPE_LEFT,
        GestureType::GESTURE_SWIPE_RIGHT
    };

    static constexpr GestureType GESTURE_DIRECTION_TO_ID[DIRECTION_NUM][DIRECTION_NUM] = {
        {
            GestureType::GESTURE_SWIPE_UP,
            GestureType::GESTURE_SWIPE_UP_THEN_DOWN,
            GestureType::GESTURE_SWIPE_UP_THEN_LEFT,
            GestureType::GESTURE_SWIPE_UP_THEN_RIGHT,
        },
        {
            GestureType::GESTURE_SWIPE_DOWN_THEN_UP,
            GestureType::GESTURE_SWIPE_DOWN,
            GestureType::GESTURE_SWIPE_DOWN_THEN_LEFT,
            GestureType::GESTURE_SWIPE_DOWN_THEN_RIGHT,

        },
        {
            GestureType::GESTURE_SWIPE_LEFT_THEN_UP,
            GestureType::GESTURE_SWIPE_LEFT_THEN_DOWN,
            GestureType::GESTURE_SWIPE_LEFT,
            GestureType::GESTURE_SWIPE_LEFT_THEN_RIGHT,

        },
        {
            GestureType::GESTURE_SWIPE_RIGHT_THEN_UP,
            GestureType::GESTURE_SWIPE_RIGHT_THEN_DOWN,
            GestureType::GESTURE_SWIPE_RIGHT_THEN_LEFT,
            GestureType::GESTURE_SWIPE_RIGHT
        }
    };

    static constexpr uint32_t GESTURE_TAP_MSG[TAP_COUNT_MAXIMUM][MAX_MULTI_FINGER_TYPE] = {
        {
            TWO_FINGER_SINGLE_TAP_MSG,
            THREE_FINGER_SINGLE_TAP_MSG,
            FOUR_FINGER_SINGLE_TAP_MSG,
        },
        {
            TWO_FINGER_DOUBLE_TAP_MSG,
            THREE_FINGER_DOUBLE_TAP_MSG,
            FOUR_FINGER_DOUBLE_TAP_MSG,
        },
        {
            TWO_FINGER_TRIPLE_TAP_MSG,
            THREE_FINGER_TRIPLE_TAP_MSG,
            FOUR_FINGER_TRIPLE_TAP_MSG,
        }
    };

    static constexpr uint32_t GESTURE_HOLD_MSG[TAP_COUNT_MAXIMUM][MAX_MULTI_FINGER_TYPE] = {
        {
            TWO_FINGER_LONG_PRESS_MSG,
            THREE_FINGER_LONG_PRESS_MSG,
            FOUR_FINGER_LONG_PRESS_MSG,
        },
        {
            TWO_FINGER_DOUBLE_TAP_AND_HOLD_MSG,
            THREE_FINGER_DOUBLE_TAP_AND_HOLD_MSG,
            FOUR_FINGER_DOUBLE_TAP_AND_HOLD_MSG,
        },
        {
            TWO_FINGER_TRIPLE_TAP_AND_HOLD_MSG,
            THREE_FINGER_TRIPLE_TAP_AND_HOLD_MSG,
            FOUR_FINGER_TRIPLE_TAP_AND_HOLD_MSG,
        }
    };

    TouchExploration();
    ~TouchExploration() {}
    void StartUp();

    void Clear();
    void HoverEventRunner();
    void SendAccessibilityEventToAA(EventType eventType);
    void SendTouchEventToAA(MMI::PointerEvent &event);
    void SendGestureEventToAA(GestureType gestureId);
    void SendEventToMultimodal(MMI::PointerEvent &event, ChangeAction action);
    void SendScreenWakeUpEvent(MMI::PointerEvent &event);
    void SendUpForDraggingDownEvent();
    void ProcessMultiFingerGesture(uint32_t eventId);
    void CancelPostEvent(uint32_t innerEventID);
    void CancelMultiFingerTapEvent();
    void CancelMultiFingerTapAndHoldEvent();

    /**
     * @brief Handle pointer events from previous event stream node.
     *
     * @param event  the pointer event to be handled.
     * @return true: the event has been processed and does not need to be passed to the next node;
     *         false: the event is not processed.
     */
    bool OnPointerEvent(MMI::PointerEvent &event) override;

    /* Set current state */
    inline void SetCurrentState(TouchExplorationState state)
    {
        currentState_ = state;
    }

    /* Get current state */
    inline TouchExplorationState GetCurrentState()
    {
        return currentState_;
    }

private:
    // Processing Functions in the State Machine
    void HandleInitStateDown(MMI::PointerEvent &event);
    void HandleInitStateUp(MMI::PointerEvent &event);
    void HandleInitStateMove(MMI::PointerEvent &event);
    void HandlePassingThroughState(MMI::PointerEvent &event);
    void HandleInvalidState(MMI::PointerEvent &event);
    void HandleOneFingerDownStateDown(MMI::PointerEvent &event);
    void HandleOneFingerDownStateUp(MMI::PointerEvent &event);
    void HandleOneFingerDownStateMove(MMI::PointerEvent &event);
    void HandleOneFingerLongPressStateDown(MMI::PointerEvent &event);
    void HandleOneFingerLongPressStateUp(MMI::PointerEvent &event);
    void HandleOneFingerLongPressStateMove(MMI::PointerEvent &event);
    void HandleOneFingerSwipeStateDown(MMI::PointerEvent &event);
    void HandleOneFingerSwipeStateUp(MMI::PointerEvent &event);
    void HandleOneFingerSwipeStateMove(MMI::PointerEvent &event);
    void HandleOneFingerSingleTapStateDown(MMI::PointerEvent &event);
    void HandleOneFingerSingleTapThenDownStateDown(MMI::PointerEvent &event);
    void HandleOneFingerSingleTapThenDownStateUp(MMI::PointerEvent &event);
    void HandleOneFingerSingleTapThenDownStateMove(MMI::PointerEvent &event);
    void HandleTwoFingersDownStateDown(MMI::PointerEvent &event);
    void HandleTwoFingersDownStateUp(MMI::PointerEvent &event);
    void HandleTwoFingersDownStateMove(MMI::PointerEvent &event);
    void HandleTwoFingersDragStateDown(MMI::PointerEvent &event);
    void HandleTwoFingersDragStateUp(MMI::PointerEvent &event);
    void HandleTwoFingersDragStateMove(MMI::PointerEvent &event);
    void HandleTwoFingersTapStateDown(MMI::PointerEvent &event);
    void HandleMultiFingersTapStateUp(MMI::PointerEvent &event);
    void HandleTwoFingersTapStateMove(MMI::PointerEvent &event);
    void HandleMultiFingersContinueDownStateDown(MMI::PointerEvent &event);
    void HandleTwoFingersContinueDownStateUp(MMI::PointerEvent &event);
    void HandleTwoFingersContinueDownStateMove(MMI::PointerEvent &event);
    void HandleTwoFingersUnknownStateDown(MMI::PointerEvent &event);
    void HandleTwoFingersUnknownStateUp(MMI::PointerEvent &event);
    void HandleTwoFingersUnknownStateMove(MMI::PointerEvent &event);
    void HandleThreeFingersDownStateDown(MMI::PointerEvent &event);
    void HandleThreeFingersDownStateUp(MMI::PointerEvent &event);
    void HandleThreeFingersDownStateMove(MMI::PointerEvent &event);
    void HandleThreeFingersSwipeStateDown(MMI::PointerEvent &event);
    void HandleThreeFingersSwipeStateUp(MMI::PointerEvent &event);
    void HandleThreeFingersSwipeStateMove(MMI::PointerEvent &event);
    void HandleThreeFingersTapStateDown(MMI::PointerEvent &event);
    void HandleThreeFingersTapStateMove(MMI::PointerEvent &event);
    void HandleThreeFingersContinueDownStateUp(MMI::PointerEvent &event);
    void HandleThreeFingersContinueDownStateMove(MMI::PointerEvent &event);
    void HandleFourFingersDownStateDown(MMI::PointerEvent &event);
    void HandleFourFingersDownStateUp(MMI::PointerEvent &event);
    void HandleFourFingersDownStateMove(MMI::PointerEvent &event);
    void HandleFourFingersSwipeStateDown(MMI::PointerEvent &event);
    void HandleFourFingersSwipeStateUp(MMI::PointerEvent &event);
    void HandleFourFingersSwipeStateMove(MMI::PointerEvent &event);
    void HandleFourFingersTapStateDown(MMI::PointerEvent &event);
    void HandleFourFingersTapStateMove(MMI::PointerEvent &event);
    void HandleFourFingersContinueDownStateUp(MMI::PointerEvent &event);
    void HandleFourFingersContinueDownStateMove(MMI::PointerEvent &event);
    void HandleCancelEvent(MMI::PointerEvent &event);

    void InitOneFingerGestureFuncMap();
    void InitTwoFingerGestureFuncMap();
    void InitThreeFingerGestureFuncMap();
    void InitFourFingerGestureFuncMap();
    void HandlePointerEvent(MMI::PointerEvent &event);
    void AddOneFingerSwipeEvent(MMI::PointerEvent &event);
    std::vector<Pointer> GetOneFingerSwipePath();
    int32_t GetSwipeDirection(const int32_t dx, const int32_t dy);
    void RecordFocusedLocation(MMI::PointerEvent &event);
    void OffsetEvent(MMI::PointerEvent &event);
    bool GetBasePointItem(MMI::PointerEvent::PointerItem &basePointerIterm, int32_t pId);
    void GetPointOffset(MMI::PointerEvent &event, std::vector<float> &firstPointOffset,
        std::vector<float> &secondPointOffset);
    float GetAngleCos(float offsetX, float offsetY, bool isGetX);
    bool IsDragGestureAccept(MMI::PointerEvent &event);
    void SendDragDownEventToMultimodal(MMI::PointerEvent event);
    bool GetPointerItemWithFingerNum(int32_t fingerNum, std::vector<MMI::PointerEvent::PointerItem> &curPoints,
        std::vector<MMI::PointerEvent::PointerItem> &prePoints, MMI::PointerEvent &event);
    bool IsMultiFingerMultiTap(MMI::PointerEvent &event, const int32_t fingerNum);
    bool IsMultiFingerMultiTapGesture(MMI::PointerEvent &event, const int32_t fingerNum);
    void HandleMultiFingersTapStateDown(MMI::PointerEvent &event, int32_t fingerNum);
    void HandleMultiFingersTapStateMove(MMI::PointerEvent &event, int32_t fingerNum);
    void HandleMultiFingersContinueDownStateUp(MMI::PointerEvent &event, int32_t fingerNum);
    void HandleMultiFingersContinueDownStateMove(MMI::PointerEvent &event, int32_t fingerNum);
    void StoreMultiFingerSwipeBaseDownPoint();
    bool GetMultiFingerSwipeBasePointerItem(MMI::PointerEvent::PointerItem &basePointerIterm, int32_t pId);
    bool SaveMultiFingerSwipeGesturePointerInfo(MMI::PointerEvent &event);
    bool RecognizeMultiFingerSwipePath(const std::vector<Pointer> &path);
    GestureType GetMultiFingerSwipeGestureId(int32_t fingerNum);
    void HandleMultiFingersSwipeStateUp(MMI::PointerEvent &event, int32_t fingerNum);
    std::map<uint32_t, GestureType> GetMultiFingerMsgToGestureMap();

    inline float CalculateMoveThreshold(int dpi)
    {
        return dpi * (1.0f / 25.4f) * MM_PER_CM;
    }

    std::shared_ptr<TouchExplorationEventHandler> handler_ = nullptr;
    std::shared_ptr<AppExecFwk::EventRunner> runner_ = nullptr;
    using HandleEventFunc = std::function<void(MMI::PointerEvent &)>;
    std::map<TouchExplorationState, std::map<int32_t, HandleEventFunc>> handleEventFuncMap_ = {};

    TouchExplorationState currentState_ = TouchExplorationState::TOUCH_INIT;
    std::list<MMI::PointerEvent> receivedPointerEvents_ {};
    std::list<MMI::PointerEvent> injectedPointerDownEvents_ {};

    // single-finger gesture
    int32_t offsetX_ = 0;
    int32_t offsetY_ = 0;
    float moveThreshold_ = 0;
    float xMinPixels_ = 0;
    float yMinPixels_ = 0;
    std::vector<Pointer> oneFingerSwipeRoute_ {};
    MMI::PointerEvent::PointerItem oneFingerSwipePrePointer_ {};

    // multi-finger gesture
    int32_t draggingPid_ = -1;
    int32_t multiTapNum_ = 0;
    int32_t multiTapOffsetThresh_ = 0;
    int32_t multiFingerSwipeDirection_ = -1;
    float mMinPixelsBetweenSamplesX_ = 0;
    float mMinPixelsBetweenSamplesY_ = 0;
    std::map<int32_t, std::vector<Pointer>> multiFingerSwipeRoute_ {};
    std::map<int32_t, std::shared_ptr<MMI::PointerEvent>> multiFingerSwipePrePoint_ {};
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_TOUCH_EXPLORATION_H