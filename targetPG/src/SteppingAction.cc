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

#include "G4ios.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4HadronicProcess.hh"
#include "G4Isotope.hh"
#include "G4VProcess.hh"
#include "SteppingAction.hh"
#include "AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(AnalysisManager* pAnalysis)
{ 
analysis = pAnalysis;
fSecondary = 0; 
}

SteppingAction::~SteppingAction()
{ 
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  const G4StepPoint* endPoint = step->GetPostStepPoint();
  G4StepPoint* prestep = step->GetPreStepPoint();
  G4VProcess* process   = 
                   const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());
  if(endPoint->GetProcessDefinedStep() -> GetProcessName() == "protonInelastic")
  {
    G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();
    G4String partName = particle->GetParticleName();
    G4String nuclearChannel = partName;
    G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
    const G4Isotope* target = NULL;
    if (hproc) target = hproc->GetTargetIsotope();
    G4String targetName = "XXXX";  
    if (target) targetName = target->GetName();
    nuclearChannel += " + " + targetName + " --> ";

    const std::vector<const G4Track*>* secondary 
                                    = step->GetSecondaryInCurrentStep();
    G4cout << step -> GetStepLength()/um<< "\t";
    G4cout <<prestep->GetKineticEnergy()/MeV << partName << "*" << endPoint->GetKineticEnergy()/MeV << " + " + targetName + " --> ";
    G4int countGamma = 0;
    for (size_t lp=0; lp<(*secondary).size(); lp++) {
      particle = (*secondary)[lp]->GetDefinition(); 
      G4String name   = particle->GetParticleName();
      G4String type   = particle->GetParticleType();      
      G4double energy = (*secondary)[lp]->GetKineticEnergy()/MeV;
      G4cout << name<<"*" << energy << " + ";
      nuclearChannel += " + " + name;
      if(name == "gamma") countGamma++;
    }

    G4cout << G4endl<< countGamma<<G4endl;
    //G4cout <<nuclearChannel << G4endl;
  } 
}
