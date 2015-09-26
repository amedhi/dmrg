/*---------------------------------------------------------------------------
* Job scheduler: Classes for handling a job.
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 13:33:19
* Last Modified by:   amedhi
* Last Modified time: 2015-09-26 16:54:57
*----------------------------------------------------------------------------*/
// File: jobparms.h 
// Class declarations for job parameters

#ifndef SCHEDULER_JOBPARAMS_H
#define SCHEDULER_JOBPARAMS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

namespace input {


class Parameters;  // forward declaration

class JobParameters
{
public:
  JobParameters() {n_tasks=n_params=0; valid=false;} 
  JobParameters(const std::string& inputfile); 
  void read_params(const std::string& inputfile);
  unsigned int task_size(void) {return n_tasks;}
  void get_task_param(const unsigned& task_id); 
  void init_task_param(Parameters& p);
  void set_task_param(Parameters& p, const unsigned& task_id); 

private:
  enum class value_type {boo, num, str};
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

  friend void JobParameters::init_task_param(Parameters& p);
  friend void JobParameters::set_task_param(Parameters& p, const unsigned& task_id);

private:
  struct pval {bool is_const; bool bool_val; double num_val; std::string str_val;};
  std::map<std::string, pval> params;
  unsigned int n_params;
};



} // end namespace

#endif
