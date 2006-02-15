#include "xdaq/include/xdaq/Application.h"
#include "xdata/include/xdata/String.h"
#include "xdata/include/xdata/Integer.h"
#include "xdata/include/xdata/Boolean.h"
#include "xgi/include/xgi/Input.h"
#include "xgi/include/xgi/Output.h"
#include "xgi/include/xgi/exception/Exception.h"
#include "FWCore/Utilities/interface/ProblemTracker.h"

#include "EventFilter/Utilities/interface/EPStateMachine.h"
#include "EventFilter/Utilities/interface/Css.h"

class TaskGroup;
namespace evf
{
  class EventProcessor;

  class FUEventProcessor : public xdaq::Application
    {
    public:
      XDAQ_INSTANTIATOR();
      FUEventProcessor(xdaq::ApplicationStub *s);
      ~FUEventProcessor();
      
    private:
      void configureAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
      void enableAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
      virtual void suspendAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
      virtual void resumeAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
      virtual void haltAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
      virtual void nullAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
      xoap::MessageReference fireEvent(xoap::MessageReference msg)
	throw (xoap::exception::Exception);

      void defaultWebPage
	(xgi::Input  *in, xgi::Output *out) throw (xgi::exception::Exception);
      void css(xgi::Input  *in,
	       xgi::Output *out) throw (xgi::exception::Exception)
	{css_.css(in,out);}
      void moduleWeb
	(xgi::Input  *in, xgi::Output *out) throw (xgi::exception::Exception);
	  
      xdata::String offConfig_;
      EventProcessor *proc_;
      TaskGroup *group_;
      EPStateMachine *fsm_;
      edm::AssertHandler *ah_;
      Css css_;
      friend class EPStateMachine;
    };
}




