//
// Created by Christian Kellner on 06/11/15.
//

#ifndef LPM_CFG_H
#define LPM_CFG_H

#include <data.h>
#include <map>

namespace lpm {


class cfg {
public:
    static cfg default_cfg();
    cfg(iris::data::store base) : store(base) { }

    // the led mapping for the led pseudo monochromator
    std::map<uint8_t, uint16_t> lpm_leds() const;

    // the pwm mapping for the led pseudo monochromator
    std::map<uint16_t, uint16_t> lpm_pwm() const;

private:
    iris::data::store store;
};


}

#endif //LPM_CFG_H
