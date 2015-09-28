/*---------------------------------------------------------------------------
* InputParameter: Classes for processing input parameters.
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 13:33:19
* Last Modified by:   amedhi
* Last Modified time: 2015-09-28 20:27:16
*----------------------------------------------------------------------------*/
// File: inputparams.h 

#ifndef INPUT_PARAMETERS_H
#define INPUT_PARAMETERS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

namespace input {

enum class value_type {boo, num, str, nan};

class Parameters;  // forward declaration

class InputParameters
{
public:
  InputParameters() {n_tasks=n_params=0; valid=false;} 
  InputParameters(const std::string& inputfile); 
  bool read_params(const std::string& inputfile);
  bool not_valid(void) const {return !valid;};
  unsigned int task_size(void) {return n_tasks;}
  void get_task_param(const unsigned& task_id); 
  void init_task_param(Parameters& p);
  void set_task_param(Parameters& p, const unsigned& task_id); 

private:
  struct parameter {std::string name; value_type type; unsigned size;};
  unsigned int n_params;
  unsigned int n_tasks;
  bool valid;
  std::string infile;
  std::vector<parameter> param_list;
  std::map<std::string, std::vector<bool> > boo_params;
  std::map<std::string, std::vector<double> > num_params;
  std::map<std::string, std::vector<std::string> > str_params;

  unsigned int parse(const std::string& inputfile);

  // bad_input exception
  class bad_input: public std::runtime_error
  {
  public:
    bad_input(const std::string& msg, const int& ln=-1);
    std::string message(void) const; 
  private:
    int lnum;
  };
};


class Parameters 
{
public:
  Parameters() {};

  void show(const unsigned&) const;
  friend void InputParameters::init_task_param(Parameters& p);
  friend void InputParameters::set_task_param(Parameters& p, const unsigned& task_id);

private:
  struct pval {bool is_const; value_type type; bool bool_val; double num_val; std::string str_val;};
  std::map<std::string, pval> params;
  unsigned int n_params;
};



} // end namespace input

#endif
