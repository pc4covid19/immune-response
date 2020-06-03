#include "./immune_submodels.h" 

using namespace PhysiCell; 

std::string immune_submodels_version = "0.0.1"; 
// Submodel_Information Immune_submodels_info; // not needed for now 

Submodel_Information CD8_submodel_info; 
Submodel_Information Macrophage_submodel_info; 
Submodel_Information Neutrophil_submodel_info; 


std::vector<int> vascularized_voxel_indices;

//extern std::vector<int> vascularized_voxel_indices;
void choose_initialized_voxels( void )
{
	// read in percentage of tissue that's vascularised
	double percentage_vascularised = parameters.doubles("perecentage_tissue_vascularized");
	int max_voxel_index = microenvironment.mesh.voxels.size() - 1; 
	int number_of_vascularized_voxels = (int) ( percentage_vascularised/100 * ( max_voxel_index+1) ); 

	 // choose which voxels are veins
	 for( int n = 0 ; n < number_of_vascularized_voxels ; n++ )
	 {
		int index_vascularised_voxel = (int) ( UniformRandom() * max_voxel_index ); 
		vascularized_voxel_indices.push_back( index_vascularised_voxel ); 
	 }
	 
	 return;
}
void create_infiltrating_immune_cell( Cell_Definition* pCD )
{
	
	Cell* pC = create_cell( *pCD ); 
	
	std::vector<double> position = choose_vascularized_position();
	//pCell->set_position( position )

	pC->assign_position( position );
	
	return; 
}

void create_infiltrating_immune_cell_initial( Cell_Definition* pCD )
{
	
	Cell* pC = create_cell( *pCD ); 
	
	// randomly place macrophages intially
	double Xmin = microenvironment.mesh.bounding_box[0]; 
	double Ymin = microenvironment.mesh.bounding_box[1]; 
	double Zmin = microenvironment.mesh.bounding_box[2]; 

	double Xmax = microenvironment.mesh.bounding_box[3]; 
	double Ymax = microenvironment.mesh.bounding_box[4]; 
	double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	double Xrange = (Xmax - Xmin); 
	double Yrange = (Ymax - Ymin); 
	double Zrange = (Zmax - Zmin); 
	
	// keep cells away from the outer edge 
	
	Xmin += 0.1*Xrange; 
	Ymin += 0.1*Yrange; 
	Zmin = 0;
	
	Xrange *= 0.8;
	Yrange *= 0.8;
	Zrange = 0.0; 
	
	// create some of each type of cell 
	
	std::vector<double> position = {0,0,0}; 
	position[0] = Xmin + UniformRandom()*Xrange; 
	position[1] = Ymin + UniformRandom()*Yrange; 

	pC->assign_position( position );
	
	return; 
}

std::vector<double> choose_vascularized_position( void )
{
	//extern std::vector<int> vascularized_voxel_indices;
	int my_voxel_index = (int) ( UniformRandom() * (vascularized_voxel_indices.size()-1) );
	int n = vascularized_voxel_indices[ my_voxel_index ] ; 
	
 return microenvironment.mesh.voxels[n].center; 
}

void create_infiltrating_immune_cell( std::string cell_name )
{
	create_infiltrating_immune_cell( find_cell_definition( cell_name ) ); 
	
	return;
}

void create_infiltrating_neutrophil(void)
{
	static Cell_Definition* pCD = find_cell_definition( "neutrophil" );
	create_infiltrating_immune_cell( pCD ); 
	
	return; 
	
	static double Xmin = microenvironment.mesh.bounding_box[0]; 
	static double Ymin = microenvironment.mesh.bounding_box[1]; 
	static double Zmin = microenvironment.mesh.bounding_box[2]; 

	double Xmax = microenvironment.mesh.bounding_box[3]; 
	double Ymax = microenvironment.mesh.bounding_box[4]; 
	double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	double Xrange = (Xmax - Xmin); 
	double Yrange = (Ymax - Ymin); 
	double Zrange = (Zmax - Zmin); 
	
	// keep cells away from the outer edge 
	
	Xmin += 0.1*Xrange; 
	Ymin += 0.1*Yrange; 
	Zmin = 0;
	
	Xrange *= 0.8;
	Yrange *= 0.8;
	Zrange = 0.0; 
	
	// create some of each type of cell 
	
	Cell* pC;
	
	std::vector<double> position = {0,0,0}; 
	position[0] = Xmin + UniformRandom()*Xrange; 
	position[1] = Ymin + UniformRandom()*Yrange; 
	//position[2] = Zmin + UniformRandom()*Zrange; 
		
	pC = create_cell( get_cell_definition("neutrophil" ) ); 
	pC->assign_position( position );
	
	static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
	static int debris_index = microenvironment.find_density_index( "debris");
	static int chemokine_index = microenvironment.find_density_index( "chemokine");
			
	pC->phenotype.secretion.uptake_rates[chemokine_index] = 
		parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
	pC->phenotype.secretion.uptake_rates[proinflammatory_cytokine_index] = 
		parameters.doubles("activated_cell_cytokine_uptake_rate"); // 10;
	pC->phenotype.secretion.uptake_rates[debris_index] = 
		parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
	
	return;
}

