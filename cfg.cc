//
// Created by Christian Kellner on 06/11/15.
//

#include "cfg.h"
#include <yaml-cpp/yaml.h>

namespace lpm {

lpm::cfg lpm::cfg::default_cfg() {
    return lpm::cfg(iris::data::store::default_store());
}


std::map<uint8_t, uint16_t> cfg::lpm_leds() const {

    std::map<uint8_t, uint16_t> lpm_led_map;

    const fs::file base = store.location();
    fs::file valuesFile = base.child("lpm/pwmLed");
    std::string data = valuesFile.read_all();
    YAML::Node root = YAML::Load(data);

    YAML::Node led_node = root["leds"];

    for(YAML::const_iterator it = led_node.begin(); it != led_node.end(); it++) {
        std::string pin_no = it->first.as<std::string>();
        std::string led_wavelength = it->second.as<std::string>();

        uint8_t pin_int = std::stoi(pin_no);
        uint16_t pwm_val_int = std::stoi(led_wavelength);

        lpm_led_map.insert( std::pair<uint8_t ,uint16_t >(pin_int, pwm_val_int) );
    }

    return lpm_led_map;
}

std::map<uint16_t, uint16_t> cfg::lpm_pwm() const {

    std::map<uint16_t, uint16_t>lpm_led_map;

    const fs::file base = store.location();
    fs::file valuesFile = base.child("lpm/pwm");
    std::string data = valuesFile.read_all();
    YAML::Node root = YAML::Load(data);

    YAML::Node led_node = root["pwm"];

    for(YAML::const_iterator it = led_node.begin(); it != led_node.end(); it++) {
        std::string wavelength = it->first.as<std::string>();
        std::string led_pwm = it->second.as<std::string>();

        uint16_t wavelength_int = std::stoi(wavelength);
        uint16_t led_pwm_int = std::stoi(led_pwm);

        lpm_led_map.insert( std::pair<uint16_t ,uint16_t >(wavelength_int, led_pwm_int) );
    }

    return lpm_led_map;
}

} // lpm::