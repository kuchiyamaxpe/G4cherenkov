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

#include "B4aSteppingAction.hh"
#include "B4aEventAction.hh"
#include "B4DetectorConstruction.hh"
#include "B4RunAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4aSteppingAction::B4aSteppingAction(
    const B4DetectorConstruction *detectorConstruction)
    : G4UserSteppingAction(),
      fDetConstruction(detectorConstruction),

      fScoringVol(0)
{
  auto runmanager = G4RunManager::GetRunManager();
  auto runaction = const_cast<G4UserRunAction *>(runmanager->GetUserRunAction());
  runaction_u = dynamic_cast<B4RunAction *>(runaction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4aSteppingAction::~B4aSteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4aSteppingAction::UserSteppingAction(const G4Step *step)
{

  if (!fScoringVol)
  {
    const B4DetectorConstruction *geometry = static_cast<const B4DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVol = geometry->GetScoringVol();
  }
  G4StepPoint *preStepPoint = step->GetPreStepPoint();
  G4StepPoint *postStepPoint = step->GetPostStepPoint();

  G4TouchableHandle preTouchable = preStepPoint->GetTouchableHandle();

  G4LogicalVolume *preVolume = preTouchable->GetVolume()->GetLogicalVolume();

  G4String preVolName = preVolume->GetName();

  G4ThreeVector position_World = preStepPoint->GetPosition();
  G4ThreeVector postposition = postStepPoint->GetPosition();

  G4Track *track = step->GetTrack();

  G4String postVolName = "END";
  if (track->GetNextVolume())
  {
    postVolName = track->GetNextTouchable()->GetVolume()->GetLogicalVolume()->GetName();
  }

  G4ThreeVector momentum = track->GetMomentum();
  G4double kinEnergy = track->GetKineticEnergy();
  G4String ParticleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

  /*
  G4String ProcessName = "None";
  if (track->GetCreatorProcess())
  {
    G4int trackid = track->GetTrackID();
    //ProcessName = track->GetCreatorProcess()->GetProcessName();
    ProcessName = track->GetCreatorProcess()->GetMasterProcess()->GetProcessName();
    G4cout << "TrackID: " << trackid << " Master Process: " << track->GetCreatorProcess()->GetMasterProcess() << G4endl;
  }
  G4cout << "ProcessName: " << ProcessName << " VolumeNames: " << preVolName << preVolume << " -> " << postVolName << " "
         << "ParticleName: " << ParticleName << " Position: " << position_World.x() << " -> " << postposition.x() << " " << position_World.y() << " -> " << postposition.y() << " " << position_World.z() << " -> " << postposition.z() << G4endl;
*/

  auto filepath = runaction_u->GetDirName() + "/" + runaction_u->GetFileName();
  std::ofstream writing_file;
  writing_file.open(filepath, std::ios::app);
  //if step is within the scoring volume
  fScoringVol = fDetConstruction->GetScoringVol();
  if (preVolume == fScoringVol && ParticleName == "opticalphoton")
  {
    //G4cout << "WRITING TO FILE... " << postposition.x() << " " << postposition.y() << " " << postposition.z() << " " << 1240 / kinEnergy * eV << G4endl;

    writing_file << postposition.x() << " " << postposition.y() << " " << postposition.z() << " " << 1240 / kinEnergy * eV << std::endl;
  }
  else
  {
  }
}
// Collect energy and track length step by step
// get volume of the current step
/*
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();

  // step length
  G4double stepLength = 0.;
  if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
  {
    stepLength = step->GetStepLength();
  }

  if (volume == fDetConstruction->GetAbsorberPV())
  {
    fEventAction->AddAbs(edep, stepLength);
  }

  if (volume == fDetConstruction->GetGapPV())
  {
    fEventAction->AddGap(edep, stepLength);
  }
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
