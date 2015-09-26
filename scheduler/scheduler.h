/*---------------------------------------------------------------------------
* DMRG Project: DMRG using Matrix Product States 
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 13:33:19
* Last Modified by:   amedhi
* Last Modified time: 2015-09-26 16:52:25
*----------------------------------------------------------------------------*/
// File: scheduler.h 
// Classes for handling job scheduling

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H

#include <iostream>
#include "cmdargs.h"
#include "jobparams.h"

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
  input::JobParameters input;
  unsigned int task_size;
};

} // end namespace

#endif
