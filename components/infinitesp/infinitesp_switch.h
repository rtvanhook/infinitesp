#pragma once
#include "esphome/components/switch/switch.h"
#include "infinitesp.h"

namespace esphome {
namespace infinitesp {

class InfinitESPSwitch : public switch_::Switch, public InfinitESPEntity {
 public:
  void write_state(bool state) override;
  void on_register_update(uint8_t device_addr, uint16_t register_key) override;
  void set_switch_type(const std::string &type) { switch_type_ = type; }

 protected:
  std::string switch_type_;
  int8_t current_{-1};  // last published bit (-1 = none yet) so the first read publishes
};

}  // namespace infinitesp
}  // namespace esphome
