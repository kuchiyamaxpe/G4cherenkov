#ifndef G4SidetectorSD_h
#define G4SidetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4SidetectorHit.hh"
#include "B4RunAction.hh"
class G4Step;
class G4HCofThisEvent;
class G4SidetectorSD : public G4VSensitiveDetector
{
public:
  G4SidetectorSD(const G4String &name,
                 const G4String &hitsCollectionName,
                 G4int nofCells);
  virtual ~G4SidetectorSD();

  // methods from base class
  virtual void Initialize(G4HCofThisEvent *hitCollection);
  virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
  virtual void EndOfEvent(G4HCofThisEvent *hitCollection);
  B4RunAction *runaction_u;

private:
  G4SidetectorHitsCollection *fHitsCollection;
  G4int fNofCells;
};

#endif
