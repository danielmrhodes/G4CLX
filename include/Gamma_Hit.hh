#ifndef Gamma_Hit_h
#define Gamma_Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class Gamma_Hit : public G4VHit {
  
public:

  Gamma_Hit();
  ~Gamma_Hit();

  inline void* operator new(size_t);
  inline void operator delete(void *aHit);

  void SetEdep(G4double de) {edep = de;}
  void SetGlobalTime(G4double t) {globalTime = t;}

  void SetPos(G4ThreeVector vec) {pos = vec;}
  void SetPosM(G4ThreeVector vec) {posM = vec;}
  
  void SetFEP() {fep = true;}
  void SetProjFEP() {pfep = true;}
  
  void SetDetNum(G4int id) {det = id;}
  void SetSegNum(G4int id) {seg = id;}
  
  G4bool IsFEP() const {return fep;}
  G4bool IsProjFEP() const {return pfep;}
  
  G4int GetSegment() const {return seg;}
  G4int GetDetector() const {return det;}

  G4double GetEdep() const {return edep;}
  G4double GetGlobalTime() {return globalTime;}
  
  G4ThreeVector GetPos() const {return pos;}
  G4ThreeVector GetPosM() const {return posM;}
  
  /*
  void SetTrackID  (G4int track) {trackID = track;}
  void SetParticleID (G4String particle) {particleID = particle;}
  void SetProcess (G4String proc) {process = proc;}
  void SetParentTrackID (G4int parent)   {parentTrackID = parent;}
  void SetCreatorProcess (G4String proc) {creatorProcess = proc;}   
      
  void SetKE(G4double e) {ke = e;}
  void SetTrackOrigin(G4ThreeVector xyz) {trackOrigin = xyz;}
  
  G4int GetTrackID() {return trackID;}
  G4String GetParticleID() {return particleID;}
  G4String GetProcess() {return process;}
  G4int GetParentTrackID() {return parentTrackID;}
  G4String GetCreatorProcess() {return creatorProcess;}
  G4double GetKE() {return ke;}
  G4ThreeVector GetTrackOrigin() {return trackOrigin;}
  
  */
  
private:

  G4bool fep, pfep;
  //G4int det, seg, trackID, parentTrackID;
  G4int det, seg;
  
  //G4double edep, ke, globalTime;
  G4double edep, globalTime;
  
  //G4ThreeVector pos, posM, trackOrigin;
  G4ThreeVector pos, posM;

  //G4String particleID, process, creatorProcess;
  
};

typedef G4THitsCollection<Gamma_Hit> Gamma_Hit_Collection;

extern G4ThreadLocal G4Allocator<Gamma_Hit>* Gamma_Hit_Allocator;

inline void* Gamma_Hit::operator new(size_t) {
  
  if(!Gamma_Hit_Allocator)
    Gamma_Hit_Allocator = new G4Allocator<Gamma_Hit>;
  
  return (void*)Gamma_Hit_Allocator->MallocSingle();
}

inline void Gamma_Hit::operator delete(void* aHit) {
  Gamma_Hit_Allocator->FreeSingle((Gamma_Hit*)aHit);
}

#endif
