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

#ifndef MSDP_MANAGER_H
#define MSDP_MANAGER_H

#include <cstdint>

namespace OHOS {
namespace Accessibility {
class MsdpManager {
public:
    static int32_t SubscribeVoiceRecognition();
    static void UnSubscribeVoiceRecognition();
};
} // namespace Accessibility
} // namespace OHOS
#endif // MSDP_MANAGER_H