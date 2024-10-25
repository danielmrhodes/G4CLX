/*
#include "Physics_List.hh"

#include "G4SystemOfUnits.hh"

#include "G4GenericIon.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4PhysicsListHelper.hh"
#include "G4StepLimiter.hh"

#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4PEEffectFluoModel.hh"
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"

Physics_List::Physics_List() {}
Physics_List::~Physics_List() {}

void Physics_List::ConstructProcess() {

  AddTransportation();
  
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  //Electron
  ph->RegisterProcess(new G4eMultipleScattering(),G4Electron::Definition());
  ph->RegisterProcess(new G4eIonisation(),G4Electron::Definition());
  ph->RegisterProcess(new G4eBremsstrahlung(),G4Electron::Definition());
  
  //Positron
  ph->RegisterProcess(new G4eMultipleScattering(),G4Positron::Definition());
  ph->RegisterProcess(new G4eIonisation(),G4Positron::Definition());
  ph->RegisterProcess(new G4eBremsstrahlung(),G4Positron::Definition());
  ph->RegisterProcess(new G4eplusAnnihilation(),G4Positron::Definition());
  
  //Generic Ion
  G4NuclearStopping* inuc = new G4NuclearStopping();
  
  G4ionIonisation* ionIoni = new G4ionIonisation();
  ionIoni->SetEmModel(new G4IonParametrisedLossModel());
  ionIoni->SetStepFunction(0.1,1*um);

  //ph->RegisterProcess(new G4hMultipleScattering("ionmsc"),G4GenericIon::Definition());
  ph->RegisterProcess(ionIoni,G4GenericIon::Definition());
  ph->RegisterProcess(inuc,G4GenericIon::Definition());
  ph->RegisterProcess(new G4StepLimiter(),G4GenericIon::Definition());

  inuc->SetMaxKinEnergy(MeV);
  
  //Gamma
  G4PhotoElectricEffect* pe = new G4PhotoElectricEffect();
  pe->SetEmModel(new G4LivermorePhotoElectricModel());
  ph->RegisterProcess(pe,G4Gamma::Definition());

  // Compton scattering
  G4ComptonScattering* cs = new G4ComptonScattering;
  cs->SetEmModel(new G4KleinNishinaModel());
  
  G4VEmModel* theLowEPComptonModel = new G4LowEPComptonModel();
  theLowEPComptonModel->SetHighEnergyLimit(20*MeV);
  cs->AddEmModel(0,theLowEPComptonModel);
  ph->RegisterProcess(cs,G4Gamma::Definition());

  // Gamma conversion
  G4GammaConversion* gc = new G4GammaConversion();
  G4VEmModel* thePenelopeGCModel = new G4PenelopeGammaConversionModel();
  
  thePenelopeGCModel->SetHighEnergyLimit(1*GeV);
  gc->SetEmModel(thePenelopeGCModel);
  ph->RegisterProcess(gc,G4Gamma::Definition());

  // Rayleigh scattering
  ph->RegisterProcess(new G4RayleighScattering(),G4Gamma::Definition());
  
  return;
}

void Physics_List::ConstructParticle() {

  G4Gamma::Definition();
  G4Proton::Definition();
  G4Electron::Definition();
  G4Positron::Definition();
  
  G4GenericIon::Definition();

  return;
  
}
*/


#include "Physics_List.hh"
#include "G4EmStandardPhysics_option4.hh"

Physics_List::Physics_List() : G4VModularPhysicsList() {;}
Physics_List::~Physics_List() {;}

void Physics_List::ConstructProcess() {
  
  AddTransportation();

  G4EmStandardPhysics_option4 emPhysicsList;
  emPhysicsList.ConstructProcess();
  
  return;
}

void Physics_List::ConstructParticle() {
  
  G4EmStandardPhysics_option4 emPhysicsList;
  emPhysicsList.ConstructParticle();
  
  return;
  
}


