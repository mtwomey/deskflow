/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "OSXScrollConvertTests.h"

#include "platform/OSXScrollConvert.h"

using namespace deskflow::scroll;

void OSXScrollConvertTests::conversionRoundTrip()
{
  QCOMPARE(pixelsToUnits(1.0), 4.0);
  QCOMPARE(unitsToPixels(4.0), 1.0);
  QCOMPARE(unitsToPixels(pixelsToUnits(-7.5)), -7.5);
}

void OSXScrollConvertTests::notchIsFixedPixelCount()
{
  QCOMPARE(unitsToPixels(kUnitsPerNotch), kPixelsPer120);
  QCOMPARE(unitsToPixels(-2 * kUnitsPerNotch), -2 * kPixelsPer120);
}

void OSXScrollConvertTests::accumulatorCarriesRemainder()
{
  AxisAccumulator acc;
  QCOMPARE(acc.push(0.75), 0);
  QCOMPARE(acc.remainder(), 0.75);
  QCOMPARE(acc.push(0.75), 1);
  QCOMPARE(acc.remainder(), 0.5);
  QCOMPARE(acc.push(2.5), 3);
  QCOMPARE(acc.remainder(), 0.0);
}

void OSXScrollConvertTests::accumulatorKeepsSmallDeltas()
{
  // 30 deltas of one protocol unit (a quarter pixel each) scroll 7 pixels, none are lost.
  AxisAccumulator acc;
  int32_t total = 0;
  for (int i = 0; i < 30; ++i) {
    total += acc.push(unitsToPixels(1.0));
  }
  QCOMPARE(total, 7);
  QCOMPARE(acc.remainder(), 0.5);

  AxisAccumulator neg;
  total = 0;
  for (int i = 0; i < 8; ++i) {
    total += neg.push(-0.5);
  }
  QCOMPARE(total, -4);
}

void OSXScrollConvertTests::accumulatorDropsRemainderOnReversal()
{
  AxisAccumulator acc;
  QCOMPARE(acc.push(0.9), 0);
  QCOMPARE(acc.push(-0.4), 0);
  QCOMPARE(acc.remainder(), -0.4);
  QCOMPARE(acc.push(-0.6), -1);
}

void OSXScrollConvertTests::accumulatorIgnoresZero()
{
  AxisAccumulator acc;
  QCOMPARE(acc.push(0.6), 0);
  QCOMPARE(acc.push(0.0), 0);
  QCOMPARE(acc.remainder(), 0.6);
}

void OSXScrollConvertTests::accumulatorReset()
{
  AxisAccumulator acc;
  acc.push(0.6);
  acc.reset();
  QCOMPARE(acc.remainder(), 0.0);
  QCOMPARE(acc.push(0.6), 0);
}

QTEST_MAIN(OSXScrollConvertTests)
