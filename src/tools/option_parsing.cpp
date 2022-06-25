#include "option_parsing.hpp"

#include <boost/program_options.hpp>
#include <iostream>

using namespace boost::program_options;

inline int help(char* program) {
    std::cout << "Usage: " << program << " [OPTIONS]" << '\n'
              << "\t-h [ --help ]\tDisplay this usage guide" << '\n'
              << "\t--rx resolution" << '\t' << "Resolution's width" << '\n'
              << "\t--ry resolution" << '\t' << "Resolution's height" << std::endl;
    return -1;
}

int parse_options(int argc, char** argv, struct options& opt) {
    try
    {
        options_description desc{ "Options" };
        desc.add_options()
            ("help,h", "Help")
            ("rx", value<std::string>(), "Resolution's width")
            ("ry", value<std::string>(), "Resolution's height");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        // HELP OPTION : Display help
        if (vm.count("help"))
            return help(argv[0]);

        // RX OPTION : Change width resolution;
        if (vm.count("rx"))
        {
            opt.rx = atoi(vm["rx"].as<std::string>().c_str());
            if (opt.rx < 50) {
                throw std::invalid_argument("Width resolution must be greater or equal to 50.");
            }
        }

        // RY OPTION : CHange height resolution
        if (vm.count("ry"))
        {
            opt.ry = atoi(vm["ry"].as<std::string>().c_str());
            if (opt.ry < 50) {
                throw std::invalid_argument("Height resolution must be greater or equal to 50.");
            }
        }
    }
    catch (const error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}