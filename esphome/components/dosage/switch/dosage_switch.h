#pragma once

#include "esphome/core/component.h"
#include "esphome/components/gpio/switch/gpio_switch.h"
#include "esphome/components/mqtt/custom_mqtt_device.h"

static const char *TAG = "dosage_switch";

namespace esphome {
namespace dosage {

class DosageSwitch : public gpio::GPIOSwitch, public mqtt::CustomMQTTDevice {
 public:
  void set_dosage(uint32_t dosage) { this->dosage_ = dosage; };
  void dose_for_ms(uint32_t time_in_ms) ;
  void dose_for_seconds(uint32_t time_in_seconds) ;
  void dose();

  void on_dose_for_ms(const std::string &topic, const std::string &payload) { this->dose_for_ms(uint32_t(atoi(payload.c_str()))); }
  void on_dose_for_seconds(const std::string &topic, const std::string &payload) { this->dose_for_seconds(uint32_t(atoi(payload.c_str()))); }
  void on_set_dosage(const std::string &topic, const std::string &payload) { 
    ESP_LOGI(TAG,"Updating dosage to be %d ms",payload.c_str());
    this->set_dosage(uint32_t(atoi(payload.c_str()))); 
  }
  void on_dose(const std::string &topic, const std::string &payload) { this->dose(); }

  void setup() override;
  void dump_config() override;
  std::string component_type() const { return "switch"; }
  std::string get_default_object_id() const { return sanitize_string_whitelist(to_lowercase_underscore(this->get_name()), HOSTNAME_CHARACTER_WHITELIST); }

 protected:
  uint32_t dosage_;
  std::string topic_dose_for_seconds;
  std::string topic_dose_for_ms;
  std::string topic_set_dosage;
  std::string topic_dose;
};

}  // namespace dosage
}  // namespace esphome
