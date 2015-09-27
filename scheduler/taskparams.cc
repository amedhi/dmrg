/*---------------------------------------------------------------------------
* Parameters: Handles the parameter values for individual tasks.
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-09-27 00:31:12
* Last Modified by:   amedhi
* Last Modified time: 2015-09-27 11:17:57
*----------------------------------------------------------------------------*/
// File: taskparams.cc

#include "inputparams.h"

namespace input {

void Parameters::show(const unsigned& set_id) const
{

  std::cout << "Parameter set = " << set_id << ":" << std::endl; 
  std::cout << "---------------------" << std::endl; 
  std::map<std::string, pval>::const_iterator it;
  for (it=params.begin(); it!=params.end(); ++it) {
    std::cout << " " << it->first << " = "; 
    switch (it->second.type) {
      case value_type::boo:
        std::cout << it->second.bool_val; break;
      case value_type::num:
        std::cout << it->second.num_val; break;
      case value_type::str:
        std::cout << it->second.str_val; break;
      case value_type::nan:
        throw std::logic_error("Undefined parameter type detected"); 
        break;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}



} // name space input
