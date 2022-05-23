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

namespace OHOS {
namespace Accessibility {
const size_t POINTER_COUNT_1 = 1;
const float TAP_MIN_DISTANCE = 8.0f;
const int32_t MULTI_TAP_TIMER = 300; // ms
const int32_t LONG_PRESS_TIMER = 500; // ms
const int64_t US_TO_MS = 1000;
const float MIN_SCALE_SPAN = 27.0f; // 27mm
const float DOUBLE_TAP_SLOP = 100.0f;
const float HALF = 0.5f;
const uint32_t TRIPLE_TAP_COUNT = 3;

AccessibilityZoomGesture::AccessibilityZoomGesture(Rosen::DisplayId displayId)
{
    HILOG_DEBUG();

    zoomHandler_ = std::make_shared<AccessibilityZoomHandler>(displayId);
    zoomGestureEventHandler_ = std::make_shared<ZoomGestureEventHandler>(
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetMainRunner(), *this);

    tapDistance_ = TAP_MIN_DISTANCE;

    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    auto display = displayMgr.GetDefaultDisplay();
    if (!display) {
        HILOG_ERROR("get display is nullptr");
        return;
    }

    float densityPixels = display->GetVirtualPixelRatio();
    multiTapDistance_ = densityPixels * DOUBLE_TAP_SLOP + 0.5f;
}

void AccessibilityZoomGesture::OnPointerEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG("state_ is %{public}d.", state_);

