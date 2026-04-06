#ifndef PWM_H
#define PWM_H

#include <stdio.h>
#include <memory>
#include "VMXPi.h"

namespace studica_driver {

enum class PWMType {
    Standard,
    Continuous,
    Linear
};

class PWM {
public:
    PWM(VMXChannelIndex port, PWMType type, int min = -150, int max = 150, std::shared_ptr<VMXPi> vmx = std::make_shared<VMXPi>(true, 50));
    ~PWM();
    void SetBounds(double min, double center, double max);

protected:
    VMXChannelIndex port_;
    PWMType type_;
    int min_;
    int max_;
    std::shared_ptr<VMXPi> vmx_;
    VMXResourceHandle pwm_res_handle_ = VMXResourceHandle();
    int min_us_ = 0;
    int max_us_ = 0;
    int center_us_ = 0;
    int prev_pwm_pwm_value_;
    bool initialized_ = false;

    int Map(int value);
    bool IsInitialized() const { return initialized_; }
    void DisplayVMXError(VMXErrorCode vmxerr);
};

}

#endif // PWM_H
