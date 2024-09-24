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

#include "accessibility_zoom_gesture.h"
#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"
#include "window_accessibility_controller.h"
#include "accessibility_window_manager.h"
#include "utils.h"
#include "cfloat"
#ifdef OHOS_BUILD_ENABLE_POWER_MANAGER
#include "accessibility_power_manager.h"
#endif

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr size_t POINTER_COUNT_1 = 1;
    constexpr size_t POINTER_COUNT_2 = 2;
    constexpr float TAP_MIN_DISTANCE = 8.0f;
    constexpr int32_t MULTI_TAP_TIMER = 250; // ms
    constexpr int32_t LONG_PRESS_TIMER = 300; // ms
    constexpr int64_t US_TO_MS = 1000;
    constexpr float DOUBLE_TAP_SLOP = 100.0f;
    constexpr float HALF = 0.5f;
    constexpr uint32_t DOUBLE = 2;
    constexpr uint32_t TRIPLE_TAP_COUNT = 3;
    constexpr float DEFAULT_SCALE = 2.0f;
    constexpr float NORMAL_SCALE = 1.0f;
    constexpr float MAX_SCALE = 8.0f;
    constexpr uint32_t INPUT_METHOD_WINDOW_TYPE = 2105;
    constexpr float EPS = 1e-6;
    constexpr float MIN_SCROLL_SPAN = 2.0f;
    constexpr float MIN_SCALE_SPAN = 2.0f;
    constexpr float DEFAULT_ANCHOR = 0.5f;
} // namespace

AccessibilityZoomGesture::AccessibilityZoomGesture()
{
    HILOG_DEBUG();

    zoomGestureEventHandler_ = std::make_shared<ZoomGestureEventHandler>(
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetMainRunner(), *this);

    tapDistance_ = TAP_MIN_DISTANCE;

#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    auto display = displayMgr.GetDefaultDisplay();
    if (!display) {
        HILOG_ERROR("get display is nullptr");
        return;
    }

    float densityPixels = display->GetVirtualPixelRatio();
    multiTapDistance_ = densityPixels * DOUBLE_TAP_SLOP + 0.5f;
#else
    HILOG_DEBUG("not support display manager");
    multiTapDistance_ = 1 * DOUBLE_TAP_SLOP + 0.5f;
#endif
}

bool AccessibilityZoomGesture::IsTapOnInputMethod(MMI::PointerEvent &event)
{
    size_t pointerCount = event.GetPointerIds().size();
    if (pointerCount != POINTER_COUNT_1) {
        HILOG_DEBUG("not single finger.");
        return false;
    }
    std::vector<AccessibilityWindowInfo> windowInfos =
        Singleton<AccessibilityWindowManager>::GetInstance().GetAccessibilityWindows();
    for (auto &window : windowInfos) {
        if (window.GetWindowType() == INPUT_METHOD_WINDOW_TYPE) {
            Rect inputRect = window.GetRectInScreen();
            int32_t leftTopX = inputRect.GetLeftTopXScreenPostion();
            int32_t leftTopY = inputRect.GetLeftTopYScreenPostion();
            int32_t rightBottomX = inputRect.GetRightBottomXScreenPostion();
            int32_t rightBottomY = inputRect.GetRightBottomYScreenPostion();
            
            MMI::PointerEvent::PointerItem item;
            event.GetPointerItem(event.GetPointerId(), item);
            int32_t itemX = item.GetDisplayX();
            int32_t itemY = item.GetDisplayY();
            if ((itemX >= leftTopX) && (itemX <= rightBottomX) &&
            (itemY >= leftTopY) && (itemY <= rightBottomY)) {
                HILOG_INFO("tap on input method window.");
                return true;
            }
        }
    }
    HILOG_DEBUG("have no input method window.");
    return false;
}

bool AccessibilityZoomGesture::OnPointerEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG("state_ is %{public}d.", state_);

    int32_t sourceType = event.GetSourceType();
    if (sourceType != MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN) {
        EventTransmission::OnPointerEvent(event);
        return false;
    }

    if (IsTapOnInputMethod(event)) {
        EventTransmission::OnPointerEvent(event);
        return true;
    }

    switch (state_) {
        case READY_STATE:
            CacheEvents(event);
            RecognizeInReadyState(event);
            break;
        case ZOOMIN_STATE:
            CacheEvents(event);
            RecognizeInZoomState(event);
            break;
        case SLIDING_STATE:
            RecognizeInSlidingState(event);
            break;
        default:
            break;
    }
    return true;
}

