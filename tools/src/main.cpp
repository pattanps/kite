/****************************************************************/
/*                                                              */
/*  Copyright (C) 2018, M. Andelkovic, L. Covaci, A. Ferreira,  */
/*                    S. M. Joao, J. V. Lopes, T. G. Rappoport  */
/*                                                              */
/****************************************************************/

#include <vector>
#include <iostream>
#include <complex>
#include <string>
#include <Eigen/Dense>
#include "ComplexTraits.hpp"
#include "H5Cpp.h"

#include "myHDF5.hpp"
#include "parse_input.hpp"
#include "calculate_simple.hpp"
#include "macros.hpp"
#include "messages.hpp"


int main(int argc, char *argv[]){
    shell_input variables(argc, argv);
    print_header_message();
    print_info_message();
    print_flags_message();

    verbose_message("\nStarting program...\n\n");

    choose_simulation_type(argv[1], variables);
    verbose_message("Complete.\n");
	return 0;
}
