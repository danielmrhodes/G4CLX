#include "Primary_Generator_Messenger.hh"

Primary_Generator_Messenger::Primary_Generator_Messenger(Primary_Generator* gen) : generator(gen) {
  
  //Simulation mode command
  mode_cmd = new G4UIcmdWithAString("/Mode",this);
  mode_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  mode_cmd->SetCandidates("Scattering Source Full");
  mode_cmd->SetGuidance("Set simulation mode");

  update_cmd = new G4UIcmdWithoutParameter("/UpdateGenerator",this);
  update_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  update_cmd->SetGuidance("Update Primary Generator");

  sEn_cmd = new G4UIcmdWithADoubleAndUnit("/Source/Energy",this);
  sEn_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sEn_cmd->SetGuidance("Set energy of simple isotropic gamma-ray");

  sPos_cmd = new G4UIcmdWith3VectorAndUnit("/Source/Position",this);
  sPos_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sPos_cmd->SetGuidance("Set position of gamma source");
  
  //Incoming beam directory
  incoming_dir = new G4UIdirectory("/Beam/");

  //Incoming beam parameters
  beamX_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/PositionX",this);
  beamX_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  beamX_cmd->SetGuidance("Set X position of incoming beam spot");

  beamY_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/PositionY",this);
  beamY_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  beamY_cmd->SetGuidance("Set Y position of incoming beam spot");

  beamAX_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/AngleX",this);
  beamAX_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  beamAX_cmd->SetGuidance("Set angle about X axis of incoming beam");

  beamAY_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/AngleY",this);
  beamAY_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  beamAY_cmd->SetGuidance("Set angle about Y axis of incoming beam");
  
  beamEn_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/Energy",this);
  beamEn_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  beamEn_cmd->SetGuidance("Set kinetic energy of incoming beam");

  sigX_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/SigmaX",this);
  sigX_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sigX_cmd->SetGuidance("Set Gaussian sigma of X position distribution");

  sigY_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/SigmaY",this);
  sigY_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sigY_cmd->SetGuidance("Set Gaussian sigma of Y position distribution");

  sigAX_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/SigmaAX",this);
  sigAX_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sigAX_cmd->SetGuidance("Set Gaussian sigma of X angle distribution");

  sigAY_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/SigmaAY",this);
  sigAY_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sigAY_cmd->SetGuidance("Set Gaussian sigma of Y angle distribution");

  sigEn_cmd = new G4UIcmdWithADoubleAndUnit("/Beam/SigmaEn",this);
  sigEn_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  sigEn_cmd->SetGuidance("Set Gaussian sigma of kinetic energy distribution");

  //Scattering angle commands
  toS3_cmd = new G4UIcmdWithoutParameter("/Reaction/Optimize",this);
  toS3_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  toS3_cmd->SetGuidance("Only sample parts of the Rutherford distribution which result in a particle entering S3");

  onlyP_cmd = new G4UIcmdWithoutParameter("/Reaction/OnlyProjectiles",this);
  onlyP_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  onlyP_cmd->SetGuidance("Only consider the projectile when defining desired LAB scattering angle ranges");
  
  onlyR_cmd = new G4UIcmdWithoutParameter("/Reaction/OnlyRecoils",this);
  onlyR_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  onlyR_cmd->SetGuidance("Only consider the recoil when defining desired LAB scattering angle ranges");
  
  //Scattering Q-value
  inEl_cmd = new G4UIcmdWithADoubleAndUnit("/Reaction/DeltaE",this);
  inEl_cmd->AvailableForStates(G4ApplicationState::G4State_PreInit,G4ApplicationState::G4State_Idle);
  inEl_cmd->SetGuidance("Set (positive) deltaE for inelastic scattering");

}

Primary_Generator_Messenger::~Primary_Generator_Messenger() {

  delete incoming_dir;

  delete beamX_cmd;
  delete beamY_cmd;
  delete beamAX_cmd;
  delete beamAY_cmd;
  delete beamEn_cmd;
  
  delete sigX_cmd;
  delete sigY_cmd;
  delete sigAX_cmd;
  delete sigAY_cmd;
  delete sigEn_cmd;

  delete toS3_cmd;

  delete onlyP_cmd;
  delete onlyR_cmd;

  delete inEl_cmd;
  
  delete sEn_cmd;
  delete sPos_cmd;

  delete mode_cmd;
  delete update_cmd;
}

