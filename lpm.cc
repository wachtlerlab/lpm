/*
 * LED Pseudo Monochromator tool for communicating with Arduino that itself
 * has firmware for controlling the attached LEDs; supports different commands
 */

#include <iostream>
#include <serial.h>
#include <boost/program_options.hpp>
#include <data.h>

#include "lpm.h"

// Supported commands:
// info         (for getting information of all LED pins from arduino
// reset        (for resetting and turning off all LEDs that are on)
// pwm 10,2000  (Turning on one LED on particular pin with pwm)
// shoot        (For triggering IR LED to take picture from camera)

static int
cmd_pwm(device::lpm &lpm, const std::vector<std::string> &args)
{
    namespace po = boost::program_options;

    uint16_t pwm;
    uint8_t pin;

    po::options_description pwm_opts("pwm options");
    pwm_opts.add_options()
            ("led", po::value<uint8_t>(&pin), "LED")
            ("pwm", po::value<uint16_t>(&pwm), "PIN");

    po::variables_map vm;
    po::store(po::command_line_parser(args).options(pwm_opts).run(), vm);

    std::cerr << "[D] led: " << pin << " pwm: " <<  pwm << std::endl;
    lpm.led(pin, pwm);
    return 0;
}


int main(int argc, char **argv) {
    namespace po = boost::program_options;

    std::string device;
    std::string input;

    po::options_description opts("LED Pseudo Monochromatic Command Line tool");
    opts.add_options()
            ("help",    "Flag for Help")
            ("device", po::value<std::string>(&device), "device file for aurdrino")
            ("input", po::value<std::string>(&input), "Specify command (info, pwm, reset, shoot)")
            ("args",  po::value<std::vector<std::string>>(), "Arguments for command");

    po::positional_options_description pos;
    pos.add("input", 1).add("args", -1);

    po::variables_map vm;
    std::vector<std::string> args;

    try {
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(opts).positional(pos).run();
        po::store(parsed, vm);
        po::notify(vm);
        args = po::collect_unrecognized(parsed.options, po::include_positional);
        args.erase(args.begin());
    } catch (const std::exception &e) {
        std::cerr << "Error while parsing commad line options: " << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
        return 1;
    }

    if(vm.count("help")) {
        //show program options
        std::cout << opts << std::endl;
        std::cout << "Supported Commands:" << std::endl << std::endl;

        std::cout << "info         (for getting information of all LED pins from arduino" << std::endl;
        std::cout << "reset        (for resetting and turning off all LEDs that are on)" << std::endl;
        std::cout << "pwm 10,2000  (Turning on one LED on particular pin with pwm)" << std::endl;
        std::cout << "shoot        (For triggering IR LED to take picture from camera)" << std::endl;

        std::cout << std::endl;

    }

    std::cerr << "[D] Device: " << device << std::endl;
    std::cerr << "[D] Input: \"" << input << "\"" << std::endl;

    device::lpm lpm = device::lpm::open(device);

    if (input == "pwm") {
        cmd_pwm(lpm, args);
    } else if(input == "info") {
        lpm.getInfo();
    } else if(input == "reset") {
        lpm.reset();
    } else if(input == "shoot") {
        lpm.shoot();
    } else {
        std::cout <<"[E] Unkown command! " << std::endl;
        return -1;
    }

    std::cout  << "from arduino: " << std::endl;
    lpm.receiveArduinoOutput();    //print stream from arduino

    return 0;
}