/*---------------------------------------------------------------------------
* Scheduler: A class that handles running of user jobs.  
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 13:33:19
* Last Modified by:   amedhi
* Last Modified time: 2015-09-27 11:05:50
*----------------------------------------------------------------------------*/
// File: scheduler.h 
// Definition of the Scheduler class.

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H

#include <iostream>
#include "cmdargs.h"
#include "inputparams.h"

namespace scheduler {

int start(int argc, const char *argv[]);

class Scheduler 
{
public:
  Scheduler(): simmaster(0) {};
  ~Scheduler() {};
  virtual int run(void);

private:
  int simmaster;
  //TaskParams parms;
};

class MasterScheduler : public Scheduler
{
public:
  MasterScheduler(int argc, const char *argv[]);
  MasterScheduler() = delete;
  ~MasterScheduler() {};
  int run(void);

private:
  CmdArg cmdarg;
  input::InputParameters input;
  unsigned int task_size;
};

} // end namespace

#endif
