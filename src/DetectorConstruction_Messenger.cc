#include "DetectorConstruction_Messenger.hh"

DetectorConstruction_Messenger::DetectorConstruction_Messenger(DetectorConstruction* pTarget) : myTarget(pTarget) {
  
  const char *aLine;
  G4String commandName;

  geometry_dir = new G4UIdirectory("/Geometry/");

  commandName = "/Geometry/CheckOverlaps";
  aLine = commandName.c_str();
  check_cmd = new G4UIcmdWithoutParameter(aLine, this);
  check_cmd->SetGuidance("Check for overlapping physical volumes.");
  check_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  commandName = "/Geometry/Gretina/NoDetectors";
  aLine = commandName.c_str();
  NoGretCmd = new G4UIcmdWithoutParameter(aLine, this);
  NoGretCmd->SetGuidance("Omit the GRETINA detectors.");
  NoGretCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  commandName = "/Geometry/Gretina/Shell";
  aLine = commandName.c_str();
  ShellCmd = new G4UIcmdWithAString(aLine, this);
  ShellCmd->SetGuidance("Construct the mounting shell (full/north/south).");
  ShellCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  commandName = "/Geometry/Gretina/NorthOffset";
  aLine = commandName.c_str();
  NorthOffCmd = new G4UIcmdWithADoubleAndUnit(aLine, this);
  NorthOffCmd->SetGuidance("North mounting shell offset (positive -> away from target).");
  NorthOffCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  commandName = "/Geometry/Gretina/SouthOffset";
  aLine = commandName.c_str();
  SouthOffCmd = new G4UIcmdWithADoubleAndUnit(aLine, this);
  SouthOffCmd->SetGuidance("South mounting shell offset (positive -> away from target).");
  SouthOffCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  commandName = "/Geometry/Gretina/LaBr";
  aLine = commandName.c_str();
  LaBrCmd = new G4UIcmdWithoutParameter(aLine, this);
  LaBrCmd->SetGuidance("Construct the LaBr.");
  LaBrCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //S3 commands
  placeS3_cmd = new G4UIcmdWithoutParameter("/Geometry/S3/Construct",this);
  placeS3_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  placeS3_cmd->SetGuidance("Place the S3 detectors");

  offsetUS_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/S3/UpstreamOffset",this);
  offsetUS_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				   G4ApplicationState::G4State_Idle);
  offsetUS_cmd->SetGuidance("Set (positive) z-offset of upstream detector (Default: 3 cm)");
  
  offsetDS_cmd = new G4UIcmdWithADoubleAndUnit("/Geometry/S3/DownstreamOffset",this);
  offsetDS_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				   G4ApplicationState::G4State_Idle);
  offsetDS_cmd->SetGuidance("Set (positive) z-offset of downstream detector (Default: 3 cm)");

  //ChicoX commands
  placeChicoX_cmd = new G4UIcmdWithoutParameter("/Geometry/ChicoX/Construct",this);
  placeChicoX_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  placeChicoX_cmd->SetGuidance("Place the ChicoX array");

  //Chambers
  commandName = "/Geometry/GretaChamber/Construct";
  aLine = commandName.c_str();
  GretaChamberCmd = new G4UIcmdWithoutParameter(aLine, this);
  GretaChamberCmd->SetGuidance("Construct the Greta chamber.");
  GretaChamberCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  commandName = "/Geometry/WUChamber/Construct";
  aLine = commandName.c_str();
  WUChamberCmd = new G4UIcmdWithoutParameter(aLine, this);
  WUChamberCmd->SetGuidance("Construct the WU chamber.");
  WUChamberCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  commandName = "/Geometry/BeamTube/Construct";
  aLine = commandName.c_str();
  BeamTubeCmd = new G4UIcmdWithoutParameter(aLine, this);
  BeamTubeCmd->SetGuidance("Construct the beam tube.");
  BeamTubeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //Target Commands
  placeTarg_cmd = new G4UIcmdWithoutParameter("/Geometry/Target/Construct",this);
  placeTarg_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
				    G4ApplicationState::G4State_Idle);
  placeTarg_cmd->SetGuidance("Place the target");
  
  Z_cmd = new G4UIcmdWithAnInteger("/Geometry/Target/Z",this);
  Z_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,
			    G4ApplicationState::G4State_Idle);
  Z_cmd->SetGuidance("Set target Z");

  A_cmd = new G4UIcmdWithAnInteger("/Geometry/Target/A",this);
  A_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  A_cmd->SetGuidance("Set target A");
  
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

