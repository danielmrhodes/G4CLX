#include "PPAC_SD.hh"
#include "G4SystemOfUnits.hh"
#include "vector"

PPAC_SD::PPAC_SD(G4String name) : G4VSensitiveDetector(name) {
  collectionName.insert("ppacCollection");
}

PPAC_SD::~PPAC_SD() {}

void PPAC_SD::Initialize(G4HCofThisEvent*) {
  
  HC = new PPAC_Hit_Collection(SensitiveDetectorName,collectionName[0]);
  
  return;
}

G4bool PPAC_SD::ProcessHits(G4Step* step, G4TouchableHistory*) {

  G4String name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  if(!G4StrUtil::contains(name,proj_name) && !G4StrUtil::contains(name,recoil_name))
    return true;
  
  PPAC_Hit* hit = new PPAC_Hit();
  hit->SetPos(step->GetPreStepPoint()->GetPosition());
  hit->SetEdep(step->GetTotalEnergyDeposit());
  hit->SetTime(step->GetPostStepPoint()->GetGlobalTime());
  hit->SetId(step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo());

  if(G4StrUtil::contains(name,proj_name)) {
    hit->SetIsProjectile();
    hit->SetIsNotRecoil();
  }
  else {
    hit->SetIsNotProjectile();
    hit->SetIsRecoil();
  }
 
  HC->insert(hit);
  
  return true;
}

void PPAC_SD::EndOfEvent(G4HCofThisEvent* HCE) {
  
  ConsolidateHits();
  HCE->AddHitsCollection(HCE->GetNumberOfCollections(),HC);
  
  return;
  
}

void PPAC_SD::ConsolidateHits() {

 label:
  
  for(unsigned int i=0;i<HC->entries();i++) {

    PPAC_Hit* hit1 = (PPAC_Hit*)HC->GetHit(i);
    for(unsigned int j=i+1;j<HC->entries();j++) {
      
      PPAC_Hit* hit2 = (PPAC_Hit*)HC->GetHit(j);
      if(hit1->GetId() != hit2->GetId())
	continue;
      
      if(hit1->GetTime() < hit2->GetTime()) {
	hit1->SetEdep(hit1->GetEdep()+hit2->GetEdep());
      
	delete hit2;
	hit2=NULL;
      
	std::vector<PPAC_Hit*>* vec = HC->GetVector();
	vec->erase(vec->begin()+j);
      }
      else {
	hit2->SetEdep(hit1->GetEdep()+hit2->GetEdep());
      
	delete hit1;
	hit1=NULL;
      
	std::vector<PPAC_Hit*>* vec = HC->GetVector();
	vec->erase(vec->begin()+i);
      }
      
      goto label;
      
    }
  }
  
  return;
  
}
