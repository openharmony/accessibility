/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

import type { AsyncCallback } from './basic';
import type { Callback } from './basic';

/**
 * Accessibility
 *
 * @namespace accessibility
 * @syscap SystemCapability.BarrierFree.Accessibility.Core
 * @since 7
 */
declare namespace accessibility {
  /**
   * The type of the Ability app.
   *
   * @type { 'audible' | 'generic' | 'haptic' | 'spoken' | 'visual' }
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  /**
   * The type of the Ability app.
   *
   * @type { 'audible' | 'generic' | 'haptic' | 'spoken' | 'visual' | 'all' }
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 9
   */
  type AbilityType = 'audible' | 'generic' | 'haptic' | 'spoken' | 'visual' | 'all';

  /**
   * The action that the ability can execute.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  type Action = 'accessibilityFocus' | 'clearAccessibilityFocus' | 'focus' | 'clearFocus' | 'clearSelection' |
    'click' | 'longClick' | 'cut' | 'copy' | 'paste' | 'select' | 'setText' | 'delete' |
    'scrollForward' | 'scrollBackward' | 'setSelection' | 'setCursorPosition' | 'common';

  /**
   * The type of the accessibility event.
   * windowsChange/windowContentChange/windowStateChange/announcement/notificationChange/textTraversedAtMove
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  type EventType = 'accessibilityFocus' | 'accessibilityFocusClear' |
    'click' | 'longClick' | 'focus' | 'select' | 'hoverEnter' | 'hoverExit' |
    'textUpdate' | 'textSelectionUpdate' | 'scroll' | 'requestFocusForAccessibility' | 'announceForAccessibility';

  /**
   * The change type of the windowsChange event.
   * It's used when received the {@code windowsChange} event.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  type WindowUpdateType = 'add' | 'remove' | 'bounds' | 'active' | 'focus';

  /**
   * The type of the ability state.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  type AbilityState = 'enable' | 'disable' | 'install';

  /**
   * The ability that accessibility subsystem support.
   * touchExplorer: Describes the capability to talkback.
   * magnification: Describes the capability to request to control the display magnification.
   * gesturesSimulation: Describes the capability to request to simulate the gesture.
   * windowContent: Describes the capability to search for the content of the active window.
   * filterKeyEvents: Describes the capability to request to filter key events.
   * fingerprintGesture: Describes the capability to request to fingerprint gesture.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  type Capability = 'retrieve' | 'touchGuide' | 'keyEventObserver' | 'zoom' | 'gesture';

  /**
   * The granularity of text move.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  type TextMoveUnit = 'char' | 'word' | 'line' | 'page' | 'paragraph';

  /**
   * Checks whether accessibility ability is enabled.
   *
   * @param { AsyncCallback<boolean> } callback Asynchronous callback interface.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   * @deprecated since 10
   * @useinstead ohos.accessibility#isOpenAccessibilitySync
   */
  function isOpenAccessibility(callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether accessibility ability is enabled.
   *
   * @returns { Promise<boolean> } Returns {@code true} if the accessibility is enabled; returns {@code false} otherwise.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   * @deprecated since 10
   * @useinstead ohos.accessibility#isOpenAccessibilitySync
   */
  function isOpenAccessibility(): Promise<boolean>;

  /**
   * Checks whether accessibility ability is enabled.
   *
   * @returns { boolean } Returns true if the accessibility is enabled; returns false otherwise.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 10
   */
  function isOpenAccessibilitySync(): boolean;

  /**
   * Checks touch browser ability (which is used by talkback) is enabled.
   *
   * @param { AsyncCallback<boolean> } callback Asynchronous callback interface.
   * @syscap SystemCapability.BarrierFree.Accessibility.Vision
   * @since 7
   * @deprecated since 10
   * @useinstead ohos.accessibility#isOpenTouchGuideSync
   */
  function isOpenTouchGuide(callback: AsyncCallback<boolean>): void;

  /**
   * Checks touch browser ability (which is used by talkback) is enabled.
   *
   * @returns { Promise<boolean> } Returns {@code true} if the touch browser is enabled; returns {@code false} otherwise.
   * @syscap SystemCapability.BarrierFree.Accessibility.Vision
   * @since 7
   * @deprecated since 10
   * @useinstead ohos.accessibility#isOpenTouchGuideSync
   */
  function isOpenTouchGuide(): Promise<boolean>;

  /**
   * Checks touch browser ability (which is used by talkback) is enabled.
   *
   * @returns { boolean } Returns true if the touch browser is enabled; returns false otherwise.
   * @syscap SystemCapability.BarrierFree.Accessibility.Vision
   * @since 10
   */
  function isOpenTouchGuideSync(): boolean;

  /**
   * Checks screen reader ability is enabled.
   *
   * @returns { boolean } Returns true if the screen reader is enabled; returns false otherwise.
   * @syscap SystemCapability.BarrierFree.Accessibility.Vision
   * @since 16
   */
    function isScreenReaderOpenSync(): boolean;

  /**
   * Queries the list of accessibility abilities.
   *
   * @param { AbilityType } abilityType The type of the accessibility ability. {@code AbilityType} eg.spoken
   * @param { AbilityState } stateType The state of the accessibility ability.  {@code AbilityState} eg.installed
   * @param { AsyncCallback<Array<AccessibilityAbilityInfo>> } callback
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   * @deprecated since 9
   */
  function getAbilityLists(
    abilityType: AbilityType,
    stateType: AbilityState,
    callback: AsyncCallback<Array<AccessibilityAbilityInfo>>
  ): void;

  /**
   * Queries the list of accessibility abilities.
   *
   * @param { AbilityType } abilityType The type of the accessibility ability. {@code AbilityType} eg.spoken
   * @param { AbilityState } stateType The state of the accessibility ability.  {@code AbilityState} eg.installed
   * @returns { Promise<Array<AccessibilityAbilityInfo>> } Returns the list of abilityInfos.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   * @deprecated since 9
   */
  function getAbilityLists(abilityType: AbilityType, stateType: AbilityState): Promise<Array<AccessibilityAbilityInfo>>;

  /**
   * Queries the list of accessibility abilities.
   *
   * @param { AbilityType } abilityType The type of the accessibility ability. {@code AbilityType} eg.spoken
   * @param { AbilityState } stateType The state of the accessibility ability.  {@code AbilityState} eg.installed
   * @returns { Promise<Array<AccessibilityAbilityInfo>> } Returns the list of abilityInfos.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 9
   */
  function getAccessibilityExtensionList(
    abilityType: AbilityType,
    stateType: AbilityState
  ): Promise<Array<AccessibilityAbilityInfo>>;

  /**
   * Queries the list of accessibility abilities.
   *
   * @param { AbilityType } abilityType The type of the accessibility ability. {@code AbilityType} eg.spoken
   * @param { AbilityState } stateType The state of the accessibility ability.  {@code AbilityState} eg.installed
   * @param { AsyncCallback<Array<AccessibilityAbilityInfo>> } callback
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 9
   */
  function getAccessibilityExtensionList(
    abilityType: AbilityType,
    stateType: AbilityState,
    callback: AsyncCallback<Array<AccessibilityAbilityInfo>>
  ): void;

  /**
   * Queries the list of accessibility abilities.
   *
   * @param { AbilityType } abilityType The type of the accessibility ability. {@code AbilityType} eg.spoken
   * @param { AbilityState } stateType The state of the accessibility ability.  {@code AbilityState} eg.installed
   * @returns { Array<AccessibilityAbilityInfo> } Returns the list of abilityInfos.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 12
   */
  function getAccessibilityExtensionListSync(
    abilityType: AbilityType,
    stateType: AbilityState
  ): Array<AccessibilityAbilityInfo>;

  /**
   * Send accessibility Event.
   *
   * @param { EventInfo } event The object of the accessibility {@code EventInfo} .
   * @param { AsyncCallback<void> } callback Asynchronous callback interface.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   * @deprecated since 9
   */
  function sendEvent(event: EventInfo, callback: AsyncCallback<void>): void;

  /**
   * Send accessibility Event.
   *
   * @param { EventInfo } event The object of the accessibility {@code EventInfo} .
   * @returns { Promise<void> } Returns {@code true} if success ; returns {@code false} otherwise.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   * @deprecated since 9
   */
  function sendEvent(event: EventInfo): Promise<void>;

  /**
   * Send accessibility event.
   *
   * @param { EventInfo } event The object of the accessibility {@code EventInfo} .
   * @param { AsyncCallback<void> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 9
   */
  function sendAccessibilityEvent(event: EventInfo, callback: AsyncCallback<void>): void;

  /**
   * Send accessibility event.
   *
   * @param { EventInfo } event The object of the accessibility {@code EventInfo} .
   * @returns { Promise<void> } Returns {@code true} if success ; returns {@code false} otherwise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 9
   */
  function sendAccessibilityEvent(event: EventInfo): Promise<void>;

  /**
   * Register the observe of the accessibility state changed.
   *
   * @param { 'accessibilityStateChange' } type state event type.
   * @param { Callback<boolean> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  function on(type: 'accessibilityStateChange', callback: Callback<boolean>): void;

  /**
   * Register the observe of the touchGuide state changed.
   *
   * @param { 'touchGuideStateChange' } type state event type.
   * @param { Callback<boolean> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Vision
   * @since 7
   */
  function on(type: 'touchGuideStateChange', callback: Callback<boolean>): void;

  /**
   * Register the observe of the screen reader state changed.
   *
   * @param { 'screenReaderStateChange' } type state event type.
   * @param { Callback<boolean> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 16
   */
  function on(type: 'screenReaderStateChange', callback: Callback<boolean>): void;

  /**
   * Deregister the observe of the accessibility state changed.
   *
   * @param { 'accessibilityStateChange' } type state event type
   * @param { Callback<boolean> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  function off(type: 'accessibilityStateChange', callback?: Callback<boolean>): void;

  /**
   * Deregister the observe of the touchGuide state changed.
   *
   * @param { 'touchGuideStateChange' } type state event type
   * @param { Callback<boolean> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  function off(type: 'touchGuideStateChange', callback?: Callback<boolean>): void;

    /**
   * Deregister the observe of the screen reader state changed.
   *
   * @param { 'screenReaderStateChange' } type state event type
   * @param { Callback<boolean> } callback Asynchronous callback interface.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                           1. Mandatory parameters are left unspecified;
   *                           2. Incorrect parameter types;
   *                           3. Parameter verification failed.
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 16
   */
    function off(type: 'screenReaderStateChange', callback?: Callback<boolean>): void;

  /**
   * Get the captions manager.
   *
   * @returns { CaptionsManager } Returns the captions manager.
   * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
   * @since 8
   */
  function getCaptionsManager(): CaptionsManager;

  /**
   * Indicates the captions manager.
   *
   * @typedef CaptionsManager
   * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
   * @since 8
   */
  interface CaptionsManager {
    /**
     * Indicates whether captions are enabled.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    enabled: boolean;
    /**
     * Indicates the style of captions.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    style: CaptionsStyle;

    /**
     * Register the observe of the enable state.
     *
     * @param { 'enableChange' } type
     * @param { Callback<boolean> } callback
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                           1. Mandatory parameters are left unspecified;
     *                           2. Incorrect parameter types;
     *                           3. Parameter verification failed.
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    on(type: 'enableChange', callback: Callback<boolean>): void;

    /**
     * Register the observer of the style.
     *
     * @param { 'styleChange' } type
     * @param { Callback<CaptionsStyle> } callback
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                           1. Mandatory parameters are left unspecified;
     *                           2. Incorrect parameter types;
     *                           3. Parameter verification failed.
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    on(type: 'styleChange', callback: Callback<CaptionsStyle>): void;

    /**
     * Deregister the observe of the enable state.
     *
     * @param { 'enableChange' } type
     * @param { Callback<boolean> } callback
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                           1. Mandatory parameters are left unspecified;
     *                           2. Incorrect parameter types;
     *                           3. Parameter verification failed.
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    off(type: 'enableChange', callback?: Callback<boolean>): void;

    /**
     * Deregister the observer of the style.
     *
     * @param { 'styleChange' } type
     * @param { Callback<CaptionsStyle> } callback
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                           1. Mandatory parameters are left unspecified;
     *                           2. Incorrect parameter types;
     *                           3. Parameter verification failed.
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    off(type: 'styleChange', callback?: Callback<CaptionsStyle>): void;
  }

  /**
   * Indicates the edge type of the captions font.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
   * @since 8
   */
  type CaptionsFontEdgeType = 'none' | 'raised' | 'depressed' | 'uniform' | 'dropShadow';
  /**
   * Indicates the font family of captions.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
   * @since 8
   */
  type CaptionsFontFamily = 'default' | 'monospacedSerif' | 'serif' |
    'monospacedSansSerif' | 'sansSerif' | 'casual' | 'cursive' | 'smallCapitals';
  /**
   * Indicates the style of captions.
   *
   * @typedef CaptionsStyle
   * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
   * @since 8
   */
  interface CaptionsStyle {
    /**
     * Indicates the font family of captions.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    fontFamily: CaptionsFontFamily;
    /**
     * Indicates the font scaling of captions.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    fontScale: number;
    /**
     * Indicates the font color of captions.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    fontColor: number | string;
    /**
     * Indicates the edge type of the captions font.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    fontEdgeType: CaptionsFontEdgeType;
    /**
     * Indicates the background color of captions.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    backgroundColor: number | string;
    /**
     * Indicates the window color of captions.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Hearing
     * @since 8
     */
    windowColor: number | string;
  }

  /**
   * Indicates the info of accessibility.
   *
   * @typedef AccessibilityAbilityInfo
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  interface AccessibilityAbilityInfo {
    /**
     * The ability id.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    readonly id: string;

    /* The ability name.
     * @since 7
     */
    readonly name: string;

    /* The bundle name of the ability.
     * @since 7
     */
    readonly bundleName: string;

    /* The target bundle name for the observation.
     * @since 9
     */
    readonly targetBundleNames: Array<string>;

    /**
     * The type of the ability.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    readonly abilityTypes: Array<AbilityType>;

    /**
     * The capabilities of the ability.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    readonly capabilities: Array<Capability>;

    /**
     * The description of the ability.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    readonly description: string;

    /**
     * The events which the accessibility ability wants to observe.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    readonly eventTypes: Array<EventType>;

    /**
     * The whether the extended service needs to be hidden.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 12
     */
    readonly needHide: boolean;
  }

  /**
   * Indicates the info of events.
   *
   * @syscap SystemCapability.BarrierFree.Accessibility.Core
   * @since 7
   */
  class EventInfo {
    constructor(jsonObject);
    /**
     * The type of an accessibility event.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    type: EventType;

    /**
     * The type of the window change event.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    windowUpdateType?: WindowUpdateType;

    /**
     * The bundle name of the target application.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    bundleName: string;

    /**
     * The type of the event source component,such as button, chart.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    componentType?: string;

    /**
     * The page id of the event source.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    pageId?: number;

    /**
     * The accessibility event description.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    description?: string;

    /**
     * The action that triggers the accessibility event, for example, clicking or focusing a view.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    triggerAction: Action;

    /**
     * The movement step used for reading texts.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    textMoveUnit?: TextMoveUnit;

    /**
     * The content list.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    contents?: Array<string>;

    /**
     * The content changed before.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    lastContent?: string;

    /**
     * The start index of listed items on the screen.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    beginIndex?: number;

    /**
     * The index of the current item on the screen.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    currentIndex?: number;

    /**
     * The end index of listed items on the screen.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    endIndex?: number;

    /**
     * The total of the items, talkback used it when scroll.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 7
     */
    itemCount?: number;

    /**
     * The id of element.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 12
     */
    elementId?: number;

    /**
     * The context of announce accessibility text.
     *
     * @syscap SystemCapability.BarrierFree.Accessibility.Core
     * @since 12
     */
    textAnnouncedForAccessibility?: string;
  }
}
export default accessibility;
