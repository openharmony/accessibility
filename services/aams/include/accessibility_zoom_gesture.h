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

#ifndef ACCESSIBILITY_ZOOM_GESTURE_H
#define ACCESSIBILITY_ZOOM_GESTURE_H

#include "accessibility_event_transmission.h"
#include "event_handler.h"
#include "pointer_event.h"
#include "dm_common.h"
#include "full_screen_magnification_manager.h"
#include "magnification_menu_manager.h"

namespace OHOS {
namespace Accessibility {
enum ACCESSIBILITY_ZOOM_STATE {
    READY_STATE,
    ZOOMIN_STATE,
    SLIDING_STATE,
    MENU_SLIDING_STATE,
    DRAGGING_STATE
};

enum ACCESSIBILITY_ZOOM_GESTURE_MSG : uint32_t {
    MULTI_TAP_MSG,
};

struct ZOOM_FOCUS_COORDINATE {
    float centerX;
    float centerY;
};

class AccessibilityZoomGesture : public EventTransmission {
public:
    AccessibilityZoomGesture(std::shared_ptr<FullScreenMagnificationManager> fullScreenManager,
        std::shared_ptr<MagnificationMenuManager> menuManager);
    ~AccessibilityZoomGesture() = default;

    virtual bool OnPointerEvent(MMI::PointerEvent &event) override;
    void DestroyEvents() override;
    // flag = true shield zoom gesture | flag = false restore zoom gesture
    void ShieldZoomGesture(bool state);
    void GetWindowParam(bool needRefresh = false);
    void StartMagnificationInteract();
    void DisableGesture();
    inline ACCESSIBILITY_ZOOM_STATE GetZoomState()
    {
        return state_;
    }

private:
    class ZoomGestureEventHandler : public AppExecFwk::EventHandler {
    public:
        ZoomGestureEventHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner,
            AccessibilityZoomGesture &zoomGesture);
        virtual ~ZoomGestureEventHandler() = default;

        virtual void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;
    private:
        AccessibilityZoomGesture &zoomGesture_;
    };

    void TransferState(ACCESSIBILITY_ZOOM_STATE state);
    void CacheEvents(MMI::PointerEvent &event);
    void SendCacheEventsToNext();
    void ClearCacheEventsAndMsg();
    void RecognizeInReadyState(MMI::PointerEvent &event);
    void RecognizeInZoomStateDownEvent(MMI::PointerEvent &event);
    void RecognizeInZoomStateMoveEvent(MMI::PointerEvent &event);
    void RecognizeInZoomState(MMI::PointerEvent &event);
    void RecognizeInSlidingState(MMI::PointerEvent &event);
    void RecognizeInMenuSlidingState(MMI::PointerEvent &event);
    void RecognizeInDraggingState(MMI::PointerEvent &event);
    void RecognizeScroll(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate);
    void RecognizeScale(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate);
    void CalcFocusCoordinate(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE &coordinate);
    float CalcScaleSpan(MMI::PointerEvent &event, ZOOM_FOCUS_COORDINATE coordinate);
    bool IsTapOnInputMethod(MMI::PointerEvent &event);
    bool IsDownValid();
    bool IsUpValid();
    bool IsMoveValid();
    bool IsLongPress();
    bool IsKnuckles(MMI::PointerEvent &event);
    bool IsTripleTaps();
    void OnTripleTap(MMI::PointerEvent &event);
    int64_t CalcIntervalTime(std::shared_ptr<MMI::PointerEvent> firstEvent,
        std::shared_ptr<MMI::PointerEvent> secondEvent);
    float CalcSeparationDistance(std::shared_ptr<MMI::PointerEvent> firstEvent,
        std::shared_ptr<MMI::PointerEvent> secondEvent);
    void OnZoom(int32_t centerX, int32_t centerY, bool showMenu);
    void OffZoom();
    void OnScroll(float offsetX, float offsetY);
    void OnScale(float scaleSpan);
    void Clear();
    void OnDrag();

    bool startScaling_ = false;
    bool isLongPress_ = false;
    float preSpan_ = 0.0f;
    float lastSpan_ = 0.0f;
    float screenSpan_ = 0.0f;
    float lastScrollFocusX_ = 0.0f;
    float lastScrollFocusY_ = 0.0f;
    float tapDistance_ = 0.0f;
    float multiTapDistance_ = 0.0f;
    float scale_ = DEFAULT_SCALE;
    bool isScale_ = false;
    uint64_t screenId_ = -1;
    uint32_t screenWidth_ = 0;
    uint32_t screenHeight_ = 0;
    float anchorPointX_ = 0.0f;
    float anchorPointY_ = 0.0f;
    int32_t centerX_ = 0;
    int32_t centerY_ = 0;
    float scaleRatio_ = 2.0f;
    int32_t downPid_ = -1;
    ACCESSIBILITY_ZOOM_STATE state_ = READY_STATE;
    OHOS::Rosen::DisplayOrientation orientation_ =
        OHOS::Rosen::DisplayOrientation::UNKNOWN;
    std::shared_ptr<MMI::PointerEvent> preLastDownEvent_ = nullptr;
    std::shared_ptr<MMI::PointerEvent> lastDownEvent_ = nullptr;
    std::shared_ptr<MMI::PointerEvent> preLastUpEvent_ = nullptr;
    std::shared_ptr<MMI::PointerEvent> lastUpEvent_ = nullptr;
    std::shared_ptr<MMI::PointerEvent> currentMoveEvent_ = nullptr;
    std::shared_ptr<MMI::PointerEvent> longPressDownEvent_ = nullptr;
    std::shared_ptr<MMI::PointerEvent> lastSlidingEvent_ = nullptr;
    std::shared_ptr<ZoomGestureEventHandler> zoomGestureEventHandler_ = nullptr;
    std::vector<std::shared_ptr<MMI::PointerEvent>> cacheEvents_;
    std::shared_ptr<FullScreenMagnificationManager> fullScreenManager_ = nullptr;
    std::shared_ptr<MagnificationMenuManager> menuManager_ = nullptr;
    
    std::atomic<bool> shieldZoomGestureFlag_ = false;
    bool isTapOnMenu_ = false;
    uint32_t gestureType_ = 0;
    bool isTripleDown_ = false;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_ZOOM_GESTURE_H