void create_infiltrating_Tcell(void)
{
	static Cell_Definition* pCD = find_cell_definition( "CD8 Tcell" );
	create_infiltrating_immune_cell( pCD ); 

	return; 
	static double Xmin = microenvironment.mesh.bounding_box[0]; 
	static double Ymin = microenvironment.mesh.bounding_box[1]; 
	static double Zmin = microenvironment.mesh.bounding_box[2]; 

	static double Xmax = microenvironment.mesh.bounding_box[3]; 
	static double Ymax = microenvironment.mesh.bounding_box[4]; 
	static double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	static bool setup_done = false; 
	
	if( default_microenvironment_options.simulate_2D == true && setup_done == false )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	static double Xrange = (Xmax - Xmin); 
	static double Yrange = (Ymax - Ymin); 
	static double Zrange = (Zmax - Zmin); 
	
	// keep cells away from the outer edge 
	
	if( setup_done == false )
	{
		Xmin += 0.1*Xrange; 
		Ymin += 0.1*Yrange; 
		Zmin = 0;
		
		Xrange *= 0.8;
		Yrange *= 0.8;
		Zrange = 0.0; 
		setup_done = true; 
	}
	
	// create some of each type of cell 
	
	Cell* pC;
	
	std::vector<double> position = {0,0,0}; 
	position[0] = Xmin + UniformRandom()*Xrange; 
	position[1] = Ymin + UniformRandom()*Yrange; 
	//position[2] = Zmin + UniformRandom()*Zrange; 
		
	pC = create_cell( get_cell_definition("CD8 Tcell" ) ); 
	pC->assign_position( position );
		
	static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
	static int debris_index = microenvironment.find_density_index( "debris");
	static int chemokine_index = microenvironment.find_density_index( "chemokine");
	
	pC->phenotype.secretion.uptake_rates[chemokine_index] = 
		parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
	pC->phenotype.secretion.uptake_rates[proinflammatory_cytokine_index] = 
		parameters.doubles("activated_cell_cytokine_uptake_rate"); // 10;
	pC->phenotype.secretion.uptake_rates[debris_index] = 
		parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
	return;
}

void CD8_Tcell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// for 
/*	
	if( pCell->state.neighbors.size() > 0 )
	{ std::cout << "adhered Tcell " << pCell << std::endl; } 
*/
	
	return; 
}

void CD8_Tcell_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if I'm dead, don't bother 
	if( phenotype.death.dead == true )
	{
		// the cell death functions don't automatically turn off custom functions, 
		// since those are part of mechanics. 
		
		// Let's just fully disable now. 
		pCell->functions.custom_cell_rule = NULL; 
		return; 
	}	
	
	// if I am not adhered to a cell, turn motility on 
	if( pCell->state.neighbors.size() == 0 )
	{ phenotype.motility.is_motile = true; }
	else
	{ phenotype.motility.is_motile = false; }	
	
	// check for contact with infected cell 
	
	// if I'm adhered to something ... 
	if( pCell->state.neighbors.size() > 0 )
	{
		// add the elastic forces 
		extra_elastic_attachment_mechanics( pCell, phenotype, dt );
		
		// induce damage to whatever we're adhered to 
		#pragma omp critical
		{
			for( int n = 0; n < pCell->state.neighbors.size() ; n++ )
			{
				pCell->state.neighbors[n]->custom_data["TCell_contact_time"] += dt; 
			}
		}

		// decide whether to detach 
		bool detach_me = false; 
		
		if( UniformRandom() < dt / ( pCell->custom_data["cell_attachment_lifetime"] + 1e-15 ) )
		{ detach_me = true; }
		
		// if I detach, go through the process 
		if( detach_me )
		{
			// detach all attached cells 
			for( int n = 0; n < pCell->state.neighbors.size() ; n++ )
			{
				detach_cells( pCell, pCell->state.neighbors[n] ); 
			}
			// resume motile behavior 
			phenotype.motility.is_motile = true; 
		}
		return; 
	}
	
	// I'm not attached, look for cells nearby and try to attach
	
	// if this returns non-NULL, we're now attached to a cell 
	if( immune_cell_check_neighbors_for_attachment( pCell , dt) )
	{
		// set motility off 
		phenotype.motility.is_motile = false; 
		return; 
	}
	phenotype.motility.is_motile = true; // I suggest eliminating this. 
	
	return; 
}

