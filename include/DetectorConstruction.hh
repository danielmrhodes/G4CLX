#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "DetectorConstruction_Messenger.hh"
#include "Beam_Tube.hh"
#include "Greta_Chamber.hh"
#include "WU_Chamber.hh"
#include "Gretina_NSCL_Shell.hh"
#include "Greta_Shell.hh"
#include "Gretina_Array.hh"
#include "S3.hh"
#include "ChicoX.hh"
#include "LaBr.hh"
#include "GammaSD.hh"
#include "IonSD.hh"
#include "PPAC_SD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"

class Gretina_Array;
class DetectorConstruction_Messenger;
class DetectorConstruction : public G4VUserDetectorConstruction {
public:

  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume* Construct();
  Gretina_Array* GetGretina() {return the_Gretina_Array;}

  G4LogicalVolume*   HallLog() {return ExpHall_log;}
  G4VPhysicalVolume* HallPhys() {return ExpHall_phys;}

  void SetGretinaStatus(G4bool stat) {gretinaStatus = stat;}
  void SetS3Status(G4bool stat) {s3Status = stat;}
  void SetChicoXStatus(G4bool stat) {chicoXStatus = stat;}
  void SetLaBrStatus(G4bool stat) {laBrStatus = stat;}

  void SetTargetStatus(G4bool stat) {targetStatus = stat;}
  void SetBeamTubeStatus(G4bool stat) {beamTubeStatus = stat;}
  void SetGretaChamberStatus(G4bool stat) {gretaChamberStatus = stat;}
  void SetWUChamberStatus(G4bool stat) {WUChamberStatus = stat;}
  void SetCheckOverlaps() {check = true;}

  void SetShellStatus(G4String stat) {shellStatus = stat;}
  void SetNorthOffset(G4double off) {northOffset = off;}
  void SetSouthOffset(G4double off) {southOffset = off;}

  void SetUS_Offset(G4double off) {US_Offset = off;}
  void SetDS_Offset(G4double off) {DS_Offset = off;}

  void SetTargetZ(G4int Z)  {target_Z = Z;}
  void SetTargetA(G4int A)  {target_A = A;}
  void SetTargetDensity(G4double dens)  {target_density = dens;}
  void SetTargetMass(G4double mass)  {target_mass = mass;}
  void SetTargetThickness(G4double thick)  {target_thickness = thick;}
  void SetTargetRadius(G4double r)  {target_radius = r;}
  void SetTargetStepSize(G4double step) {target_step = step;}
  
  void SetTarget(G4String target);
  void PrintTarget();
  void PlaceTarget();

  G4Material* GetTargetMaterial() {return target_mat;}
  G4double GetTargetThickness() {return target_thickness;}
  G4double GetTargetDensity() {return target_density;}
  
  G4double GetUS_Offset() {return US_Offset;}
  G4double GetDS_Offset() {return DS_Offset;}

private:
  
  DetectorConstruction_Messenger *myMessenger;

  void DefineMaterials();
  void ConstructSDandField();
  
  G4bool gretinaStatus;
  G4bool s3Status;
  G4bool chicoXStatus;
  G4bool laBrStatus;
  
  G4bool targetStatus;
  G4bool beamTubeStatus;
  G4bool gretaChamberStatus;
  G4bool WUChamberStatus;
  G4bool check;

  G4String shellStatus;
  G4double northOffset;
  G4double southOffset;
  
  //BambinoX Z-offsest
  G4double US_Offset;
  G4double DS_Offset;
  
  //Target parameters
  G4int target_Z;
  G4int target_A;
  G4double target_density;
  G4double target_mass;
  G4double target_thickness;
  G4double target_radius;
  G4double target_step;
  G4Material* target_mat;
  
  Gretina_Array* the_Gretina_Array;
  
  G4LogicalVolume* ExpHall_log;
  G4VPhysicalVolume* ExpHall_phys;
  

};


#endif
