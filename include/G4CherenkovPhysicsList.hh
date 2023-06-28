#ifndef G4CherenkovPhysicsList_h
#define G4CherenkovPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class G4CherenkovPhysicsList: public G4VModularPhysicsList
{
private:
    /* data */
public:
    G4CherenkovPhysicsList(/* args */);
    virtual ~G4CherenkovPhysicsList();
    void ConstructSpecialParticle();
    virtual void SetCuts();

};


#endif
