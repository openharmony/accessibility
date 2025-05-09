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

#ifndef XCOLLIE_HELPER_H
#define XCOLLIE_HELPER_H

#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace Accessibility {
class XCollieHelper {
public:
    XCollieHelper(const std::string &name, unsigned int timeout)
    {
        id_ = HiviewDFX::XCollie::GetInstance()
            .SetTimer(name, timeout, nullptr, nullptr, HiviewDFX::XCOLLIE_FLAG_LOG);
    }
    ~XCollieHelper()
    {
        HiviewDFX::XCollie::GetInstance().CancelTimer(id_);
    }
private:
    int id_;
};
} // namespace Accessibility
} // namespace OHOS
#endif // XCOLLIE_HELPER_H