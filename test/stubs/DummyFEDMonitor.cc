/** \file
 * 
 * dummy FED unpacking module: loops over feds and fills a datasize histogram
 *   
 * 
 * $Date: 2006/05/16 14:52:18 $
 * $Revision: 1.3 $
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

using namespace edm;
using namespace std;

namespace test{

  class DummyFEDMonitor: public EDAnalyzer{
  
  private:
    unsigned int count_;
    MonitorElement * hfedsize;
    MonitorElement * hfedprof;
    MonitorElement * hfedcacca;
    MonitorElement ** hindfed;

  public:

    DummyFEDMonitor(const ParameterSet& pset):count_(0)
    {
      DaqMonitorBEInterface *dbe = 
	edm::Service<DaqMonitorBEInterface>().operator->();
      dbe->setCurrentFolder("FEDs/Summary");
      hfedsize = dbe->book1D("fedsize","FED Size Distribution",100,0.,10000.);
      hfedprof = dbe->bookProfile("fedprof","FED Size by ID", 2048,0.,2048.,
				  0,0.,5000.);
      hindfed = new MonitorElement*[FEDNumbering::lastFEDId()];
    }
    void beginJob(EventSetup const&es)
    {
      for(unsigned int i =0; i<FEDNumbering::lastFEDId(); i++)
	hindfed[i] = 0;
    }

    void analyze(const Event & e, const EventSetup& c){
      
      ++count_;

      Handle<FEDRawDataCollection> rawdata;
      e.getByLabel("DaqSource", rawdata);
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
		  ostringstream os1;
		  ostringstream os2;
		  os1 << "fed" << i;
		  os2 << "FED #" << i << " Size Distribution";
		  hindfed[i] = dbe->book1D(os1.str(),os2.str(),100,0.,10000.);
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
  };
DEFINE_FWK_MODULE(DummyFEDMonitor)
}

