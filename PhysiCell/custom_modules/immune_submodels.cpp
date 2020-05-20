#include "./immune_submodels.h" 

using namespace PhysiCell; 

std::string immune_submodels_version = "0.0.1"; 

// Submodel_Information Immune_submodels_info; // not needed for now 

Submodel_Information CD8_submodel_info; 
Submodel_Information Macrophage_submodel_info; 
Submodel_Information Neutrophil_submodel_info; 

void CD8_Tcell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	
	
	
	
	
	
	return; 
}

void CD8_Tcell_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	
	return; 
}

void macrophage_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	return; 
}

void macrophage_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	return; 
}

void neutrophil_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	return; 
}

void neutrophil_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	return; 
}


/*
void immune_submodel_setup( void )
{
	immune_submodel_info.name = "immune submodel"; 
	immune_submodel_info.version = "0.0.1";
	immune_submodel_info.main_function= NULL; // receptor_dynamics_model; 
	
	// what variables and parameters do you need? 
	
	immune_submodel_info.cell_variables.push_back( "unbound external ACE2" ); 
	immune_submodel_info.cell_variables.push_back( "bound external ACE2" ); 
	immune_submodel_info.cell_variables.push_back( "unbound internal ACE2" ); 
	immune_submodel_info.cell_variables.push_back( "bound internal ACE2" ); 
	
	immune_submodel_info.cell_variables.push_back( "ACE2 binding rate" ); 
	immune_submodel_info.cell_variables.push_back( "ACE2 endocytosis rate" ); 
	immune_submodel_info.cell_variables.push_back( "ACE2 cargo release rate" ); 	
	immune_submodel_info.cell_variables.push_back( "ACE2 recycling rate" ); 
	
	// submodel_registry.register_model( immune_submodel_info ); 
	immune_submodel_info.register_model(); 
	
	return; 
}
*/
/*
extern Cell_Definition lung_epithelium; 
*/

/*
void receptor_dynamics_model( Cell* pCell, Phenotype& phenotype, double dt )
{
	// bookkeeping -- find microenvironment variables we need

	static int nV_external = microenvironment.find_density_index( "virion" ); 
	static int nA_external = microenvironment.find_density_index( "assembled virion" ); 
	
	static int nV_internal = pCell->custom_data.find_variable_index( "virion" ); 
	static int nA_internal = pCell->custom_data.find_variable_index( "assembled virion" ); 

	// bookkeeping -- find custom data we need 
	
	static int nR_EU = pCell->custom_data.find_variable_index( "unbound external ACE2" ); 
	static int nR_EB = pCell->custom_data.find_variable_index( "bound external ACE2" ); 
	static int nR_IU = pCell->custom_data.find_variable_index( "unbound internal ACE2" ); 
	static int nR_IB = pCell->custom_data.find_variable_index( "bound internal ACE2" ); 
	
	static int nR_bind = pCell->custom_data.find_variable_index( "ACE2 binding rate" ); 
	static int nR_endo = pCell->custom_data.find_variable_index( "ACE2 endocytosis rate" ); 
	static int nR_release = pCell->custom_data.find_variable_index( "ACE2 cargo release rate" ); 	
	static int nR_recycle = pCell->custom_data.find_variable_index( "ACE2 recycling rate" ); 
	
	// do nothing if dead 
	if( phenotype.death.dead == true )
	{ return; } 

	// if not lung epithelium, do nothing 
	if( pCell->type != lung_epithelium.type )
	{ return; } 
	
	// actual model goes here 
	
	// internalized virus tells us how many have recently bound to receptors 
	double newly_bound = phenotype.molecular.internalized_total_substrates[nV_external]; 
	// if it tried to bind to more virus than there are receptors, compensate 
	double excess_binding = newly_bound - pCell->custom_data[nR_EU]; 
	if( excess_binding > 0.0 )
	{
		// don't bring in more virus than there are receptors 
		newly_bound = pCell->custom_data[nR_EU]; 
		// dump any excess back into the microenvironment
		static double one_virion_to_density = 1.0 / microenvironment.mesh.dV; 
		// this needs omp critical because 2 cells writing to 1 voxel is not thread safe 
		#pragma omp critical 
		{
			pCell->nearest_density_vector()[nV_external] += excess_binding * one_virion_to_density; 
		}
	}
	phenotype.molecular.internalized_total_substrates[nV_external] = 0.0; 
	
	// add newly bound receptor to R_EB
	
	pCell->custom_data[nR_EB] += newly_bound; 
	
	// remove newly bound receptor from R_EU 

	pCell->custom_data[nR_EU] -= newly_bound; 
	
	// endocytosis 
	
	double dR_IB = dt*pCell->custom_data[nR_endo]*pCell->custom_data[nR_EB];
	if( dR_IB > pCell->custom_data[nR_EB] )
	{ dR_IB = pCell->custom_data[nR_EB]; }
	pCell->custom_data[nR_EB] -= dR_IB; // move from external bound
	pCell->custom_data[nR_IB] += dR_IB; // move to internal bound
	
	// viral release from endosomes 
	
	double dR_IU = dt*pCell->custom_data[nR_release]*pCell->custom_data[nR_IB];
	if( dR_IU > pCell->custom_data[nR_IB] )
	{ dR_IU = pCell->custom_data[nR_IB]; }
	pCell->custom_data[nR_IB] -= dR_IU; // move from internal bound 
	pCell->custom_data[nR_IU] += dR_IU; // move to internal unbound 
	pCell->custom_data[nV_internal] += dR_IU; // release virus into cytoplasm 
	
	// receptor recycling 
	
	double dR_EU = dt*pCell->custom_data[nR_recycle]*pCell->custom_data[nR_IU];
	if( dR_EU > pCell->custom_data[nR_IU] )
	{ dR_EU = pCell->custom_data[nR_IU]; }
	pCell->custom_data[nR_IU] -= dR_EU; // move from internal unbound 
	pCell->custom_data[nR_EU] += dR_EU; // move to external unbound 
	
	// update the virion uptake rate 
	
	phenotype.secretion.uptake_rates[nV_external] = 
		pCell->custom_data[nR_bind] * pCell->custom_data[nR_EU]; 
	
	return; 
}
*/

