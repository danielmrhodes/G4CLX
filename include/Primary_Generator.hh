#ifndef Primary_Generator_h
#define Primary_Generator_h 1

#include "Primary_Generator_Messenger.hh"

#include "Reaction.hh"
#include "Gamma_Source.hh"
#include "Excitation.hh"
#include "Data_Format.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4DataInterpolation.hh"
#include "G4Tubs.hh"

class Primary_Generator_Messenger;
class Primary_Generator : public G4VUserPrimaryGeneratorAction {
  
public:
   
  Primary_Generator();
  ~Primary_Generator();

  enum MODE {Scattering,Source,Full};

  void GeneratePrimaries(G4Event* evt);
  
  void SetBeamX(G4double X) {beam_X = X;}
  void SetBeamY(G4double Y) {beam_Y = Y;}
  void SetBeamAX(G4double AX) {beam_AX = AX;}
  void SetBeamAY(G4double AY) {beam_AY = AY;}
  void SetBeamEn(G4double En) {beam_En = En;}
  
  void SetSigmaX(G4double sigX) {sigma_X = sigX;}
  void SetSigmaY(G4double sigY) {sigma_Y = sigY;}
  void SetSigmaAX(G4double sigAX) {sigma_AX = sigAX;}
  void SetSigmaAY(G4double sigAY) {sigma_AY = sigAY;}
  void SetSigmaEn(G4double sigE) {sigma_En = sigE;}

  void SetDeltaE(G4double dE) {deltaE = dE;}

  void SetSourceEnergy(G4double En) {source_energy = En;}
  void SetSourcePosition(G4ThreeVector pos) {source_pos = pos;}
  G4bool IsSimpleSource() {return source_energy > 0.0;}
  
  void Optimize() {optimize = true;}
  void OnlyProjectile() {onlyP = true; onlyR = false;}
  void OnlyRecoil() {onlyR = true; onlyP = false;}
  
  void SetMode(G4String md);
  MODE GetMode() {return mode;}
  
  G4String GetProjectileName() {return projGS->GetParticleName();}
  G4String GetRecoilName() {return recoilGS->GetParticleName();}

  G4int GetProjectileIndex() {return pI;}
  G4int GetRecoilIndex() {return rI;}
  G4double GetThetaCM() {return th/deg;}
  G4double GetBeamEnergy() {return en/MeV;}

  G4int GetZ(G4bool proj);
  G4int GetA(G4bool proj);
  G4double GetMass(G4bool proj);
  std::vector<G4double> GetExcitedStateLifetimes(G4bool proj);
  std::vector<G4double> GetExcitedStateSpins(G4bool proj);
  
  void Update();
  
private:

  inline void GenerateScatteringPrimaries(G4Event* event);
  inline void GenerateSourcePrimaries(G4Event* event);
  inline void GenerateFullPrimaries(G4Event* event);

  inline G4bool CheckIntersections(const G4ThreeVector& bdir, const G4ThreeVector& rdir,
				   const G4ThreeVector& pos);

  inline G4bool Intersects(const G4ThreeVector& dir, const G4ThreeVector& S3pos);

  void UpdateReaction();
  
  Primary_Generator_Messenger* messenger;
  MODE mode;
  G4ParticleGun* gun;
  
  Reaction* reac;
  Gamma_Source* source;
  Excitation* exciteP;
  Excitation* exciteR;
  
  G4ParticleDefinition* projGS; //Projectile ground state
  G4ParticleDefinition* recoilGS; //Recoil ground state

  //Used to for event-by-event diagnostic info
  G4int pI;
  G4int rI;
  G4double th;
  G4double en;

  //Source parameters
  G4double source_energy;
  G4ThreeVector source_pos;
  
  //used for incoming energy loss in target
  G4double dedx;
  //G4DataInterpolation* dedx1;
  G4double width;
  
  G4double beam_X; //X position of beam
  G4double beam_Y; //Y position of beam
  G4double beam_AX; //X angle of beam
  G4double beam_AY; //Y angle of beam
  G4double beam_En; //Kinetic energy of beam

  G4double sigma_X;  //spread in X position
  G4double sigma_Y;  //spread in Y position
  G4double sigma_AX; //spread in angle about X axis
  G4double sigma_AY; //spread in angle about Y axis
  G4double sigma_En; //spread in energy

  G4double deltaE; //For inelastic scattering

  //optimize sampling of scattering angle
  G4bool optimize; //only use sampled angle if a particle will hit an S3
  G4bool onlyP; //only consider projectile for optimization
  G4bool onlyR; //only consider recoil for optimization
  
  G4Tubs* s3; //Shape of S3
  G4ThreeVector s3_0, s3_1; //S3 positions
  
};

#endif
