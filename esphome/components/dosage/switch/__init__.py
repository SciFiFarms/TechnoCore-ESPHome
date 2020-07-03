import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.components.gpio.switch import CONFIG_SCHEMA 

# TODO: CONF_DOSAGE_DEFAULT isn't in esphome.const.
from esphome.const import CONF_ID, CONF_DOSAGE_DEFAULT
# TODO: These were copied from gpio.switch.
from esphome.const import CONF_INTERLOCK, CONF_PIN, CONF_RESTORE_MODE
from .. import dosage_ns

DosageSwitch = dosage_ns.class_('DosageSwitch', switch.Switch, cg.Component)

DEFAULT_DOSAGE = 2001

CONFIG_SCHEMA = CONFIG_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DosageSwitch),
    cv.Optional(CONF_DOSAGE_DEFAULT, default=DEFAULT_DOSAGE): cv.uint16_t,
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield switch.register_switch(var, config)

    cg.add(var.set_dosage(dosage_))

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
