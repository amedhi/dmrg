/*---------------------------------------------------------------------------
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-09-28 12:08:30
* Last Modified by:   amedhi
* Last Modified time: 2015-09-29 16:09:15
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
  double U = parms.set_value("U", 0.0);
  double M = parms.set_value("M", 0.0);
  std::cout << "U = " << U << std::endl;
  std::cout << "M = " << M << std::endl;

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
