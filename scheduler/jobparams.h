/*---------------------------------------------------------------------------
* Job scheduler: Classes for handling a job.
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 13:33:19
* Last Modified by:   amedhi
* Last Modified time: 2015-09-25 01:15:24
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

namespace scheduler {

class JobParams
{
public:
  JobParams() {n_tasks=n_params=0; valid=false;} 
  JobParams(const std::string& inputfile); 
  unsigned int fill(const std::string& inputfile);
  unsigned int task_size(void) {return n_tasks;}
  void get_task_param(const unsigned& task_id); 

private:
  enum val_t {bool_t, num_t, str_t};
  struct param_t {val_t type; unsigned size;};
  unsigned int n_params;
  unsigned int n_tasks;
  bool valid;
  std::string infile;
  std::map<std::string, param_t> param_list;
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


} // end namespace

#endif
