#include "EventFilter/Processor/interface/EventProcessor.h"
#include "FWCore/ServiceRegistry/interface/ServicesManager.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/test/stubs/DummyService.h"
#include "FWCore/Utilities/interface/ProblemTracker.h"

using namespace evf;
using namespace std;

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
  edm::AssertHandler ah;



  string config = "process TEST = {\n"
    "path p = { print }\n"
    "module print = AsciiOutputModule { }\n"
    "source = EmptySource {untracked int32 maxEvents = 20}\n"
    "}";
  EventProcessor proc(0);
  proc.init(config);
  proc.beginRun();
  proc.run();
  proc.endRun();
  return 0;
}
  
    