void AccessibilityZoomGesture::TransferState(ACCESSIBILITY_ZOOM_STATE state)
{
    HILOG_DEBUG("old state= %{public}d, new state= %{public}d", state_, state);

    state_ = state;
}

void AccessibilityZoomGesture::CacheEvents(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointerIds().size();
    std::shared_ptr<MMI::PointerEvent> pointerEvent = std::make_shared<MMI::PointerEvent>(event);

    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            if (pointerCount == POINTER_COUNT_1) {
                HILOG_DEBUG("Cache pointer down");
                preLastDownEvent_ = lastDownEvent_;
                lastDownEvent_ = pointerEvent;
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if (pointerCount == POINTER_COUNT_1) {
                HILOG_DEBUG("Cache pointer up");
                preLastUpEvent_ = lastUpEvent_;
                lastUpEvent_ = pointerEvent;
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            if (pointerCount == POINTER_COUNT_1) {
                HILOG_DEBUG("Cache pointer move.");
                currentMoveEvent_ = pointerEvent;
            }
            break;
        default:
            HILOG_DEBUG("Action is %{public}d", action);
            break;
    }
    cacheEvents_.emplace_back(pointerEvent);
}

void AccessibilityZoomGesture::SendCacheEventsToNext()
{
    HILOG_DEBUG();

    bool isStartNewAction = false;
    int32_t action = MMI::PointerEvent::POINTER_ACTION_UNKNOWN;
    std::vector<std::shared_ptr<MMI::PointerEvent>> cacheEventsTmp;
    std::copy(cacheEvents_.begin(), cacheEvents_.end(), std::back_inserter(cacheEventsTmp));

    ClearCacheEventsAndMsg();

    size_t cacheEventsNum = 0;
    size_t cacheEventsTotalNum = cacheEventsTmp.size();
    for (auto &pointerEvent : cacheEventsTmp) {
        cacheEventsNum++;
        action = pointerEvent->GetPointerAction();
        if ((cacheEventsNum > 1) &&
            (cacheEventsNum == cacheEventsTotalNum) &&
            (action == MMI::PointerEvent::POINTER_ACTION_DOWN)) {
            HILOG_DEBUG("The down event needs to be parsed again");
            isStartNewAction = true;
        }
        if (isStartNewAction) {
            OnPointerEvent(*pointerEvent);
        } else {
            pointerEvent->SetActionTime(Utils::GetSystemTime() * US_TO_MS);
            EventTransmission::OnPointerEvent(*pointerEvent);
        }
    }
}

void AccessibilityZoomGesture::ClearCacheEventsAndMsg()
{
    HILOG_DEBUG();

    cacheEvents_.clear();
    preLastDownEvent_ = nullptr;
    lastDownEvent_ = nullptr;
    preLastUpEvent_ = nullptr;
    lastUpEvent_ = nullptr;
    zoomGestureEventHandler_->RemoveEvent(MULTI_TAP_MSG);
}

void AccessibilityZoomGesture::RecognizeInReadyState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointerIds().size();
    bool isTripleTaps = false;

    HILOG_DEBUG("action:%{public}d, pointerCount:%{public}zu", action, pointerCount);
    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            zoomGestureEventHandler_->RemoveEvent(MULTI_TAP_MSG);
            if ((pointerCount == POINTER_COUNT_1) && IsDownValid()) {
                zoomGestureEventHandler_->SendEvent(MULTI_TAP_MSG, 0, MULTI_TAP_TIMER);
            } else {
                SendCacheEventsToNext();
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if ((pointerCount == POINTER_COUNT_1) && IsUpValid()) {
                isTripleTaps = IsTripleTaps();
            } else {
                SendCacheEventsToNext();
                HILOG_DEBUG("action:%{public}d, pointerCount:%{public}zu", action, pointerCount);
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            if ((pointerCount == POINTER_COUNT_1) && IsMoveValid()) {
                HILOG_DEBUG("move valid.");
            } else {
                SendCacheEventsToNext();
                HILOG_DEBUG("action:%{public}d, pointerCount:%{public}zu", action, pointerCount);
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_CANCEL:
            SendCacheEventsToNext();
            break;
        default:
            break;
    }

    if (isTripleTaps) {
        OnTripleTaps(event);
    }
}

void AccessibilityZoomGesture::RecognizeInZoomStateDownEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    std::vector<int32_t> pointerIdList = event.GetPointerIds();
    size_t pointerCount = pointerIdList.size();
    zoomGestureEventHandler_->RemoveEvent(MULTI_TAP_MSG);
    if (pointerCount == POINTER_COUNT_1) {
        isLongPress_ = false;
        std::shared_ptr<MMI::PointerEvent> pointerEvent = std::make_shared<MMI::PointerEvent>(event);
        longPressDownEvent_ = pointerEvent;
        downPid_ = event.GetPointerId();
        if (IsDownValid()) {
            zoomGestureEventHandler_->SendEvent(MULTI_TAP_MSG, 0, MULTI_TAP_TIMER);
        } else {
            SendCacheEventsToNext();
        }
    } else if (pointerCount == POINTER_COUNT_2) {
        if (isLongPress_ || IsKnuckles(event)) {
            HILOG_INFO("not transferState sliding.");
            SendCacheEventsToNext();
        } else {
            TransferState(SLIDING_STATE);
            ClearCacheEventsAndMsg();
            ZOOM_FOCUS_COORDINATE focusXY = {0.0f, 0.0f};
            CalcFocusCoordinate(event, focusXY);
            lastScrollFocusX_ = focusXY.centerX;
            lastScrollFocusY_ = focusXY.centerY;
            float span = CalcScaleSpan(event, focusXY);
            preSpan_ = lastSpan_ = span;
        }
    } else {
        HILOG_INFO("invalid pointer count.");
    }
}

void AccessibilityZoomGesture::RecognizeInZoomState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    std::vector<int32_t> pointerIdList = event.GetPointerIds();
    size_t pointerCount = pointerIdList.size();
    bool isTripleTaps = false;

    HILOG_DEBUG("action:%{public}d, pointerCount:%{public}zu", action, pointerCount);
    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            RecognizeInZoomStateDownEvent(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if (downPid_ == event.GetPointerId()) {
                isLongPress_ = false;
            }
            if ((pointerCount == POINTER_COUNT_1) && IsUpValid()) {
                isTripleTaps = IsTripleTaps();
            } else {
                SendCacheEventsToNext();
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            if ((pointerCount == POINTER_COUNT_1) && !IsLongPress() && IsMoveValid()) {
                HILOG_DEBUG("move valid.");
            } else {
                SendCacheEventsToNext();
                HILOG_DEBUG("action:%{public}d, pointerCount:%{public}zu", action, pointerCount);
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_CANCEL:
            SendCacheEventsToNext();
            HILOG_DEBUG("action:%{public}d", action);
            break;
        default:
            break;
    }

    if (isTripleTaps) {
        OnTripleTaps(event);
    }
}

void AccessibilityZoomGesture::RecognizeInSlidingState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointerIds().size();
    ZOOM_FOCUS_COORDINATE coordinate = {0.0f, 0.0f};
    CalcFocusCoordinate(event, coordinate);

    if (pointerCount == POINTER_COUNT_2) {
        RecognizeScale(event, coordinate);
        RecognizeScroll(event, coordinate);
    }

    HILOG_DEBUG("action:%{public}d, pointerCount:%{public}zu", action, pointerCount);
    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if (pointerCount == POINTER_COUNT_1) {
                TransferState(ZOOMIN_STATE);
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_CANCEL:
            TransferState(ZOOMIN_STATE);
            break;
        default:
            break;
    }
}

void AccessibilityZoomGesture::RecognizeScroll(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
        case MMI::PointerEvent::POINTER_ACTION_UP:
            lastScrollFocusX_ = coordinate.centerX;
            lastScrollFocusY_ = coordinate.centerY;
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE: {
            float offsetX = coordinate.centerX - lastScrollFocusX_;
            float offsetY = coordinate.centerY - lastScrollFocusY_;
            if ((abs(offsetX) > MIN_SCROLL_SPAN) || (abs(offsetY) > MIN_SCROLL_SPAN)) {
                lastScrollFocusX_ = coordinate.centerX;
                lastScrollFocusY_ = coordinate.centerY;
                OnScroll(offsetX, offsetY);
            }
            break;
        }
        default:
            break;
    }
}