void macrophage_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 
	static int apoptosis_index = phenotype.death.find_death_model_index( "apoptosis" ); 
	static Cell_Definition* pCD = find_cell_definition( "macrophage" ); 
	static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
	static int chemokine_index = microenvironment.find_density_index( "chemokine");
	static int debris_index = microenvironment.find_density_index( "debris");
			
	// determine bias_direction for macrophage based on "eat me" signals and chemokine
	double sensitivity_chemokine = parameters.doubles("sensitivity_to_chemokine_chemotaxis");
	double sensitivity_eat_me = parameters.doubles("sensitivity_to_eat_me_chemotaxis");
	pCell->phenotype.motility.migration_bias_direction = sensitivity_chemokine*pCell->nearest_gradient(chemokine_index)+sensitivity_eat_me*pCell->nearest_gradient(debris_index);
	normalize( &( phenotype.motility.migration_bias_direction) );

	// make changes to volume change rate??

	// if too much debris, comit to apoptosis 	
	
//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 

	double change_in_ingested_debris = ( phenotype.volume.total/pCD->phenotype.volume.total ); 
	if( change_in_ingested_debris > pCell->custom_data[ "relative_maximum_volume" ] )
	{
		pCell->start_death( apoptosis_index ); 
		pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = 0; 
		pCell->phenotype.secretion.secretion_rates[debris_index] = parameters.doubles("debris_secretion_rate"); 
		
//		std::cout << " I ate to much and must therefore die " << std::endl; 
//		system("pause"); 
	}

//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 

	// check for cells to eat 
	std::vector<Cell*> neighbors = pCell->cells_in_my_container(); 

//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 
	
	// at least one of the cells is pCell 
	if( neighbors.size() < 2 )
	{ return; } 
	
//	std::cout << "\t\t" << __FUNCTION__ << " " << __LINE__ << std::endl; 
	double macrophage_probability_of_phagocytosis = parameters.doubles("macrophage_probability_of_phagocytosis");

	int n = 0; 
	Cell* pTestCell = neighbors[n]; 
//	std::cout << pCell << " vs " ; 
	while( n < neighbors.size() )
	{
		pTestCell = neighbors[n]; 
//		std::cout << pTestCell << " "; 
		// if it is not me and not a macrophage 
		if( pTestCell != pCell && pTestCell->phenotype.death.dead == true && 
			pTestCell->phenotype.flagged_for_removal == false && UniformRandom()<macrophage_probability_of_phagocytosis)
		{
//			std::cout << std::endl; 
//			std::cout << "\t\tnom nom nom" << std::endl; 
//			std::cout << "\t\t\t" << pCell->type << " eats " << pTestCell->type << std::endl; 
//			std::cout << "\t\t\t" << pCell  << " eats " << pTestCell << std::endl; 
			pCell->ingest_cell( pTestCell ); 
			
			pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = 
				parameters.doubles("activated_macrophage_secretion_rate"); // 10;
			pCell->phenotype.secretion.uptake_rates[chemokine_index] = 
				parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
			pCell->phenotype.secretion.uptake_rates[proinflammatory_cytokine_index] = 
				parameters.doubles("activated_cell_cytokine_uptake_rate"); // 10;
			pCell->phenotype.secretion.uptake_rates[debris_index] = 
				parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
					
      
      // Paul M says: This should be read from a parameter value instead of hard-coded. 

			pCell->phenotype.motility.migration_speed = 
				parameters.doubles("activated_macrophage_speed"); 
			
//			system("pause");
			return; 
		}
//		else
//		{
//			std::cout << " (" << (int) pTestCell->phenotype.death.dead << " " << 
//			(int) pTestCell->phenotype.flagged_for_removal << ") " ; 
//		}
		
		n++; 
	}
