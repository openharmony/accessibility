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

#include <ani.h>
#include <array>
#include <iostream>
#include <vector>
#include "hilog_wrapper.h"
#include "ani_accessibility_common.h"
#include <ani_signature_builder.h>

using namespace OHOS::Accessibility;
using namespace arkts::ani_signature;

std::string ANICommon::ANIStringToStdString(ani_env *env, ani_string ani_str)
{
    ani_size strSize;
    env->String_GetUTF8Size(ani_str, &strSize);
   
    std::vector<char> buffer(strSize + 1); // +1 for null terminator
    char* utf8_buffer = buffer.data();

    ani_size bytes_written = 0;
    env->String_GetUTF8(ani_str, utf8_buffer, strSize + 1, &bytes_written);
    
    utf8_buffer[bytes_written] = '\0';
    std::string content = std::string(utf8_buffer);
    return content;
}

bool ANICommon::GetStringField(ani_env *env, std::string fieldName, ani_object object, std::string &fieldValue)
{
    ani_ref ref;
    if (env->Object_GetFieldByName_Ref(object, fieldName.c_str(), &ref) != ANI_OK) {
        HILOG_ERROR("get field %{public}s failed", fieldName.c_str());
        return false;
    }
    ani_boolean isUndefined;
    if (env->Reference_IsUndefined(ref, &isUndefined) != ANI_OK) {
        HILOG_ERROR("get field %{public}s undefined failed", fieldName.c_str());
        return false;
    }
    if (!isUndefined) {
        fieldValue = ANIStringToStdString(env, static_cast<ani_string>(ref));
        return true;
    }
    return false;
}

bool ANICommon::GetIntField(ani_env *env, std::string fieldName, ani_object object, int32_t &fieldValue)
{
    ani_ref ref;
    if (env->Object_GetFieldByName_Ref(object, fieldName.c_str(), &ref) != ANI_OK) {
        HILOG_ERROR("get field %{public}s failed", fieldName.c_str());
        return false;
    }
    ani_boolean isUndefined;
    if (env->Reference_IsUndefined(ref, &isUndefined) != ANI_OK) {
        HILOG_ERROR("get field %{public}s undefined failed", fieldName.c_str());
        return false;
    }
    if (!isUndefined) {
        if (env->Object_CallMethodByName_Int(static_cast<ani_object>(ref), "toInt", ":i", &fieldValue) ==
            ANI_OK) {
            return true;
        }
    }
    return false;
}

bool ANICommon::GetArrayStringField(ani_env *env, std::string fieldName, ani_object object,
    std::vector<std::string> &fieldValue)
{
    ani_ref ref;
    if (env->Object_GetFieldByName_Ref(object, fieldName.c_str(), &ref) != ANI_OK) {
        HILOG_ERROR("get field %{public}s failed", fieldName.c_str());
        return false;
    }
    ani_boolean isUndefined;
    if (env->Reference_IsUndefined(ref, &isUndefined) != ANI_OK) {
        HILOG_ERROR("get field %{public}s undefined failed", fieldName.c_str());
        return false;
    }
    if (isUndefined) {
        return false;
    }

    fieldValue.clear();
    ani_class arrayCls;
    if (env->FindClass(Builder::BuildClass("escompat.Array").Descriptor().c_str(), &arrayCls) != ANI_OK) {
        return false;
    }

    ani_method arrayLengthMethod;
    SignatureBuilder objectBuilder{};
    objectBuilder.SetReturnClass("std.core.Object");
    std::string objectBuilderDescriptor = objectBuilder.BuildSignatureDescriptor();
    if (env->Class_FindMethod(arrayCls, "length", objectBuilderDescriptor.c_str(), &arrayLengthMethod) != ANI_OK) {
        return false;
    }

    ani_ref length;
    if (env->Object_CallMethod_Ref(static_cast<ani_object>(ref), arrayLengthMethod, &length) != ANI_OK) {
        return false;
    }

    int32_t lengthInt;
    if (env->Object_CallMethodByName_Int(static_cast<ani_object>(length), "toInt", ":i", &lengthInt) != ANI_OK ||
        lengthInt <= 0) {
        return false;
    }

    ani_method arrayPopMethod;
    if (env->Class_FindMethod(arrayCls, "pop", objectBuilderDescriptor.c_str(), &arrayPopMethod) != ANI_OK) {
        return false;
    }

    ani_ref string;
    std::string result;
    for (int32_t i = 0; i < lengthInt; i++) {
        if (env->Object_CallMethod_Ref(static_cast<ani_object>(ref), arrayPopMethod, &string) != ANI_OK) {
            return false;
        }
        result = ANIStringToStdString(env, static_cast<ani_string>(string));
        fieldValue.insert(fieldValue.begin(), result);
    }

    return true;
}

