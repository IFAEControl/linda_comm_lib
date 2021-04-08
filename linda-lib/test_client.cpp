#include <iostream>

#include <cli/cli.h>
#include <cli/clifilesession.h>

#include "linda-lib.hpp"

using namespace cli;

int main() {
    unsigned data[14400];
    InitCommunication("172.16.17.94", 32000, 32001);

    auto rootMenu = std::make_unique<Menu>("linda");

    rootMenu -> Insert(
            "start", [&](std::ostream& out, int chip) { 
                AcqInfo info{10,10,10,true,true,false};
                ACQuisitionCont(info, data, 1<<chip);
                out << "done\n"; 
        },
            "Start acquisition" 
    );

    rootMenu -> Insert(
            "start_2", [&](std::ostream& out, int chip) { 
                AcqInfo info{10,10,10,true,true,false};
                ACQuisition(info, 1, data, 1<<chip);
                out << "done\n"; 
        },
            "Start non continuous acquisition" 
    );

    rootMenu -> Insert(
            "pop_frame", [&](std::ostream& out) {
                PopFrame(data);
                out << "done\n"; 
        },
            "Pop frame" 
    );

    rootMenu -> Insert(
            "stop", [&](std::ostream& out) { 
                ACQuisitionStop();
                out << "done\n"; 
        },
            "Stop acquisition" 
    );

    rootMenu -> Insert(
            "read_id", [&](std::ostream& out, int chip) { 
                unsigned id;
                ReadEricaID(&id, 1<<chip);
                out << "Chip id: " << id << "\n"; 
        },
            "Read chip id" 
    );

    rootMenu -> Insert(
            "read_temp", [&](std::ostream& out, int chip) { 
                unsigned temp;
                ReadTemperature(&temp, 1<<chip);
                out << "Temperature: " << temp << "\n"; 
        },
            "Read temperature" 
    );

    rootMenu -> Insert(
            "reset", [&](std::ostream& out) { 
                CameraReset();
                out << "done \n"; 
        },
            "Reset camera" 
    );
    Cli cli(std::move(rootMenu));
    CliFileSession input(cli);
    SetColor();
    input.Start();
}
