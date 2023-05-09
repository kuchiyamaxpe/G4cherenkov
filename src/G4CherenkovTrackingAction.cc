//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B4aSteppingAction.cc
/// \brief Implementation of the B4aSteppingAction class

#include "G4CherenkovTrackingAction.hh"
#include "B4aEventAction.hh"
#include "B4DetectorConstruction.hh"
#include "B4RunAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4UserTrackingAction.hh"
#include "G4UnitsTable.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4CherenkovTrackingAction::G4CherenkovTrackingAction()
    : G4UserTrackingAction(),
      fScoringVol(0)
{
  auto runmanager = G4RunManager::GetRunManager();
  auto runaction = const_cast<G4UserRunAction *>(runmanager->GetUserRunAction());
  runaction_u = dynamic_cast<B4RunAction *>(runaction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4CherenkovTrackingAction::~G4CherenkovTrackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4CherenkovTrackingAction::PreUserTrackingAction(const G4Track *track)
{

  G4ThreeVector momentum = track->GetMomentum();
  auto tlength = track->GetTrackLength();
  G4double kinEnergy = track->GetKineticEnergy();
  G4String ParticleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

  G4String ProcessName = "Generator";
  G4String MasterProcessName = "Generator";
  G4int trackid = track->GetTrackID();

  if (track->GetCreatorProcess())
  {
    ProcessName = track->GetCreatorProcess()->GetProcessName();
    MasterProcessName = track->GetCreatorProcess()->GetMasterProcess()->GetProcessName();

    // ProcessName = track->GetCreatorProcess()->GetMasterProcess()->GetProcessName();
    // G4cout << "TrackID: " << trackid << " Master Process: " << track->GetCreatorProcess()->GetMasterProcess() << " ProcessName: " << ProcessName << " ParticleName: "<<ParticleName << G4endl;
  }

  G4String Volname = track->GetTouchable()->GetVolume()->GetLogicalVolume()->GetName();
  G4String postVolName = "END";
  if (track->GetNextVolume())
  {
    postVolName = track->GetNextTouchable()->GetVolume()->GetLogicalVolume()->GetName();
  }

  ///............Writing Track Infomation...................
  auto filepath = runaction_u->GetTrackInfoDirName() + "/" + runaction_u->GetFileName();
  std::ofstream writing_file;
  writing_file.open(filepath, std::ios::app);
  // if step is within the scoring volume
  writing_file << Volname << " " << postVolName << " " << trackid << " " << MasterProcessName << " " << ProcessName << " " << ParticleName << " " << kinEnergy / MeV << std::endl;
}
