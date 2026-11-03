/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include <QTest>

class OSXScrollConvertTests : public QObject
{
  Q_OBJECT
private Q_SLOTS:
  void conversionRoundTrip();
  void notchIsFixedPixelCount();
  void accumulatorCarriesRemainder();
  void accumulatorKeepsSmallDeltas();
  void accumulatorDropsRemainderOnReversal();
  void accumulatorIgnoresZero();
  void accumulatorReset();
};