void AccessibilityZoomGesture::RecognizeScale(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointerIds().size();
    if (((action == MMI::PointerEvent::POINTER_ACTION_UP) && (pointerCount != POINTER_COUNT_2)) ||
        (action == MMI::PointerEvent::POINTER_ACTION_CANCEL)) {
        HILOG_DEBUG("Scaling is end");
        startScaling_ = false;
        preSpan_ = lastSpan_ = 0;
        return;
    }

    float span = CalcScaleSpan(event, coordinate);

    if (action == MMI::PointerEvent::POINTER_ACTION_MOVE) {
        if (abs(preSpan_ - span) >= MIN_SCALE_SPAN) {
            startScaling_ = true;
            HILOG_DEBUG("start scaling.");
        }
    }
    if (!startScaling_) {
        // When the span is greater than or equal to MIN_SCALE_SPAN, start scaling.
        if (abs(preSpan_ - span) >= MIN_SCALE_SPAN) {
            startScaling_ = true;
            HILOG_DEBUG("start scaling.");
        }
    } else {
        // When the span is smaller than the MIN_SCALE_SPAN,
        // the scale recognition will be restarted.
        if (abs(lastSpan_ - span) < 1) {
            startScaling_ = false;
            preSpan_ = lastSpan_ = span;
        }
        if ((action == MMI::PointerEvent::POINTER_ACTION_UP) ||
            (action == MMI::PointerEvent::POINTER_ACTION_DOWN)) {
            preSpan_ = lastSpan_ = span;
        }
    }

    if (!startScaling_) {
        HILOG_DEBUG("Current is not scaling");
        return;
    }

    if (action != MMI::PointerEvent::POINTER_ACTION_MOVE) {
        HILOG_DEBUG("Action(%{public}d) is not move", action);
        return;
    }

    float scaleSpan = (span - lastSpan_) * scaleRatio_;
    if (fabs(scaleSpan) < FLT_EPSILON) {
        OnScale(scaleSpan);
        lastSpan_ = span;
    }
}

