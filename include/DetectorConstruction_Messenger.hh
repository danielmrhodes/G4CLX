#ifndef DetectorConstruction_Messenger_H
#define DetectorConstruction_Messenger_H 1

#include "DetectorConstruction.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class DetectorConstruction;
class DetectorConstruction_Messenger: public G4UImessenger {

public:
  
  DetectorConstruction_Messenger(DetectorConstruction*);
  ~DetectorConstruction_Messenger();
  
  void SetNewValue(G4UIcommand*, G4String);

private:
  
  DetectorConstruction* myTarget;
  
  G4UIdirectory* geometry_dir;

  //Check overlaps
  G4UIcmdWithoutParameter* check_cmd;

  G4UIcmdWithoutParameter* UpdateCmd;
  G4UIcmdWithoutParameter* NoGretCmd;
  
  G4UIcmdWithAString* ShellCmd;
  G4UIcmdWithADoubleAndUnit* NorthOffCmd;
  G4UIcmdWithADoubleAndUnit* SouthOffCmd;
  G4UIcmdWithoutParameter* LaBrCmd;
  
  G4UIcmdWithoutParameter* BeamTubeCmd;
  G4UIcmdWithoutParameter* GretaChamberCmd;
  G4UIcmdWithoutParameter* WUChamberCmd;

  //S3 commands
  G4UIcmdWithoutParameter* placeS3_cmd;
  G4UIcmdWithADoubleAndUnit* offsetUS_cmd;
  G4UIcmdWithADoubleAndUnit* offsetDS_cmd;

  //ChicoX commands
  G4UIcmdWithoutParameter* placeChicoX_cmd;
  
  //Target commands
  G4UIcmdWithoutParameter* placeTarg_cmd;
  G4UIdirectory* target_dir;
  G4UIcmdWithAnInteger* Z_cmd;
  G4UIcmdWithAnInteger* A_cmd;
  G4UIcmdWithADoubleAndUnit* density_cmd;
  G4UIcmdWithADoubleAndUnit* mass_cmd;
  G4UIcmdWithADoubleAndUnit* thickness_cmd;
  G4UIcmdWithADoubleAndUnit* radius_cmd;
  G4UIcmdWithAString* target_cmd;
  G4UIcmdWithADoubleAndUnit* step_cmd;
  G4UIcmdWithoutParameter* print_targ_cmd;
  
};

#endif