bool ANICommon::CheckObserverEqual(ani_env *env, ani_ref fnRef, ani_env *iterEnv, ani_ref iterFn)
{
    if (env != iterEnv) {
        HILOG_DEBUG("not the same env");
        return false;
    }
    ani_boolean isEquals = false;
    if (env->Reference_StrictEquals(fnRef, iterFn, &isEquals) != ANI_OK) {
        HILOG_ERROR("check observer equal failed!");
        return false;
    }
    return isEquals;
}

EventType ANICommon::ConvertStringToEventInfoTypes(const std::string &type)
{
    static const std::map<const std::string, EventType> eventInfoTypesTable = {
        {"click", EventType::TYPE_VIEW_CLICKED_EVENT},
        {"longClick", EventType::TYPE_VIEW_LONG_CLICKED_EVENT},
        {"select", EventType::TYPE_VIEW_SELECTED_EVENT},
        {"focus", EventType::TYPE_VIEW_FOCUSED_EVENT},
        {"textUpdate", EventType::TYPE_VIEW_TEXT_UPDATE_EVENT},
        {"hoverEnter", EventType::TYPE_VIEW_HOVER_ENTER_EVENT},
        {"hoverExit", EventType::TYPE_VIEW_HOVER_EXIT_EVENT},
        {"scroll", EventType::TYPE_VIEW_SCROLLED_EVENT},
        {"textSelectionUpdate", EventType::TYPE_VIEW_TEXT_SELECTION_UPDATE_EVENT},
        {"accessibilityFocus", EventType::TYPE_VIEW_ACCESSIBILITY_FOCUSED_EVENT},
        {"accessibilityFocusClear", EventType::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED_EVENT},
        {"requestFocusForAccessibility", EventType::TYPE_VIEW_REQUEST_FOCUS_FOR_ACCESSIBILITY},
        {"announceForAccessibility", EventType::TYPE_VIEW_ANNOUNCE_FOR_ACCESSIBILITY}};

    if (eventInfoTypesTable.find(type) == eventInfoTypesTable.end()) {
        HILOG_WARN("invalid key[%{public}s]", type.c_str());
        return TYPE_VIEW_INVALID;
    }

    return eventInfoTypesTable.at(type);
}

ActionType ANICommon::ConvertStringToAccessibleOperationType(const std::string &type)
{
    std::map<const std::string, ActionType> accessibleOperationTypeTable = {
        {"focus", ActionType::ACCESSIBILITY_ACTION_FOCUS},
        {"clearFocus", ActionType::ACCESSIBILITY_ACTION_CLEAR_FOCUS},
        {"select", ActionType::ACCESSIBILITY_ACTION_SELECT},
        {"clearSelection", ActionType::ACCESSIBILITY_ACTION_CLEAR_SELECTION},
        {"click", ActionType::ACCESSIBILITY_ACTION_CLICK},
        {"longClick", ActionType::ACCESSIBILITY_ACTION_LONG_CLICK},
        {"accessibilityFocus", ActionType::ACCESSIBILITY_ACTION_ACCESSIBILITY_FOCUS},
        {"clearAccessibilityFocus", ActionType::ACCESSIBILITY_ACTION_CLEAR_ACCESSIBILITY_FOCUS},
        {"scrollForward", ActionType::ACCESSIBILITY_ACTION_SCROLL_FORWARD},
        {"scrollBackward", ActionType::ACCESSIBILITY_ACTION_SCROLL_BACKWARD},
        {"copy", ActionType::ACCESSIBILITY_ACTION_COPY},
        {"paste", ActionType::ACCESSIBILITY_ACTION_PASTE},
        {"cut", ActionType::ACCESSIBILITY_ACTION_CUT},
        {"setSelection", ActionType::ACCESSIBILITY_ACTION_SET_SELECTION},
        {"setCursorPosition", ActionType::ACCESSIBILITY_ACTION_SET_CURSOR_POSITION},
        {"common", ActionType::ACCESSIBILITY_ACTION_COMMON},
        {"setText", ActionType::ACCESSIBILITY_ACTION_SET_TEXT},
        {"delete", ActionType::ACCESSIBILITY_ACTION_DELETED},
        {"home", ActionType::ACCESSIBILITY_ACTION_HOME},
        {"back", ActionType::ACCESSIBILITY_ACTION_BACK},
        {"recentTask", ActionType::ACCESSIBILITY_ACTION_RECENTTASK},
        {"notificationCenter", ActionType::ACCESSIBILITY_ACTION_NOTIFICATIONCENTER},
        {"controlCenter", ActionType::ACCESSIBILITY_ACTION_CONTROLCENTER},
        {"spanClick", ActionType::ACCESSIBILITY_ACTION_SPAN_CLICK}};

    if (accessibleOperationTypeTable.find(type) == accessibleOperationTypeTable.end()) {
        HILOG_WARN("invalid key[%{public}s]", type.c_str());
        return ACCESSIBILITY_ACTION_INVALID;
    }

    return accessibleOperationTypeTable.at(type);
}

