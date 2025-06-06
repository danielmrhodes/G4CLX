#include "GammaSD.hh"
#include "DetectorConstruction.hh"
#include "Gretina_Array.hh"

#include "G4MTRunManager.hh"
#include "G4WorkerRunManager.hh"
#include "G4SystemOfUnits.hh"

GammaSD::GammaSD(G4String name) : G4VSensitiveDetector(name) {
  collectionName.insert("gammaCollection");

  posRes = 0.0;
}

GammaSD::~GammaSD() {}

void GammaSD::Initialize(G4HCofThisEvent*) {
  
  HC = new Gamma_Hit_Collection(SensitiveDetectorName,collectionName[0]);
  detMap.clear();
  
  return;
}

G4bool GammaSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  
  G4double edep = step->GetTotalEnergyDeposit();
  if(!(edep > 0.0))
    return false;
  
  G4TouchableHandle theTouchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4VSolid* solid = theTouchable->GetSolid(0);
  
  G4int depth = 1;
  G4int detCode = theTouchable->GetReplicaNumber(0);
  if(detCode == 0){
    detCode = theTouchable->GetReplicaNumber(depth);
    solid = theTouchable->GetSolid(depth);
  }
  G4int detNum = detCode%1000;
  
  G4VPhysicalVolume* topVolume = theTouchable->GetVolume(depth);
  
  G4ThreeVector frameTrans = topVolume->GetFrameTranslation();
  const G4RotationMatrix* rot = topVolume->GetFrameRotation();
                                  
  G4RotationMatrix frameRot;
  if(rot)
    frameRot = *rot;
  
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  G4ThreeVector posSol = frameRot(position);
  posSol += frameRot(frameTrans);
  
  G4ThreeVector positionM = position;
  if(posRes > 0) {
    
    positionM.setX(position.getX() + CLHEP::RandGauss::shoot(0,posRes));
    positionM.setY(position.getY() + CLHEP::RandGauss::shoot(0,posRes));
    positionM.setZ(position.getZ() + CLHEP::RandGauss::shoot(0,posRes));
    
    G4ThreeVector posSolM = frameRot(positionM);
    posSolM += frameRot(frameTrans);
    
    if(solid->Inside(posSolM) == kOutside) {
      
      //Displacement in Solid (Crystal-frame) coordinates
      G4ThreeVector deltaSol = posSolM-posSol;
      
      //Distance to the surface along the displacement vector.
      G4double dr = solid->DistanceToOut(posSol,deltaSol/deltaSol.mag());
      
      //Displacement in world coordinates
      G4ThreeVector delta = positionM - position;
      
      //Corrected "smeared" position (at surface)
      positionM = position + delta/delta.mag()*dr;
      posSolM = frameRot(positionM);
      posSolM += frameRot(frameTrans);
      
    }
  }
  
  G4WorkerRunManager* runManager = (G4WorkerRunManager*)G4MTRunManager::GetRunManager();
  DetectorConstruction* theDetector = (DetectorConstruction*)runManager->GetUserDetectorConstruction();
  Gretina_Array* gret = theDetector->GetGretina();
  
  G4int segCode = gret->GetSegmentNumber(detCode,posSol);
  G4int slice  = segCode/10;
  G4int sector = segCode%10;
  
  if(sector>0) 
    sector--;
  else
    sector=5;
  
  if(detNum%2) {
    if(sector>0) 
      sector--;
    else
      sector=5;
  }
  segCode = sector + 6 * slice;

  Gamma_Hit* hit = new Gamma_Hit();
  hit->SetDetNum(detNum+1);
  hit->SetSegNum(segCode+1);
  hit->SetEdep(edep);
  hit->SetGlobalTime(step->GetPostStepPoint()->GetGlobalTime());
  hit->SetPos(position);
  hit->SetPosM(positionM);

  /*
  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
  G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
  hit->SetTrackID(step->GetTrack()->GetTrackID());
  hit->SetParticleID(particleName);
  hit->SetProcess(processName);
  hit->SetParentTrackID(step->GetTrack()->GetParentID());
  
  if(step->GetTrack()->GetCreatorProcess())
    hit->SetCreatorProcess(step->GetTrack()->GetCreatorProcess()->GetProcessName());
  else
    hit->SetCreatorProcess("primary");

  hit->SetKE(step->GetTrack()->GetKineticEnergy());
  hit->SetTrackOrigin(step->GetTrack()->GetVertexPosition());
  */
  
  HC->insert(hit);

  G4int id = step->GetTrack()->GetTrackID();
  if(std::find(detMap[detNum].begin(),detMap[detNum].end(),id) == detMap[detNum].end())
    detMap[detNum].push_back(id);
  
  return true;
}

void GammaSD::EndOfEvent(G4HCofThisEvent* HCE) {
  
 label:

  for(unsigned int i=0;i<HC->entries();i++) {
    Gamma_Hit* hit1 = (Gamma_Hit*)HC->GetHit(i);
    
    for(unsigned int j=i+1;j<HC->entries();j++) {  
      Gamma_Hit* hit2 = (Gamma_Hit*)HC->GetHit(j);
      
      if(hit1->GetSegment() == hit2->GetSegment() && hit1->GetDetector() == hit2->GetDetector()) {
	if(hit1->GetGlobalTime() < hit2->GetGlobalTime()) {
	  
	  hit1->SetEdep(hit1->GetEdep()+hit2->GetEdep());
	  
	  delete hit2;
	  hit2=NULL;
	  
	  std::vector<Gamma_Hit*>* vec = HC->GetVector();
	  vec->erase(vec->begin()+j);
	}
	else {
	  
	  hit2->SetEdep(hit1->GetEdep()+hit2->GetEdep());
	  
	  delete hit1;
	  hit1=NULL;
	  
	  std::vector<Gamma_Hit*>* vec = HC->GetVector();
	  vec->erase(vec->begin()+i);
	}
	goto label;
      }
    }
  }

  const int nCores = 120;
  G4double cores[nCores] = {0.0};
  
  for(unsigned int i=0;i<HC->entries();i++) {
    
    Gamma_Hit* hit = (Gamma_Hit*)HC->GetHit(i);	
    cores[hit->GetDetector()-1] += hit->GetEdep();
    
  }

  std::map<G4int,std::vector<G4int>> idMap = trkAct->GetIDMap();
  std::map<G4int,G4double> enMap = trkAct->GetEnergyMap();
  std::vector<G4int> projGams = trkAct->GetProjGammas();
  trkAct->Clear(); //reset for next event
  
  for(int i=0;i<nCores;i++) {
    
    if(cores[i] < 0.01*keV)
      continue;

    Gamma_Hit* hit = new Gamma_Hit();
    hit->SetPos(G4ThreeVector());
    hit->SetDetNum(i+1);
    hit->SetSegNum(0);
    hit->SetEdep(cores[i]);

    G4bool bad = false;
    for(auto it = idMap.begin();it != idMap.end();++it) {

      bad = false;
      for(unsigned int id : detMap[i+1]) {
	if(std::find(it->second.begin(),it->second.end(),id) == it->second.end()) {
	  bad = true;
	  break;
	}
      }
      if(bad)
	continue;

      G4double diff = enMap[it->first] - cores[i];
      if(diff*diff < (0.01*keV)*(0.01*keV)) {
	  
	hit->SetFEP();
	if(std::find(projGams.begin(),projGams.end(),it->first) != projGams.end()) {
	  hit->SetProjFEP();
	}
      }
    }
    
    HC->insert(hit);
     
  }
  
  HCE->AddHitsCollection(HCE->GetNumberOfCollections(),HC);
  
  return;
  
}
