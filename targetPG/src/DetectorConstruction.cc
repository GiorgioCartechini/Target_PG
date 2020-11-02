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
// Authors: Susanna Guatelli, susanna@uow.edu.au,
// Authors: Jeremy Davis, jad028@uowmail.edu.au
//

#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4AssemblyVolume.hh"
#include "G4Region.hh"
#include "G4Isotope.hh"

using namespace std;

DetectorConstruction::DetectorConstruction(AnalysisManager* analysis_manager):
logicTreatmentRoom(0), physicalTreatmentRoom(0), lTarget(0), pTarget(0)
{
  analysis = analysis_manager;
  TargetSize.setX(1000*um);
  TargetSize.setY(1000*um);
  TargetSize.setZ(100*um);

   Cu63 = MaterialWithSingleIsotope("Copper63", "Cu63", 10.78206*g/cm3, 29, 63);
   Sc45 = MaterialWithSingleIsotope("Scandium45", "Sc45", 7*g/cm3, 9, 19);
   Y89 = MaterialWithSingleIsotope("Yttrium89", "Y89", 15.233*g/cm3, 39, 89);
   Cr50 = MaterialWithSingleIsotope("Chromium52", "Cr50", 1*g/cm3, 24, 50);

   TargetMat = Cu63;

}

DetectorConstruction::~DetectorConstruction(){

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	//messenger to move the TEPC
	dMessenger = new DetectorConstructionMessenger(this);

  
    // Treatment room sizes
    const G4double worldX = 50 *cm;
    const G4double worldY = 50 *cm;
    const G4double worldZ = 50 *cm;
    G4bool isotopes = false;
    
    //G4Material* airNist =  G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR", isotopes);
    G4Material* vacuumNist =  G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic", isotopes);

    G4Material* worldMaterial = vacuumNist;
    
    G4Box* treatmentRoom = new G4Box("World",
                                     worldX,
                                     worldY,
                                     worldZ);
    
    logicTreatmentRoom = new G4LogicalVolume(treatmentRoom,
                                             worldMaterial,
                                             "World",
                                             0,0,0);
    
    physicalTreatmentRoom = new G4PVPlacement(0,
                                              G4ThreeVector(),
                                              "World",
                                              logicTreatmentRoom,
                                              0,
                                              false,
                                              0);
    
    
    // The treatment room is invisible in the Visualisation
    logicTreatmentRoom -> SetVisAttributes (G4VisAttributes::GetInvisible());
    

    sTarget = new G4Box("sTarget",
                                     TargetSize.getX()/2.,
                                     TargetSize.getY()/2.,
                                     TargetSize.getZ()/2.);
    
    lTarget = new G4LogicalVolume(sTarget,
                                             TargetMat,
                                             "lTarget",
                                             0,0,0);
    
    pTarget = new G4PVPlacement(0,
                                G4ThreeVector(TargetSize.getX()/2.,0.,0.),
                                              lTarget,
                                              "pTarget",
                                              logicTreatmentRoom,
                                              0,
                                              false,
                                              0);
    
    

        
return physicalTreatmentRoom; 

}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////// MESSENGER ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void DetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
	if (G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice, false) )
	{
		if (pttoMaterial)
		{
			TargetMat  = pttoMaterial;
			lTarget -> SetMaterial(pttoMaterial);
			G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
		}
	}
        else if(materialChoice == "F19")
        {
                TargetMat  = F19;
                lTarget -> SetMaterial(TargetMat);
                G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
        }
        else if(materialChoice == "Cu63")
        {
                TargetMat  = Cu63;
                lTarget -> SetMaterial(TargetMat);
                G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
        }
        else if(materialChoice == "Sc45")
        {
                TargetMat  = Sc45;
                lTarget -> SetMaterial(TargetMat);
                G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
        }
        else if(materialChoice == "Y89")
        {
                TargetMat  = Y89;
                lTarget -> SetMaterial(TargetMat);
                G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
        }
	else if(materialChoice == "Cr50")
        {
                TargetMat  = Cr50;
                lTarget -> SetMaterial(TargetMat);
                G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
        }

	else
	{
		G4cout << "WARNING: material \"" << materialChoice << "\" doesn't exist in NIST elements/materials"
			" table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
		G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
	}

}

G4Material* DetectorConstruction::MaterialWithSingleIsotope( G4String name,
                           G4String symbol, G4double density, G4int Z, G4int A)
{
 // define a material from an isotope
 //
 G4int ncomponents;
 G4double abundance, massfraction;

 G4Isotope* isotope = new G4Isotope(symbol, Z, A);

 G4Element* element  = new G4Element(name, symbol, ncomponents=1);
 element->AddIsotope(isotope, abundance= 100.*perCent);
 
 G4Material* material = new G4Material(name, density, ncomponents=1);
 material->AddElement(element, massfraction=100.*perCent);

 return material;
}

void DetectorConstruction::SetTargetSize(G4ThreeVector size)
{
  TargetSize = size;
  G4cout << "TARGET SIZE CHANGED:" << TargetSize.getX()/mm <<" "<<TargetSize.getY()/mm <<" " <<TargetSize.getZ()/mm <<G4endl;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

/*void DetectorConstruction::SetIsotopeFraction(G4double fraction)
{
    if (fraction < 0)
    	fFraction = 100;
    else
	{
        G4Material* 
       	TargetMat  = pttoMaterial;
        lTarget -> SetMaterial(pttoMaterial);
        G4cout << "The material of the Tumor has been changed to " << materialChoice << G4endl;
    	}
  else
    {
      G4cout << "WARNING: material \"" << materialChoice << "\" doesn't exist in NIST elements/materials"
            " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
    }

}
*/
