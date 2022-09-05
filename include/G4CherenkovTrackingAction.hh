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
/// \file B4aSteppingAction.hh
/// \brief Definition of the B4aSteppingAction class

#ifndef G4CherenkovTrackingAction_h
#define G4CherenkovTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "B4RunAction.hh"
class B4DetectorConstruction;
class B4aEventAction;
class G4LogicalVolume;
/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in B4aEventAction.

class G4CherenkovTrackingAction : public G4UserTrackingAction
{
public:
  G4CherenkovTrackingAction();
  virtual ~G4CherenkovTrackingAction();

  virtual void PreUserTrackingAction(const G4Track *track);

  B4RunAction *runaction_u;

private:
  const B4DetectorConstruction *fDetConstruction;
  G4LogicalVolume *fScoringVol;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
