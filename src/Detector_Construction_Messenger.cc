#include "Detector_Construction_Messenger.hh"
#include "Detector_Construction.hh"

Detector_Construction_Messenger::Detector_Construction_Messenger(Detector_Construction* con)
  : construction(con) {

  //All geometries accessed through this directory
  geometry_dir = new G4UIdirectory("/Geometry/");

  check_cmd = new G4UIcmdWithoutParameter("/Geometry/CheckOverlaps",this);
  check_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				G4ApplicationState::G4State_Idle);
  check_cmd->SetGuidance("Check for overlapping volumes");

  //SeGA directory
  sega_dir = new G4UIdirectory("/Geometry/SeGA/");

  placeSega_cmd = new G4UIcmdWithoutParameter("/Geometry/SeGA/Construct",this);
  placeSega_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
  				   G4ApplicationState::G4State_Idle);
  placeSega_cmd->SetGuidance("Place SeGA");
  
  //S3 directory
  s3_dir = new G4UIdirectory("/Geometry/S3/");

  placeSi_cmd = new G4UIcmdWithoutParameter("/Geometry/S3/Construct",this);
  placeSi_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  placeSi_cmd->SetGuidance("Place the S3 detectors");

  offsetUS_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/S3/UpstreamOffset",this);
  offsetUS_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				   G4ApplicationState::G4State_Idle);
  offsetUS_cmd->SetGuidance("Set (positive) z-offset of upstream detector (Default: 3 cm)");
  
  offsetDS_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/S3/DownstreamOffset",this);
  offsetDS_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				   G4ApplicationState::G4State_Idle);
  offsetDS_cmd->SetGuidance("Set (positive) z-offset of downstream detector (Default: 3 cm)");
  
  //Target directory
  target_dir = new G4UIdirectory("/Geometry/Target/");

  placeTarg_cmd = new G4UIcmdWithoutParameter("/Geometry/Target/Construct",this);
  placeTarg_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				    G4ApplicationState::G4State_Idle);
  placeTarg_cmd->SetGuidance("Place the target");
  
  Z_cmd = new G4UIcmdWithAnInteger("/Geometry/Target/Z",this);
  Z_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
			    G4ApplicationState::G4State_Idle);
  Z_cmd->SetGuidance("Set target Z");

  N_cmd = new G4UIcmdWithAnInteger("/Geometry/Target/N",this);
  N_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  N_cmd->SetGuidance("Set target N");
  
  density_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/Target/Density",this);
  density_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				  G4ApplicationState::G4State_Idle);
  density_cmd->SetGuidance("Set target density");
  
  mass_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/Target/Mass",this);
  mass_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  mass_cmd->SetGuidance("Set target mass");
  
  thickness_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/Target/Thickness",this);
  thickness_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				    G4ApplicationState::G4State_Idle);
  thickness_cmd->SetGuidance("Set target linear thickness (length)");
  
  radius_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/Target/Radius",this);
  radius_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				 G4ApplicationState::G4State_Idle);
  radius_cmd->SetGuidance("Set target radius");
  
  target_cmd = new G4UIcmdWithAString("/Geometry/Target/StandardTarget",this);
  target_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				 G4ApplicationState::G4State_Idle);
  target_cmd->SetCandidates("48Ti Ti48 48ti ti48 208Pb Pb208 208pb pb208 196Pt Pt196 196pt pt196 194Pt Pt194 194pt pt194 110Pd Pd110 110pd pd110 197Au Au197 197au au197");
  target_cmd->SetGuidance("Construct a standard target: 208Pb, 48Ti, 196Pt, 110Pd, or 197Au");

  step_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/Target/StepSize",this);
  step_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
			       G4ApplicationState::G4State_Idle);
  step_cmd->SetGuidance("Set simulation step size in the target");
  
  print_targ_cmd = new G4UIcmdWithoutParameter("/Geometry/Target/Print",this);
  print_targ_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				     G4ApplicationState::G4State_Idle);
  print_targ_cmd->SetGuidance("Print target parameters");
  
}

Detector_Construction_Messenger::~Detector_Construction_Messenger() {

  delete geometry_dir;
  delete check_cmd;

  delete sega_dir;
  delete placeSega_cmd;
  
  delete s3_dir;
  delete placeSi_cmd;
  delete offsetUS_cmd;
  delete offsetDS_cmd;
    
  delete target_dir;
  delete placeTarg_cmd;
  delete Z_cmd;
  delete N_cmd;
  delete density_cmd;
  delete mass_cmd;
  delete thickness_cmd;
  delete radius_cmd;
  delete target_cmd;
  delete step_cmd;
  
}

void Detector_Construction_Messenger::SetNewValue(G4UIcommand* command, G4String newValue) {

  if(command == check_cmd) {
    construction->CheckOverlaps();
    G4cout << "Will check the geometry for overlapping physical volumes" << G4endl;
  }
  
  /////SeGA commands/////
  else if(command == placeSega_cmd) {
    construction->SetPlaceSeGA();
    G4cout << "Simulation will include the SeGA array" << G4endl;
  }
  //////////////////////////
  
  /////S3 commands/////
  else if(command == placeSi_cmd) {
    construction->SetPlaceS3();
    G4cout << "Simulation will include the silicon detectors" << G4endl;
  }

  else if(command == offsetUS_cmd) {
    construction->SetUS_Offset(offsetUS_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting upstream S3 offset to " << newValue << G4endl;
  }
  
  else if(command == offsetDS_cmd) {
    construction->SetDS_Offset(offsetDS_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting downstream S3 offset to " << newValue << G4endl;
  }
  ///////////////////////////
  
  /////Target commands/////
  else if(command == placeTarg_cmd) {
    construction->SetPlaceTarget();
    G4cout << "Simulation will include the target" << G4endl;
  }
  
  else if(command == Z_cmd) {
    construction->SetTargetZ(Z_cmd->GetNewIntValue(newValue));
    G4cout << "Setting target Z to " << newValue << G4endl;
  }

  else if(command == N_cmd) {
    construction->SetTargetN(N_cmd->GetNewIntValue(newValue));
    G4cout << "Setting target N to " << newValue << G4endl;
  }

  else if(command == density_cmd) {
    construction->SetTargetDensity(density_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target density to " << newValue << G4endl;
  }

  else if(command == mass_cmd) {
    construction->SetTargetMass(mass_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target mass to " << newValue << G4endl;
  }

  else if(command == thickness_cmd) {
    construction->SetTargetThickness(thickness_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target thickness to " << newValue << G4endl;
  }

  else if(command == radius_cmd) {
    construction->SetTargetRadius(radius_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target radius to " << newValue << G4endl;
  }

  else if(command == step_cmd) {
    G4cout << "Setting step size in the target to " << newValue << G4endl;
    construction->SetTargetStepSize(step_cmd->GetNewDoubleValue(newValue));
  }

  else if(command == target_cmd) {
    G4cout << "Setting parameters for a standard " << newValue << " target" << G4endl;
    construction->SetTarget(newValue);
  }

  else if(command == print_targ_cmd) {
    construction->PrintTarget();
  }
  /////////////////////////

  return;
}
