#include "EventFilter/Processor/interface/EventProcessor.h"
#include "FWCore/ServiceRegistry/interface/ServicesManager.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/test/stubs/DummyService.h"
#include "FWCore/Utilities/interface/ProblemTracker.h"

#include "toolbox/include/TaskGroup.h"

using namespace evf;
using namespace std;

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
  edm::AssertHandler ah;



  string config1 = "process TEST1 = {\n"
    "path p = { print }\n"
    "module print = AsciiOutputModule { }\n"
    "source = EmptySource {untracked int32 maxEvents = 10000}\n"
    "}";
  string config2 = "process TEST2 = {\n"
    "path p = { print }\n"
    "module print = AsciiOutputModule { }\n"
    "source = EmptySource {untracked int32 maxEvents = 100000\n"
    "untracked uint32 firstRun = 100}\n"
    "}";
  EventProcessor *proc = new EventProcessor(0);
  TaskGroup *group = new TaskGroup(); //ownership goes to task
  proc->initTaskGroup(group);
  proc->init(config1);
  proc->beginRun();
  cout << "activate processor thread and wait 1s before killing" << endl;
  proc->activate();
  //  proc.run();
  ::sleep(1);
  cout << " call endRun " << endl;
  proc->endRun();
  group->join(); //wait for eventprocessor thread to exit
  delete proc; proc = 0;
  proc = new EventProcessor(0);
  group = new TaskGroup();
  proc->initTaskGroup(group);
  proc->init(config2);
  proc->beginRun();
  cout << "activate processor thread and wait 2s before killing" << endl;
  proc->activate();
  //  proc.run();
  ::sleep(2);
  cout << " call endRun " << endl;
  proc->endRun();
  group->join(); //wait for eventprocessor thread to exit
  delete proc; proc = 0;
  
  return 0;
}
  
    