WindowUpdateType ANICommon::ConvertStringToWindowUpdateTypes(const std::string &type)
{
    static const std::map<const std::string, WindowUpdateType> windowsUpdateTypesTable = {
        {"accessibilityFocus", WindowUpdateType::WINDOW_UPDATE_ACCESSIBILITY_FOCUSED},
        {"focus", WindowUpdateType::WINDOW_UPDATE_FOCUSED},
        {"active", WindowUpdateType::WINDOW_UPDATE_ACTIVE},
        {"add", WindowUpdateType::WINDOW_UPDATE_ADDED},
        {"remove", WindowUpdateType::WINDOW_UPDATE_REMOVED},
        {"bounds", WindowUpdateType::WINDOW_UPDATE_BOUNDS},
        {"title", WindowUpdateType::WINDOW_UPDATE_TITLE},
        {"layer", WindowUpdateType::WINDOW_UPDATE_LAYER},
        {"parent", WindowUpdateType::WINDOW_UPDATE_PARENT},
        {"children", WindowUpdateType::WINDOW_UPDATE_CHILDREN},
        {"pip", WindowUpdateType::WINDOW_UPDATE_PIP},
        {"property", WindowUpdateType::WINDOW_UPDATE_PROPERTY}};

    if (windowsUpdateTypesTable.find(type) == windowsUpdateTypesTable.end()) {
        HILOG_WARN("invalid key[%{public}s]", type.c_str());
        return WINDOW_UPDATE_INVALID;
    }

    return windowsUpdateTypesTable.at(type);
}

TextMoveUnit ANICommon::ConvertStringToTextMoveUnit(const std::string &type)
{
    static const std::map<const std::string, TextMoveUnit> textMoveUnitTable = {{"char", TextMoveUnit::STEP_CHARACTER},
        {"word", TextMoveUnit::STEP_WORD},
        {"line", TextMoveUnit::STEP_LINE},
        {"page", TextMoveUnit::STEP_PAGE},
        {"paragraph", TextMoveUnit::STEP_PARAGRAPH}};

    if (textMoveUnitTable.find(type) == textMoveUnitTable.end()) {
        HILOG_WARN("invalid key[%{public}s]", type.c_str());
        return STEP_INVALID;
    }

    return textMoveUnitTable.at(type);
}

ani_object ANICommon::CreateBoolObject(ani_env *env, ani_boolean value)
{
    Type boolClass = Builder::BuildClass("std.core.Boolean");
    ani_class cls {};
    if (env->FindClass(boolClass.Descriptor().c_str(), &cls) != ANI_OK) {
        HILOG_ERROR("find class Boolean failed");
        return nullptr;
    }

    ani_method ctor;
    std::string ctorName = Builder::BuildConstructorName();
    SignatureBuilder sb{};
    sb.AddBoolean();
    if (env->Class_FindMethod(cls, ctorName.c_str(), sb.BuildSignatureDescriptor().c_str(), &ctor) != ANI_OK) {
        HILOG_ERROR("find method Boolean.constructor failed");
        return nullptr;
    }

    ani_object boolObject;
    if (env->Object_New(cls, ctor, &boolObject, value) != ANI_OK) {
        HILOG_ERROR("create Boolean object failed");
        return nullptr;
    }
    return boolObject;
}