void AccessibilityZoomGesture::CalcFocusCoordinate(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate)
{
    HILOG_DEBUG();

    float sumX = 0.0f;
    float sumY = 0.0f;
    int32_t upPointerId = -1;
    int32_t action = event.GetPointerAction();
    std::vector<int32_t> pointerIdList = event.GetPointerIds();
    size_t count = pointerIdList.size();
    if (!count) {
        HILOG_DEBUG("The size of PointerIds is 0");
        return;
    }

    if (action == MMI::PointerEvent::POINTER_ACTION_UP) {
        upPointerId = event.GetPointerId();
        HILOG_DEBUG("The pointer id of up is %{public}d", upPointerId);
        count--;
    }

    if (!count) {
        HILOG_DEBUG("The size of PointerIds(down) is invalid");
        return;
    }

    for (int32_t pointerId : pointerIdList) {
        if (pointerId == upPointerId) {
            continue;
        }
        MMI::PointerEvent::PointerItem item;
        event.GetPointerItem(pointerId, item);
        sumX += static_cast<float>(item.GetRawDisplayX());
        sumY += static_cast<float>(item.GetRawDisplayY());
    }

    coordinate.centerX = sumX / count;
    coordinate.centerY = sumY / count;
    HILOG_DEBUG("centerX:%{public}f, centerY:%{public}f", coordinate.centerX, coordinate.centerY);
}

