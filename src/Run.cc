#include "Run.hh"
#include "Ion_Hit.hh"
#include "PPAC_Hit.hh"
#include "Gamma_Hit.hh"
#include "Data_Format.hh"
#include "G4SystemOfUnits.hh"

Run::Run() {

  gen = NULL;
  output = NULL;
  diagnostics = NULL;

  gammaTrigger = 0;
  
  owc = false;
  write_diag = false;
  
}

void Run::RecordEvent(const G4Event* evt) {

  std::vector<S3Data> s3_data;
  std::vector<ChicoXData> ch_data;
  std::vector<GretaData> gr_data;

  G4bool pFlagDS = false;
  G4bool pFlagUS = false;
  G4bool rFlag = false;
  
  G4int gammaMult = 0;
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  for(G4int i=0;i<HCE->GetNumberOfCollections();i++) {
    
    G4String name = HCE->GetHC(i)->GetName();
    if(name == "ionCollection") { //S3

      Ion_Hit_Collection* iHC = (Ion_Hit_Collection*)HCE->GetHC(i);  
      for(unsigned int j=0;j<iHC->entries();j++) {
	
        Ion_Hit* hit = (Ion_Hit*)iHC->GetHit(j);
	int det = hit->GetDetector();
        int ring = hit->GetRing();
        int sect = hit->GetSector();
	
        float en = float(hit->GetEdep()/MeV);
	
        G4ThreeVector pos = hit->GetPos();
        float x = float(pos.x()/cm);
        float y = float(pos.y()/cm);
        float z = float(pos.z()/cm);
	
        bool proj = hit->IsProjectile();
        bool rec = hit->IsRecoil();
	
	if(proj) {
	  if(det)
	    pFlagDS = true;
	  else
	    pFlagUS = true;
	}
	if(rec)
	  rFlag = true;
	
	s3_data.push_back({det,ring,sect,en,x,y,z,proj,rec});
	
      }
    }
    if(name == "ppacCollection") { //ChicoX

      PPAC_Hit_Collection* pHC = (PPAC_Hit_Collection*)HCE->GetHC(i);  
      for(unsigned int j=0;j<pHC->entries();j++) {
	
        PPAC_Hit* hit = (PPAC_Hit*)pHC->GetHit(j);
	int id = hit->GetId();
	
        float en = float(hit->GetEdep()/MeV);
	float tm = float(hit->GetTime()/ns);

        G4ThreeVector pos = hit->GetPos();
        float x = float(pos.x()/cm);
        float y = float(pos.y()/cm);
        float z = float(pos.z()/cm);
	
        bool proj = hit->IsProjectile();
        bool rec = hit->IsRecoil();
	
	if(proj) {
	  if(id < 11)
	    pFlagDS = true;
	  else
	    pFlagUS = true;
	}
	if(rec)
	  rFlag = true;
	
	ch_data.push_back({id,en,tm,x,y,z,proj,rec});
	
      }
    }
    else if(name == "gammaCollection") { //Greta
      
      /*
      Gamma_Hit_Collection* gHC = (Gamma_Hit_Collection*)HCE->GetHC(i);
      G4int Nhits = gHC->entries();
      if(Nhits>0) {
	
	const int MAX_INTPTS = 30;
	const G4double hitTolerance = 0.00001*mm;
	const G4double packingRes = 0.0*mm;

	// Packing: consolidate interaction points within segments 
	// based on proximity. 
	G4int trackID[100*MAX_INTPTS];
	G4int detNum[100*MAX_INTPTS];
	G4int segNum[100*MAX_INTPTS];
	G4double ipEdep[100*MAX_INTPTS];
	G4double segmentEdep[100*MAX_INTPTS];
	G4double ipX[100*MAX_INTPTS];
	G4double ipY[100*MAX_INTPTS];
	G4double ipZ[100*MAX_INTPTS];
	G4double ipMX[100*MAX_INTPTS]; // "smeared" IP positions
	G4double ipMY[100*MAX_INTPTS]; // if posRes > 0
	G4double ipMZ[100*MAX_INTPTS]; // (M = "measured")
	G4double X0[100*MAX_INTPTS];
	G4double Y0[100*MAX_INTPTS];
	G4double Z0[100*MAX_INTPTS];
	G4double globalTime[100*MAX_INTPTS];
	G4int NCons[100*MAX_INTPTS];
	G4double packingRes2 = packingRes*packingRes;

	G4int NIP = 0;
	G4double totalEdep = 0;

	// Prevent seg fault in events with very large hit collections.
	// This arises rarely in high-energy muon (and likely other
	// high-energy charged particle) events.
	if(Nhits > 3000) {
	  G4cerr << "Warning: hit collection with " << Nhits
		 << " entries. Processing the first 3000."
		 << " (event " << evt->GetEventID() << ")"
		 << G4endl;
	  Nhits = 3000;
	}
      
	for(G4int j=0;j<Nhits;j++){

	  Gamma_Hit* hit = (Gamma_Hit*)gHC->GetHit(j);
	
	  G4ThreeVector pos = hit->GetPos();
	  G4ThreeVector posM = hit->GetPosM();
	
	  G4double x = pos.getX()/cm;
	  G4double y = pos.getY()/cm;
	  G4double z = pos.getZ()/cm;
	  G4double xM = posM.getX()/cm;
	  G4double yM = posM.getY()/cm;
	  G4double zM = posM.getZ()/cm;	  
	  
	  G4double en = hit->GetEdep()/keV;
	  totalEdep += en;
	  
	  G4double gt = hit->GetGlobalTime()*1000.0;
	
	  NCons[j] = -1;
	  G4bool processed = false;	

	  //Initialize a new interaction point for each gamma-ray hit.
	  if(hit->GetParticleID() == "gamma") {

	    //Combine multiple gamma hits at the same position.
	    //This is rare, but it happens.
	    if(j > 0 
	       && (x - ipX[j-1])*(x - ipX[j-1]) < hitTolerance*hitTolerance
	       && (y - ipY[j-1])*(y - ipY[j-1]) < hitTolerance*hitTolerance
	       && (z - ipZ[j-1])*(z - ipZ[j-1]) < hitTolerance*hitTolerance) {

	      ipEdep[NIP-1] += en; 
	      processed = true;

	    } 
	    else {

	      trackID[NIP] = hit->GetTrackID();
	      detNum[NIP] = hit->GetDetector();
	      segNum[NIP] = hit->GetSegment();

	      //This becomes the total energy deposit associated with this interaction.
	      ipEdep[NIP] = en; 

	      //This becomes the barycenter of all energy depositions associated with this interaction.
	      ipX[NIP] = x;
	      ipY[NIP] = y;
	      ipZ[NIP] = z;
	      ipMX[NIP] = xM;
	      ipMY[NIP] = yM;
	      ipMZ[NIP] = zM;
	      trackID[NIP] = hit->GetTrackID();

	      //Position of the initial interaction. We use position to identify the tracks produced by this interaction.
	      X0[NIP] = pos.getX()/cm;
	      Y0[NIP] = pos.getY()/cm;
	      Z0[NIP] = pos.getZ()/cm;

	      globalTime[NIP] = hit->GetGlobalTime()*1000.0;
	    
	      NCons[NIP] = 1;
	      NIP++;
	      processed = true;
	    } 
	  }
	  else { //Combine secondary-particle hits with their parent interaction points.
	    
	    for(G4int k=0;k<NIP;k++) { // Compare hit j with existing interaction points.
	      
	      G4ThreeVector pos0 = hit->GetTrackOrigin();
	      G4double x0 = pos0.getX()/cm;
	      G4double y0 = pos0.getY()/cm;
	      G4double z0 = pos0.getZ()/cm;
	    
	      if(hit->GetParentTrackID() == trackID[k]  // correct parent
		  && (x0 - X0[k])*(x0 - X0[k]) < hitTolerance*hitTolerance
		  && (y0 - Y0[k])*(y0 - Y0[k]) < hitTolerance*hitTolerance
		  && (z0 - Z0[k])*(z0 - Z0[k]) < hitTolerance*hitTolerance // correct interaction point
		  && hit->GetDetector() == detNum[k]) {    // same crystal

		// Energy-weighted average position (barycenter)
		if(ipEdep[k] == 0 && en == 0) {
		  
		  ipX[k]  = (ipX[k] + x)/2.;
		  ipY[k]  = (ipY[k] + y)/2.;
		  ipZ[k]  = (ipZ[k] + z)/2.;
		  ipMX[k] = (ipMX[k] + xM)/2.;
		  ipMY[k] = (ipMY[k] + yM)/2.;
		  ipMZ[k] = (ipMZ[k] + zM)/2.;
		
		  // Assign the earliest global time of the first raw hit in this IP
		  globalTime[k] = std::min(gt,globalTime[k]);
		  
		} 
		else {
		  
		  ipX[k] = (ipEdep[k]*ipX[k] + en*x)/(ipEdep[k] + en);
		  ipY[k] = (ipEdep[k]*ipY[k] + en*y)/(ipEdep[k] + en);
		  ipZ[k] = (ipEdep[k]*ipZ[k] + en*z)/(ipEdep[k] + en);
		  ipMX[k] = (ipEdep[k]*ipMX[k] + en*xM)/(ipEdep[k] + en);
		  ipMY[k] = (ipEdep[k]*ipMY[k] + en*yM)/(ipEdep[k] + en);
		  ipMZ[k] = (ipEdep[k]*ipMZ[k] + en*zM)/(ipEdep[k] + en);
		  ipEdep[k] += en;

		  //Assign the earliest global time of the first raw hit in this IP
		  globalTime[k] = std::min(gt, globalTime[k]);
		  
		}
	      
		NCons[k]++;
		processed = true;
	      
	      }
	    }
	  }
	  
	  // If hit i is not a gamma-ray hit and cannot be consolidated with 
	  // an existing gamma-ray interaction point, it's a positron or an
	  // electron multiple-scattering event that isn't associated with 
	  // energy deposition in the sensitive volume of GRETINA by a gamma 
	  // ray. (The gamma-ray interaction happened in dead material.)
	  // We'll initialize a new interaction point and treat it as a 
	  // gamma-ray interaction.
	  if(!processed) {

	    trackID[NIP] = hit->GetTrackID();
	    detNum[NIP] = hit->GetDetector();
	    segNum[NIP] = hit->GetSegment();
	    ipEdep[NIP] = en;
	    ipX[NIP] = x;
	    ipY[NIP] = y;
	    ipZ[NIP] = z;
	    ipMX[NIP] = xM;
	    ipMY[NIP] = yM;
	    ipMZ[NIP] = zM;
	  
	    // This is not a gamma ray. We need to trick its siblings into 
	    // treating it as the parent gamma.
	    trackID[NIP] = hit->GetParentTrackID();

	    G4ThreeVector pos0 = hit->GetTrackOrigin();
	    X0[NIP] = pos0.getX()/cm;
	    Y0[NIP] = pos0.getY()/cm;
	    Z0[NIP] = pos0.getZ()/cm;

	    globalTime[NIP] = hit->GetGlobalTime()*1000.;

	    NCons[NIP] = 1;
	    NIP++;
	    processed = true;

	  }

	  if(!processed)
	    G4cerr << "Warning: Could not find a home for hit " << j
		   << " of event " << evt->GetEventID()
		   << G4endl;

	  if(NIP >= 100*MAX_INTPTS){
	    G4cout << "Error: too many decomposed hits. Increase hit processing array dimension."
		   << G4endl;
	    exit(EXIT_FAILURE);
	  }
	
	}

	// Packing: Consolidate gamma-ray interaction points within a single
	// segment that are closer than the PackingRes parameter.
	G4int NGammaHits = NIP;
	for(G4int j=0;j<NIP;j++) {
	  for(G4int k=k+1;k<NIP;k++) {

	    if( ((ipX[j] - ipX[k])*(ipX[j] - ipX[k])
		 + (ipY[j] - ipY[k])*(ipY[j] - ipY[k])
		 + (ipZ[j] - ipZ[k])*(ipZ[j] - ipZ[k])
		 < packingRes2)                        // proximal
		&& detNum[j] == detNum[k]              // same crystal
		&& segNum[j] == segNum[k]              // same segment
		&& (NCons[j] > 0 && NCons[k] > 0) ) {   // not already consolidated

	      // Energy-weighted average
	      if(ipEdep[j] == 0 && ipEdep[k] == 0) {
		
		ipX[j]  = (ipX[j] + ipX[k])/2.;
		ipY[j]  = (ipY[j] + ipY[k])/2.;
		ipZ[j]  = (ipZ[j] + ipZ[k])/2.;
		ipMX[j] = (ipMX[j] + ipMX[k])/2.;
		ipMY[j] = (ipMY[j] + ipMY[k])/2.;
		ipMZ[j] = (ipMZ[j] + ipMZ[k])/2.;

		// Assign the earliest global time of the first raw hit in this IP
		globalTime[j] = std::min(globalTime[j], globalTime[k]);
		
	      } 
	      else {
		
		ipX[j]  = (ipEdep[j]*ipX[j] + ipEdep[k]*ipX[k])/(ipEdep[j]+ipEdep[k]);
		ipY[j]  = (ipEdep[j]*ipY[j] + ipEdep[k]*ipY[k])/(ipEdep[j]+ipEdep[k]);
		ipZ[j]  = (ipEdep[j]*ipZ[j] + ipEdep[k]*ipZ[k])/(ipEdep[j]+ipEdep[k]);
		ipMX[j] = (ipEdep[j]*ipMX[j] + ipEdep[k]*ipMX[k])/(ipEdep[j]+ipEdep[k]);
		ipMY[j] = (ipEdep[j]*ipMY[j] + ipEdep[k]*ipMY[k])/(ipEdep[j]+ipEdep[k]);
		ipMZ[j] = (ipEdep[j]*ipMZ[j] + ipEdep[k]*ipMZ[k])/(ipEdep[j]+ipEdep[k]);
		ipEdep[j] += ipEdep[k];

		// Assign the earliest global time of the first raw hit in this IP
		globalTime[j] = std::min(globalTime[j], globalTime[k]);
		
	      }
	      NCons[k] = -1; // IP k remains in the arrays but will be ignored.
	      NGammaHits--;
	    }
	  }
	}

	// Calculate the total energy deposited in each segment.
	for(G4int j=0;j<NIP;j++) // initialize
	  if(NCons[j] > 0)
	    segmentEdep[j] = ipEdep[j];
	
	for(G4int j=0;j<NIP;j++) {
	  for(G4int k=j+1;k<NIP;k++) {
	    if(NCons[j] > 0 && NCons[k] > 0
	       && detNum[j] == detNum[k]    // same crystal
	       && segNum[j] == segNum[k]){ // same segment
	      segmentEdep[j] += ipEdep[k];
	      segmentEdep[k] += ipEdep[j];
	    }
	  }
	}

	// Write decomposed gamma event(s) to the output file
	//writeDecomp(timestamp,NIP,detNum,segNum,NCons,ipMX,ipMY,ipMZ,ipEdep,segmentEdep,globalTime);
      
      }
      */
      
      Gamma_Hit_Collection* gHC = (Gamma_Hit_Collection*)HCE->GetHC(i);
      for(unsigned int j=0;j<gHC->entries();j++) {
	
        Gamma_Hit* hit = (Gamma_Hit*)gHC->GetHit(j);
	int det = hit->GetDetector();
        int seg = hit->GetSegment();

        float en = float(hit->GetEdep()/keV);

        G4ThreeVector pos = hit->GetPosM();
        float x = float(pos.x()/cm);
        float y = float(pos.y()/cm);
        float z = float(pos.z()/cm);

        bool fep = hit->IsFEP();
        bool pfep = hit->IsProjFEP();
	
	if(!seg)
	  gammaMult++;
	
	gr_data.push_back({det,seg,en,x,y,z,fep,pfep});
	
      }
      
    }
  }
  
  G4Run::RecordEvent(evt);

  G4int num = evt->GetEventID();
  if(write_diag) {
  
    INFO info;
    info.evtNum = num;
    
    info.indexP = gen->GetProjectileIndex();
    info.indexR = gen->GetRecoilIndex();

    info.beamEn = gen->GetBeamEnergy();
    info.thetaCM = gen->GetThetaCM();
  
    info.projDS = pFlagDS;
    info.projUS = pFlagUS;
    info.rec = rFlag;
    
    fwrite(&info,info.bytes(),1,diagnostics);
    
  }

  G4int nS = s3_data.size();
  G4int nC = ch_data.size();
  G4int nG = gr_data.size();
  
  int numP = nS;
  if(nC > 0)
    numP = nC;

  if(numP == 0 && nG == 0)
    return;

  if(owc && (numP == 0 || nG == 0))
    return;

  if(gammaMult < gammaTrigger)
    return;
  
  Header header;
  header.evtNum = num;
  header.nSdata = numP;
  header.nGdata = nG;
  
  fwrite(&header,header.bytes(),1,output);
  fwrite(&s3_data[0],sizeof(S3Data),nS,output);
  fwrite(&ch_data[0],sizeof(ChicoXData),nC,output);
  fwrite(&gr_data[0],sizeof(GretaData),nG,output);
  
  return;
}

void Run::Merge(const G4Run* aRun) {

  const Run* run = static_cast<const Run*>(aRun);
  fclose(run->GetOutputFile());

  if(run->GetWriteDiagnostics())
    fclose(run->GetDiagnosticsFile());
  
  G4Run::Merge(aRun);
  
  return;
}
