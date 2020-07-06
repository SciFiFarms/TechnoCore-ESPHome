#include "dosage_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dosage {

void DosageSwitch::dump_config() { LOG_SWITCH("", "Dosage Switch", this); }
void DosageSwitch::setup() {
  using gpio::GPIOSwitch;
  ESP_LOGW(TAG,"DosageSwitch::setup()");
  this->GPIOSwitch::setup();

  subscribe(this->topic_dose_for_seconds, &DosageSwitch::on_dose_for_seconds);
  subscribe(this->topic_dose_for_ms, &DosageSwitch::on_dose_for_ms);
  subscribe(this->topic_set_dosage, &DosageSwitch::on_set_dosage);
  subscribe(this->topic_dose, &DosageSwitch::on_dose);
}

void DosageSwitch::dose_for_ms(uint32_t time_in_ms) {
  ESP_LOGD(TAG,"Dosing for %d ms", time_in_ms);
  this->turn_on();
  delay(time_in_ms);
  this->turn_off();
}

void DosageSwitch::dose_for_seconds(uint32_t time_in_seconds) {
  uint32_t time_in_ms;
  time_in_ms = time_in_seconds * 1000;
  this->dose_for_ms(time_in_ms);
}

void DosageSwitch::dose(){
  this->dose_for_ms((this->dosage_->state));
}

}  // namespace dosage
}  // namespace esphome
