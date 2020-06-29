#pragma once

#include "esphome/core/component.h"
#include "esphome/components/gpio/switch/gpio_switch.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace dosage {

class DosageSwitch : public gpio::GPIOSwitch { //, public Component {
 public:
  void set_dosage(uint32_t dosage) { this->dosage_ = dosage; };
  void dose_for_ms(uint32_t *time_in_ms) ;
  void dose_for_seconds(uint32_t *time_in_seconds) ;
  void dose();

  void setup() override;
  void dump_config() override;

 protected:
  uint32_t dosage_;
};

}  // namespace dosage
}  // namespace esphome