    int32_t sourceType = event.GetSourceType();
    if (sourceType != MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN) {
        EventTransmission::OnPointerEvent(event);
        return;
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
}

void AccessibilityZoomGesture::TransferState(ACCESSIBILITY_ZOOM_STATE state)
{
    HILOG_DEBUG("state:%{public}d.", state);

    state_ = state;
}

void AccessibilityZoomGesture::CacheEvents(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointersIdList().size();
    std::shared_ptr<MMI::PointerEvent> pointerEvent = std::make_shared<MMI::PointerEvent>(event);

    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            if (pointerCount == POINTER_COUNT_1) {
                preLastDownEvent_ = lastDownEvent_;
                lastDownEvent_ = pointerEvent;
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if (pointerCount == POINTER_COUNT_1) {
                preLastUpEvent_ = lastUpEvent_;
                lastUpEvent_ = pointerEvent;
            }
            break;
        default:
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
    for (std::shared_ptr<MMI::PointerEvent> pointerEvent : cacheEvents_) {
        cacheEventsTmp.emplace_back(pointerEvent);
    }

    ClearCacheEventsAndMsg();

    size_t cacheEventsNum = 0;
    size_t cacheEventsTotalNum = cacheEventsTmp.size();
    for (std::shared_ptr<MMI::PointerEvent> pointerEvent : cacheEventsTmp) {
        cacheEventsNum++;
        action = pointerEvent->GetPointerAction();
        if ((cacheEventsNum > 1) &&
            (cacheEventsNum == cacheEventsTotalNum) &&
            (action == MMI::PointerEvent::POINTER_ACTION_DOWN)) {
            isStartNewAction = true;
        }
        if (isStartNewAction) {
            OnPointerEvent(*pointerEvent);
        } else {
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
    size_t pointerCount = event.GetPointersIdList().size();
    bool isTripleTaps = false;

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

void AccessibilityZoomGesture::RecognizeInZoomState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointersIdList().size();
    bool isTripleTaps = false;

    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            zoomGestureEventHandler_->RemoveEvent(MULTI_TAP_MSG);
            if (pointerCount == POINTER_COUNT_1) {
                if (IsDownValid()) {
                    zoomGestureEventHandler_->SendEvent(MULTI_TAP_MSG, 0, MULTI_TAP_TIMER);
                } else {
                    SendCacheEventsToNext();
                }
            } else if (pointerCount > POINTER_COUNT_1) {
                TransferState(SLIDING_STATE);
                ClearCacheEventsAndMsg();
                ZOOM_FOCUS_COORDINATE focusXY = {0.0f, 0.0f};
                CalcFocusCoordinate(event, focusXY);
                // Used for scroll algorithm.
                lastScrollFocusX_ = focusXY.centerX;
                lastScrollFocusY_ = focusXY.centerY;

                // Used for scale algorithm.
                float span = CalcScaleSpan(event, focusXY);
                if (span >= MIN_SCALE_SPAN) {
                    startScaling_ = true;
                    preSpan_ = lastSpan_ = span;
                }
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if ((pointerCount == POINTER_COUNT_1) && IsUpValid()) {
                isTripleTaps = IsTripleTaps();
            } else {
                SendCacheEventsToNext();
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

void AccessibilityZoomGesture::RecognizeInSlidingState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointersIdList().size();

    // Recognize scroll and zoom gestures.
    RecognizeScroll(event);
    RecognizeScale(event);

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

void AccessibilityZoomGesture::RecognizeScroll(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    ZOOM_FOCUS_COORDINATE coordinate = {0.0f, 0.0f};
    CalcFocusCoordinate(event, coordinate);

    switch (action) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            lastScrollFocusX_ = coordinate.centerX;
            lastScrollFocusY_ = coordinate.centerY;
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE: {
            float offsetX = lastScrollFocusX_ - coordinate.centerX;
            float offsetY = lastScrollFocusY_ - coordinate.centerY;
            if ((abs(offsetX) > 1) || (abs(offsetY) > 1)) {
                lastScrollFocusX_ = coordinate.centerX;
                lastScrollFocusY_ = coordinate.centerY;
                zoomHandler_->OnScroll(offsetX, offsetY);
            }
            break;
        }
        case MMI::PointerEvent::POINTER_ACTION_UP:
            lastScrollFocusX_ = coordinate.centerX;
            lastScrollFocusY_ = coordinate.centerY;
            break;
        default:
            break;
    }
}

void AccessibilityZoomGesture::RecognizeScale(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    int32_t action = event.GetPointerAction();
    size_t pointerCount = event.GetPointersIdList().size();
    if (((action == MMI::PointerEvent::POINTER_ACTION_UP) && (pointerCount == POINTER_COUNT_1)) ||
        (action == MMI::PointerEvent::POINTER_ACTION_CANCEL)) {
        startScaling_ = false;
        preSpan_ = lastSpan_ = 0;
        return;
    }

    ZOOM_FOCUS_COORDINATE focusXY = {0.0f, 0.0f};
    CalcFocusCoordinate(event, focusXY);
    float span = CalcScaleSpan(event, focusXY);
    if (!startScaling_) {
        // When the span is greater than or equal to MIN_SCALE_SPAN, start scaling.
        if (span >= MIN_SCALE_SPAN) {
            startScaling_ = true;
            preSpan_ = lastSpan_ = span;
        }
    } else {
        // When the span is smaller than the MIN_SCALE_SPAN,
        // the scale recognition will be restarted.
        if (span < MIN_SCALE_SPAN) {
            startScaling_ = false;
            preSpan_ = lastSpan_ = span;
        }
        if ((action == MMI::PointerEvent::POINTER_ACTION_UP) ||
            (action == MMI::PointerEvent::POINTER_ACTION_DOWN)) {
            preSpan_ = lastSpan_ = span;
        }
    }

    if (!startScaling_) {
        return;
    }

    if (action != MMI::PointerEvent::POINTER_ACTION_MOVE) {
        return;
    }
    
    lastSpan_ = span;
    float ratio = lastSpan_ / preSpan_;
    if (ratio != 1) {
        zoomHandler_->OnScale(ratio, focusXY.centerX, focusXY.centerY);
        preSpan_ = lastSpan_;
    }
}

void AccessibilityZoomGesture::CalcFocusCoordinate(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate)
{
    HILOG_DEBUG();

    float sumX = 0.0f;
    float sumY = 0.0f;
    int32_t upPointerId = -1;
    int32_t action = event.GetPointerAction();
    std::vector<int32_t> pointerIdList = event.GetPointersIdList();
    size_t count = pointerIdList.size();
    if (!count) {
        return;
    }

    if (action == MMI::PointerEvent::POINTER_ACTION_UP) {
        upPointerId = event.GetPointerId();
        count--;
    }

    if (count <= 0) {
        return;
    }

    for (int32_t pointerId : pointerIdList) {
        if (pointerId == upPointerId) {
            continue;
        }
        MMI::PointerEvent::PointerItem item;
        event.GetPointerItem(pointerId, item);
        sumX += static_cast<float>(item.GetGlobalX());
        sumY += static_cast<float>(item.GetGlobalY());
    }

    coordinate.centerX = sumX / count;
    coordinate.centerY = sumY / count;
}

float AccessibilityZoomGesture::CalcScaleSpan(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE coordinate)
{
    HILOG_DEBUG();

    float span = 0.0f;
    float sumSpanX = 0.0f;
    float sumSpanY = 0.0f;
    int32_t upPointerId = -1;
    int32_t action = event.GetPointerAction();
    std::vector<int32_t> pointerIdList = event.GetPointersIdList();
    size_t count = pointerIdList.size();
    if (!count) {
        return span;
    }

    if (action == MMI::PointerEvent::POINTER_ACTION_UP) {
        upPointerId = event.GetPointerId();
        count--;
    }

    if (count <= 0) {
        return span;
    }

    for (int32_t pointerId : pointerIdList) {
        if (pointerId == upPointerId) {
            continue;
        }
        MMI::PointerEvent::PointerItem item;
        event.GetPointerItem(pointerId, item);
        sumSpanX += static_cast<float>(abs(item.GetGlobalX() - coordinate.centerX));
        sumSpanY += static_cast<float>(abs(item.GetGlobalY() - coordinate.centerY));
    }

    float spanX = sumSpanX / count;
    float spanY = sumSpanY / count;
    span = hypot(spanX, spanY) / HALF;
    return span;
}

bool AccessibilityZoomGesture::IsDownValid()
{
    HILOG_DEBUG();

    if (!preLastDownEvent_) {
        return true;
    }

    if (CalcSeparationDistance(preLastDownEvent_, lastDownEvent_) >= multiTapDistance_) {
        return false;
    }
    return true;
}

bool AccessibilityZoomGesture::IsUpValid()
{
    HILOG_DEBUG();

    if (!lastDownEvent_) {
        return false;
    }

    if (CalcIntervalTime(lastDownEvent_, lastUpEvent_) >= LONG_PRESS_TIMER) {
        return false;
    }

    if (CalcSeparationDistance(lastDownEvent_, lastUpEvent_) >= tapDistance_) {
        return false;
    }
    return true;
}

bool AccessibilityZoomGesture::IsTripleTaps()
{
    HILOG_DEBUG();

    uint32_t upEventCount = 0;
    int32_t action = MMI::PointerEvent::POINTER_ACTION_UNKNOWN;
    for (std::shared_ptr<MMI::PointerEvent> pointerEvent : cacheEvents_) {
        action = pointerEvent->GetPointerAction();
        if (action == MMI::PointerEvent::POINTER_ACTION_UP) {
            upEventCount++;
        }
    }

    if (upEventCount >= TRIPLE_TAP_COUNT) {
        return true;
    }

    return false;
}

int64_t AccessibilityZoomGesture::CalcIntervalTime(std::shared_ptr<MMI::PointerEvent> firstEvent,
    std::shared_ptr<MMI::PointerEvent> secondEvent)
{
    HILOG_DEBUG();

    if (!firstEvent || !secondEvent) {
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
        return 0;
    }

    MMI::PointerEvent::PointerItem firstItem;
    MMI::PointerEvent::PointerItem secondItem;
    firstEvent->GetPointerItem(firstEvent->GetPointerId(), firstItem);
    secondEvent->GetPointerItem(secondEvent->GetPointerId(), secondItem);
    int32_t durationX = secondItem.GetGlobalX() - firstItem.GetGlobalX();
    int32_t durationY = secondItem.GetGlobalY() - firstItem.GetGlobalY();
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
            int32_t centerX = item.GetGlobalX();
            int32_t centerY = item.GetGlobalY();
            zoomHandler_->OnZoomIn(centerX, centerY);
            break;
        }
        case ZOOMIN_STATE:
            TransferState(READY_STATE);
            zoomHandler_->OnZoomOut();
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
} // namespace Accessibility
} // namespace OHOS
