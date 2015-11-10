#include <serial.h>


namespace device {


    class lpm {

    public:

        device::serial io;

        lpm(const device::serial &io) : io(io) { }

        static lpm open(const std::string &path) {
            lpm lpm(serial::open(path));
            return lpm;
        }

        void led(uint8_t pin, uint16_t pwm) {
            std::stringstream cmd;
            cmd << "pwm " << pin << "," << pwm;
            io.send_data(cmd.str());
        }

        void setPWM(std::string cmd) {
            io.send_data(cmd);
        }

        void getInfo() {
            io.send_data("info");
        }

        void reset() {
            io.send_data("reset");
        }

        void shoot() {
            io.send_data("shoot");
        }

        void receiveArduinoOutput() {

            while(1) {
                std::string data = io.recv_line(1000);
                std::cout << data << std::endl;

                if(data == std::string("*")) {
                    break;
                }
            }
        }

    };


}