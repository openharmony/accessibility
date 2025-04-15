/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_IPC_INTERFACE_CODE_H
#define ACCESSIBILITY_IPC_INTERFACE_CODE_H
/* SAID:3503 */
namespace OHOS {
namespace Accessibility {
    enum AccessibilityInterfaceCode {
        SET_RESULT_BY_ACCESSIBILITY_ID = 100,
        SET_RESULT_BY_TEXT,
        SET_RESULT_FOCUSED_INFO,
        SET_RESULT_FOCUS_MOVE,
        SET_RESULT_PERFORM_ACTION,
        SET_RESULT_CURSOR_RESULT,
        SET_RESULT_BY_WINDOW_ID,

        SEARCH_BY_ACCESSIBILITY_ID = 200,
        SEARCH_BY_TEXT,
        FIND_FOCUSED_INFO,
        FOCUS_FIND,
        PERFORM_ACTION_ELEMENT,
        CURSOR_POSITION,
        CLEAR_FOCUS,
        OUTSIDE_TOUCH,
        SET_CHILDTREEID,
        SET_BELONGTREEID,
        SET_PARENTWINDOWID,
        SEARCH_BY_WINDOW_ID,

        ON_ACCESSIBILITY_ENABLE_ABILITY_LISTS_CHANGED = 300,
        ON_ACCESSIBILITY_INSTALL_ABILITY_LISTS_CHANGED,

        SEARCH_ELEMENTINFO_BY_ACCESSIBILITY_ID = 400,
        SEARCH_ELEMENTINFOS_BY_TEXT,
        FIND_FOCUSED_ELEMENTINFO,
        FOCUS_MOVE_SEARCH,
        PERFORM_ACTION,
        SET_CURTAIN_SCREEN,
        GET_WINDOW,
        GET_WINDOWS,
        GET_WINDOWS_BY_DISPLAY_ID,
        DISABLE_ABILITY,
        SET_ON_KEY_PRESS_EVENT_RESULT,
        SEND_SIMULATE_GESTURE_PATH,
        SET_TARGET_BUNDLE_NAME,
        GET_CURSOR_POSITION,
        SEARCH_DEFAULTFOCUSED_BY_WINDOW_ID,
        HOLD_RUNNING_LOCK,
        UNHOLD_RUNNING_LOCK,
        
        INIT = 500,
        DISCONNECT,
        ON_ACCESSIBILITY_EVENT,
        ON_KEY_PRESS_EVENT,
        EXECUTE_DISCONNECT_CALLBACK,

        ON_PROPERTY_CHANGED = 600,

        ON_CONFIG_STATE_CHANGED = 700,
        ON_DALTONIZATION_COLOR_FILTER_CHANGED,
        ON_CONTENT_TIMEOUT_CHANGED,
        ON_MOUSE_AUTOCLICK_CHANGED,
        ON_BRIGHTNESS_DISCOUNT_CHANGED,
        ON_AUDIO_BALANCE_CHANGED,
        ON_SHORTKEY_TARGET_CHANGED,
        ON_CAPTION_STYLE_CHANGED,
        ON_CLICK_RESPONSE_TIME,
        ON_IGNORE_REPEAT_CLICK_TIME,
        ON_SHORTKEY_MULTI_TARGET_CHANGED,

        SEND_EVENT = 800,
        REGISTER_STATE_CALLBACK,
        GET_ABILITYLIST,
        REGISTER_INTERACTION_CONNECTION,
        CARDREGISTER_INTERACTION_CONNECTION,
        DEREGISTER_INTERACTION_CONNECTION,
        CARDDEREGISTER_INTERACTION_CONNECTION,
        GET_CAPTION_PROPERTY,
        SET_CAPTION_PROPERTY,
        SET_CAPTION_STATE,
        REGISTER_CAPTION_PROPERTY_CALLBACK,
        GET_ENABLED,
        GET_CAPTION_STATE,
        GET_TOUCH_GUIDE_STATE,
        GET_GESTURE_STATE,
        GET_KEY_EVENT_OBSERVE_STATE,
        ENABLE_ABILITIES,
        GET_ENABLED_OBJECT,
        DISABLE_ABILITIES,
        ENABLE_UI_TEST_ABILITY,
        DISABLE_UI_TEST_ABILITY,
        GET_ACTIVE_WINDOW,
        SET_SCREENMAGNIFIER_STATE,
        SET_SHORTKEY_STATE,
        SET_MOUSEKEY_STATE,
        SET_SHORTKEY_TARGET,
        SET_MOUSEKEY_AUTOCLICK,
        SET_INVERTCOLOR_STATE,
        SET_HIGHCONTRASTTEXT_STATE,
        SET_AUDIOMONO_STATE,
        SET_ANIMATIONOFF_STATE,
        SET_DALTONIZATION_STATE,
        SET_DALTONIZATION_COLORFILTER,
        SET_CONTENT_TIMEOUT,
        SET_BRIGHTNESS_DISCOUNT,
        SET_AUDIO_BALANCE,
        SET_CLICK_RESPONSE_TIME,
        SET_IGNORE_REPEAT_CLICK_STATE,
        SET_IGNORE_REPEAT_CLICK_TIME,
        GET_SCREENMAGNIFIER_STATE,
        GET_SHORTKEY_STATE,
        GET_MOUSEKEY_STATE,
        GET_SHORTKEY_TARGET,
        GET_MOUSEKEY_AUTOCLICK,
        GET_INVERTCOLOR_STATE,
        GET_HIGHCONTRASTTEXT_STATE,
        GET_DALTONIZATION_STATE,
        GET_AUDIOMONO_STATE,
        GET_ANIMATIONOFF_STATE,
        GET_DALTONIZATION_COLORFILTER,
        GET_CONTENT_TIMEOUT,
        GET_BRIGHTNESS_DISCOUNT,
        GET_AUDIO_BALANCE,
        GET_CLICK_RESPONSE_TIME,
        GET_IGNORE_REPEAT_CLICK_STATE,
        GET_IGNORE_REPEAT_CLICK_TIME,
        GET_ALL_CONFIGS,
        REGISTER_ENABLE_ABILITY_LISTS_OBSERVER,
        REGISTER_CONFIG_CALLBACK,
        GET_WINDOW_AND_ELEMENT_ID,
        GET_SCENE_BOARD_INNER_WINDOW_ID,
        SET_SHORTKEY_MULTI_TARGET,
        GET_SHORTKEY_MULTI_TARGET,
        GET_ROOT_PARENT_ID,
        GET_ALL_TREE_ID,
        GET_FOCUSED_WINDOW_ID,
        REMOVE_REQUEST_ID,
        SERVICE_CODE_END,
        GET_SCREEN_READER_STATE,
        SET_IS_REGISTER_DISCONNECT_CALLBACK,
        NOTIFY_DISCONNECT,

        ON_STATE_CHANGED = 900,
    };
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_IPC_INTERFACE_CODE_H