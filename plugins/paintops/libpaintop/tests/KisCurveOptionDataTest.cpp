/*
 *  SPDX-FileCopyrightText: 2022 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "KisCurveOptionDataTest.h"

#include <KisCurveOptionData.h>

void KisCurveOptionDataTest::testCurveOptionData()
{
    KisCurveOptionData data(KoID("Opacity"),
                            KisPaintOpOption::GENERAL,
                            true,
                            true);

    KisCurveOptionData data2 = data;

    QVERIFY(data == data2);

    data2.isChecked = false;

    QVERIFY(data != data2);

    data2 = data;

    QVERIFY(data == data2);

    data2.sensorStruct().sensorPressure.isActive = true;
    data2.sensorStruct().sensorPressure.curve = "0.0,0.5;1,1;";

    QVERIFY(data != data2);

    data2 = data;

    QVERIFY(data == data2);

    data2.sensorStruct().sensorPressure.isActive = true;
    data2.sensorStruct().sensorPressure.curve = "0.0,0.5;1,1;";

    KisPropertiesConfiguration config;

    data2.write(&config);

    QVERIFY(data != data2);

    QVERIFY(data.read(&config));

    QVERIFY(data == data2);

}

void KisCurveOptionDataTest::testSerializeDisabledSensors()
{
    KisCurveOptionData data(KoID("Opacity"),
                            KisPaintOpOption::GENERAL);

    // sensor is disabled!
    data.sensorStruct().sensorPressure.isActive = false;
    data.sensorStruct().sensorPressure.curve = "0.0,0.5;1,1;";

    data.sensorStruct().sensorRotation.isActive = true;
    data.sensorStruct().sensorRotation.curve = "0.0,0.5;1,1;";

    KisCurveOptionData data2(KoID("Opacity"),
                             KisPaintOpOption::GENERAL);

    data2 = data;
    QVERIFY(data == data2);

    data2 = KisCurveOptionData(KoID("Opacity"),
                               KisPaintOpOption::GENERAL);
    QVERIFY(data != data2);

    KisPropertiesConfiguration config;

    data.write(&config);
    QVERIFY(data2.read(&config));

    /**
     * The disabled sensor is **not** saved into the serialized
     * form, so the roundtripping such data will not result
     * in the same data in C++ sense.
     */

    QCOMPARE(data2.sensorStruct().sensorPressure.isActive, false);
    QCOMPARE(data2.sensorStruct().sensorRotation.isActive, true);

    QVERIFY(data != data2);
    data2.sensorStruct().sensorPressure.curve = data.sensorStruct().sensorPressure.curve;
    QVERIFY(data == data2);

}

void KisCurveOptionDataTest::testSerializeNoSensors()
{
    KisCurveOptionData data(KoID("Opacity"),
                            KisPaintOpOption::GENERAL);


    KisCurveOptionData data2(KoID("Opacity"),
                             KisPaintOpOption::GENERAL);

    QVERIFY(data == data2);

    KisPropertiesConfiguration config;

    data.write(&config);
    QVERIFY(data2.read(&config));

    /**
     * When Krita loads a configuration with no sensors
     * available, it automatically acitvates a pressure
     * sensors with the default curve.
     */

    QCOMPARE(data.sensorStruct().sensorPressure.isActive, false);
    QCOMPARE(data2.sensorStruct().sensorPressure.isActive, true);
    QCOMPARE(data2.sensorStruct().sensorPressure.curve, DEFAULT_CURVE_STRING);
    QVERIFY(data != data2);
}

SIMPLE_TEST_MAIN(KisCurveOptionDataTest)