/*
#include "Physics_List.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4LossTableManager.hh"
#include "G4EmParameters.hh"
#include "G4EmBuilder.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4PEEffectFluoModel.hh"
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"
#include "G4BetheHeitler5DModel.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4MscStepLimitType.hh"
#include "G4UrbanMscModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4DummyModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4Generator2BS.hh"
#include "G4Generator2BN.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4ePairProduction.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"
// Begin ATIMA Stopping Power Modifications
#include "G4AtimaEnergyLossModel.hh"
#include "G4AtimaFluctuations.hh"
#include "G4BraggIonModel.hh"
// End   ATIMA Stopping Power Modifications
#include "G4eplusAnnihilation.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4GenericIon.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4EmModelActivator.hh"
#include "G4GammaGeneralProcess.hh"

#include "G4PhysicsConstructorFactory.hh"
G4_DECLARE_PHYSCONSTR_FACTORY(Physics_List_Constructor);

Physics_List_Constructor::Physics_List_Constructor(G4int ver, const G4String&)
  : G4VPhysicsConstructor("Physics_List_Constructor"), verbose(ver) {

  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(ver);
  param->SetMinEnergy(100*CLHEP::eV);
  param->SetLowestElectronEnergy(100*CLHEP::eV);
  param->SetNumberOfBinsPerDecade(20);
  param->ActivateAngularGeneratorForIonisation(true);
  param->SetStepFunction(0.2, 10*CLHEP::um);
  param->SetStepFunctionMuHad(0.1, 50*CLHEP::um);
  param->SetStepFunctionLightIons(0.1, 20*CLHEP::um);
  param->SetStepFunctionIons(0.1, 1*CLHEP::um);
  param->SetUseMottCorrection(true); // use Mott-correction for e-/e+ msc gs
  param->SetMscStepLimitType(fUseSafetyPlus); // for e-/e+ msc gs
  param->SetMscSkin(3);              // error-free stepping for e-/e+ msc gs
  param->SetMscRangeFactor(0.08);    // error-free stepping for e-/e+ msc gs
  param->SetMuHadLateralDisplacement(true);
  param->SetFluo(true);
  param->SetUseICRU90Data(true);
  param->SetMaxNIELEnergy(1*CLHEP::MeV);
  SetPhysicsType(bElectromagnetic);

  return;
}

Physics_List_Constructor::~Physics_List_Constructor() {;}

void Physics_List_Constructor::ConstructParticle() {
  
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();
  
  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();
  
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();
  
  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();
  
  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
  
  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
  
  return;

}

void Physics_List_Constructor::ConstructProcess() {
  
  G4EmBuilder::PrepareEMPhysics();

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // processes used by several particles
  G4ePairProduction* ee = new G4ePairProduction();
  G4hMultipleScattering* hmsc = new G4hMultipleScattering("ionmsc");

  // nuclear stopping
  G4double nielEnergyLimit = G4EmParameters::Instance()->MaxNIELEnergy();
  G4NuclearStopping* pnuc = new G4NuclearStopping();
  pnuc->SetMaxKinEnergy(nielEnergyLimit);

  // high energy limit for e+- scattering models and bremsstrahlung
  G4double highEnergyLimit = G4EmParameters::Instance()->MscEnergyLimit();

  // Add gamma EM Processes
  G4ParticleDefinition* particle = G4Gamma::Gamma();

  // Photoelectric
  G4PhotoElectricEffect* pe = new G4PhotoElectricEffect();
  G4VEmModel* theLivermorePEModel = new G4LivermorePhotoElectricModel();
  pe->SetEmModel(theLivermorePEModel);

  // Compton scattering
  G4ComptonScattering* cs = new G4ComptonScattering;
  cs->SetEmModel(new G4KleinNishinaModel());
  G4VEmModel* theLowEPComptonModel = new G4LowEPComptonModel();
  theLowEPComptonModel->SetHighEnergyLimit(20*CLHEP::MeV);
  cs->AddEmModel(0, theLowEPComptonModel);

  // Gamma conversion
  G4GammaConversion* gc = new G4GammaConversion();
  G4VEmModel* conv = new G4BetheHeitler5DModel();
  gc->SetEmModel(conv);

  if(G4EmParameters::Instance()->GeneralProcessActive()) {
    G4GammaGeneralProcess* sp = new G4GammaGeneralProcess();
    sp->AddEmProcess(pe);
    sp->AddEmProcess(cs);
    sp->AddEmProcess(gc);
    sp->AddEmProcess(new G4RayleighScattering());
    G4LossTableManager::Instance()->SetGammaGeneralProcess(sp);
    ph->RegisterProcess(sp, particle);
  } else {
    ph->RegisterProcess(pe, particle);
    ph->RegisterProcess(cs, particle);
    ph->RegisterProcess(gc, particle);
    ph->RegisterProcess(new G4RayleighScattering(), particle);
  }

  // e-
  particle = G4Electron::Electron();

  // multiple scattering
  G4eMultipleScattering* msc = new G4eMultipleScattering();
  // e-/e+ msc gs with Mott-correction
  // (Mott-correction is set through G4EmParameters)
  G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
  G4WentzelVIModel* msc2 = new G4WentzelVIModel();
  msc1->SetHighEnergyLimit(highEnergyLimit);
  msc2->SetLowEnergyLimit(highEnergyLimit);
  msc->SetEmModel(msc1);
  msc->SetEmModel(msc2);

  G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
  G4CoulombScattering* ss = new G4CoulombScattering();
  ss->SetEmModel(ssm); 
  ss->SetMinKinEnergy(highEnergyLimit);
  ssm->SetLowEnergyLimit(highEnergyLimit);
  ssm->SetActivationLowEnergyLimit(highEnergyLimit);

  // ionisation
  G4eIonisation* eioni = new G4eIonisation();
  G4VEmModel* theIoniLiv = new G4LivermoreIonisationModel();
  theIoniLiv->SetHighEnergyLimit(0.1*CLHEP::MeV); 
  eioni->AddEmModel(0, theIoniLiv, new G4UniversalFluctuation() );

  // bremsstrahlung
  G4eBremsstrahlung* brem = new G4eBremsstrahlung();
  G4SeltzerBergerModel* br1 = new G4SeltzerBergerModel();
  G4eBremsstrahlungRelModel* br2 = new G4eBremsstrahlungRelModel();
  br1->SetAngularDistribution(new G4Generator2BS());
  br2->SetAngularDistribution(new G4Generator2BS());
  brem->SetEmModel(br1);
  brem->SetEmModel(br2);
  br1->SetHighEnergyLimit(CLHEP::GeV);

  // register processes
  ph->RegisterProcess(msc, particle);
  ph->RegisterProcess(eioni, particle);
  ph->RegisterProcess(brem, particle);
  ph->RegisterProcess(ee, particle);
  ph->RegisterProcess(ss, particle);

  // e+
  particle = G4Positron::Positron();

  // multiple scattering
  msc = new G4eMultipleScattering();
  // e-/e+ msc gs with Mott-correction
  // (Mott-correction is set through G4EmParameters)
  msc1 = new G4GoudsmitSaundersonMscModel();
  msc2 = new G4WentzelVIModel();
  msc1->SetHighEnergyLimit(highEnergyLimit);
  msc2->SetLowEnergyLimit(highEnergyLimit);
  msc->SetEmModel(msc1);
  msc->SetEmModel(msc2);

  ssm = new G4eCoulombScatteringModel(); 
  ss = new G4CoulombScattering();
  ss->SetEmModel(ssm); 
  ss->SetMinKinEnergy(highEnergyLimit);
  ssm->SetLowEnergyLimit(highEnergyLimit);
  ssm->SetActivationLowEnergyLimit(highEnergyLimit);

  // ionisation
  eioni = new G4eIonisation();
  G4VEmModel* pen = new G4PenelopeIonisationModel();
  pen->SetHighEnergyLimit(0.1*CLHEP::MeV);
  eioni->AddEmModel(0, pen, new G4UniversalFluctuation());

  // bremsstrahlung
  brem = new G4eBremsstrahlung();
  br1 = new G4SeltzerBergerModel();
  br2 = new G4eBremsstrahlungRelModel();
  br1->SetAngularDistribution(new G4Generator2BS());
  br2->SetAngularDistribution(new G4Generator2BS());
  brem->SetEmModel(br1);
  brem->SetEmModel(br2);
  br1->SetHighEnergyLimit(CLHEP::GeV);

  // register processes
  ph->RegisterProcess(msc, particle);
  ph->RegisterProcess(eioni, particle);
  ph->RegisterProcess(brem, particle);
  ph->RegisterProcess(ee, particle);
  ph->RegisterProcess(new G4eplusAnnihilation(), particle);
  ph->RegisterProcess(ss, particle);

  // generic ion
  particle = G4GenericIon::GenericIon();
  G4ionIonisation* ionIoni = new G4ionIonisation();
  
  // Begin ATIMA Stopping Power Modifications
  //ionIoni->SetEmModel(new G4IonParametrisedLossModel(),0);
  ionIoni->SetEmModel(new G4BraggIonModel(),0);
  ionIoni->SetEmModel(new G4AtimaEnergyLossModel(),1);
  ionIoni->SetFluctModel(new G4AtimaFluctuations());
  // End ATIMA Stopping Power Modifications
  
  ph->RegisterProcess(hmsc, particle);
  ph->RegisterProcess(ionIoni, particle);
  ph->RegisterProcess(pnuc, particle);

  // muons, hadrons, ions
  G4EmBuilder::ConstructCharged(hmsc, pnuc);

  // extra configuration
  G4EmModelActivator mact(GetPhysicsName());

  return;
}

Physics_List::Physics_List() {

  fEmPhysicsList = new Physics_List_Constructor();
  return;
}
Physics_List::~Physics_List() {;}

void Physics_List::ConstructParticle() {
  
  fEmPhysicsList->ConstructParticle();

  return;
}

void Physics_List::ConstructProcess() {
  
  AddTransportation();
  fEmPhysicsList->ConstructProcess();

  return;
}
*/