/*
void receptor_dynamics_model( double dt )
{
	#pragma omp parallel for 
	for( int n=0; n < (*all_cells).size() ; n++ )
	{
		Cell* pC = (*all_cells)[n]; 
		receptor_dynamics_model( pC, pC->phenotype , dt ); 	
	}
	
	return; 
}
*/


void immune_submodels_setup( void )
{
	Cell_Definition* pCD;
	
	// 
	// set up CD8 Tcells
		// set version info 
	CD8_submodel_info.name = "CD8 Tcell model"; 
	CD8_submodel_info.version = immune_submodels_version; 
		// set functions 
	CD8_submodel_info.main_function = NULL; 
	CD8_submodel_info.phenotype_function = CD8_Tcell_phenotype; 
	CD8_submodel_info.mechanics_function = CD8_Tcell_mechanics; 
		// what microenvironment variables do you expect? 
	CD8_submodel_info.microenvironment_variables.push_back( "virion" ); 
	CD8_submodel_info.microenvironment_variables.push_back( "interferon 1" ); 
	CD8_submodel_info.microenvironment_variables.push_back( "pro-inflammatory cytokine" ); 
	CD8_submodel_info.microenvironment_variables.push_back( "chemokine" ); 
		// what custom data do I need? 
	//CD8_submodel_info.cell_variables.push_back( "something" ); 
		// register the submodel  
	CD8_submodel_info.register_model();	
		// set functions for the corresponding cell definition 
	pCD = find_cell_definition( "CD8 Tcell" ); 
	pCD->functions.update_phenotype = CD8_submodel_info.phenotype_function;
	pCD->functions.custom_cell_rule = CD8_submodel_info.mechanics_function;
	
	// set up macrophages
	Macrophage_submodel_info = CD8_submodel_info; // much shared information 
		// set version info 
	Macrophage_submodel_info.name = "macrophage model"; 
	Macrophage_submodel_info.version = immune_submodels_version; 
		// set functions 
	Macrophage_submodel_info.main_function = NULL; 
	Macrophage_submodel_info.phenotype_function = macrophage_phenotype; 
	Macrophage_submodel_info.mechanics_function = macrophage_mechanics; 
		// what microenvironment variables do you expect? 
	// nothing unique 
		// what custom data do I need? 
	//CD8_submodel_info.cell_variables.push_back( "something" ); 
		// register the submodel  
	Macrophage_submodel_info.register_model();	
		// set functions for the corresponding cell definition 
	pCD = find_cell_definition( "macrophage" ); 
	pCD->functions.update_phenotype = Macrophage_submodel_info.phenotype_function;
	pCD->functions.custom_cell_rule = Macrophage_submodel_info.mechanics_function;
	
	// set up neutrophils 
	// set up macrophages
	Neutrophil_submodel_info = CD8_submodel_info; // much shared information 
		// set version info 
	Neutrophil_submodel_info.name = "neutrophil model"; 
	Neutrophil_submodel_info.version = immune_submodels_version; 
		// set functions 
	Neutrophil_submodel_info.main_function = NULL; 
	Neutrophil_submodel_info.phenotype_function = neutrophil_phenotype; 
	Neutrophil_submodel_info.mechanics_function = neutrophil_mechanics; 
		// what microenvironment variables do you expect? 
	// nothing unique 
		// what custom data do I need? 
	//CD8_submodel_info.cell_variables.push_back( "something" ); 
		// register the submodel  
	Neutrophil_submodel_info.register_model();	
		// set functions for the corresponding cell definition 
	pCD = find_cell_definition( "neutrophil" ); 
	pCD->functions.update_phenotype = Neutrophil_submodel_info.phenotype_function;
	pCD->functions.custom_cell_rule = Neutrophil_submodel_info.mechanics_function;	
	
}
