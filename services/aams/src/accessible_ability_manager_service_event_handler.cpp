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

#include "accessible_ability_manager_service_event_handler.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
AAMSEventHandler::AAMSEventHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner) : AppExecFwk::EventHandler(runner) {

}

AAMSEventHandler::~AAMSEventHandler() {}

void AAMSEventHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    switch (event->GetInnerEventId()) {
        // case ACCESSIBILITY_XXX_XXX:
        default:
            HILOG_ERROR("the eventId is not supported");
            break;
    }
}
} //namespace Accessibility
} //namespace OHOS