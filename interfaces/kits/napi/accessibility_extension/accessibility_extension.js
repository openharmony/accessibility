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

class AccessibilityExtensionAbility {
  onConnect() {
    console.log('onConnect');
  }

  onDisconnect() {
    console.log('onDisconnect');
  }

  onAccessibilityEvent(accessibilityEvent) {
    console.log('onAccessibilityEvent');
  }

  onKeyEvent(keyEvent) {
    console.log('onKeyEvent');
    return false;
  }

  onAccessibilityConnect() {
    console.log('onAccessibilityConnect');
  }

  onAccessibilityDisconnect() {
    console.log('onAccessibilityDisconnect');
  }

  onAccessibilityEventInfo(accessibilityEventInfo) {
    console.log('onAccessibilityEventInfo');
  }

  onAccessibilityKeyEvent(keyEvent) {
    console.log('onAccessibilityKeyEvent');
    return false;
  }
}

export default AccessibilityExtensionAbility;
