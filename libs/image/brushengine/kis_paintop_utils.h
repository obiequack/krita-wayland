/*
 *  Copyright (c) 2014 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __KIS_PAINTOP_UTILS_H
#define __KIS_PAINTOP_UTILS_H

#include "kis_global.h"
#include "kis_paint_information.h"
#include "kis_distance_information.h"

namespace KisPaintOpUtils {

template <class PaintOp>
bool paintFan(PaintOp &op,
              const KisPaintInformation &pi1,
              const KisPaintInformation &pi2,
              KisDistanceInformation *currentDistance,
              qreal fanCornersStep)
{
    const qreal angleStep = fanCornersStep;
    const qreal initialAngle = currentDistance->lastDrawingAngle();
    const qreal finalAngle = pi2.drawingAngleSafe(*currentDistance);
    const qreal fullDistance = shortestAngularDistance(initialAngle,
                                                       finalAngle);
    qreal lastAngle = initialAngle;

    int i = 0;

    while (shortestAngularDistance(lastAngle, finalAngle) > angleStep) {
        lastAngle = incrementInDirection(lastAngle, angleStep, finalAngle);

        qreal t = angleStep * i++ / fullDistance;

        QPointF pt = pi1.pos() + t * (pi2.pos() - pi1.pos());
        KisPaintInformation pi = KisPaintInformation::mix(pt, t, pi1, pi2);
        pi.overrideDrawingAngle(lastAngle);
        pi.paintAt(op, currentDistance);
    }

    return i;
}


template <class PaintOp>
void paintLine(PaintOp &op,
               const KisPaintInformation &pi1,
               const KisPaintInformation &pi2,
               KisDistanceInformation *currentDistance,
               bool fanCornersEnabled,
               qreal fanCornersStep)
{
    QPointF end = pi2.pos();
    qreal endTime = pi2.currentTime();

    KisPaintInformation pi = pi1;
    qreal t = 0.0;

    while ((t = currentDistance->getNextPointPosition(pi.pos(), end, pi.currentTime(), endTime)) >= 0.0) {
        pi = KisPaintInformation::mix(t, pi, pi2);

        if (fanCornersEnabled &&
            currentDistance->hasLastPaintInformation()) {

            paintFan(op,
                     currentDistance->lastPaintInformation(),
                     pi,
                     currentDistance,
                     fanCornersStep);
        }

        /**
         * A bit complicated part to ensure the registration
         * of the distance information is done in right order
         */
        pi.paintAt(op, currentDistance);
    }

    // Perform a spacing update between dabs if appropriate.
    if (currentDistance->needsSpacingUpdate()) {
        op.updateSpacing(pi2, *currentDistance);
    }
}

/**
 * A special class containing the previous position of the cursor for
 * the sake of painting the outline of the paint op. The main purpose
 * of this class is to ensure that the saved point does not equal to
 * the current one, which would cause a outline flicker. To echieve
 * this the class stores two previosly requested points instead of the
 * last one.
 */
class PositionHistory
{
public:
    /**
     * \return the previously used point, which is guaranteed not to
     *         be equal to \p pt and updates the history if needed
     */
    QPointF pushThroughHistory(const QPointF &pt) {
        QPointF result;
        const qreal pointSwapThreshold = 7.0;

        /**
         * We check x *and* y separately, because events generated by
         * a mouse device tend to come separately for x and y offsets.
         * Efficienty generating the 'stairs' pattern.
         */
        if (qAbs(pt.x() - m_second.x()) > pointSwapThreshold &&
            qAbs(pt.y() - m_second.y()) > pointSwapThreshold) {

            result = m_second;
            m_first = m_second;
            m_second = pt;
        } else {
            result = m_first;
        }

        return result;
    }

private:
    QPointF m_first;
    QPointF m_second;
};

bool checkSizeTooSmall(qreal scale, qreal width, qreal height)
{
    return scale * width < 0.01 || scale * height < 0.01;
}

inline qreal calcAutoSpacing(qreal value, qreal coeff)
{
    return coeff * (value < 1.0 ? value : sqrt(value));
}

QPointF calcAutoSpacing(const QPointF &pt, qreal coeff, qreal lodScale)
{
    const qreal invLodScale = 1.0 / lodScale;
    const QPointF lod0Point = invLodScale * pt;

    return lodScale * QPointF(calcAutoSpacing(lod0Point.x(), coeff), calcAutoSpacing(lod0Point.y(), coeff));
}

KisSpacingInformation effectiveSpacing(qreal dabWidth,
                                       qreal dabHeight,
                                       qreal extraScale,
                                       qreal rateExtraScale,
                                       bool distanceSpacingEnabled,
                                       bool isotropicSpacing,
                                       qreal rotation,
                                       bool axesFlipped,
                                       qreal spacingVal,
                                       bool autoSpacingActive,
                                       qreal autoSpacingCoeff,
                                       bool timedSpacingEnabled,
                                       qreal timedSpacingInterval,
                                       qreal lodScale)
{
    QPointF spacing;

    if (!isotropicSpacing) {
        if (autoSpacingActive) {
            spacing = calcAutoSpacing(QPointF(dabWidth, dabHeight), autoSpacingCoeff, lodScale);
        } else {
            spacing = QPointF(dabWidth, dabHeight);
            spacing *= spacingVal;
        }
    }
    else {
        qreal significantDimension = qMax(dabWidth, dabHeight);
        if (autoSpacingActive) {
            significantDimension = calcAutoSpacing(significantDimension, autoSpacingCoeff);
        } else {
            significantDimension *= spacingVal;
        }
        spacing = QPointF(significantDimension, significantDimension);
        rotation = 0.0;
        axesFlipped = false;
    }

    spacing *= extraScale;

    qreal scaledInterval = rateExtraScale <= 0.0 ? LONG_TIME :
                                                   timedSpacingInterval / rateExtraScale;

    return KisSpacingInformation(distanceSpacingEnabled, spacing, rotation, axesFlipped,
                                 timedSpacingEnabled, scaledInterval);
}

}

#endif /* __KIS_PAINTOP_UTILS_H */
