#include "titan_enable_protocol.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <set>
#include <utility>
#include <vector>

namespace titan_protocol = studica_driver::titan_protocol;

using Request = std::pair<titan_protocol::EnableFrame, int32_t>;

TEST(TitanEnableProtocol, CancelsDisableScheduleBeforeEnabling) {
  std::vector<Request> requests;
  std::vector<uint32_t> delays;

  const bool success = titan_protocol::apply_enable_transition(
      true,
      [&](titan_protocol::EnableFrame frame, int32_t period_ms) {
        requests.emplace_back(frame, period_ms);
        return true;
      },
      [&](uint32_t delay_ms) { delays.push_back(delay_ms); });

  EXPECT_TRUE(success);
  EXPECT_EQ(requests, (std::vector<Request>{
                          {titan_protocol::EnableFrame::DISABLED,
                           titan_protocol::kCancelPeriodicFrame},
                          {titan_protocol::EnableFrame::ENABLED,
                           titan_protocol::kEnableHeartbeatPeriodMs},
                      }));
  EXPECT_TRUE(delays.empty());
}

TEST(TitanEnableProtocol, DoesNotEnableWhenDisableCancellationFails) {
  std::vector<Request> requests;

  const bool success = titan_protocol::apply_enable_transition(
      true,
      [&](titan_protocol::EnableFrame frame, int32_t period_ms) {
        requests.emplace_back(frame, period_ms);
        return false;
      },
      [](uint32_t) {});

  EXPECT_FALSE(success);
  ASSERT_EQ(requests.size(), 1u);
  EXPECT_EQ(requests.front().first, titan_protocol::EnableFrame::DISABLED);
  EXPECT_EQ(requests.front().second, titan_protocol::kCancelPeriodicFrame);
}

TEST(TitanEnableProtocol, CancelsEnableScheduleBeforeDisabling) {
  std::vector<Request> requests;
  std::vector<uint32_t> delays;

  const bool success = titan_protocol::apply_enable_transition(
      false,
      [&](titan_protocol::EnableFrame frame, int32_t period_ms) {
        requests.emplace_back(frame, period_ms);
        return true;
      },
      [&](uint32_t delay_ms) { delays.push_back(delay_ms); });

  EXPECT_TRUE(success);
  ASSERT_EQ(requests.size(), 4u);
  EXPECT_EQ(requests.front().first, titan_protocol::EnableFrame::ENABLED);
  EXPECT_EQ(requests.front().second, titan_protocol::kCancelPeriodicFrame);
  for (size_t index = 1; index < requests.size(); ++index) {
    EXPECT_EQ(requests[index].first, titan_protocol::EnableFrame::DISABLED);
    EXPECT_EQ(requests[index].second,
              titan_protocol::kDisableHeartbeatPeriodMs);
  }
  EXPECT_EQ(delays,
            (std::vector<uint32_t>(titan_protocol::kDisableWriteAttempts,
                                   titan_protocol::kDisableRetryDelayMs)));
}

TEST(TitanEnableProtocol, RepeatedTransitionsLeaveOnlyRequestedHeartbeat) {
  std::set<titan_protocol::EnableFrame> scheduled;
  const auto send = [&](titan_protocol::EnableFrame frame, int32_t period_ms) {
    if (period_ms < 0)
      scheduled.erase(frame);
    else if (period_ms > 0)
      scheduled.insert(frame);
    return true;
  };

  for (const bool enable : {false, true, false, true, true, false}) {
    ASSERT_TRUE(titan_protocol::apply_enable_transition(
        enable, send, [](uint32_t) {}));
    const auto expected = enable ? titan_protocol::EnableFrame::ENABLED :
                                   titan_protocol::EnableFrame::DISABLED;
    EXPECT_EQ(scheduled, (std::set<titan_protocol::EnableFrame>{expected}));
  }
}

TEST(TitanEnableProtocol, EnableHeartbeatFailureIsReported) {
  EXPECT_FALSE(titan_protocol::apply_enable_transition(
      true,
      [](titan_protocol::EnableFrame frame, int32_t) {
        return frame == titan_protocol::EnableFrame::DISABLED;
      },
      [](uint32_t) {}));
}

TEST(TitanEnableProtocol, FailedCancellationStillAttemptsEveryDisableWrite) {
  int disable_attempts = 0;
  EXPECT_FALSE(titan_protocol::apply_enable_transition(
      false,
      [&](titan_protocol::EnableFrame frame, int32_t) {
        if (frame == titan_protocol::EnableFrame::ENABLED)
          return false;
        ++disable_attempts;
        return true;
      },
      [](uint32_t) {}));
  EXPECT_EQ(disable_attempts, titan_protocol::kDisableWriteAttempts);
}
