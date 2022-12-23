/*
 *  SPDX-FileCopyrightText: 2022 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef KISSTANDARDOPTIONDATA_H
#define KISSTANDARDOPTIONDATA_H

#include <KisCurveOptionData.h>


struct KisOpacityOptionData : KisCurveOptionData
{
    KisOpacityOptionData(bool isCheckable = false, const QString &prefix = QString())
        : KisCurveOptionData(
              prefix,
              KoID("Opacity", i18n("Opacity")),
              isCheckable, !isCheckable)
    {
    }
};

struct KisFlowOptionData : KisCurveOptionData
{
    KisFlowOptionData(bool isCheckable = false, const QString &prefix = QString())
        : KisCurveOptionData(
              prefix,
              KoID("Flow", i18n("Flow")),
              isCheckable,
              !isCheckable)
    {
    }
};


struct KisRatioOptionData : KisCurveOptionData
{
    KisRatioOptionData(const QString &prefix = QString())
        : KisCurveOptionData(
              prefix,
              KoID("Ratio", i18n("Ratio")))
    {
    }
};

struct KisSoftnessOptionData : KisCurveOptionData
{
    KisSoftnessOptionData()
        : KisCurveOptionData(
              KoID("Softness", i18n("Softness")),
              true, false,
              0.1, 1.0)
    {}
};

struct KisRotationOptionData : KisCurveOptionData
{
    KisRotationOptionData(const QString &prefix = QString())
        : KisCurveOptionData(
              prefix,
              KoID("Rotation", i18n("Rotation")))
    {
    }
};

struct KisDarkenOptionData : KisCurveOptionData
{
    KisDarkenOptionData()
        : KisCurveOptionData(
              KoID("Darken", i18n("Darken")))
    {}
};

struct KisMixOptionData : KisCurveOptionData
{
    KisMixOptionData()
        : KisCurveOptionData(
              KoID("Mix", i18nc("Mixing of colors", "Mix")))
    {}
};

struct KisHueOptionData : KisCurveOptionData
{
    KisHueOptionData()
        : KisCurveOptionData(
              KoID("h", i18n("Hue")))
    {}
};

struct KisSaturationOptionData : KisCurveOptionData
{
    KisSaturationOptionData()
        : KisCurveOptionData(
              KoID("s", i18n("Saturation")))
    {}
};

struct KisValueOptionData : KisCurveOptionData
{
    KisValueOptionData()
        : KisCurveOptionData(
              KoID("v", i18nc("Label of Brightness value in Color Smudge brush engine options", "Value")))
    {}
};

struct KisRateOptionData : KisCurveOptionData
{
    KisRateOptionData()
        : KisCurveOptionData(
              KoID("Rate", i18n("Rate")))
    {}
};

struct KisStrengthOptionData : KisCurveOptionData
{
    KisStrengthOptionData()
        : KisCurveOptionData(
              KoID("Texture/Strength/", i18n("Strength")))
    {}
};

struct KisLightnessStrengthOptionData : KisCurveOptionData
{
    KisLightnessStrengthOptionData()
        : KisCurveOptionData(
              KoID("LightnessStrength", i18n("Lightness Strength")))
    {
    }
};


class KisCurveOptionWidget2;

namespace KisPaintOpOptionWidgetUtils {

PAINTOP_EXPORT KisCurveOptionWidget2* createOpacityOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createFlowOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createRatioOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createSoftnessOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createRotationOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createDarkenOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMixOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createHueOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createSaturationOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createValueOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createRateOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createStrengthOptionWidget();

PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingOpacityOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingSizeOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingFlowOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingRatioOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingRotationOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingMirrorOptionWidget();
PAINTOP_EXPORT KisCurveOptionWidget2* createMaskingScatterOptionWidget();

}

#endif // KISSTANDARDOPTIONDATA_H