//	std::cout << " " << std::endl; 
				
	return; 
}

void macrophage_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	return; 
}

void neutrophil_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 
	static int apoptosis_index = phenotype.death.find_death_model_index( "apoptosis" ); 
	static Cell_Definition* pCD = find_cell_definition( "neutrophil" ); 
	static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
	static int debris_index = microenvironment.find_density_index( "debris" ); 
	static int chemokine_index = microenvironment.find_density_index( "chemokine");
			
	// determine bias_direction for macrophage based on "eat me" signals and chemokine
	double sensitivity_chemokine = parameters.doubles("sensitivity_to_chemokine_chemotaxis");
	double sensitivity_eat_me = parameters.doubles("sensitivity_to_eat_me_chemotaxis");
	pCell->phenotype.motility.migration_bias_direction = sensitivity_chemokine*pCell->nearest_gradient(chemokine_index)+sensitivity_eat_me*pCell->nearest_gradient(debris_index);
	normalize( &( phenotype.motility.migration_bias_direction) );
	
	// make changes to volume change rate??

	// if too much debris, comit to apoptosis 	
	
//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 

	double change_in_ingested_debris = ( phenotype.volume.total/pCD->phenotype.volume.total ); 
	if( change_in_ingested_debris > pCell->custom_data[ "relative_maximum_volume" ] )
	{
		pCell->start_death( apoptosis_index ); 
		pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = 0; 
		pCell->phenotype.secretion.secretion_rates[debris_index] = parameters.doubles("debris_secretion_rate"); 
		
//		std::cout << " I ate to much and must therefore die " << std::endl; 
//		system("pause"); 
	}

//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 

	// check for cells to eat 
	std::vector<Cell*> neighbors = pCell->cells_in_my_container(); 

//	std::cout << __FUNCTION__ << " " << __LINE__ << std::endl; 
	
	// at least one of the cells is pCell 
	if( neighbors.size() < 2 )
	{ return; } 
	
//	std::cout << "\t\t" << __FUNCTION__ << " " << __LINE__ << std::endl; 

	int n = 0; 
	Cell* pTestCell = neighbors[n]; 
//	std::cout << pCell << " vs " ; 

	double neutrophil_probability_of_phagocytosis = parameters.doubles("neutrophil_probability_of_phagocytosis");

	while( n < neighbors.size() )
	{
		pTestCell = neighbors[n]; 
//		std::cout << pTestCell << " "; 
		// if it is not me and not a macrophage 
		if( pTestCell != pCell && pTestCell->phenotype.death.dead == true && 
			pTestCell->phenotype.flagged_for_removal == false && UniformRandom()<neutrophil_probability_of_phagocytosis)
		{
//			std::cout << std::endl; 
//			std::cout << "\t\tnom nom nom" << std::endl; 
//			std::cout << "\t\t\t" << pCell->type << " eats " << pTestCell->type << std::endl; 
//			std::cout << "\t\t\t" << pCell  << " eats " << pTestCell << std::endl; 
			pCell->ingest_cell( pTestCell ); 
			
			static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
			static int chemokine_index = microenvironment.find_density_index( "chemokine");
			
			pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = 
				parameters.doubles("activated_macrophage_secretion_rate"); // 10;
			pCell->phenotype.secretion.uptake_rates[chemokine_index] = 
				parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
			pCell->phenotype.secretion.uptake_rates[proinflammatory_cytokine_index] = 
				parameters.doubles("activated_cell_cytokine_uptake_rate"); // 10;
			pCell->phenotype.secretion.uptake_rates[debris_index] = 
				parameters.doubles("activated_cell_chemokine_uptake_rate"); // 10;
				
					
      
      // Paul M says: This should be read from a parameter value instead of hard-coded. 

			pCell->phenotype.motility.migration_speed = 
				parameters.doubles("activated_neutrophil_speed" ); 
			
//			system("pause");
			return; 
		}
//		else
//		{
//			std::cout << " (" << (int) pTestCell->phenotype.death.dead << " " << 
//			(int) pTestCell->phenotype.flagged_for_removal << ") " ; 
//		}
		
		n++; 
	}
