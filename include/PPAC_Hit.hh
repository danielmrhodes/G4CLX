#ifndef PPAC_Hit_h
#define PPAC_Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class PPAC_Hit : public G4VHit {
  
public:

  PPAC_Hit();
  PPAC_Hit(const PPAC_Hit* other);
  ~PPAC_Hit();

  inline void* operator new(size_t);
  inline void operator delete(void *aHit);

  void SetId(G4int num) {id = num;}
  void SetEdep(G4double de) {edep = de;}
  void SetTime(G4double tm) {time = tm;}
  void SetPos(G4ThreeVector vec) {pos = vec;}
  
  void SetIsProjectile() {proj = true;}
  void SetIsRecoil() {rec = true;}
  void SetIsNotProjectile() {proj = false;}
  void SetIsNotRecoil() {rec = false;}
  
  G4int GetId() const {return id;}
  G4double GetEdep() const {return edep;}
  G4double GetTime() const {return time;}
  G4ThreeVector GetPos() const {return pos;}

  G4bool IsProjectile() const {return proj;}
  G4bool IsRecoil() const {return rec;}
  
private:
  
  G4bool proj;
  G4bool rec;
  G4int id;
  
  G4double edep;
  G4double time;
  G4ThreeVector pos;
  
};

typedef G4THitsCollection<PPAC_Hit> PPAC_Hit_Collection;

extern G4ThreadLocal G4Allocator<PPAC_Hit>* PPAC_Hit_Allocator;

inline void* PPAC_Hit::operator new(size_t) {
  
  if(!PPAC_Hit_Allocator)
    PPAC_Hit_Allocator = new G4Allocator<PPAC_Hit>;
  
  return (void*)PPAC_Hit_Allocator->MallocSingle();
}

inline void PPAC_Hit::operator delete(void* aHit) {
  PPAC_Hit_Allocator->FreeSingle((PPAC_Hit*)aHit);
}

#endif
