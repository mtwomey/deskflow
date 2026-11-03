/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include <cmath>
#include <cstdint>

/**
 * @brief Pixel-precise scroll conversion used by the macOS smooth scroll option.
 *
 * Protocol wheel deltas are in "120 units" (one classic wheel notch = 120).
 * A fixed ratio of kPixelsPer120 pixels per 120 units is used (1 px == 4 units).
 *  - Server (capture): continuous (trackpad / Magic Mouse) pixel deltas are
 *    converted to units; the fractional remainder is carried so nothing is lost.
 *  - Client (inject): units are converted back to pixels; the fractional
 *    remainder is carried so sub-pixel and sub-120 deltas are never dropped.
 *
 * Kept free of Apple headers so it can be unit tested on its own.
 */
namespace deskflow::scroll {

inline constexpr double kUnitsPerNotch = 120.0;
inline constexpr double kPixelsPer120 = 30.0;

/**
 * @brief Carries the fractional part of a single scroll axis between calls.
 */
class AxisAccumulator
{
public:
  /**
   * @brief Adds @p value and returns the whole part to emit now (truncated toward zero).
   * The remainder is kept for the next call; a direction reversal discards a stale remainder.
   */
  int32_t push(double value)
  {
    if (value == 0.0)
      return 0;
    if (m_remainder != 0.0 && (value > 0) != (m_remainder > 0))
      m_remainder = 0.0;
    m_remainder += value;
    const double whole = std::trunc(m_remainder);
    m_remainder -= whole;
    return static_cast<int32_t>(whole);
  }

  double remainder() const
  {
    return m_remainder;
  }

  void reset()
  {
    m_remainder = 0.0;
  }

private:
  double m_remainder = 0.0;
};

//! Server side: pixel delta to protocol units.
inline double pixelsToUnits(double px)
{
  return px * kUnitsPerNotch / kPixelsPer120;
}

//! Client side: protocol units to pixel delta.
inline double unitsToPixels(double units)
{
  return units * kPixelsPer120 / kUnitsPerNotch;
}

//! Per-axis accumulators for one scroll direction (capture or inject).
struct PixelScrollState
{
  AxisAccumulator x;
  AxisAccumulator y;
};

} // namespace deskflow::scroll
