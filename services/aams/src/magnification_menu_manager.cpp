/*
 * Copyright (C) 2025-2025 Huawei Device Co., Ltd.
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

// LCOV_EXCL_START
#include "magnification_menu_manager.h"
#include "accessibility_def.h"
#include "accessibility_display_manager.h"
#include "accessible_ability_manager_service.h"
#include "utils.h"

namespace OHOS {
namespace Accessibility {
namespace {
    const std::string FULL_SCREEN_PATH = "/system/etc/accessibility/fullScreen.png";
    const std::string WINDOW_PATH = "/system/etc/accessibility/window.png";
}

std::shared_ptr<MagnificationMenuManager> MagnificationMenuManager::menuMgr_ = nullptr;

std::shared_ptr<MagnificationMenuManager> MagnificationMenuManager::GetInstance()
{
    HILOG_DEBUG();
    if (menuMgr_ == nullptr) {
        menuMgr_ = std::make_shared<MagnificationMenuManager>();
    }
    return menuMgr_;
}

void MagnificationMenuManager::DeleteInstance()
{
    HILOG_DEBUG();
    if (menuMgr_ == nullptr) {
        HILOG_ERROR("menuMgr_ is nullptr.");
        return;
    }
    menuMgr_ = nullptr;
}

MagnificationMenuManager::MagnificationMenuManager()
{
    HILOG_DEBUG();
    manager_ = Singleton<AccessibleAbilityManagerService>::GetInstance().GetMagnificationMgr();
}

MagnificationMenuManager::~MagnificationMenuManager()
{
    if (menuWindow_ != nullptr) {
        menuWindow_->Hide();
        menuWindow_->Destroy();
        menuWindow_ = nullptr;
    }
    surfaceNode_ = nullptr;
    canvasNode_ = nullptr;
    bgpixelmap_ = nullptr;
    rosenImage_ = nullptr;
}

void MagnificationMenuManager::CreateMenuWindow()
{
    HILOG_DEBUG();
    GetWindowParam();
    menuRect_ = {(static_cast<int32_t>(screenWidth_ - menuSize_) - margin_), (
        static_cast<int32_t>(screenHeight_ - menuSize_) - margin_), menuSize_, menuSize_};
    sptr<Rosen::WindowOption> windowOption = new(std::nothrow) Rosen::WindowOption();
    if (windowOption == nullptr) {
        HILOG_ERROR("windowOption is null");
        return;
    }
    windowOption->SetWindowType(Rosen::WindowType::WINDOW_TYPE_MAGNIFICATION_MENU);
    windowOption->SetWindowMode(Rosen::WindowMode::WINDOW_MODE_FLOATING);
    windowOption->SetWindowRect(menuRect_);
    menuWindow_ = Rosen::Window::Create(MENU_NAME, windowOption);
    if (menuWindow_ == nullptr) {
        HILOG_ERROR("create failed");
        return;
    }
    menuWindow_->SetCornerRadius(MENU_CORNER_RADIUS);
    surfaceNode_ = menuWindow_->GetSurfaceNode();
    if (surfaceNode_ == nullptr) {
        HILOG_ERROR("surfaceNode_ is nullptr.");
        return;
    }
    rsUIContext_ = surfaceNode_->GetRSUIContext();
    canvasNode_ = Rosen::RSCanvasNode::Create(false, false, rsUIContext_);
    if (canvasNode_ == nullptr) {
        HILOG_ERROR("create canvasNode_ fail.");
        return;
    }
    canvasNode_->SetSkipCheckInMultiInstance(true);
    surfaceNode_->SetAbilityBGAlpha(BG_ALPHA);
    surfaceNode_->AddChild(canvasNode_, -1);
    canvasNode_->SetBounds(0, 0, menuSize_, menuSize_);
    canvasNode_->SetFrame(0, 0, menuSize_, menuSize_);
    canvasNode_->SetPositionZ(Rosen::RSSurfaceNode::POINTER_WINDOW_POSITION_Z);
    canvasNode_->SetRotation(0);
    rosenImage_ = std::make_shared<Rosen::RSImage>();
}

void MagnificationMenuManager::LoadMenuBgImage(uint32_t mode)
{
    HILOG_DEBUG();
    std::string path = FULL_SCREEN_PATH;
    if (mode == WINDOW_MAGNIFICATION) {
        path = WINDOW_PATH;
    }
    bgpixelmap_ = DecodePixelMap(path, allocatorType_);
    if (bgpixelmap_ == nullptr) {
        HILOG_ERROR("DecodePixelMap failed.");
        return;
    }
    if (canvasNode_ == nullptr) {
        HILOG_ERROR("canvasNode_ is nullptr.");
        return;
    }
    canvasNode_->SetBgImageWidth(menuSize_);
    canvasNode_->SetBgImageHeight(menuSize_);
    canvasNode_->SetBgImagePositionX(0);
    canvasNode_->SetBgImagePositionY(0);
    if (rosenImage_ == nullptr) {
        HILOG_ERROR("rosenImage_ is nullptr.");
        return;
    }
    rosenImage_->SetPixelMap(bgpixelmap_);
    rosenImage_->SetImageFit(static_cast<int>(Rosen::ImageFit::FILL));
    canvasNode_->SetBgImage(rosenImage_);
    menuWindow_->Show();
}

void MagnificationMenuManager::ShowMenuWindow(uint32_t mode)
{
    HILOG_INFO();
    GetWindowParam();
    currentType_ = Singleton<AccessibleAbilityManagerService>::GetInstance().GetMagnificationType();
    menuMode_ = Singleton<AccessibleAbilityManagerService>::GetInstance().GetMagnificationMode();
    if (currentType_ != SWITCH_MAGNIFICATION) {
        HILOG_WARN("no need show menu.");
        return;
    }

    if (menuWindow_ == nullptr) {
        HILOG_ERROR("window is null. need create.");
        CreateMenuWindow();
    }
    
    if (menuWindow_ == nullptr) {
        HILOG_ERROR("create window failed.");
        return;
    }

    LoadMenuBgImage(mode);
    FlushImplicitTransaction();
    isMenuShown_ = true;
}

void MagnificationMenuManager::DisableMenuWindow()
{
    HILOG_INFO();
    std::lock_guard<ffrt::mutex> lock(mutex_);
    if (!isMenuShown_) {
        HILOG_INFO("menu not shown.");
        return;
    }
    if (surfaceNode_ != nullptr) {
        surfaceNode_->SetVisible(false);
        surfaceNode_->ClearChildren();
        FlushImplicitTransaction();
    }

    isMenuShown_ = false;
    if (menuWindow_ != nullptr) {
        menuWindow_->Hide();
        menuWindow_->Destroy();
        menuWindow_ = nullptr;
    }
    surfaceNode_ = nullptr;
    canvasNode_ = nullptr;
    rsUIContext_ = nullptr;
    bgpixelmap_ = nullptr;
    rosenImage_ = nullptr;
}

void MagnificationMenuManager::MoveMenuWindow(int32_t deltaX, int32_t deltaY)
{
    HILOG_DEBUG();
    if (menuWindow_ == nullptr) {
        HILOG_ERROR("menuWindow_ is null.");
        return;
    }
    int32_t menuPosX = menuRect_.posX_ + deltaX;
    int32_t menuPosY = menuRect_.posY_ + deltaY;

    menuRect_.posX_ = menuPosX;
    menuRect_.posY_ = menuPosY;
    
    AdjustMenuPosition();
    menuWindow_->MoveTo(menuRect_.posX_, menuRect_.posY_);
}

void MagnificationMenuManager::AttachToEdge()
{
    if (menuWindow_ == nullptr) {
        HILOG_ERROR("menuWindow_ is null.");
        return;
    }
    
    if (menuRect_.posX_ < static_cast<int32_t>(screenWidth_ / DIVISOR_TWO)) {
        menuRect_.posX_ = margin_;
    } else {
        menuRect_.posX_ = static_cast<int32_t>(screenWidth_ - menuSize_) - margin_;
    }
    menuWindow_->MoveTo(menuRect_.posX_, menuRect_.posY_);
}

void MagnificationMenuManager::SetCurrentType(uint32_t type)
{
    currentType_ = type;
}

bool MagnificationMenuManager::IsTapOnMenu(int32_t posX, int32_t posY)
{
    if (!isMenuShown_) {
        return false;
    }
    return Utils::IsInRect(posX, posY, menuRect_);
}

void MagnificationMenuManager::OnMenuTap()
{
    if (manager_ == nullptr) {
        HILOG_ERROR("manager_ is null.");
        return;
    }
    DisableMenuWindow();
    ChangeMode();
    manager_->OnModeChanged(menuMode_);
    ShowMenuWindow(menuMode_);
}

void MagnificationMenuManager::ChangeMode()
{
    if (menuMode_ == WINDOW_MAGNIFICATION) {
        menuMode_ = FULL_SCREEN_MAGNIFICATION;
        return;
    }
    if (menuMode_ == FULL_SCREEN_MAGNIFICATION) {
        menuMode_ = WINDOW_MAGNIFICATION;
        return;
    }
}

void MagnificationMenuManager::AdjustMenuPosition()
{
    if (menuRect_.posX_ < margin_) {
        menuRect_.posX_ = margin_;
    }

    if (menuRect_.posY_ < margin_) {
        menuRect_.posY_ = margin_;
    }

    if (menuRect_.posX_ + static_cast<int32_t>(menuRect_.width_) + margin_ > static_cast<int32_t>(screenWidth_)) {
        menuRect_.posX_ = static_cast<int32_t>(screenWidth_) - static_cast<int32_t>(menuRect_.width_) - margin_;
    }

    if (menuRect_.posY_ + static_cast<int32_t>(menuRect_.height_) + margin_ > static_cast<int32_t>(screenHeight_)) {
        menuRect_.posY_ = static_cast<int32_t>(screenHeight_) - static_cast<int32_t>(menuRect_.height_) - margin_;
    }
}

std::shared_ptr<Media::PixelMap> MagnificationMenuManager::DecodePixelMap(
    const std::string& pathName, const Media::AllocatorType& allocatorType)
{
    uint32_t errCode = 0;
    std::unique_ptr<Media::ImageSource> imageSource =
        Media::ImageSource::CreateImageSource(pathName, Media::SourceOptions(), errCode);
    if (imageSource == nullptr || errCode != 0) {
        HILOG_ERROR("CreateImageSource failed.");
        return nullptr;
    }

    Media::DecodeOptions decodeOpt;
    decodeOpt.allocatorType = allocatorType;
    std::shared_ptr<Media::PixelMap> pixelmap = imageSource->CreatePixelMap(decodeOpt, errCode);
    if (pixelmap == nullptr || errCode != 0) {
        HILOG_ERROR("CreatePixelMap failed.");
        return nullptr;
    }
    return pixelmap;
}

void MagnificationMenuManager::GetWindowParam()
{
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    uint64_t screenId_ = displayMgr.GetDefaultDisplayId();
    OHOS::Rosen::DisplayOrientation currentOrientation = displayMgr.GetOrientation();
    orientation_ = currentOrientation;
    sptr<OHOS::Rosen::Display> display = displayMgr.GetDisplay(screenId_);
    if (display == nullptr) {
        HILOG_ERROR("display is nullptr.");
        return;
    }
    screenWidth_ = static_cast<uint32_t>(display->GetWidth());
    screenHeight_ = static_cast<uint32_t>(display->GetHeight());
    screenRect_ = {0, 0, screenWidth_, screenHeight_};
#else
    HILOG_INFO("not support");
#endif
}

void MagnificationMenuManager::RefreshWindowParam()
{
    HILOG_DEBUG();
    if (isMenuShown_) {
        DisableMenuWindow();
        ShowMenuWindow(menuMode_);
    } else {
        GetWindowParam();
    }
}

void MagnificationMenuManager::FlushImplicitTransaction()
{
    if (rsUIContext_ != nullptr) {
        auto rsTransaction = rsUIContext_->GetRSTransaction();
        if (rsTransaction != nullptr) {
            rsTransaction->FlushImplicitTransaction();
        }
    } else {
        Rosen::RSTransaction::FlushImplicitTransaction();
    }
}
} // namespace Accessibility
} // namespace OHOS
// LCOV_EXCL_STOP