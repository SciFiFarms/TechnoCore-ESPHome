#include "dosage_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dosage {

static const char *TAG = "dosage.switch";


void DosageSwitch::dump_config() { LOG_SWITCH("", "Dosage Switch", this); }
void DosageSwitch::setup() {
  this->GPIOSwitch::setup();

}

void DosageSwitch::dose_for_ms(uint32_t *time_in_ms) {
  ESP_LOGD(TAG,"DosageSwitch::dose_for_ms()");
  this->turn_on();
  delay(*time_in_ms);
  this->turn_off();
}

void DosageSwitch::dose_for_seconds(uint32_t *time_in_seconds) {
  uint32_t time_in_ms;
  time_in_ms = *time_in_seconds * 1000;
  this->dose_for_ms(&time_in_ms);
}

void DosageSwitch::dose(){
  ESP_LOGD(TAG,"Dosing: %d", this->dosage_);
  this->dose_for_ms((&this->dosage_));
}

}  // namespace dosage
}  // namespace esphome
