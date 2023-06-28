#include "G4CherenkovPhysicsList.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4StoppingPhysics.hh"

#include "G4EmStandardPhysics_option3.hh"

#include "G4DecayPhysics.hh"

#include "G4RadioactiveDecayPhysics.hh"

#include "G4OpticalPhysics.hh"
#include "G4SystemOfUnits.hh"
G4CherenkovPhysicsList::G4CherenkovPhysicsList()
:G4VModularPhysicsList()
{
  G4int verb = 1;
  SetVerboseLevel(verb);
  
  RegisterPhysics( new G4HadronElasticPhysicsHP(verb) );
  ////RegisterPhysics( new G4HadronElasticPhysicsXS(verb) );
    
  // Hadron Inelastic Physics
  ////RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC(verb));
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
  ////RegisterPhysics( new G4HadronInelasticQBBC(verb));
  ////RegisterPhysics( new G4HadronPhysicsINCLXX(verb));
  
  // Ion Elastic scattering
  //
  RegisterPhysics( new G4IonElasticPhysics(verb));
  
  // Ion Inelastic physics
  RegisterPhysics( new G4IonPhysicsXS(verb));
  ////RegisterPhysics( new G4IonINCLXXPhysics(verb));
  
  // stopping Particles
  RegisterPhysics( new G4StoppingPhysics(verb));
      
  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("gamma"));
  ////RegisterPhysics( new GammaNuclearPhysicsLEND("gamma"));
    
  // EM physics
  ///RegisterPhysics(new ElectromagneticPhysics());
  RegisterPhysics(new G4EmStandardPhysics_option3());
  
  // Decay
  //RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  //RegisterPhysics(new RadioactiveDecayPhysics());
  ////RegisterPhysics(new G4RadioactiveDecayPhysics()); 

  RegisterPhysics(new G4OpticalPhysics()); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4CherenkovPhysicsList::~G4CherenkovPhysicsList()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4CherenkovPhysicsList::SetCuts()
{
  SetCutValue(0.5*mm, "proton");
  SetCutValue(0.5*mm, "e-");
  SetCutValue(0.5*mm, "e+");
  SetCutValue(0.5*mm, "gamma");      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
