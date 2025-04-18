/*
 * Copyright (C) 2022-2025 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_ELEMENT_INFO_PARCEL_H
#define ACCESSIBILITY_ELEMENT_INFO_PARCEL_H

#include "accessibility_element_info.h"
#include "parcel.h"

namespace OHOS {
namespace Accessibility {
/*
* class define the action on Accessibility info
*/
class AccessibleActionParcel : public AccessibleAction, public Parcelable {
public:
    /**
     * @brief Construct
     */
    AccessibleActionParcel() = default;

    /**
     * @brief Construct
     * @param action The object of AccessibleAction
     */
    explicit AccessibleActionParcel(const AccessibleAction &action);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @return true: Read parcel date successfully; otherwise is not
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @return Read AccessibleActionParcel from parcel data
     */
    static AccessibleActionParcel *Unmarshalling(Parcel &parcel);
};

/**
 * @brief  Define the RangInfo for progress bar
 * @note
 * @retval None
 */
class RangeInfoParcel : public RangeInfo, public Parcelable {
public:
    /**
     * @brief Construct
     */
    RangeInfoParcel() = default;

    /**
     * @brief Construct
     * @param rangeInfo The object of RangeInfo.
     */
    explicit RangeInfoParcel(const RangeInfo &rangeInfo);

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    static RangeInfoParcel *Unmarshalling(Parcel &parcel);
};

/**
 * @brief  Define the list/grid component
 * @note
 * @retval None
 */
class GridInfoParcel : public GridInfo, public Parcelable {
public:
    /**
     * @brief Construct
     * @since 3
     * @sysCap Accessibility
     */
    GridInfoParcel() = default;

    /**
     * @brief Construct
     * @param gridInfo The object of GridInfo.
     * @since 3
     * @sysCap Accessibility
     */
    explicit GridInfoParcel(const GridInfo &gridInfo);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    static GridInfoParcel *Unmarshalling(Parcel &parcel);
};

class ExtraElementInfoParcel : public ExtraElementInfo, public Parcelable {
public:
    /**
     * @brief Construct
     * @sysCap Accessibility
     */
    ExtraElementInfoParcel() = default;

    /**
     * @brief Construct
     * @param gridInfo The object of GridInfo.
     * @sysCap Accessibility
     */
    explicit ExtraElementInfoParcel(const ExtraElementInfo &extraElementInfo);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @sysCap Accessibility
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @sysCap Accessibility
     */
    virtual bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @sysCap Accessibility
     */
    static ExtraElementInfoParcel *Unmarshalling(Parcel &parcel);
};

class GridItemInfoParcel : public GridItemInfo, public Parcelable {
public:
    /**
     * @brief Construct
     * @since 3
     * @sysCap Accessibility
     */
    GridItemInfoParcel() = default;

    /**
     * @brief Construct
     * @param itemInfo The object of GridItemInfo.
     * @since 3
     * @sysCap Accessibility
     */
    explicit GridItemInfoParcel(const GridItemInfo &itemInfo);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    static GridItemInfoParcel *Unmarshalling(Parcel &parcel);
};

class RectParcel : public Rect, public Parcelable {
public:
    /**
     * @brief Construct
     * @since 3
     * @sysCap Accessibility
     */
    RectParcel() = default;

    /**
     * @brief Construct
     * @param rect The object of Rect.
     * @since 3
     * @sysCap Accessibility
     */
    explicit RectParcel(const Rect &rect);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @since 3
     * @sysCap Accessibility
     */
    static RectParcel *Unmarshalling(Parcel &parcel);
};

/*
* class define the span info
*/
class SpanInfoParcel : public SpanInfo, public Parcelable {
public:
    /**
     * @brief Construct
     */
    SpanInfoParcel() = default;

    /**
     * @brief Construct
     * @param rangeInfo The object of SpanInfo.
     */
    explicit SpanInfoParcel(const SpanInfo &spanInfo);

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    virtual bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     */
    static SpanInfoParcel *Unmarshalling(Parcel &parcel);
};

/*
* The class supply the api to set/get ui component property
*/
class AccessibilityElementInfoParcel : public AccessibilityElementInfo, public Parcelable {
public:
    /**
     * @brief Construct
     * @since 3
     * @sysCap Accessibility
     */
    AccessibilityElementInfoParcel() = default;

    /**
     * @brief Construct
     * @param elementInfo The object of AccessibilityElementInfo.
     * @since 3
     * @sysCap Accessibility
     */
    explicit AccessibilityElementInfoParcel(const AccessibilityElementInfo &elementInfo);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @return true: Read parcel data successfully; ohterwise is not.
     * @since 3
     * @sysCap Accessibility
     */
    bool ReadFromParcel(Parcel &parcel);

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @return true: Write parcel data successfully; ohterwise is not.
     * @since 3
     * @sysCap Accessibility
     */
    bool Marshalling(Parcel &parcel) const override;

    /**
     * @brief Used for IPC communication
     * @param parcel
     * @return Read AccessibilityElementInfoParcel from parcel data
     * @since 3
     * @sysCap Accessibility
     */
    static AccessibilityElementInfoParcel *Unmarshalling(Parcel &parcel);

private:
     /**
     * @brief Used for IPC communication first part
     * @param parcel
     * @sysCap Accessibility
     */
    bool ReadFromParcelFirstPart(Parcel &parcel);

     /**
     * @brief Used for IPC communication second part
     * @param parcel
     * @sysCap Accessibility
     */
    bool ReadFromParcelSecondPart(Parcel &parcel);

     /**
     * @brief Used for IPC communication third part
     * @param parcel
     * @sysCap Accessibility
     */
    bool ReadFromParcelThirdPart(Parcel &parcel);

     /**
     * @brief Used for IPC communication fourth part
     * @param parcel
     * @sysCap Accessibility
     */
    bool ReadFromParcelFourthPart(Parcel &parcel);

    /**
     * @brief Used for IPC communication first part
     * @param parcel
     * @sysCap Accessibility
     */
    bool MarshallingFirstPart(Parcel &parcel) const;

    /**
     * @brief Used for IPC communication second part
     * @param parcel
     * @sysCap Accessibility
     */
    bool MarshallingSecondPart(Parcel &parcel) const;

    /**
     * @brief Used for IPC communication second part
     * @param parcel
     * @sysCap Accessibility
     */
    bool MarshallingThirdPart(Parcel &parcel) const;
};
} // namespace Accessibility
} // namespace OHOS
#endif