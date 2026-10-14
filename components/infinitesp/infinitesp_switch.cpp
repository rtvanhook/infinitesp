#include "infinitesp_switch.h"

namespace esphome {
namespace infinitesp {

void InfinitESPSwitch::write_state(bool state) {
  if (switch_type_ == "unoccupied") {
    parent_->set_zone_unoccupied(zone_, state);
    // Optimistic: the mirror already carries the new bit, and the thermostat's
    // next 3B02 confirms or reverts it through on_register_update.
    current_ = state ? 1 : 0;
    publish_state(state);
  }
}

void InfinitESPSwitch::on_register_update(uint8_t device_addr, uint16_t register_key) {
  // Per-zone unoccupied from SAM 3B02 offset 21 (zones_unoccupied bitmap): bit
  // (zone-1) set = the thermostat holds this zone in Away. Same byte the
  // occupancy binary sensor reads, opposite polarity.
  if (switch_type_ == "unoccupied" && register_key == REG_SAM_STATE && zone_ >= 1) {
    auto *data = parent_->get_register(parent_->get_sam_address(), REG_SAM_STATE);
    if (data && data->size() > REG3B02_UNOCCUPIED) {
      int8_t bit = ((*data)[REG3B02_UNOCCUPIED] >> (zone_ - 1)) & 1;
      if (bit != current_) {
        current_ = bit;
        publish_state(bit != 0);
      }
    }
  }
}

}  // namespace infinitesp
}  // namespace esphome
