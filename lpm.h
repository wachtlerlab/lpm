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

        std::string getInfo() {
            std::string info = send_and_receive("info");
            return info;
        }

        void reset() {
            send_and_receive("reset");
        }

        void shoot() {
            send_and_receive("shoot");
        }

        std::string send_and_receive(const std::string &data) {
            io.send_data(data);

            std::stringstream response;

            while (true) {
                std::string data = io.recv_line(1000);

                if (data == "\u0004") {
                    break;
                }

                response << data << "\n";
            }

            return response.str();
        }

        void receiveArduinoOutput() {

            while(1) {
                std::string data = io.recv_line(1000);
                std::cout << data << std::endl;

                if (data == std::string("\0004")) {
                    break;
                }
            }
        }

    };


}