/*
 * Copyright (C) 2026-2026 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBLE_TEXT_CALL_MANAGER_H
#define ACCESSIBLE_TEXT_CALL_MANAGER_H

#include "singleton.h"
#ifdef OHOS_BUILD_TEXT_CALL
#include "call_manager_client.h"
#endif
#include "ability_connect_callback_stub.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace Accessibility {

#ifdef  OHOS_BUILD_TEXT_CALL
class CallServiceListener : public Telephony::CallManagerCallback {
public:
    CallServiceListener() = default;
    ~CallServiceListener() = default;
    int32_t OnCallDetailsChange(const Telephony::CallAttributeInfo &info) override;
    int32_t OnCallEventChange(const Telephony::CallEventInfo &info) override { return 0; }
    int32_t OnCallDisconnectedCause(const Telephony::DisconnectedDetails &details) override { return 0; }
    int32_t OnReportAsyncResults(Telephony::CallResultReportId reportId,
        AppExecFwk::PacMap &resultInfo) override { return 0; }
    int32_t OnOttCallRequest(Telephony::OttCallRequestId requestId, AppExecFwk::PacMap &info) override { return 0; }
    int32_t OnReportMmiCodeResult(const Telephony::MmiCodeInfo &info) override { return 0; }
    int32_t OnReportAudioDeviceChange(const Telephony::AudioDeviceInfo &info) override { return 0; };
    int32_t OnReportPostDialDelay(const std::string &str) override { return 0; }
    int32_t OnUpdateImsCallModeChange(const Telephony::CallMediaModeInfo &imsCallModeInfo) override { return 0; }
    int32_t OnCallSessionEventChange(const Telephony::CallSessionEvent &callSessionEventOptions) override { return 0; }
    int32_t OnPeerDimensionsChange(const Telephony::PeerDimensionsDetail &peerDimensionsDetail) override { return 0; }
    int32_t OnCallDataUsageChange(const int64_t dataUsage) override { return 0; }
    int32_t OnUpdateCameraCapabilities(const Telephony::CameraCapabilities &cameraCapabilities) override { return 0; }
};
#endif //  OHOS_BUILD_TEXT_CALL

class AICallServiceConnection : public AAFwk::AbilityConnectionStub {
public:
    AICallServiceConnection() = default;
    ~AICallServiceConnection() = default;
    void OnAbilityConnectDone(const AppExecFwk::ElementName &element,
        const sptr<IRemoteObject> &remoteObject, int32_t resultCode) override;
    void OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int32_t resultCode) override;
    sptr<IRemoteObject> GetRemoteObject();

private:
    sptr<IRemoteObject> remoteObject_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

enum class ServiceState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
};

class AccessibleTextCallManager {
    DECLARE_SINGLETON(AccessibleTextCallManager);

public:
    void SetServiceState(bool state);
#ifdef OHOS_BUILD_TEXT_CALL
    bool SendRequest(Telephony::TelCallState callState);
#endif // OHOS_BUILD_TEXT_CALL
    void OnServiceConnectDone();
    void OnServiceDisconnectDone();

private:
    bool RegisterCallServiceListener();
    void DeregisterCallServiceListener();
    bool ConnectAICallService();

private:
    std::mutex mutex_;
    ServiceState serviceState_ { ServiceState::DISCONNECTED };
    sptr<AICallServiceConnection> serviceConn_ { nullptr };
};
} // namespace Accessibility
} // namespace OHOS

#endif // ACCESSIBLE_TEXT_CALL_MANAGER_H
