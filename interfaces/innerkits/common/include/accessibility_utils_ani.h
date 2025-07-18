/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_UTILS_ANI_H
#define ACCESSIBILITY_UTILS_ANI_H

#include <ani.h>
#include "accessibility_def.h"

namespace OHOS {
namespace AccessibilityAni {
NAccessibilityErrMsg QueryRetMsg(OHOS::Accessibility::RetError errorCode);
void ThrowBusinessError(ani_env *env, NAccessibilityErrMsg errMsg);
} // namespace AccessibilityAni
} // OHOS
#endif // ACCESSIBILITY_UTILS_ANI_H