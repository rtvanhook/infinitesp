import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID, CONF_TYPE
from .. import InfinitESPEntity, CONF_INFINITESP_ID, infinitesp_ns, register_infinitesp_entity

CONF_ZONE = "zone"

InfinitESPSwitch = infinitesp_ns.class_("InfinitESPSwitch", switch.Switch, InfinitESPEntity)

SWITCH_TYPES = {
    # Per-zone: the thermostat's own Away state, SAM 3B02 byte 21 (zones_unoccupied).
    # Reads track the bit like the occupancy binary sensor (inverted); writes send the
    # CHANGE_UNOCCUPIED 3B02 write. On = Away by name on the wall control.
    "unoccupied": {"key": "unoccupied"},
}

CONFIG_SCHEMA = switch.switch_schema(InfinitESPSwitch).extend(
    {
        cv.GenerateID(CONF_INFINITESP_ID): cv.use_id(CONF_INFINITESP_ID),
        cv.Required(CONF_TYPE): cv.one_of(*SWITCH_TYPES, lower=True),
        cv.Optional(CONF_ZONE, default=1): cv.int_range(min=1, max=8),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await switch.register_switch(var, config)
    cg.add(var.set_zone(config[CONF_ZONE]))
    cg.add(var.set_switch_type(SWITCH_TYPES[config[CONF_TYPE]]["key"]))
    await register_infinitesp_entity(var, config)
