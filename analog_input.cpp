#include "analog_input.h"
using namespace studica_driver;

AnalogInput::AnalogInput(VMXChannelIndex port, std::shared_ptr<VMXPi> vmx)
    : vmx_(vmx), port_(port), accumulator_res_handle_(VMXResourceHandle()) {

    VMXErrorCode vmxerr;
    float full_scale_voltage = 0.0f;
    if (!vmx_ || !vmx_->IsOpen()) {
        std::cout << "VMX is not open." << std::endl;
        return;
    } else {
        if (vmx_->io.Accumulator_GetFullScaleVoltage(full_scale_voltage, &vmxerr)) {
            std::cout << "Analog input voltage: " << full_scale_voltage << std::endl;
        } else { 
            std::cerr << "ERROR acquiring Analog Input Voltage." << std::endl;
            DisplayVMXError(vmxerr);
        }
    }

    AccumulatorConfig accum_config;
    accum_config.SetNumAverageBits(9);
    
    // Configure the analog accumulator for the specified channel
    if (!vmx_->io.ActivateSinglechannelResource(
            VMXChannelInfo(port_, VMXChannelCapability::AccumulatorInput),
            &accum_config, 
            accumulator_res_handle_, 
            &vmxerr)) {
        std::cerr << "Error activating analog channel " << port_ << std::endl;
        DisplayVMXError(vmxerr);
        return;
    }
    std::cout << "Analog Input Channel " << port_ << "activated." << std::endl;
    initialized_ = true;
}

AnalogInput::~AnalogInput() {
    if (!initialized_ || !vmx_ || !vmx_->IsOpen()) {
        return;
    }
    VMXErrorCode vmxerr;
    if (!vmx_->io.DeallocateResource(accumulator_res_handle_, &vmxerr)) {
        std::cerr << "Failed to deallocate analog channel " << port_ << std::endl;
        DisplayVMXError(vmxerr);
    }
}

bool AnalogInput::GetAverageVoltage(float &volt) {
    if (!initialized_ || !vmx_ || !vmx_->IsOpen()) {
        std::cerr << "AnalogInput on channel " << port_ << " is not initialized." << std::endl;
        return false;
    }
    VMXErrorCode vmxerr;
    
    if (vmx_->io.Accumulator_GetAverageVoltage(accumulator_res_handle_, volt, &vmxerr)) {
        return true;
    } else {
        std::cerr << "Error getting Average Voltage for channel " << port_ << std::endl;
        DisplayVMXError(vmxerr);
        return false;
    }
}

void AnalogInput::DisplayVMXError(VMXErrorCode vmxerr) {
    const char *p_err_description = GetVMXErrorString(vmxerr);
    std::cerr << "VMXError " << vmxerr << ": " << p_err_description << std::endl;
}
