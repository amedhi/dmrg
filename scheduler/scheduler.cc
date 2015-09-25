/*---------------------------------------------------------------------------
* Scheduler: A job scheduler unit
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 12:44:04
* Last Modified by:   amedhi
* Last Modified time: 2015-09-16 00:43:00
*----------------------------------------------------------------------------*/
// File: scheduler.cpp 
// Classes for handling job scheduling

#include <iostream>
#include "scheduler.h"

namespace scheduler {

int start(int argc, const char *argv[])
{

  Scheduler* theScheduler;
  theScheduler = new MasterScheduler(argc, argv);
  int res = theScheduler->run();

	return res;	
}

MasterScheduler::MasterScheduler(int argc, const char *argv[])
  : Scheduler{}, cmdarg{argc, argv}, pstore{}, task_size{0}
{
}

int MasterScheduler::run(void) 
{
  std::cout << " starting..." << std::endl;
  bool valid = cmdarg.process_options();
  if (!valid) return 0;

  // job parameters
  task_size = pstore.fill(cmdarg.input_file());
  // TaskParams params;
  for (unsigned task_id=0; task_id<task_size; ++task_id) {
    // run the tasks
    //params << pstore(task_id);
  }

  return 0;
}

int Scheduler::run(void) 
{
  return 0;
}

} // end namespace
