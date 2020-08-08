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
  ESP_LOGD(TAG,"Dosing %s for %d ms", this->get_name().c_str(), time_in_ms);
  this->toggle();
  if(this->exact_timing_) {
    delay(time_in_ms);
    this->toggle();
  }
  else {
    this->set_timeout(this->dosage_->get_object_id(), time_in_ms, [this]() {
      this->toggle();
    });
  }
}

void DosageSwitch::dose_for_seconds(uint32_t time_in_seconds) {
  uint32_t time_in_ms;
  time_in_ms = time_in_seconds * 1000;
  this->dose_for_ms(time_in_ms);
}

void DosageSwitch::dose(){
  this->dose_for_ms((this->dosage_->state));
}

void DosageSwitch::cancel_dose(){
  // cancel_timeout returns true if a timeout was cancelled. 
  int rc = this->cancel_timeout(this->dosage_->get_object_id());
  if(rc)
  {
    // Only toggle if there was an existing timeout.
    ESP_LOGD(TAG,"Canceling dose for %s", this->get_name().c_str());
    this->toggle();
  }
}

}  // namespace dosage
}  // namespace esphome
