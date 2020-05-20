#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM; 
using namespace PhysiCell;

#include "./submodel_data_structures.h" 

#ifndef __immune_submodels__
#define __immune_submodels__

/*
void immune_submodel_setup( void );
void immune_submodel_model( Cell* pCell, Phenotype& phenotype, double dt );
*/

void CD8_Tcell_phenotype( Cell* pCell, Phenotype& phenotype, double dt ); 
void CD8_Tcell_mechanics( Cell* pCell, Phenotype& phenotype, double dt ); 

void macrophage_phenotype( Cell* pCell, Phenotype& phenotype, double dt ); 
void macrophage_mechanics( Cell* pCell, Phenotype& phenotype, double dt ); 

void neutrophil_phenotype( Cell* pCell, Phenotype& phenotype, double dt ); 
void neutrophil_mechanics( Cell* pCell, Phenotype& phenotype, double dt ); 

void immune_submodels_setup( void ); 

// this needs to be done on faster time scale; 
// void receptor_dynamics_model( double dt ); 

#endif 