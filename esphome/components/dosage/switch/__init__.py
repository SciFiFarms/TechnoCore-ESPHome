import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch, sensor, mqtt
from esphome.core import CORE
import esphome.components.gpio.switch as gpio_switch

# TODO: CONF_DOSAGE_DEFAULT isn't in esphome.const.
from esphome.const import CONF_ID, CONF_DOSAGE, CONF_DOSAGE_SENSOR, CONF_MQTT_ID
# TODO: These were copied from gpio.switch.
from esphome.const import CONF_INTERLOCK, CONF_PIN, CONF_RESTORE_MODE, CONF_HUMIDITY, CONF_NAME, CONF_COMMAND_TOPIC, CONF_MQTT, CONF_MQTT_PARENT_ID 
from .. import dosage_ns

import re
DEPENDENCIES = ['mqtt']
IS_PLATFORM_COMPONENT = True

DEFAULT_DOSAGE = "2s"
CONF_EXACT_TIMING = 'exact_timing'

DosageSwitch = dosage_ns.class_('DosageSwitch', gpio_switch.GPIOSwitch, cg.Component)

CONFIG_SCHEMA = gpio_switch.CONFIG_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DosageSwitch),
    cv.Optional(CONF_DOSAGE_SENSOR, default={CONF_NAME: "Default"}): sensor.sensor_schema("ms", "mdi:mdiBeakerOutline", 0),
    cv.Optional(CONF_DOSAGE, default=DEFAULT_DOSAGE): cv.positive_time_period_milliseconds,
    cv.Optional(CONF_EXACT_TIMING, default=False): cv.boolean,
}).extend(cv.COMPONENT_SCHEMA)

def get_topic(suffix, config):
    whitelist = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_'
    sanitized_name = ''.join(x for x in config[CONF_NAME].lower().replace(' ', '_') if x in whitelist)

    system_id = re.sub(r'(.*\d+)_.*', r'\1', sanitized_name, 1)
    subsystem_id = re.sub(r'.*\d+_(.*)', r'\1', sanitized_name, 1)
    TOPIC_PREFIX = "seedship"
    return f"{ TOPIC_PREFIX }/{ system_id }/{ subsystem_id }/{ suffix }"

def to_code(config):
    # TODO: Seems like there should be a more object oriented way to do this.
    # Include the gpio_switch code incase a gpio.switch isn't used.
    CORE.extra_source_files.update({'esphome/components/gpio/switch/gpio_switch.h': '/usr/src/app/esphome/components/gpio/switch/gpio_switch.h'})
    CORE.extra_source_files.update({'esphome/components/gpio/switch/gpio_switch.cpp': '/usr/src/app/esphome/components/gpio/switch/gpio_switch.cpp'})

    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield switch.register_switch(var, config)

    if config[CONF_DOSAGE_SENSOR][CONF_NAME] == "Default":
        config[CONF_DOSAGE_SENSOR][CONF_NAME] = f"{ config[CONF_NAME] } Dosage"

    sens = yield sensor.new_sensor(config[CONF_DOSAGE_SENSOR]) 
    cg.add(var.set_sensor(sens))

    cg.add(var.set_dosage(config[CONF_DOSAGE]))
    cg.add(var.set_topic_dose_for_seconds(get_topic("dose_for_seconds", config)))
    cg.add(var.set_topic_dose_for_ms(get_topic("dose_for_ms", config)))
    cg.add(var.set_topic_dose(get_topic("dose", config)))
    cg.add(var.set_topic_set_dosage(get_topic("set_dosage", config)))

    if CONF_EXACT_TIMING in config:
        cg.add(var.set_exact_timing(config[CONF_EXACT_TIMING]))

    # TODO: Everything below here came from GPIOSwitch. It'd be better to find a way to use this DRYly. 
    pin = yield cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))

    cg.add(var.set_restore_mode(config[CONF_RESTORE_MODE]))

    if CONF_INTERLOCK in config:
        interlock = []
        for it in config[CONF_INTERLOCK]:
            lock = yield cg.get_variable(it)
            interlock.append(lock)
        cg.add(var.set_interlock(interlock))
        cg.add(var.set_interlock_wait_time(config[CONF_INTERLOCK_WAIT_TIME]))
