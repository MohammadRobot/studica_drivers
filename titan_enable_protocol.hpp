#pragma once

#include <cstdint>

namespace studica_driver::titan_protocol {

enum class EnableFrame {
  ENABLED,
  DISABLED,
};

constexpr int32_t kCancelPeriodicFrame = -1;
constexpr int32_t kEnableHeartbeatPeriodMs = 100;
constexpr int32_t kDisableHeartbeatPeriodMs = 10;
constexpr uint32_t kDisableRetryDelayMs = 50;
constexpr int kDisableWriteAttempts = 3;

/**
 * Apply the VMXCAN scheduling sequence for a Titan enable-state transition.
 *
 * VMXCAN retains every positive-period CAN frame until the same CAN ID is
 * cancelled with a negative period. The opposite state must therefore be
 * cancelled before the requested state is scheduled.
 */
template <typename Send, typename Delay>
bool apply_enable_transition(bool enable, Send &&send, Delay &&delay) {
  if (enable) {
    if (!send(EnableFrame::DISABLED, kCancelPeriodicFrame))
      return false;
    return send(EnableFrame::ENABLED, kEnableHeartbeatPeriodMs);
  }

  bool success = send(EnableFrame::ENABLED, kCancelPeriodicFrame);
  for (int attempt = 0; attempt < kDisableWriteAttempts; ++attempt) {
    success = send(EnableFrame::DISABLED, kDisableHeartbeatPeriodMs) && success;
    delay(kDisableRetryDelayMs);
  }
  return success;
}

} // namespace studica_driver::titan_protocol