DetectorConstruction_Messenger::~DetectorConstruction_Messenger() {
  
  delete geometry_dir;
  delete check_cmd;
  
  delete NoGretCmd;
  delete ShellCmd;
  delete NorthOffCmd;
  delete SouthOffCmd;
  
  delete LaBrCmd;
  
  delete BeamTubeCmd;
  delete GretaChamberCmd;
  delete WUChamberCmd;
  
  delete placeS3_cmd;
  delete offsetUS_cmd;
  delete offsetDS_cmd;
    
  delete placeChicoX_cmd;

  delete target_dir;
  delete placeTarg_cmd;
  delete Z_cmd;
  delete A_cmd;
  delete density_cmd;
  delete mass_cmd;
  delete thickness_cmd;
  delete radius_cmd;
  delete target_cmd;
  delete step_cmd;

}

void DetectorConstruction_Messenger::SetNewValue(G4UIcommand* command, G4String newValue) { 
  
  if(command == check_cmd) {
    myTarget->SetCheckOverlaps();
    G4cout << "Will check the geometry for overlapping physical volumes" << G4endl;
  }

  else if(command == NoGretCmd) {
    myTarget->SetGretinaStatus(false);
    G4cout << "Gretina will not be present in the simulation" << G4endl;
  }
  else if( command == ShellCmd ) {
    myTarget->SetShellStatus(newValue);
  }
  else if( command == NorthOffCmd ) {
    myTarget->SetNorthOffset(NorthOffCmd->GetNewDoubleValue(newValue));
  }
  else if( command == SouthOffCmd ) {
    myTarget->SetSouthOffset(SouthOffCmd->GetNewDoubleValue(newValue));
  }
  else if( command == LaBrCmd ) {
    myTarget->SetLaBrStatus(true);
  } 

  else if( command == GretaChamberCmd ) {
    myTarget->SetGretaChamberStatus(true);
  }
  else if( command == BeamTubeCmd ) {
    myTarget->SetBeamTubeStatus(true);
  } 
  else if( command == WUChamberCmd ) {
    myTarget->SetWUChamberStatus(true);
  }
  
  /////S3 commands/////
  else if(command == placeS3_cmd) {
    myTarget->SetS3Status(true);
    G4cout << "Simulation will include the silicon detectors" << G4endl;
  }

  else if(command == offsetUS_cmd) {
    myTarget->SetUS_Offset(offsetUS_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting upstream S3 offset to " << newValue << G4endl;
  }
  
  else if(command == offsetDS_cmd) {
    myTarget->SetDS_Offset(offsetDS_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting downstream S3 offset to " << newValue << G4endl;
  }
  ///////////////////////////

  /////ChicoX commands/////
  else if(command == placeChicoX_cmd) {
    myTarget->SetChicoXStatus(true);
    G4cout << "Simulation will include the ChicoX array" << G4endl;
  }
  
  /////Target commands/////
  else if(command == placeTarg_cmd) {
    myTarget->SetTargetStatus(true);
    G4cout << "Simulation will include the target" << G4endl;
  }
  
  else if(command == Z_cmd) {
    myTarget->SetTargetZ(Z_cmd->GetNewIntValue(newValue));
    G4cout << "Setting target Z to " << newValue << G4endl;
  }

  else if(command == A_cmd) {
    myTarget->SetTargetA(A_cmd->GetNewIntValue(newValue));
    G4cout << "Setting target A to " << newValue << G4endl;
  }

  else if(command == density_cmd) {
    myTarget->SetTargetDensity(density_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target density to " << newValue << G4endl;
  }

  else if(command == mass_cmd) {
    myTarget->SetTargetMass(mass_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target mass to " << newValue << G4endl;
  }

  else if(command == thickness_cmd) {
    myTarget->SetTargetThickness(thickness_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target thickness to " << newValue << G4endl;
  }

  else if(command == radius_cmd) {
    myTarget->SetTargetRadius(radius_cmd->GetNewDoubleValue(newValue));
    G4cout << "Setting target radius to " << newValue << G4endl;
  }

  else if(command == step_cmd) {
    G4cout << "Setting step size in the target to " << newValue << G4endl;
    myTarget->SetTargetStepSize(step_cmd->GetNewDoubleValue(newValue));
  }

  else if(command == target_cmd) {
    G4cout << "Setting parameters for a standard " << newValue << " target" << G4endl;
    myTarget->SetTarget(newValue);
  }

  else if(command == print_targ_cmd) {
    myTarget->PrintTarget();
  }
  ////////////////////////

  return;

}
