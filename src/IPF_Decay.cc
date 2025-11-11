#include "IPF_Decay.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4SystemOfUnits.hh"

#include "G4DynamicParticle.hh"
#include "G4DecayProducts.hh"
#include "Randomize.hh"
#include "G4HadronicException.hh"

IPF_Decay::IPF_Decay(G4ParticleDefinition* Parent, G4ParticleDefinition* daughter, G4double cc, G4bool emt)
  : G4VDecayChannel("IPFDecay",Parent->GetParticleName(),1.0,3,daughter->GetParticleName(),"e-","e+"), theDaughterMasses(0) {
  
  SetParent(Parent);
  parentmass = Parent->GetPDGMass();
  
  SetDaughter(0,daughter);
  SetDaughter(1,G4Electron::Definition());
  SetDaughter(2,G4Positron::Definition());

  convCoef = cc;
  emit = emt;
  
  return;

}

IPF_Decay::~IPF_Decay() {;}

G4DecayProducts* IPF_Decay::DecayIt(G4double) {

  // Algorithm of this code originally written in GDECA3 of GEANT3
  
  // parent mass
  //G4double parentmass = current_parent_mass.Get();

  //daughters'mass
  G4double daughtermass[3];
  if(theDaughterMasses) { 
    daughtermass[0]= *(theDaughterMasses);
    daughtermass[1] = *(theDaughterMasses+1);
    daughtermass[2] = *(theDaughterMasses+2);
  } 
  else {   
    daughtermass[0] = G4MT_daughters[0]->GetPDGMass();
    daughtermass[1] = G4MT_daughters[1]->GetPDGMass();
    daughtermass[2] = G4MT_daughters[2]->GetPDGMass();
  }
  G4double sumofdaughtermass = daughtermass[0] + daughtermass[1] + daughtermass[2];

  // create parent G4DynamicParticle at rest
  G4ThreeVector dummy;
  auto parentparticle = new G4DynamicParticle(G4MT_parent, dummy, 0.0, parentmass);

  // create G4Decayproducts
  auto products = new G4DecayProducts(*parentparticle);
  delete parentparticle;

  if (sumofdaughtermass > parentmass) {
  
      G4cout << "G4PhaseSpaceDecayChannel::ThreeBodyDecayIt()" << G4endl
             << "Sum of daughter mass is larger than parent mass!" << G4endl;
      G4cout << "Parent :" << G4MT_parent->GetParticleName() << "  "
             << parentmass / GeV << G4endl;
      G4cout << "Daughter 1 :" << G4MT_daughters[0]->GetParticleName() << "  "
             << daughtermass[0] / GeV << G4endl;
      G4cout << "Daughter 2:" << G4MT_daughters[1]->GetParticleName() << "  "
             << daughtermass[1] / GeV << G4endl;
      G4cout << "Daughter 3:" << G4MT_daughters[2]->GetParticleName() << "  "
             << daughtermass[2] / GeV << G4endl;
        
    G4Exception("G4PhaseSpaceDecayChannel::ThreeBodyDecayIt", "PART112", JustWarning,
                "Can not create decay products: sum of daughter mass \
                 is larger than parent mass!");
    return products;
      
  }

  // calculate daughter momentum
  // Generate two
  G4double rd1, rd2, rd;
  G4double daughtermomentum[3];
  G4double momentummax = 0.0, momentumsum = 0.0;
  G4double energy;
  const std::size_t MAX_LOOP = 10000;

  for (std::size_t loop_counter = 0; loop_counter < MAX_LOOP; ++loop_counter) {
    rd1 = G4UniformRand();
    //rd2 = G4UniformRand();
    rd2 = 0.00000000001;
    if (rd2 > rd1) {
      rd = rd1;
      rd1 = rd2;
      rd2 = rd;
    }
    momentummax = 0.0;
    momentumsum = 0.0;
    
    // daughter 0
    energy = rd2 * (parentmass - sumofdaughtermass);
    daughtermomentum[0] = std::sqrt(energy * energy + 2.0 * energy * daughtermass[0]);
    if (daughtermomentum[0] > momentummax) momentummax = daughtermomentum[0];
    momentumsum += daughtermomentum[0];
    
    // daughter 1
    energy = (1. - rd1) * (parentmass - sumofdaughtermass);
    daughtermomentum[1] = std::sqrt(energy * energy + 2.0 * energy * daughtermass[1]);
    if (daughtermomentum[1] > momentummax) momentummax = daughtermomentum[1];
    momentumsum += daughtermomentum[1];
    
    // daughter 2
    energy = (rd1 - rd2) * (parentmass - sumofdaughtermass);
    daughtermomentum[2] = std::sqrt(energy * energy + 2.0 * energy * daughtermass[2]);
    if (daughtermomentum[2] > momentummax) momentummax = daughtermomentum[2];
    momentumsum += daughtermomentum[2];
    if (momentummax <= momentumsum - momentummax) { 
      //std::cout << "Breaking at " << loop_counter << std::endl;
      break;
    }
    else if(loop_counter == MAX_LOOP-1) {
      std::cout << "Bad IPF Decay" << std::endl;
      return products;
    }
  }

  // output message
  if (GetVerboseLevel() > 1) {
    G4cout << "     daughter 0:" << daughtermomentum[0] / GeV << "[GeV/c]" << G4endl;
    G4cout << "     daughter 1:" << daughtermomentum[1] / GeV << "[GeV/c]" << G4endl;
    G4cout << "     daughter 2:" << daughtermomentum[2] / GeV << "[GeV/c]" << G4endl;
    G4cout << "   momentum sum:" << momentumsum / GeV << "[GeV/c]" << G4endl;
  }

  // create daughter G4DynamicParticle
  G4double costheta, sintheta, phi, sinphi, cosphi;
  G4double costhetan, sinthetan, phin, sinphin, cosphin;
  costheta = 2. * G4UniformRand() - 1.0;
  sintheta = std::sqrt((1.0 - costheta) * (1.0 + costheta));
  phi = CLHEP::twopi * G4UniformRand() * rad;
  sinphi = std::sin(phi);
  cosphi = std::cos(phi);

  G4ThreeVector direction0(sintheta * cosphi, sintheta * sinphi, costheta);
  G4double Ekin =
    std::sqrt(daughtermomentum[0] * daughtermomentum[0] + daughtermass[0] * daughtermass[0])
    - daughtermass[0];
  auto daughterparticle =
    new G4DynamicParticle(G4MT_daughters[0], direction0, Ekin, daughtermass[0]);
  products->PushProducts(daughterparticle);

  if(!emit)
    return products;
  
  G4double prob = 1.0/(convCoef + 1.0);
  if(G4UniformRand() <= prob)
    return products;

  costhetan = (daughtermomentum[1] * daughtermomentum[1] - daughtermomentum[2] * daughtermomentum[2]
               - daughtermomentum[0] * daughtermomentum[0])
              / (2.0 * daughtermomentum[2] * daughtermomentum[0]);

  //std::cout << costhetan << std::endl;
  
  sinthetan = std::sqrt((1.0 - costhetan) * (1.0 + costhetan));
  //std::cout << sinthetan << std::endl;
  
  phin = CLHEP::twopi * G4UniformRand() * rad;
  sinphin = std::sin(phin);
  cosphin = std::cos(phin);
  G4ThreeVector direction2;
  direction2.setX(sinthetan * cosphin * costheta * cosphi - sinthetan * sinphin * sinphi
                  + costhetan * sintheta * cosphi);
  direction2.setY(sinthetan * cosphin * costheta * sinphi + sinthetan * sinphin * cosphi
                  + costhetan * sintheta * sinphi);
  direction2.setZ(-sinthetan * cosphin * sintheta + costhetan * costheta);
  G4ThreeVector pmom = daughtermomentum[2] * direction2 / direction2.mag();
  Ekin = std::sqrt(pmom.mag2() + daughtermass[2] * daughtermass[2]) - daughtermass[2];
  if(emit) {
    daughterparticle =
      new G4DynamicParticle(G4MT_daughters[2], pmom / pmom.mag(), Ekin, daughtermass[2]);
    products->PushProducts(daughterparticle);
  }
  
  pmom = (direction0 * daughtermomentum[0] + direction2 * (daughtermomentum[2] / direction2.mag()))
         * (-1.0);
  Ekin = std::sqrt(pmom.mag2() + daughtermass[1] * daughtermass[1]) - daughtermass[1];
  if(emit) {
    daughterparticle =
      new G4DynamicParticle(G4MT_daughters[1], pmom / pmom.mag(), Ekin, daughtermass[1]);
    products->PushProducts(daughterparticle);
  }

  if (GetVerboseLevel() > 1) {
    G4cout << "G4PhaseSpaceDecayChannel::ThreeBodyDecayIt -";
    G4cout << " create decay products in rest frame " << G4endl;
    products->DumpInfo();
  }

  return products;

}
