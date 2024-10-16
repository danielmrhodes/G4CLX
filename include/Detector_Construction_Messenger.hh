#ifndef Detector_Construction_Messenger_h
#define Detector_Construction_Messenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

class Detector_Construction;
class Detector_Construction_Messenger: public G4UImessenger {

public:

  Detector_Construction_Messenger(Detector_Construction*);
  ~Detector_Construction_Messenger();
  
private:

  void SetNewValue(G4UIcommand* command, G4String newValue);

  Detector_Construction* construction;

  //Geometry directory and subdirectories
  G4UIdirectory* geometry_dir;
  G4UIdirectory* sega_dir;
  G4UIdirectory* s3_dir;

  //Check overlaps
  G4UIcmdWithoutParameter* check_cmd;
  
  //SeGA commands
  G4UIcmdWithoutParameter* placeSega_cmd;
  
  //S3 commands
  G4UIcmdWithoutParameter* placeSi_cmd;
  G4UIcmdWithADoubleAndUnit* offsetUS_cmd;
  G4UIcmdWithADoubleAndUnit* offsetDS_cmd;
  
  //Target commands
  G4UIcmdWithoutParameter* placeTarg_cmd;
  G4UIdirectory* target_dir;
  G4UIcmdWithAnInteger* Z_cmd;
  G4UIcmdWithAnInteger* N_cmd;
  G4UIcmdWithADoubleAndUnit* density_cmd;
  G4UIcmdWithADoubleAndUnit* mass_cmd;
  G4UIcmdWithADoubleAndUnit* thickness_cmd;
  G4UIcmdWithADoubleAndUnit* radius_cmd;
  G4UIcmdWithAString* target_cmd;
  G4UIcmdWithADoubleAndUnit* step_cmd;
  G4UIcmdWithoutParameter* print_targ_cmd;
  
};

#endif
