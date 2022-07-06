#include "option_parsing.hpp"

#include <boost/program_options.hpp>
#include <iostream>

using namespace boost::program_options;

inline int help(char* program) {
    std::cout << "Usage: " << program << " [OPTIONS]" << '\n'
              << "\t-h [ --help ]\tDisplay this usage guide" << '\n'
              << "\t--rx resolution" << '\t' << "Resolution's width | default 1920" << '\n'
              << "\t--ry resolution" << '\t' << "Resolution's height | default 1080" << '\n'
              << "\t--kb layout" << '\t' << "Keyboard's config | default qwerty (available: azerty)" << '\n'
              << "\t--camx x" << '\t' << "Camera x position | default 20" << '\n'
              << "\t--camy y" << '\t' << "Camera y position | default 20" << '\n'
              << "\t--camz z" << '\t' << "Camera z position | default 20" << '\n'
              << "\t--movespeed speed" << '\t' << "Move speed | default 0.1" << '\n'
              << "\t--lookspeed speed" << '\t' << "Look speed | default 0.001" << '\n'
              << "\t--sx width" << '\t' << "Scene's width | default 300" << '\n'
              << "\t--sy height" << '\t' << "Scene's height | default 300" << '\n'
              << "\t--nx detail" << '\t' << "Scene's width detail (power of 2!) | default 128" << '\n'
              << "\t--ny detail" << '\t' << "Scene's height detail (power of 2!) | default 128" << '\n'
              << "\t--mf facto" << '\t' << "Motion factor | default 0.8" << '\n'
              << "\t--wspeed speed" << '\t' << "Wind speed | default 50" << '\n'
              << "\t--wforce force" << '\t' << "Wind force | default 2" << '\n'
              << "\t--minw size" << '\t' << "Minimum wave size | default 0.1" << '\n'
              << "\t--wheight height" << '\t' << "Wave height proportion | default 0.000005" << '\n'
              << "\t--colr red" << '\t' << "Ocean's color (red) [0-1] | default 0.212" << '\n'
              << "\t--colg green" << '\t' << "Ocean's color (green) [0-1] | default 0.611" << '\n'
              << "\t--colb blue" << '\t' << "Ocean's color (blue) [0-1] | default 1" << '\n'
              << "\t\tExamples of colors:" << '\n'
              << "\t\t\t 0, 0, 0.1 | 0.2, 0.45, 0.55 | 0.2, 0.6, 1 | 0, 0.4, 0.7 | 0.6, 0.2, 0.3"
              << std::endl;
    return -1;
}

int parse_options(int argc, char** argv, struct options& opt) {
    try {
        options_description desc{ "Options" };
        desc.add_options()
            ("help,h", "Help")
            ("rx", value<int>(), "Resolution's width")
            ("ry", value<int>(), "Resolution's height")
            ("kb", value<std::string>(), "Keyboard's config")
            ("camx", value<int>(), "Camera x position")
            ("camy", value<int>(), "Camera y position")
            ("camz", value<int>(), "Camera z position")
            ("movespeed", value<float>(), "Move speed")
            ("lookspeed", value<float>(), "Look speed")
            ("sx", value<int>(), "Scene's width")
            ("sy", value<int>(), "Scene's height")
            ("nx", value<int>(), "Scene's width detail (power of 2!)")
            ("ny", value<int>(), "Scene's height detail (power of 2!)")
            ("mf", value<float>(), "Motion factor")
            ("wspeed", value<float>(), "Wind speed")
            ("wforce", value<float>(), "Wind force")
            ("minw", value<float>(), "Minimum wave size")
            ("wheight", value<float>(), "Wave height proportion")
            ("colr", value<float>(), "Ocean's color (red) [0-1]")
            ("colg", value<float>(), "Ocean's color (green) [0-1]")
            ("colb", value<float>(), "Ocean's color (blue) [0-1]");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        // HELP OPTION : Display help
        if (vm.count("help"))
            return help(argv[0]);

        // RX OPTION : Change width resolution;
        if (vm.count("rx")) {
            opt.rx = vm["rx"].as<int>();
            if (opt.rx < 50) {
                throw std::invalid_argument("Width resolution must be greater or equal to 50.");
            }
        }

        // RY OPTION : Change height resolution
        if (vm.count("ry")) {
            opt.ry = vm["ry"].as<int>();
            if (opt.ry < 50) {
                throw std::invalid_argument("Height resolution must be greater or equal to 50.");
            }
        }

        // KB OPTION : Change keyboard navigation;
        if (vm.count("kb")) {
            std::string tmp = vm["rx"].as<std::string>();
            if (tmp == "azerty") {
                opt.kb = Camera::AZERTY;
            }
            if (tmp == "qwerty") {
                opt.kb = Camera::QWERTY;
            }
            else {
                throw std::invalid_argument("Width resolution must be greater or equal to 50.");
            }
        }

        // CAMX OPTION : Change cam x pos
        if (vm.count("camx")) {
            opt.camx = vm["camx"].as<int>();
        }

        // CAMX OPTION : Change cam y pos
        if (vm.count("camy")) {
            opt.camx = vm["camy"].as<int>();
        }

        // CAMX OPTION : Change cam z pos
        if (vm.count("camz")) {
            opt.camx = vm["camz"].as<int>();
        }

        // MOVESPEED OPTION : Change move speed
        if (vm.count("movespeed")) {
            opt.movespeed = vm["movespeed"].as<float>();
        }

        // LOOKSPEED OPTION : Change look speed
        if (vm.count("lookspeed")) {
            opt.lookspeed = vm["lookspeed"].as<float>();
        }

        // SX OPTION : Change cam width
        if (vm.count("sx")) {
            opt.sx = vm["sx"].as<int>();
        }

        // SY OPTION : Change scene height
        if (vm.count("sy")) {
            opt.sy = vm["sy"].as<int>();
        }

        // NX OPTION : Change scene width details
        if (vm.count("nx")) {
            opt.nx = vm["nx"].as<int>();
        }

        // NY OPTION : Change scene height details
        if (vm.count("ny")) {
            opt.ny = vm["ny"].as<int>();
        }

        // MOVESPEED OPTION : Change move speed
        if (vm.count("mf")) {
            opt.mf = vm["mf"].as<float>();
        }

        // WSPEED OPTION : Change wind speed
        if (vm.count("wspeed")) {
            opt.wspeed = vm["wspeed"].as<float>();
        }

        // WFORCE OPTION : Change wind force
        if (vm.count("wforce")) {
            opt.wforce = vm["wforce"].as<float>();
        }

        // MINW OPTION : Change minimum wave size
        if (vm.count("minw")) {
            opt.minw = vm["minw"].as<float>();
        }

        // WHEIGHT OPTION : Change wave height
        if (vm.count("wheight")) {
            opt.wheight = vm["wheight"].as<float>();
        }

        // COLR OPTION : Change color (red)
        if (vm.count("colr")) {
            opt.colr = vm["colr"].as<float>();
        }

        // COLG OPTION : Change color (green)
        if (vm.count("colg")) {
            opt.colg = vm["colg"].as<float>();
        }

        // COLB OPTION : Change color (blue)
        if (vm.count("colb")) {
            opt.colb = vm["colb"].as<float>();
        }
    }
    catch (const error &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}