//	std::cout << " " << std::endl; 
	
	// if neutrophil isn't killing any cell then return to normal speed
	pCell->phenotype.motility.migration_speed = 
		parameters.doubles("normal_neutrophil_speed"); 
				
	return; 
}

void neutrophil_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	return; 
}

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

Cell* check_for_live_neighbor_for_interaction( Cell* pAttacker , double dt )
{
	std::vector<Cell*> nearby = pAttacker->cells_in_my_container(); 
	int i = 0; 
	while( i < nearby.size() )
	{
		// don't try to kill yourself 
		if( nearby[i] != pAttacker && nearby[i]->phenotype.death.dead == false )
		{ return nearby[i]; }
		i++; 
	}
	return NULL; 
}

Cell* check_for_dead_neighbor_for_interaction( Cell* pAttacker , double dt )
{
	std::vector<Cell*> nearby = pAttacker->cells_in_my_container(); 
	int i = 0; 
	while( i < nearby.size() )
	{
		// don't try to kill yourself 
		if( nearby[i] != pAttacker && nearby[i]->phenotype.death.dead == true )
		{ return nearby[i]; }
		i++; 
	}
	return NULL; 
}

void add_elastic_velocity( Cell* pActingOn, Cell* pAttachedTo , double elastic_constant )
{
	std::vector<double> displacement = pAttachedTo->position - pActingOn->position; 
	axpy( &(pActingOn->velocity) , elastic_constant , displacement ); 
	
	return; 
}

void extra_elastic_attachment_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	for( int i=0; i < pCell->state.neighbors.size() ; i++ )
	{
		add_elastic_velocity( pCell, pCell->state.neighbors[i], pCell->custom_data["elastic_attachment_coefficient"] ); 
	}
	return; 
}	

void attach_cells( Cell* pCell_1, Cell* pCell_2 )
{
	#pragma omp critical
	{
		bool already_attached = false; 
		for( int i=0 ; i < pCell_1->state.neighbors.size() ; i++ )
		{
			if( pCell_1->state.neighbors[i] == pCell_2 )
			{ already_attached = true; }
		}
		if( already_attached == false )
		{ pCell_1->state.neighbors.push_back( pCell_2 ); }
		
		already_attached = false; 
		for( int i=0 ; i < pCell_2->state.neighbors.size() ; i++ )
		{
			if( pCell_2->state.neighbors[i] == pCell_1 )
			{ already_attached = true; }
		}
		if( already_attached == false )
		{ pCell_2->state.neighbors.push_back( pCell_1 ); }
	}

	return; 
}

void detach_cells( Cell* pCell_1 , Cell* pCell_2 )
{
	#pragma omp critical
	{
		bool found = false; 
		int i = 0; 
		while( !found && i < pCell_1->state.neighbors.size() )
		{
			// if cell 2 is in cell 1's list, remove it
			if( pCell_1->state.neighbors[i] == pCell_2 )
			{
				int n = pCell_1->state.neighbors.size(); 
				// copy last entry to current position 
				pCell_1->state.neighbors[i] = pCell_1->state.neighbors[n-1]; 
				// shrink by one 
				pCell_1->state.neighbors.pop_back(); 
				found = true; 
			}
			i++; 
		}
	
		found = false; 
		i = 0; 
		while( !found && i < pCell_2->state.neighbors.size() )
		{
			// if cell 1 is in cell 2's list, remove it
			if( pCell_2->state.neighbors[i] == pCell_1 )
			{
				int n = pCell_2->state.neighbors.size(); 
				// copy last entry to current position 
				pCell_2->state.neighbors[i] = pCell_2->state.neighbors[n-1]; 
				// shrink by one 
				pCell_2->state.neighbors.pop_back(); 
				found = true; 
			}
			i++; 
		}
	}
	
	return; 
}



