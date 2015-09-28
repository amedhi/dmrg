/*---------------------------------------------------------------------------
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-09-28 12:08:30
* Last Modified by:   amedhi
* Last Modified time: 2015-09-28 22:40:51
*----------------------------------------------------------------------------*/
#include <iostream>
#include "scheduler/task.h"

class DMRGTask : public scheduler::Task
{
public:
  DMRGTask() {};
  virtual ~DMRGTask() {};

  void start(input::Parameters& parms); 
  void run(); 
  void dostep(); 
  void halt(); 
} dmrg;

void DMRGTask::start(input::Parameters& parms)
{
  //double u = parms.set_value("U", 1.0);
  std::cout << "task " << parms.task_id()+1 << " of " << parms.task_size() << std::endl;
}

void DMRGTask::run()
{
  //double u = parms.set_value("U", 1.0);
}

void DMRGTask::dostep()
{
}

void DMRGTask::halt()
{
}
