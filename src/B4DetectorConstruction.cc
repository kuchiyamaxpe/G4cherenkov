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
/// \file B4DetectorConstruction.cc
/// \brief Implementation of the B4DetectorConstruction class

#include "B4DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//#include "B4TrackerSD.hh"
#include "G4SDManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
    G4GlobalMagFieldMessenger *B4DetectorConstruction::fMagFieldMessenger = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4DetectorConstruction::B4DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fAbsorberPV(nullptr),
      fGapPV(nullptr),
      fDetectorPV(nullptr),
      fCheckOverlaps(true),
      fScoringVol(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4DetectorConstruction::~B4DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *B4DetectorConstruction::Construct()
{
    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4DetectorConstruction::DefineMaterials()
{
    // Lead material defined using NIST Manager
    auto nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_Pb");
    nistManager->FindOrBuildMaterial("G4_GLASS_LEAD"); //追加。
    // auto G4Si = G4Material::GetMaterial("Galactic");
    //  Liquid argon material
    G4double a; // mass of a mole;
    G4double z; // z=mean number of protons;
    G4double density;
    G4int nelements;
    G4int numberOfatoms;
    G4Element *Si = new G4Element("Sillicon", "Si", z = 14, a = 28.08550 * g / mole);
    G4Element *N = new G4Element("Nitrogen", "N", z = 7, a = 14.01 * g / mole);
    G4Element *O = new G4Element("Oxygen", "O", z = 8, a = 16.00 * g / mole);
    G4Element *B = new G4Element("Boron", "B", z = 5, a = 10.811 * g / mole);
    G4Element *Na = new G4Element("Sodium", "Na", z = 11, a = 22.98976928 * g / mole);

    new G4Material("liquidArgon", z = 18., a = 39.95 * g / mole, density = 1.390 * g / cm3);
    // The argon by NIST Manager is a gas with a different density
    G4Material *Sillica = new G4Material("Sillica", density = 2.196 * g / cm3, nelements = 2);
    Sillica->AddElement(Si, numberOfatoms = 1);
    Sillica->AddElement(O, numberOfatoms = 2);

    G4Material *BoricOxide = new G4Material("BoricOxide", density = 2.55 * g / cm3, nelements = 2);
    BoricOxide->AddElement(B, numberOfatoms = 2);
    BoricOxide->AddElement(O, numberOfatoms = 3);

    G4Material *SodiumOxide = new G4Material("SodiumOxide", density = 2.27 * g / cm3, nelements = 2);
    SodiumOxide->AddElement(Na, numberOfatoms = 2);
    SodiumOxide->AddElement(O, numberOfatoms = 1);

    G4Material *G4BK7 = new G4Material("G4BK7", density = 2.51 * g / cm3, nelements = 3);
    // G4Material *G4BK7 = new G4Material("G4BK7", density = 0.01 * g / cm3, nelements = 3);

    G4BK7->AddMaterial(Sillica, 80 * perCent);
    G4BK7->AddMaterial(BoricOxide, 4 * perCent);
    G4BK7->AddMaterial(SodiumOxide, 3 * perCent);

    G4Material *G4silicone = new G4Material("G4silicone", density = 2.2 * g / cm3, nelements = 1);
    G4silicone->AddMaterial(Sillica, 100 * perCent);

    // Vacuum
    new G4Material("IdealAbso", z = 1., a = 1.01 * g / mole, density = universe_mean_density,
                   kStateGas, 2.73 * kelvin, 3.e-18 * pascal);
    new G4Material("IdealAbso2", z = 1., a = 1.01 * g / mole, density = universe_mean_density,
                   kStateGas, 2.73 * kelvin, 3.e-18 * pascal);

    new G4Material("Galactic", z = 1., a = 1.01 * g / mole, density = universe_mean_density,
                   kStateGas, 2.73 * kelvin, 3.e-18 * pascal);

    new G4Material("G4Grease", z = 14., a = 28.08550 * g / mole, density = 2.2 * g / cm3);

    G4Material *air =
        new G4Material("Air", density = 1.29 * mg / cm3, nelements = 2);
    air->AddElement(N, 70. * perCent);
    air->AddElement(O, 30. * perCent);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *B4DetectorConstruction::DefineVolumes()
{
    // Geometry parameters
    G4int nofLayers = 1;
    G4int nofSiLayers = 10;
    G4double SidetecotrThickness = 500. * um;
    G4double SidetecotrSizeXY = 30. * mm;
    G4double absoThickness = 20. * mm;
    G4double gapThickness = 0 * um;
    G4double SigapThickness = 500. * um;
    G4double calorSizeXY = 49. * mm;
    G4double detectorThickness = 0.0 * mm;

    auto layerThickness = absoThickness + gapThickness + detectorThickness;
    auto SilayerThickness = SidetecotrThickness + SigapThickness;

    auto calorThickness = nofLayers * layerThickness;
    auto lowerenergydetectorThickness = nofSiLayers * SilayerThickness;
    auto worldSizeXY = 1. * calorSizeXY;
    auto worldSizeZ = 1. * calorThickness + 1. * lowerenergydetectorThickness;

    // Get materials
    auto defaultMaterial = G4Material::GetMaterial("IdealAbso");
    auto cherenkovglassMaterial = G4Material::GetMaterial("G4BK7");
    // auto absorberMaterial = G4Material::GetMaterial("G4_GLASS_LEAD");
    auto gapMaterial = G4Material::GetMaterial("Galactic");
    auto detectorMaterial = G4Material::GetMaterial("IdealAbso2");
    auto opticalgrease = G4Material::GetMaterial("G4Grease");
    auto MPPCwindow = G4Material::GetMaterial("G4silicone");

    //
    // ------------ Generate & Add Material Properties Table ------------
    //

    const G4int nEntries = 2;
    G4double PhotonEnergy[nEntries] =
        {1.0 * eV, 4.0 * eV};
    G4double RefractiveIndex0[nEntries] =
        {1.0, 1.0};

    G4double RefractiveIndex1[nEntries] =
        {1.46, 1.46};

    G4double RefractiveIndex2[nEntries] =
        {1.41, 1.41};

    const G4int nEntries_BK7 = 16;
    const G4int nEntries_SK2 = 15;
    const G4int nEntries_MgF2 = 5;

    G4double Lambda_BK7[nEntries_BK7] =
        {1014.0, 852.1, 706.5, 656.3, 643.8, 632.8, 589.3, 587.6, 546.1, 486.1, 480.0, 435.8, 404.7, 365.0, 334.1, 312.6};

    G4double Lambda_SK2[nEntries_SK2] =
        {1014.0, 852.1, 706.5, 656.3, 643.8, 632.8, 589.3, 587.6, 546.1, 486.1, 480.0, 435.8, 404.7, 365.0, 334.1};

    G4double Lambda_MgF2[nEntries_MgF2] =
        {0.56e3, 0.34e3, 0.27e3, 0.23e3, 0.20e3};

    G4double n_BK7[nEntries_BK7] =
        {1.50731, 1.50980, 1.51289, 1.51432, 1.51472, 1.51509, 1.51673, 1.51680, 1.51872, 1.52238, 1.52283, 1.52668, 1.53024, 1.53627, 1.54272, 1.54862};
    G4double n_SK2[nEntries_SK2] =
        {1.59558, 1.59847, 1.60230, 1.60414, 1.60465, 1.60513, 1.60729, 1.60738, 1.60994, 1.61486, 1.61547, 1.62073, 1.62562, 1.63398, 1.64304};
    G4double n_MgF2[nEntries_MgF2] =
        {1.385, 1.395, 1.405, 1.415, 1.425};

    int i;
    G4double PhotonEnergy_BK7[nEntries_BK7];
    G4double PhotonEnergy_SK2[nEntries_SK2];
    G4double PhotonEnergy_MgF2[nEntries_MgF2];
    for (i = 0; i < nEntries_BK7; i++)
    {
        PhotonEnergy_BK7[i] = 1240 / Lambda_BK7[i] * eV;
    }

    for (i = 0; i < nEntries_SK2; i++)
    {
        PhotonEnergy_SK2[i] = 1240 / Lambda_SK2[i] * eV;
    }

    for (i = 0; i < nEntries_MgF2; i++)
    {
        PhotonEnergy_MgF2[i] = 1240 / Lambda_MgF2[i] * eV;
    }

    /*
    const G4int nEntries = 32;

    G4double PhotonEnergy[nEntries] =
        {2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV,
         2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV,
         2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
         2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV,
         2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV,
         3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
         3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV,
         3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV};
    //
    // Water
    //
    G4double RefractiveIndex1[nEntries] =
        {1.3435, 1.344, 1.3445, 1.345, 1.3455,
         1.346, 1.3465, 1.347, 1.3475, 1.348,
         1.3485, 1.3492, 1.35, 1.3505, 1.351,
         1.3518, 1.3522, 1.3530, 1.3535, 1.354,
         1.3545, 1.355, 1.3555, 1.356, 1.3568,
         1.3572, 1.358, 1.3585, 1.359, 1.3595,
         1.36, 1.3608};

    G4double Absorption1[nEntries] =
        {3.448 * m, 4.082 * m, 6.329 * m, 9.174 * m, 12.346 * m, 13.889 * m,
         15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
         45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
         52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
         30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
         17.500 * m, 14.500 * m};

    G4double ScintilFast[nEntries] =
        {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
         1.00, 1.00, 1.00, 1.00};
    G4double ScintilSlow[nEntries] =
        {0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
         7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
         3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
         4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
         7.00, 6.00, 5.00, 4.00};




    */
    G4MaterialPropertiesTable *vacuumeMPT = new G4MaterialPropertiesTable();
    vacuumeMPT->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex0, nEntries);

    G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
    // myMPT1->AddProperty("RINDEX", PhotonEnergy_BK7, n_BK7, nEntries_BK7);
    // myMPT1->AddProperty("RINDEX", PhotonEnergy_SK2, n_SK2, nEntries_SK2);
    myMPT1->AddProperty("RINDEX", PhotonEnergy_MgF2, n_MgF2, nEntries_MgF2);

    G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
    myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex1, nEntries);
    // myMPT2->AddProperty("RINDEX", PhotonEnergy_MgF2, n_MgF2, nEntries_MgF2);
    // myMPT2->AddProperty("RINDEX", PhotonEnergy_BK7, n_BK7, nEntries_BK7);

    /*
    myMPT1->AddProperty("ABSLENGTH", PhotonEnergy, Absorption1, nEntries);
    myMPT1->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintilFast, nEntries);
    myMPT1->AddProperty("SLOWCOMPONENT", PhotonEnergy, ScintilSlow, nEntries);

    myMPT1->AddConstProperty("SCINTILLATIONYIELD", 0. / MeV);
    myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
    myMPT1->AddConstProperty("FASTTIMECONSTANT", 1. * ns);
    myMPT1->AddConstProperty("SLOWTIMECONSTANT", 10. * ns);
    myMPT1->AddConstProperty("YIELDRATIO", 0.8);
*/
    G4MaterialPropertiesTable *myMPT3 = new G4MaterialPropertiesTable();
    myMPT3->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);

    // defaultMaterial->SetMaterialPropertiesTable(myMPT1);
    defaultMaterial->SetMaterialPropertiesTable(vacuumeMPT);
    cherenkovglassMaterial->SetMaterialPropertiesTable(myMPT1);
    // gapMaterial->SetMaterialPropertiesTable(vacuumeMPT);
    //  detectorMaterial->SetMaterialPropertiesTable(myMPT3);
    opticalgrease->SetMaterialPropertiesTable(myMPT2);
    MPPCwindow->SetMaterialPropertiesTable(myMPT3);

    if (!defaultMaterial || !cherenkovglassMaterial || !gapMaterial)
    {
        G4ExceptionDescription msg;
        msg << "Cannot retrieve materials already defined.";
        G4Exception("B4DetectorConstruction::DefineVolumes()",
                    "MyCode0001", FatalException, msg);
    }

    //
    // World
    //
    auto worldS = new G4Box("World",                                           // its name
                            worldSizeXY / 2, worldSizeXY / 2, worldSizeZ / 2); // its size

    auto worldLV = new G4LogicalVolume(
        worldS,          // its solid
        defaultMaterial, // its material
        "World");        // its name

    auto worldPV = new G4PVPlacement(
        0,               // no rotation
        G4ThreeVector(), // at (0,0,0)
        worldLV,         // its logical volume
        "World",         // its name
        0,               // its mother  volume
        false,           // no boolean operation
        0,               // copy number
        fCheckOverlaps); // checking overlaps

    //
    // LowerEnergyDetector
    //

    auto SiLEDunit = new G4Box("SiLEDunit",                                                                   // its name
                               SidetecotrSizeXY / 2, SidetecotrSizeXY / 2, lowerenergydetectorThickness / 2); // its size

    auto SiLEDunitLV = new G4LogicalVolume(
        SiLEDunit,       // its solid
        defaultMaterial, // its material
        "SiLEDunitLV");  // its name

    new G4PVPlacement(
        0,                                        // no rotation
        G4ThreeVector(0, 0, -calorThickness / 2), // at (0,0,0)
        SiLEDunitLV,                              // its logical volume
        "LowerEnergyDetector",                    // its name
        worldLV,                                  // its mother  volume
        false,                                    // no boolean operation
        0,                                        // copy number
        fCheckOverlaps);                          // checking overlaps

    //
    // SiLEDLayer
    //

    auto layerS = new G4Box("Layer",                                                           // its name
                            SidetecotrSizeXY / 2, SidetecotrSizeXY / 2, SilayerThickness / 2); // its size

    auto layerLV = new G4LogicalVolume(
        layerS,          // its solid
        defaultMaterial, // its material
        "Layer");        // its name

    new G4PVReplica(
        "Layer",           // its name
        layerLV,           // its logical volume
        SiLEDunitLV,       // its mother
        kZAxis,            // axis of replication
        nofSiLayers,       // number of replica
        SilayerThickness); // witdth of replica

    //
    // Si detecotr
    //
    auto SidetecotrS = new G4Box("Sidetector", SidetecotrSizeXY / 2, SidetecotrSizeXY / 2, SidetecotrThickness / 2);

    auto SidetectorLV = new G4LogicalVolume(
        SidetecotrS,   // its solid
        MPPCwindow,    // its material
        "Sidetector"); // its name

    new G4PVPlacement(
        0,                                          // no rotation
        G4ThreeVector(0., 0., -SigapThickness / 2), // its position
        SidetectorLV,                               // its logical volume
        "Sidetector",                               // its name
        layerLV,                                    // its mother  volume
        false,                                      // no boolean operation
        0,                                          // copy number
        fCheckOverlaps);                            // checking overlaps
    //
    // Si detecotr gap
    //
    auto SigapS = new G4Box("Sigap", SidetecotrSizeXY / 2, SidetecotrSizeXY / 2, SigapThickness / 2);

    auto SigapLV = new G4LogicalVolume(
        SigapS,          // its solid
        defaultMaterial, // its material
        "Sidetector");   // its name

    new G4PVPlacement(
        0,                                              // no rotation
        G4ThreeVector(0., 0., SidetecotrThickness / 2), // its position
        SigapLV,                                        // its logical volume
        "Sidetector",                                   // its name
        layerLV,                                        // its mother  volume
        false,                                          // no boolean operation
        0,                                              // copy number
        fCheckOverlaps);                                // checking overlaps

    //
    // cherenkovglass
    //
    auto cherenkovglassS = new G4Box("cherenkovglassS",                                    // its name
                                     calorSizeXY / 2, calorSizeXY / 2, absoThickness / 2); // its size

    auto cherenkovglassLV = new G4LogicalVolume(
        cherenkovglassS,        // its solid
        cherenkovglassMaterial, // its material
        "cherenkovglassLV");    // its name

    fAbsorberPV = new G4PVPlacement(
        0,                                                                                                 // no rotation
        G4ThreeVector(0., 0., -(gapThickness + detectorThickness) / 2 + lowerenergydetectorThickness / 2), // its position
        cherenkovglassLV,                                                                                  // its logical volume
        "Abso",                                                                                            // its name
        worldLV,                                                                                           // its mother  volume
        false,                                                                                             // no boolean operation
        0,                                                                                                 // copy number
        fCheckOverlaps);                                                                                   // checking overlaps

    //
    // Gap
    //
    /*
    auto gapS = new G4Box("Gap",                                               // its name
                          calorSizeXY / 2, calorSizeXY / 2, gapThickness / 2); // its size

    auto gapLV = new G4LogicalVolume(
        gapS,          // its solid
        opticalgrease, // its material
        "Gap");        // its name

    fGapPV = new G4PVPlacement(
        0,                                                                 // no rotation
        G4ThreeVector(0., 0., -detectorThickness / 2 + absoThickness / 2), // its position
        gapLV,                                                             // its logical volume
        "Gap",                                                             // its name
        worldLV,                                                           // its mother  volume
        false,                                                             // no boolean operation
        0,                                                                 // copy number
        fCheckOverlaps);                                                   // checking overlaps

    //
    // Detector
    //

    auto DetectorS = new G4Box("DetectorS",                                              // its name
                               calorSizeXY / 2, calorSizeXY / 2, detectorThickness / 2); // its size

    auto DetectorLV = new G4LogicalVolume(
        DetectorS,     // its solid
        MPPCwindow,    // its material
        "DetectorLV"); // its name

    fDetectorPV = new G4PVPlacement(
        0,                                                           // no rotation
        G4ThreeVector(0., 0., absoThickness / 2 + gapThickness / 2), // its position
        DetectorLV,                                                  // its logical volume
        "DetectorPV",                                                // its name
        worldLV,                                                     // its mother  volume
        false,                                                       // no boolean operation
        0,                                                           // copy number
        fCheckOverlaps);                                             // checking overlaps
*/
    fScoringVol = cherenkovglassLV;
    //
    // print parameters
    //
    /*
    G4cout
        << G4endl
        << "------------------------------------------------------------" << G4endl
        << "---> The calorimeter is " << nofLayers << " layers of: [ "
        << absoThickness / mm << "mm of " << absorberMaterial->GetName()
        << " + "
        << gapThickness / mm << "mm of " << gapMaterial->GetName() << " ] " << G4endl
        << "------------------------------------------------------------" << G4endl;
*/
    //
    // Visualization attributes
    //
    // worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    auto simpleBoxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    simpleBoxVisAtt->SetVisibility(true);
    // calorLV->SetVisAttributes(simpleBoxVisAtt);

    //
    // Always return the physical World
    //
    return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4DetectorConstruction::ConstructSDandField()
{ /*
    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4String SDname = "MPPCSD";
    B4TrackerSD *aTrackerSD = new B4TrackerSD(SDname, "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
    SetSensitiveDetector("DetectorLV", aTrackerSD, true);

    G4ThreeVector fieldValue;
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);

    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
