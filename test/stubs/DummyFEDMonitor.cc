/** \file
 * 
 * dummy FED unpacking module: loops over feds and fills a datasize histogram
 *   
 * 
 * $Date: 2006/11/14 10:30:47 $
 * $Revision: 1.9 $
 * \author E. Meschi PH/CMD
 *
*/

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"

#include <iostream>


namespace test{

  class DummyFEDMonitor: public edm::EDAnalyzer{
  
  private:
    unsigned int count_;
    MonitorElement * hfedsize;
    MonitorElement * hfedprof;
    MonitorElement ** hindfed;
    bool dqm;

  public:

    DummyFEDMonitor(const edm::ParameterSet& pset):count_(0), dqm(true)
    {
      DaqMonitorBEInterface *dbe = 0;
      try{
	dbe = edm::Service<DaqMonitorBEInterface>().operator->();
	dbe->setCurrentFolder("FEDs/Summary");
	hfedsize = dbe->book1D("fedsize","FED Size Distribution",100,0.,10000.);
	//	if(hfedsize == 0) dbe->findObject("fedsize");
	hfedprof = dbe->bookProfile("fedprof","FED Size by ID", 2048,0.,2048.,
				    0,0.,5000.);
	//	if(hfedprof == 0) dbe->findObject("fedprof");
	hindfed = new MonitorElement*[FEDNumbering::lastFEDId()];

      }
      catch(cms::Exception &e)
	{
	  dqm = false;
	  // do nothing, it means dqm is not available
	}
    }
    void beginJob(edm::EventSetup const&es)
    {
      for(int i = 0; i<FEDNumbering::lastFEDId(); i++)
	hindfed[i] = 0;
    }
    void endJob()
    {

      DaqMonitorBEInterface *dbe = 0;
      
      try{
	dbe = edm::Service<DaqMonitorBEInterface>().operator->();
	dbe->setCurrentFolder("FEDs/Summary");
	dbe->removeContents();
	dbe->setCurrentFolder("FEDs/Details");
	dbe->removeContents();
      }
      catch(cms::Exception &e)
	{
	  dqm = false;
	  // do nothing, it means dqm is not available
	}

      delete [] hindfed;
    }
    void analyze(const edm::Event & e, const edm::EventSetup& c){
      
      ++count_;
      if(dqm)
	{
	  edm::Handle<FEDRawDataCollection> rawdata;
	  e.getByType(rawdata);
	  for (int i = 0; i<FEDNumbering::lastFEDId(); i++){
	    const FEDRawData& data = rawdata->FEDData(i);
	    if(size_t size=data.size()) {
	      hfedsize->Fill(float(size));
	      hfedprof->Fill(float(i),float(size));
	      if(i<1024)
		{
		  if(hindfed[i]==0)
		    {
		      DaqMonitorBEInterface *dbe = 
			edm::Service<DaqMonitorBEInterface>().operator->();
		      dbe->setCurrentFolder("FEDs/Details");
		      std::ostringstream os1;
		      std::ostringstream os2;
		      os1 << "fed" << i;
		      os2 << "FED #" << i << " Size Distribution";
		      hindfed[i] = dbe->book1D(os1.str(),os2.str(),100,0.,3.*size);

		      //		      if(hindfed[i] == 0) dbe->findObject(os1.str());
		    }
		  hindfed[i]->Fill(float(size));
		  
		}
	    }
	  }
//       if ( count_==1) {
// 	   CPPUNIT_ASSERT( rawdata->FEDData(619).size()==5560);
//         CPPUNIT_ASSERT( rawdata->FEDData(620).size()==5544);     
//       }  
	}
    }
  };
DEFINE_FWK_MODULE(DummyFEDMonitor);
}