float AccessibilityZoomGesture::CalcScaleSpan(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE coordinate)
{
    HILOG_DEBUG();

    float span = 0.0f;
    float sumSpanX = 0.0f;
    float sumSpanY = 0.0f;
    int32_t upPointerId = -1;
    int32_t action = event.GetPointerAction();
    std::vector<int32_t> pointerIdList = event.GetPointerIds();
    size_t count = pointerIdList.size();
    if (!count) {
        HILOG_DEBUG("The size of PointerIds is 0");
        return span;
    }

    if (action == MMI::PointerEvent::POINTER_ACTION_UP) {
        upPointerId = event.GetPointerId();
        HILOG_DEBUG("The pointer id of up is %{public}d", upPointerId);
        count--;
    }

    if (!count) {
        HILOG_DEBUG("The size of PointerIds(down) is invalid");
        return span;
    }

    for (int32_t pointerId : pointerIdList) {
        if (pointerId == upPointerId) {
            continue;
        }
        MMI::PointerEvent::PointerItem item;
        event.GetPointerItem(pointerId, item);
        sumSpanX += static_cast<float>(abs(item.GetRawDisplayX() - coordinate.centerX));
        sumSpanY += static_cast<float>(abs(item.GetRawDisplayY() - coordinate.centerY));
    }

    float spanX = sumSpanX / count;
    float spanY = sumSpanY / count;
    span = hypot(spanX, spanY) / HALF;
    HILOG_DEBUG("The span is %{public}f", span);
    return span;
}

bool AccessibilityZoomGesture::IsDownValid()
{
    HILOG_DEBUG();

    if (!preLastDownEvent_) {
        HILOG_DEBUG("This is the first down event");
        return true;
    }

    if (CalcSeparationDistance(preLastDownEvent_, lastDownEvent_) >= multiTapDistance_) {
        HILOG_DEBUG("The down event is vailid");
        return false;
    }
    return true;
}

bool AccessibilityZoomGesture::IsUpValid()
{
    HILOG_DEBUG();

    if (!lastDownEvent_) {
        HILOG_DEBUG("The up event is invailid");
        return false;
    }

    if (CalcIntervalTime(lastDownEvent_, lastUpEvent_) >= LONG_PRESS_TIMER) {
        HILOG_DEBUG("The time has exceeded the long press time");
        return false;
    }

    if (CalcSeparationDistance(lastDownEvent_, lastUpEvent_) >= tapDistance_) {
        HILOG_DEBUG("The distance has exceeded the threshold");
        return false;
    }
    return true;
}

bool AccessibilityZoomGesture::IsMoveValid()
{
    HILOG_DEBUG();

    if (!lastDownEvent_) {
        HILOG_DEBUG("The move event is invailid");
        return false;
    }

    if (CalcIntervalTime(lastDownEvent_, currentMoveEvent_) >= LONG_PRESS_TIMER) {
        HILOG_DEBUG("The time has exceeded the long press time");
        return false;
    }

    if (CalcSeparationDistance(lastDownEvent_, currentMoveEvent_) >= tapDistance_) {
        HILOG_DEBUG("The distance has exceeded the threshold");
        return false;
    }
    return true;
}

bool AccessibilityZoomGesture::IsLongPress()
{
    HILOG_DEBUG();

    if (CalcIntervalTime(longPressDownEvent_, currentMoveEvent_) >= LONG_PRESS_TIMER) {
        HILOG_DEBUG("The time has exceeded the long press time");
        isLongPress_ = true;
        return true;
    }
    return false;
}

bool AccessibilityZoomGesture::IsKnuckles(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    std::vector<int32_t> pointerIdList = event.GetPointerIds();
    for (int32_t pointerId : pointerIdList) {
        MMI::PointerEvent::PointerItem item;
        event.GetPointerItem(pointerId, item);
        int32_t toolType = item.GetToolType();
        if (toolType == MMI::PointerEvent::TOOL_TYPE_KNUCKLE) {
            HILOG_INFO("is knuckle event.");
            return true;
        }
    }
    return false;
}

bool AccessibilityZoomGesture::IsTripleTaps()
{
    HILOG_DEBUG();

    uint32_t upEventCount = 0;
    int32_t action = MMI::PointerEvent::POINTER_ACTION_UNKNOWN;
    for (auto &pointerEvent : cacheEvents_) {
        action = pointerEvent->GetPointerAction();
        if (action == MMI::PointerEvent::POINTER_ACTION_UP) {
            upEventCount++;
        }
    }

    if (upEventCount >= TRIPLE_TAP_COUNT) {
        HILOG_DEBUG("Triple tap detected");
        return true;
    }

    return false;
}

