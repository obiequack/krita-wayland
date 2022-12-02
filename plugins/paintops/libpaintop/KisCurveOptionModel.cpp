/*
 *  SPDX-FileCopyrightText: 2022 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "KisCurveOptionModel.h"

#include <lager/lenses/tuple.hpp>
#include <KisZug.h>

auto activeCurveLens = lager::lenses::getset(
    [](const std::tuple<KisCurveOptionDataCommon, QString> &data) -> QString {
        QString activeCurve;
        const bool useSameCurve = std::get<0>(data).useSameCurve;

        if (useSameCurve) {
            activeCurve = std::get<0>(data).commonCurve;
        } else {
            const QString activeSensorId = std::get<1>(data);
            KIS_SAFE_ASSERT_RECOVER_RETURN_VALUE(!activeSensorId.isEmpty(), activeCurve);
            std::vector<const KisSensorData*> srcSensors = std::get<0>(data).sensors();
            auto it =
                std::find_if(srcSensors.begin(), srcSensors.end(),
                    [activeSensorId] (const KisSensorData *sensor) {
                        return sensor->id.id() == activeSensorId;
                    });

            KIS_SAFE_ASSERT_RECOVER_NOOP(it != srcSensors.end());

            if (it != srcSensors.end()) {
                activeCurve = (*it)->curve;
            }
        }

        return activeCurve;
    },
    [](std::tuple<KisCurveOptionDataCommon, QString> data, QString curve) {
        const bool useSameCurve = std::get<0>(data).useSameCurve;

        if (useSameCurve) {
            std::get<0>(data).commonCurve = curve;
        } else {
            const QString activeSensorId = std::get<1>(data);
            KIS_SAFE_ASSERT_RECOVER_RETURN_VALUE(!activeSensorId.isEmpty(), data);
            std::vector<KisSensorData*> srcSensors = std::get<0>(data).sensors();
            auto it =
                std::find_if(srcSensors.begin(), srcSensors.end(),
                    [activeSensorId] (const KisSensorData *sensor) {
                        return sensor->id.id() == activeSensorId;
                    });

            KIS_SAFE_ASSERT_RECOVER_NOOP(it != srcSensors.end());

            if (it != srcSensors.end()) {
                (*it)->curve = curve;
            }
        }

        return data;
    });

int calcActiveSensorLength(const KisCurveOptionDataCommon &data, const QString &activeSensorId) {
    return data.sensorData->calcActiveSensorLength(activeSensorId);
}


KisCurveOptionModel::KisCurveOptionModel(lager::cursor<KisCurveOptionDataCommon> _optionData,
                                         lager::reader<bool> externallyEnabled,
                                         std::optional<lager::reader<RangeState>> rangeOverride)
    : optionData(_optionData)
    , rangeNorm(rangeOverride ? *rangeOverride :
                      lager::with(optionData[&KisCurveOptionDataCommon::strengthMinValue],
                                  optionData[&KisCurveOptionDataCommon::strengthMaxValue]))
    , activeSensorIdData(PressureId.id())
    , LAGER_QT(isCheckable) {optionData[&KisCurveOptionDataCommon::isCheckable]}
    , LAGER_QT(isChecked) {optionData[&KisCurveOptionDataCommon::isChecked]}
    , LAGER_QT(effectiveIsChecked) {lager::with(LAGER_QT(isChecked), externallyEnabled).map(std::logical_and{})}
    , LAGER_QT(effectiveStrengthValueNorm) {
          lager::with(rangeNorm.zoom(lager::lenses::first),
                      optionData[&KisCurveOptionDataCommon::strengthValue],
                      rangeNorm.zoom(lager::lenses::second))
                  .map(&qBound<qreal>)}
    , LAGER_QT(strengthValueDenorm) {
          optionData[&KisCurveOptionDataCommon::strengthValue]
                  .zoom(kiszug::lenses::scale<qreal>(100.0))}
    , LAGER_QT(effectiveStrengthStateDenorm) {
          lager::with(LAGER_QT(effectiveStrengthValueNorm),
                      rangeNorm.zoom(lager::lenses::first),
                      rangeNorm.zoom(lager::lenses::second))
            .xform(kiszug::foreach_arg(kiszug::map_mupliply<qreal>(100.0)))}
    , LAGER_QT(useCurve) {optionData[&KisCurveOptionDataCommon::useCurve]}
    , LAGER_QT(useSameCurve) {optionData[&KisCurveOptionDataCommon::useSameCurve]}
    , LAGER_QT(curveMode) {optionData[&KisCurveOptionDataCommon::curveMode]}
    , LAGER_QT(activeSensorId) {activeSensorIdData}
    , LAGER_QT(activeSensorLength) {lager::with(optionData, activeSensorIdData).map(&calcActiveSensorLength)}
    , LAGER_QT(labelsState) {lager::with(LAGER_QT(activeSensorId), LAGER_QT(activeSensorLength))}
    , LAGER_QT(activeCurve) {lager::with(optionData,
                                         LAGER_QT(activeSensorId))
            .zoom(activeCurveLens)}
{
}

KisCurveOptionModel::~KisCurveOptionModel()
{
}

KisCurveOptionDataCommon KisCurveOptionModel::bakedOptionData() const
{
    KisCurveOptionDataCommon data = optionData.get();
    data.isChecked = effectiveIsChecked();
    std::tie(data.strengthMinValue, data.strengthMaxValue) =
        rangeNorm.get();
    data.strengthValue = effectiveStrengthValueNorm();
    return data;
}