bool attempt_immune_cell_attachment( Cell* pAttacker, Cell* pTarget , double dt )
{
	// if the target is not infected, give up 
	if( pTarget->custom_data[ "assembled_virion" ] < 1 )
	{ return false; }
		
	// if the target cell is dead, give up 
	if( pTarget->phenotype.death.dead == true )
	{ return false; } 

	// if the target cell is too far away, give up 
	std::vector<double> displacement = pTarget->position - pAttacker->position;
	double distance_scale = norm( displacement ); 
	if( distance_scale > pAttacker->custom_data["max_attachment_distance"] )
	{ return false; } 

	// now, get the attachment probability 
	
	double attachment_probability = pAttacker->custom_data["cell_attachment_rate"] * dt; 

	// don't need to cap it at 1.00: if prob > 100%, 
	// then this statement always evaluates as true, 
	// just the same as capping probability at 100% 
	if( UniformRandom() <= attachment_probability )
	{
		attach_cells( pAttacker, pTarget ); 
		return true; 
	}
	
	return false; 	
}

Cell* immune_cell_check_neighbors_for_attachment( Cell* pAttacker , double dt )
{
	std::vector<Cell*> nearby = pAttacker->cells_in_my_container(); 
	int i = 0; 
	while( i < nearby.size() )
	{
		// don't try to kill yourself 
		if( nearby[i] != pAttacker )
		{
			if( attempt_immune_cell_attachment( pAttacker, nearby[i] , dt ) )
			{
				return nearby[i]; 
			}
		}
		i++; 
	}
	
	return NULL; 
}

void TCell_induced_apoptosis( Cell* pCell, Phenotype& phenotype, double dt )
{
	static int apoptosis_index = phenotype.death.find_death_model_index( "apoptosis" ); 
	static int debris_index = microenvironment.find_density_index( "debris" ); 
	static int proinflammatory_cytokine_index = microenvironment.find_density_index("pro-inflammatory cytokine");
	
	if( pCell->custom_data["TCell_contact_time"] > pCell->custom_data["TCell_contact_death_threshold"] )
	{
		// std::cout << "I die now" << std::endl; 
		
		// make sure to get rid of all adhesions! 
		// detach all attached cells 
		for( int n = 0; n < pCell->state.neighbors.size() ; n++ )
		{
			detach_cells( pCell, pCell->state.neighbors[n] ); 
		}
		
		pCell->start_death( apoptosis_index ); 
		pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = 0; 
		pCell->phenotype.secretion.secretion_rates[debris_index] = parameters.doubles("debris_secretion_rate"); 
		pCell->functions.update_phenotype = NULL; 
	}
	
	return; 
}