int64_t AccessibilityZoomGesture::CalcIntervalTime(std::shared_ptr<MMI::PointerEvent> firstEvent,
    std::shared_ptr<MMI::PointerEvent> secondEvent)
{
    HILOG_DEBUG();

    if (!firstEvent || !secondEvent) {
        HILOG_DEBUG("The event is null");
        return 0;
    }

    int64_t firstTime = firstEvent->GetActionTime();
    int64_t secondTime = secondEvent->GetActionTime();
    int64_t intervalTime = (secondTime - firstTime) / US_TO_MS;

    return intervalTime;
}

float AccessibilityZoomGesture::CalcSeparationDistance(std::shared_ptr<MMI::PointerEvent> firstEvent,
    std::shared_ptr<MMI::PointerEvent> secondEvent)
{
    HILOG_DEBUG();

    if (!firstEvent || !secondEvent) {
        HILOG_DEBUG("The event is null");
        return 0;
    }

    MMI::PointerEvent::PointerItem firstItem;
    MMI::PointerEvent::PointerItem secondItem;
    firstEvent->GetPointerItem(firstEvent->GetPointerId(), firstItem);
    secondEvent->GetPointerItem(secondEvent->GetPointerId(), secondItem);
    int32_t durationX = secondItem.GetDisplayX() - firstItem.GetDisplayX();
    int32_t durationY = secondItem.GetDisplayY() - firstItem.GetDisplayY();
    float distance = static_cast<float>(hypot(durationX, durationY));

    return distance;
}

void AccessibilityZoomGesture::OnTripleTaps(MMI::PointerEvent &event)
{
    HILOG_DEBUG("state_ is %{public}d.", state_);

    switch (state_) {
        case READY_STATE: {
            TransferState(ZOOMIN_STATE);
            int32_t pointerId = event.GetPointerId();
            MMI::PointerEvent::PointerItem item;
            event.GetPointerItem(pointerId, item);
            int32_t anchorX = item.GetDisplayX();
            int32_t anchorY = item.GetDisplayY();
            HILOG_DEBUG("anchorX:%{private}d, anchorY:%{private}d.", anchorX, anchorY);
            OnZoom(anchorX, anchorY);
            break;
        }
        case ZOOMIN_STATE:
            TransferState(READY_STATE);
            OffZoom();
            break;
        default:
            break;
    }

    ClearCacheEventsAndMsg();
}

AccessibilityZoomGesture::ZoomGestureEventHandler::ZoomGestureEventHandler(
    const std::shared_ptr<AppExecFwk::EventRunner> &runner,
    AccessibilityZoomGesture &zoomGesture): AppExecFwk::EventHandler(runner), zoomGesture_(zoomGesture)
{
    HILOG_DEBUG();
}

void AccessibilityZoomGesture::ZoomGestureEventHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    HILOG_DEBUG();

    uint32_t eventId = event->GetInnerEventId();
    
    switch (eventId) {
        case MULTI_TAP_MSG:
            zoomGesture_.SendCacheEventsToNext();
            break;
        default:
            break;
    }
}

void AccessibilityZoomGesture::GetWindowParam()
{
    HILOG_DEBUG();
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    uint64_t currentScreen = displayMgr.GetDefaultDisplayId();
    OHOS::Rosen::DisplayOrientation currentOrientation = displayMgr.GetOrientation();
    if ((currentScreen != screenId_) || (currentOrientation != orientation_)) {
        HILOG_INFO("display id or orientation changed.");
        screenId_ = currentScreen;
        orientation_ = currentOrientation;
        sptr<Rosen::Display> display = displayMgr.GetDisplay(screenId_);
        screenWidth_ = static_cast<uint32_t>(display->GetWidth());
        screenHeight_ = static_cast<uint32_t>(display->GetHeight());
        HILOG_INFO("screenWidth_ = %{public}d, screenHeight_ = %{public}d.", screenWidth_, screenHeight_);
    }
    screenSpan_ = hypot(screenWidth_, screenHeight_);
#else
    HILOG_INFO("not support zoom");
#endif
}

