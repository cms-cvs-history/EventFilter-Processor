#ifndef FUEVENTPROCESSOR_H
#define FUEVENTPROCESSOR_H 1


#include "EventFilter/Utilities/interface/StateMachine.h"
#include "EventFilter/Utilities/interface/RunBase.h"
#include "EventFilter/Utilities/interface/Css.h"
#include "EventFilter/Utilities/interface/Exception.h"


#include "FWCore/Framework/interface/TriggerReport.h"
#include "FWCore/Modules/src/PrescaleService.h"

#include "xdaq/include/xdaq/Application.h"
#include "xdaq/NamespaceURI.h"

#include "xdata/include/xdata/String.h"
#include "xdata/include/xdata/Integer.h"
#include "xdata/include/xdata/Boolean.h"
#include "xdata/include/xdata/UnsignedInteger32.h"
#include "xdata/ActionListener.h"

#include "xgi/include/xgi/Input.h"
#include "xgi/include/xgi/Output.h"
#include "xgi/include/xgi/exception/Exception.h"


namespace edm {
  class EventProcessor;
}

namespace evf
{
  /* to be filled in with summary from paths */
  struct filter {

  };
  
  
  class FUEventProcessor : public xdaq::Application,
			   public xdata::ActionListener
  {
  public:
    //
    // construction/destruction
    //
    XDAQ_INSTANTIATOR();
    FUEventProcessor(xdaq::ApplicationStub *s);
    virtual ~FUEventProcessor();
    

    //
    // member functions
    //

    // trigger report callback
    void getTriggerReport(toolbox::Event::Reference e)
      throw (toolbox::fsm::exception::Exception);

    // trigger prescale callbacks
    xoap::MessageReference getPsReport(xoap::MessageReference msg)
      throw (xoap::exception::Exception);
    xoap::MessageReference setPsUpdate(xoap::MessageReference msg)
      throw (xoap::exception::Exception);
    xoap::MessageReference putPrescaler(xoap::MessageReference msg)
      throw (xoap::exception::Exception);

    // work loop functions to be executed during transitional states (async)
    bool configuring(toolbox::task::WorkLoop* wl);
    bool enabling(toolbox::task::WorkLoop* wl);
    bool stopping(toolbox::task::WorkLoop* wl);
    bool halting(toolbox::task::WorkLoop* wl);

    // fsm soap command callback
    xoap::MessageReference fsmCallback(xoap::MessageReference msg)
      throw (xoap::exception::Exception);
    
    // initialize the cmssw event processor
    void initEventProcessor();
    edm::EventProcessor::StatusCode stopEventProcessor();
    
    // xdata:ActionListener interface
    void actionPerformed(xdata::Event& e);
    
    // trigger report related helper functions
    std::string triggerReportToString(const edm::TriggerReport& tr);
    void        printTriggerReport(const edm::TriggerReport& tr);

    // HyperDAQ related functions
    void defaultWebPage(xgi::Input *in,xgi::Output *out)
      throw(xgi::exception::Exception);
    void taskWebPage(xgi::Input *,xgi::Output *,const std::string &);
    void moduleWeb(xgi::Input *in,xgi::Output *out) throw(xgi::exception::Exception);
    void modulePs(xgi::Input  *in, xgi::Output *out)throw(xgi::exception::Exception);
    void microState(xgi::Input *in,xgi::Output *out)throw(xgi::exception::Exception);
    void jsGen(xgi::Input *in,xgi::Output *out,std::string url) 
      throw (xgi::exception::Exception);
    void css(xgi::Input *in,xgi::Output *out) throw (xgi::exception::Exception)
    {
      css_.css(in,out);
    }
    void attachDqmToShm()   throw (evf::Exception);
    void detachDqmFromShm() throw (evf::Exception);

    
  private:
    //
    // member data
    //
    
    // finite state machine
    evf::StateMachine               fsm_;
    
    // event processor
    edm::EventProcessor             *evtProcessor_;
    edm::ServiceToken                serviceToken_;    
    bool                             servicesDone_;

    // prescale (cmssw framework-) service
    edm::service::PrescaleService*  prescaleSvc_;
    
    // parameters published to XDAQ info space(s)
    xdata::UnsignedInteger32         runNumber_;
    xdata::Boolean                   epInitialized_; 
    xdata::String                    configString_;
    xdata::String                    sealPluginPath_;
    xdata::Boolean                   outPut_;
    xdata::UnsignedInteger32         inputPrescale_;
    xdata::UnsignedInteger32         outputPrescale_;
    xdata::UnsignedInteger32         timeoutOnStop_; // in seconds
    bool                             outprev_;
    
    // dqm monitor thread configuration
    xdata::String                    dqmCollectorAddr_;
    xdata::Integer                   dqmCollectorPort_;
    xdata::Integer                   dqmCollectorDelay_;
    xdata::Integer                   dqmCollectorReconDelay_;
    xdata::String                    dqmCollectorSourceName_;

    // xdaq parameters relevant to trigger-report / prescales
    xdata::String                    triggerReportAsString_;
    xdata::String                    prescalerAsString_;

    // HyperDAQ related
    Css                              css_;
    
  };
  
} // namespace evf


#endif