void Primary_Generator_Messenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  
  G4String message = "";
  
  /////Incoming beam commands/////
  if(command == beamX_cmd) {
    generator->SetBeamX(beamX_cmd->GetNewDoubleValue(newValue));
      message =  "Setting X position of incoming beam to " + newValue;
  }

  else if(command == beamY_cmd) {
    generator->SetBeamY(beamY_cmd->GetNewDoubleValue(newValue));
    message =  "Setting Y position of incoming beam to " + newValue;
  }

  else if(command == beamAX_cmd) {
    generator->SetBeamAX(beamAX_cmd->GetNewDoubleValue(newValue));
    message =  "Setting X angle of incoming beam to " + newValue;
  }

  else if(command == beamAY_cmd) {
    generator->SetBeamAY(beamAY_cmd->GetNewDoubleValue(newValue));
    message =  "Setting Y angle of incoming beam to " + newValue;
  }
  
  else if(command == beamEn_cmd) {
    generator->SetBeamEn(beamEn_cmd->GetNewDoubleValue(newValue));
    message =  "Setting kinetic energy of incoming beam to " + newValue;
  }
 
  else if(command == sigX_cmd) {
    generator->SetSigmaX(sigX_cmd->GetNewDoubleValue(newValue));
    message =  "Setting sigma of X position distribution to " + newValue;
  }

  else if(command == sigY_cmd) {
    generator->SetSigmaY(sigY_cmd->GetNewDoubleValue(newValue));
    message =  "Setting sigma of Y position distribution to " + newValue;
  }

  else if(command == sigAX_cmd) {
    generator->SetSigmaAX(sigAX_cmd->GetNewDoubleValue(newValue));
    message =  "Setting sigma of X angle distribution to " + newValue;
  }
  
  else if(command == sigAY_cmd) {
    generator->SetSigmaAY(sigAY_cmd->GetNewDoubleValue(newValue));
    message =  "Setting sigma of Y angle distribution to " + newValue;
  }

  else if(command == sigEn_cmd) {
    generator->SetSigmaEn(sigEn_cmd->GetNewDoubleValue(newValue));
    message =  "Setting sigma of energy distribution to " + newValue;
  }
  ////////////////////////////////

  /////Scattering angle commands/////
  else if(command == toS3_cmd) {
    generator->Optimize();
    message =  "Ensuring a particle will always enter S3!";
  }
  
  else if(command == onlyP_cmd) {
    generator->OnlyProjectile();
    message =  "Only considering the projectile nucleus when determining the desired scattering angle ranges!";
  }

  else if(command == onlyR_cmd) {
    generator->OnlyRecoil();
    message =  "Only considering the recoil nucleus when determining the desired scattering angle ranges!";
  }
  ///////////////////////////////////

  else if(command == inEl_cmd) {
    generator->SetDeltaE(inEl_cmd->GetNewDoubleValue(newValue));
    message =  "Setting DeltaE = -Q of scattering reaction to " + newValue;
  }

  ///Gamma-ray source commands///
  if(command == sEn_cmd) {
    generator->SetSourceEnergy(sEn_cmd->GetNewDoubleValue(newValue));
    message = "Setting source gamma-ray energy to " + newValue;
  }
  else if(command == sPos_cmd) {
    generator->SetSourcePosition(sPos_cmd->GetNew3VectorValue(newValue));
    message = "Setting gamma source position to " + newValue;
  }
  ///////////////////////////////
  
  else if(command == mode_cmd) {
    generator->SetMode(newValue);
    message =  "Simulation mode: " + newValue;
  }

  else if(command == update_cmd) {
    generator->Update();
  }

  G4int threadID = G4Threading::G4GetThreadId();
  if(!threadID && message != "")
    std::cout << message << std::endl;
  
  return;
}
