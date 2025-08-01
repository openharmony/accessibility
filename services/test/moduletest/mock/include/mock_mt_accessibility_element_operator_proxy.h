/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef MOCK_MT_ACCESSIBILITY_ELEMENT_OPERATOR_PROXY_H
#define MOCK_MT_ACCESSIBILITY_ELEMENT_OPERATOR_PROXY_H

#include <gmock/gmock.h>
#include "accessibility_element_operator_proxy.h"

namespace OHOS {
namespace Accessibility {
class MockMtAccessibilityElementOperatorProxy : public AccessibilityElementOperatorProxy {
public:
    MockMtAccessibilityElementOperatorProxy(const sptr<IRemoteObject>& object) : AccessibilityElementOperatorProxy(object)
    {}
    virtual ~MockMtAccessibilityElementOperatorProxy() override
    {}
    MOCK_METHOD5(SearchElementInfoByAccessibilityId,
        RetError(const int64_t elementId, const int32_t requestId,
            const sptr<IAccessibilityElementOperatorCallback>& callback, const int32_t mode, bool isFilter));
    MOCK_METHOD4(
        SearchElementInfosByText, void(const int64_t elementId, const std::string& text, const int32_t requestId,
                                      const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD4(FindFocusedElementInfo, void(const int64_t elementId, const int32_t focusType, const int32_t requestId,
                                             const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD4(FocusMoveSearch, void(const int64_t elementId, const int32_t direction, const int32_t requestId,
                                      const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD5(ExecuteAction, void(const int64_t elementId, const int32_t action,
        const std::map<std::string, std::string> &actionArguments,
        int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD3(GetCursorPosition, void(const int64_t elementId,
        int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD4(SearchElementInfoBySpecificProperty, void(const int64_t elementId,
        const SpecificPropertyParam& param, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD0(ClearFocus, void());
    MOCK_METHOD0(OutsideTouch, void());
    MOCK_METHOD3(SetChildTreeIdAndWinId, void(const int64_t elementId, const int32_t treeId,
        const int32_t childWindowId));
    MOCK_METHOD1(SetBelongTreeId, void(const int32_t treeId));
    MOCK_METHOD1(SetParentWindowId, void(const int32_t parentWindowId));
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_MT_ACCESSIBILITY_ELEMENT_OPERATOR_PROXY_H