void AccessibilityZoomGesture::OnZoom(int32_t anchorX, int32_t anchorY)
{
    HILOG_INFO();
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    GetWindowParam();
    if (screenWidth_ == 0 || screenHeight_ == 0) {
        HILOG_ERROR("screen param invalid.");
        return;
    }
    anchorPointX_ = static_cast<float>(anchorX);
    anchorPointY_ = static_cast<float>(anchorY);

    float x = anchorPointX_ / screenWidth_;
    float y = anchorPointY_ / screenHeight_;
    scaleRatio_ = DEFAULT_SCALE;
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    displayMgr.SetDisplayScale(screenId_, scaleRatio_, scaleRatio_, x, y);
#else
    HILOG_INFO("not support zoom");
    return;
#endif
}

void AccessibilityZoomGesture::OffZoom()
{
    HILOG_INFO();
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    uint64_t currentScreen = displayMgr.GetDefaultDisplayId();
    displayMgr.SetDisplayScale(currentScreen, NORMAL_SCALE, NORMAL_SCALE, DEFAULT_ANCHOR, DEFAULT_ANCHOR);
#else
    HILOG_INFO("not support zoom");
    return;
#endif
}

void AccessibilityZoomGesture::OnScroll(float offsetX, float offsetY)
{
    HILOG_DEBUG("offsetX:%{public}f, offsetY:%{public}f.", offsetX, offsetY);
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    GetWindowParam();
    if (screenWidth_ == 0 || screenHeight_ == 0) {
        HILOG_ERROR("screen param invalid.");
        return;
    }

    if (abs(scaleRatio_) < EPS) {
        HILOG_ERROR("scaleRatio_ param invalid.");
        return;
    }
    anchorPointX_ -= (offsetX * DOUBLE / scaleRatio_);
    anchorPointY_ -= (offsetY * DOUBLE / scaleRatio_);

    if (anchorPointX_ < 0) {
        anchorPointX_ = 0;
    }
    if (anchorPointX_ > screenWidth_) {
        anchorPointX_ = screenWidth_;
    }
    if (anchorPointY_ < 0) {
        anchorPointY_ = 0;
    }
    if (anchorPointY_ > screenHeight_) {
        anchorPointY_ = screenHeight_;
    }

    float x = anchorPointX_ / screenWidth_;
    float y = anchorPointY_ / screenHeight_;
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    displayMgr.SetDisplayScale(screenId_, scaleRatio_, scaleRatio_, x, y);
#ifdef OHOS_BUILD_ENABLE_POWER_MANAGER
    AccessibilityPowerManager &powerMgr = Singleton<AccessibilityPowerManager>::GetInstance();
    powerMgr.RefreshActivity();
#endif
#else
    HILOG_INFO("not support zoom");
    return;
#endif
}

void AccessibilityZoomGesture::OnScale(float scaleSpan)
{
    HILOG_DEBUG();
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    GetWindowParam();
    if (screenWidth_ == 0 || screenHeight_ == 0 || abs(screenSpan_) < EPS) {
        HILOG_ERROR("screen param invalid.");
        return;
    }

    float ratio = scaleSpan / screenSpan_;
    scaleRatio_ = scaleRatio_ + ratio;
    if (scaleRatio_ > MAX_SCALE) {
        scaleRatio_ = MAX_SCALE;
    }
    if (scaleRatio_ < DEFAULT_SCALE) {
        scaleRatio_ = DEFAULT_SCALE;
    }

    float x = anchorPointX_ / screenWidth_;
    float y = anchorPointY_ / screenHeight_;
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    displayMgr.SetDisplayScale(screenId_, scaleRatio_, scaleRatio_, x, y);
#else
    HILOG_INFO("not support zoom");
    return;
#endif
}

void AccessibilityZoomGesture::Clear()
{
    HILOG_DEBUG();
    ClearCacheEventsAndMsg();
    TransferState(READY_STATE);
}

void AccessibilityZoomGesture::DestroyEvents()
{
    HILOG_INFO();
    Clear();
    EventTransmission::DestroyEvents();
}
} // namespace Accessibility
} // namespace OHOS
