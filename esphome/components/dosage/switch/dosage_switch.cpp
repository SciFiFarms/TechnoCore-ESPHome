#include "dosage_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dosage {

void DosageSwitch::dump_config() { LOG_SWITCH("", "Dosage Switch", this); }
void DosageSwitch::setup() {
  this->GPIOSwitch::setup();


  this->topic_dose_for_seconds = this->get_default_topic_for("dose_for_seconds", this->component_type(), this->get_default_object_id());
  this->topic_dose_for_ms = this->get_default_topic_for("dose_for_ms", this->component_type(), this->get_default_object_id());
  this->topic_set_dosage = this->get_default_topic_for("set_dosage", this->component_type(), this->get_default_object_id());
  this->topic_dose = this->get_default_topic_for("dose", this->component_type(), this->get_default_object_id());

  subscribe(this->topic_dose_for_seconds, &DosageSwitch::on_dose_for_seconds);
  subscribe(this->topic_dose_for_ms, &DosageSwitch::on_dose_for_ms);
  subscribe(this->topic_set_dosage, &DosageSwitch::on_set_dosage);
  subscribe(this->topic_dose, &DosageSwitch::on_dose);
}

void DosageSwitch::dose_for_ms(uint32_t time_in_ms) {
  ESP_LOGI(TAG,"Dosing for %d ms");
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
  ESP_LOGD(TAG,"Dosing: %d", this->dosage_);
  this->dose_for_ms((this->dosage_));
}

}  // namespace dosage
}  // namespace esphome
