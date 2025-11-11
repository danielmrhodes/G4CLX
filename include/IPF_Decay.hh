#ifndef IPF_Decay_h
#define IPF_Decay_h 1

#include "G4ParticleDefinition.hh"
#include "G4VDecayChannel.hh"

//Internal Pair Formation (e+/e-)
class IPF_Decay : public G4VDecayChannel {

public:

  IPF_Decay(G4ParticleDefinition* Parent,  G4ParticleDefinition* daughter, G4double cc, G4bool emt);
  ~IPF_Decay();

  G4DecayProducts* DecayIt(G4double);

private:

  G4double parentmass;
  const G4double* theDaughterMasses;

  G4bool emit;
  G4double convCoef;

};

#endif