void immune_cell_recruitment( double dt )
{
	static int proinflammatory_cytokine_index = 
		microenvironment.find_density_index("pro-inflammatory cytokine");
	
	static double dt_immune = parameters.doubles( "immune_dt" ); 
	static double t_immune = 0.0; 
	static double t_last_immune = 0.0; 
	static double t_next_immune = 0.0; 
	
	static double tolerance = 0.1 * diffusion_dt; 
	
	// is it time for the next immune recruitment? 
	if( t_immune > t_next_immune- tolerance )
	{
		double elapsed_time = (t_immune - t_last_immune );
		std::cout<<"t immune: "<<t_immune<<" elapsed time "<<elapsed_time<<std::endl;
//		std::cout << "Immune time! " << t_immune << " (elapsed: " << elapsed_time << ") " << std::endl; 
		
		// neutrophil recruitment 
		
		static double neutrophil_recruitment_rate = parameters.doubles( "neutrophil_max_recruitment_rate" ); 
		static double NR_min_signal = parameters.doubles( "neutrophil_recruitment_min_signal" ); 
		static double NR_sat_signal = parameters.doubles( "neutrophil_recruitment_saturation_signal" ); 
		static double NR_max_minus_min = NR_sat_signal - NR_min_signal; 
		
		double total_rate = 0;
		// integrate \int_domain r_max * (signal-signal_min)/(signal_max-signal_min) * dV 
		double total_scaled_signal= 0.0;
		for( int n=0; n<microenvironment.mesh.voxels.size(); n++ )
		{
			// (signal(x)-signal_min)/(signal_max/signal_min)
			double dRate = ( microenvironment(n)[proinflammatory_cytokine_index] - NR_min_signal ); 
			dRate /= NR_max_minus_min; 
			// crop to [0,1] 
			if( dRate > 1 ) 
			{ dRate = 1; } 
			if( dRate < 0 )
			{ dRate = 0; }
			total_rate += dRate; 
		}	
		// multiply by dV and rate_max 
		total_scaled_signal = total_rate; 
		
		total_rate *= microenvironment.mesh.dV; 
		total_rate *= neutrophil_recruitment_rate; 
		
		// expected number of new neutrophils 
		int number_of_new_cells = (int) round( total_rate * elapsed_time ); 
		if( number_of_new_cells )
		{
			std::cout << "\tRecruiting " << number_of_new_cells << " neutrophils ... " << std::endl; 
			
//			std::cout << "\tTotal signal/dV : " << total_scaled_signal << std::endl;
//			std::cout << "\tTotal signa : " << total_scaled_signal * microenvironment.mesh.dV << std::endl; 
//			double total_volume = microenvironment.mesh.dV * microenvironment.mesh.voxels.size() ; 
//			std::cout << "\tmean signal : " << total_scaled_signal * microenvironment.mesh.dV / total_volume << std::endl; 
			for( int n = 0; n < number_of_new_cells ; n++ )
			{ create_infiltrating_neutrophil(); }
		}
		
		// neutrophil recruitment 
		
		static double CD8_Tcell_recruitment_rate = parameters.doubles( "CD8_Tcell_max_recruitment_rate" ); 
		static double TC_min_signal = parameters.doubles( "CD8_Tcell_recruitment_min_signal" ); 
		static double TC_sat_signal = parameters.doubles( "CD8_Tcell_recruitment_saturation_signal" ); 
		static double TC_max_minus_min = TC_sat_signal - TC_min_signal; 
		
		total_rate = 0;
		// integrate \int_domain r_max * (signal-signal_min)/(signal_max-signal_min) * dV 
		total_scaled_signal= 0.0;
		for( int n=0; n<microenvironment.mesh.voxels.size(); n++ )
		{
			// (signal(x)-signal_min)/(signal_max/signal_min)
			double dRate = ( microenvironment(n)[proinflammatory_cytokine_index] - TC_min_signal ); 
			dRate /= TC_max_minus_min; 
			// crop to [0,1] 
			if( dRate > 1 ) 
			{ dRate = 1; } 
			if( dRate < 0 )
			{ dRate = 0; }
			total_rate += dRate; 
		}	
		// multiply by dV and rate_max 
		total_scaled_signal = total_rate; 
		
		total_rate *= microenvironment.mesh.dV; 
		total_rate *= CD8_Tcell_recruitment_rate; 
		
		// expected number of new neutrophils 
		number_of_new_cells = (int) round( total_rate * elapsed_time ); 
		if( number_of_new_cells )
		{
			std::cout << "\tRecruiting " << number_of_new_cells << " CD8 T cells ... " << std::endl; 
			
//			std::cout << "\tTotal signal/dV : " << total_scaled_signal << std::endl;
//			std::cout << "\tTotal signa : " << total_scaled_signal * microenvironment.mesh.dV << std::endl; 
//			double total_volume = microenvironment.mesh.dV * microenvironment.mesh.voxels.size() ; 
//			std::cout << "\tmean signal : " << total_scaled_signal * microenvironment.mesh.dV / total_volume << std::endl; 
			for( int n = 0; n < number_of_new_cells ; n++ )
			{ create_infiltrating_Tcell(); }
		}
		
		t_last_immune = t_immune; 
		t_next_immune = t_immune + dt_immune; 
		
		std::cout << "\t\tnext immune time: " << t_next_immune << std::endl;  
		
		std::cout<<"t immune: "<<t_immune<<" elapsed time "<<elapsed_time<<std::endl;
	}
	t_immune += dt; 
	
	return; 
}

void initial_immune_cell_placement( void )
{
	Cell_Definition* pCD8 = find_cell_definition( "CD8 Tcell" ); 
	Cell_Definition* pMF = find_cell_definition( "macrophage" ); 
	Cell_Definition* pN = find_cell_definition( "neutrophil" ); 

	// CD8+ T cells; 
	for( int n = 0 ; n < parameters.ints("number_of_CD8_Tcells") ; n++ )
	{ create_infiltrating_immune_cell( pCD8 ); }		

	// macrophages 
	for( int n = 0 ; n < parameters.ints("number_of_macrophages") ; n++ )
	{ create_infiltrating_immune_cell_initial( pMF ); }		

	// neutrophils 	for( int n = 0 ; n < parameters.ints("number_of_neutrophils") ; n++ )
	{ create_infiltrating_immune_cell( pN ); }		

	return;
}