bool ANICommon::ConvertEventInfoMandatoryFields(ani_env *env, ani_object eventObject,
    AccessibilityEventInfo &eventInfo)
{
    std::string type;
    if (!GetStringField(env, "type", eventObject, type)) {
        HILOG_ERROR("get type Faild!");
        return false;
    }
    OHOS::Accessibility::EventType eventType = ConvertStringToEventInfoTypes(type);
    if (eventType == TYPE_VIEW_INVALID) {
        HILOG_ERROR("event type is invalid!");
        return false;
    }
    eventInfo.SetEventType(eventType);

    std::string bundleName;
    if (!GetStringField(env, "bundleName", eventObject, bundleName)) {
        HILOG_ERROR("get bundleName Faild!");
        return false;
    }
    if (bundleName == "") {
        HILOG_ERROR("bundle name is invalid!");
        return false;
    }
    eventInfo.SetBundleName(bundleName);

    std::string triggerAction;
    if (!GetStringField(env, "triggerAction", eventObject, triggerAction)) {
        HILOG_ERROR("get triggerAction Faild!");
        return false;
    }
    OHOS::Accessibility::ActionType action = ConvertStringToAccessibleOperationType(triggerAction);
    if (action == ACCESSIBILITY_ACTION_INVALID) {
        HILOG_ERROR("action is invalid!");
        return false;
    }
    eventInfo.SetTriggerAction(action);

    return true;
}

void ANICommon::ConvertEventInfoStringFields(ani_env *env, ani_object eventObject,
    OHOS::Accessibility::AccessibilityEventInfo &eventInfo)
{
    std::string windowUpdateType;
    if (GetStringField(env, "windowUpdateType", eventObject, windowUpdateType)) {
        eventInfo.SetEventType(TYPE_WINDOW_UPDATE);
        eventInfo.SetWindowChangeTypes(ConvertStringToWindowUpdateTypes(windowUpdateType));
    }

    std::string componentType;
    if (GetStringField(env, "componentType", eventObject, componentType)) {
        eventInfo.SetComponentType(componentType);
    }

    std::string description;
    if (GetStringField(env, "description", eventObject, description)) {
        eventInfo.SetDescription(description);
    }

    std::string textMoveUnit;
    if (GetStringField(env, "textMoveUnit", eventObject, textMoveUnit)) {
        eventInfo.SetTextMovementStep(ConvertStringToTextMoveUnit(textMoveUnit));
    }

    std::vector<std::string> contents;
    if (GetArrayStringField(env, "contents", eventObject, contents)) {
        for (auto str : contents) {
            eventInfo.AddContent(str);
        }
    }

    std::string lastContent;
    if (GetStringField(env, "lastContent", eventObject, lastContent)) {
        eventInfo.SetLatestContent(lastContent);
    }

    std::string textAnnouncedForAccessibility;
    if (GetStringField(env, "textAnnouncedForAccessibility", eventObject, textAnnouncedForAccessibility)) {
        eventInfo.SetTextAnnouncedForAccessibility(textAnnouncedForAccessibility);
    }

    std::string customId;
    if (GetStringField(env, "customId", eventObject, customId)) {
        eventInfo.SetInspectorKey(customId);
    }
}

void ANICommon::ConvertEventInfoIntFields(ani_env *env, ani_object eventObject,
    OHOS::Accessibility::AccessibilityEventInfo &eventInfo)
{
    int32_t pageId;
    if (GetIntField(env, "pageId", eventObject, pageId)) {
        eventInfo.SetPageId(pageId);
    }

    int32_t beginIndex;
    if (GetIntField(env, "beginIndex", eventObject, beginIndex)) {
        eventInfo.SetBeginIndex(beginIndex);
    }

    int32_t currentIndex;
    if (GetIntField(env, "currentIndex", eventObject, currentIndex)) {
        eventInfo.SetCurrentIndex(currentIndex);
    }

    int32_t endIndex;
    if (GetIntField(env, "endIndex", eventObject, endIndex)) {
        eventInfo.SetEndIndex(endIndex);
    }

    int32_t itemCount;
    if (GetIntField(env, "itemCount", eventObject, itemCount)) {
        eventInfo.SetItemCounts(itemCount);
    }

    int32_t elementId;
    if (GetIntField(env, "elementId", eventObject, elementId)) {
        eventInfo.SetRequestFocusElementId(elementId);
    }
}

bool ANICommon::SendEventToMainThread(const std::function<void()> func)
{
    if (func == nullptr) {
        HILOG_ERROR("func is nullptr!");
        return false;
    }

    if (!mainHandler) {
        std::shared_ptr<OHOS::AppExecFwk::EventRunner> runner = OHOS::AppExecFwk::EventRunner::GetMainEventRunner();
        if (!runner) {
            HILOG_ERROR("get main event runner failed!");
            return false;
        }
        mainHandler = std::make_shared<OHOS::AppExecFwk::EventHandler>(runner);
    }
    mainHandler->PostTask(func, "", 0, OHOS::AppExecFwk::EventQueue::Priority::HIGH, {});
    return true;
}