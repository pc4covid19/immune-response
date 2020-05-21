/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "./custom.h"

// declare cell definitions here 

Cell_Definition motile_cell; 

void create_cell_types( void )
{
	// set the random seed 
	SeedRandom( parameters.ints("random_seed") );  
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 
	
	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
	initialize_cell_definitions_from_pugixml(); 
	
	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	// register the submodels 
	// (which ensures that the cells have all the internal variables they need) 
	
	//receptor_dynamics_model_setup(); 
	//internal_virus_model_setup();
	//internal_virus_response_model_setup();
	immune_submodels_setup();
	submodel_registry.display( std::cout ); 	
	
/*	
	if( parameters.bools("predators_eat_prey") == true )
	{ get_cell_definition("predator").functions.custom_cell_rule = predator_hunting_function; }

	if( parameters.bools("predators_cycle_if_big") == true )
	{ get_cell_definition("predator").functions.update_phenotype = predator_cycling_function; }

	if( parameters.bools("prey_quorom_effect") == true )
	{ get_cell_definition("prey").functions.update_phenotype = prey_cycling_function; }
*/
		
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	build_cell_definitions_maps(); 
	display_cell_definitions( std::cout ); 
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
	// put any custom code to set non-homogeneous initial conditions or 
	// extra Dirichlet nodes here. 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}

void setup_tissue( void )
{
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
	
	double Xrange = Xmax - Xmin; 
	double Yrange = Ymax - Ymin; 
	double Zrange = Zmax - Zmin; 
	
	// create some of each type of cell 
	
	Cell* pC;
	
	// uninfected cell 
	pC = create_cell( get_cell_definition("lung epithelium" ) ); 
	std::vector<double> position = {0,0,0}; 
	pC->assign_position( position ); 

	// infected cell, which secretes chemokine 
	pC = create_cell( get_cell_definition("lung epithelium" ) ); 
	position = {30,0,0}; 
	pC->assign_position( position ); 
	pC->custom_data["assembled_virion"] = 1000.0; 
	int chemokine_index = microenvironment.find_density_index( "chemokin" ); 
	pC->phenotype.secretion.secretion_rates[chemokine_index] = 1.0; 
	pC->phenotype.secretion.saturation_densities[chemokine_index] = 1.0; 
	
	// dead infected cell 
	pC = create_cell( get_cell_definition("lung epithelium" ) ); 
	position = {0,-50,0}; 
	pC->assign_position( position ); 
	pC->custom_data["assembled_virion"] = 1000.0; 
	int death_index = pC->phenotype.death.find_death_model_index( "apoptosis" ); 
	pC->start_death( death_index ); 

	// CD8 Tcell 
	pC = create_cell( get_cell_definition("CD8 Tcell" ) ); 
	position = {0, 0.4*Xrange,0}; 
	pC->assign_position( position ); 

	// macrophage
	pC = create_cell( get_cell_definition("macrophage" ) ); 
	position = {0,-0.4*Xrange,0}; 
	pC->assign_position( position ); 

	// neutrophil
	pC = create_cell( get_cell_definition("neutrophil" ) ); 
	position = {-0.4*Xrange,0,0}; 
	pC->assign_position( position ); 
	
	// place prey 
/*	
	for( int n = 0 ; n < parameters.ints("number_of_prey") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( get_cell_definition("prey") ); 
		pC->assign_position( position );
	}
*/	
	
	return; 
}

std::string blue_yellow_interpolation( double min, double val, double max )
{
// 	std::string out;
	
	double interpolation = (val-min)/(max-min+1e-16); 
	if( interpolation < 0.0 )
	{ interpolation = 0.0; } 
	if( interpolation > 1.0 )
	{ interpolation = 1.0; }
	
	int red_green = (int) floor( 255.0 * interpolation ) ; 
	int blue = 255 - red_green; 

	char color [1024]; 
	sprintf( color, "rgb(%u,%u,%u)" , red_green,red_green,blue ); 
	return color;  
}

std::vector<std::string> immune_coloring_function( Cell* pCell )
{
	static int lung_epithelial_type = get_cell_definition( "lung epithelium" ).type; 
	static int CD8_Tcell_type = get_cell_definition( "CD8 Tcell" ).type; 
	static int Macrophage_type = get_cell_definition( "macrophage" ).type; 
	static int Neutrophil_type = get_cell_definition( "neutrophil" ).type; 
	
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = {"white", "black", "white" , "white" };	
	// false_cell_coloring_cytometry(pCell); 
	
	if( pCell->phenotype.death.dead == true )
	{
		output[0] = "black"; 		
		output[2] = "black"; 		
		output[3] = "black"; 		
		return output; 
	}

	if( pCell->phenotype.death.dead == false && pCell->type == lung_epithelial_type )
	{
		// color by virion 
		std::string color = blue_yellow_interpolation( 0.0 , pCell->custom_data["assembled_virion"] , 1000.0 ); 
		output[0] = color;   
		output[2] = color; 
		output[3] = color; 
		return output; 
	}
	
	if( pCell->phenotype.death.dead == false && pCell->type == CD8_Tcell_type )
	{
		output[0] = parameters.strings("CD8_Tcell_color");  
		output[2] = parameters.strings("CD8_Tcell_color");  
		output[3] = parameters.strings("CD8_Tcell_color"); 
		return output; 
	}

	if( pCell->phenotype.death.dead == false && pCell->type == Macrophage_type )
	{
		output[0] = parameters.strings("Macrophage_color");  
		output[2] = parameters.strings("Macrophage_color");  
		output[3] = parameters.strings("Macrophage_color"); 
		return output; 
	}

	if( pCell->phenotype.death.dead == false && pCell->type == Neutrophil_type )
	{
		output[0] = parameters.strings("Neutrophil_color");  
		output[2] = parameters.strings("Neutrophil_color");  
		output[3] = parameters.strings("Neutrophil_color");  
		return output; 
	}

	return output; 
}

/*
void predator_hunting_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	Cell* pTestCell = NULL; 
	
	double sated_volume = pCell->parameters.pReference_live_phenotype->volume.total * 
		parameters.doubles("relative_sated_volume" ); 
	
	for( int n=0; n < pCell->cells_in_my_container().size() ; n++ )
	{
		pTestCell = pCell->cells_in_my_container()[n]; 
		// if it's not me, not dead, and not my type, eat it 
		
		if( pTestCell != pCell && pTestCell->type != pCell->type && pTestCell->phenotype.death.dead == false )
		{
			// only eat if I'm not full 
			if( phenotype.volume.total < sated_volume )
			{
				pCell->ingest_cell(pTestCell); 
				return; 
			}
	
		}
	}
	
	return; 
}

void predator_cycling_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	double sated_volume = pCell->parameters.pReference_live_phenotype->volume.total * 
		parameters.doubles("relative_sated_volume" ); 
	
	if( phenotype.volume.total > sated_volume )
	{ phenotype.cycle.data.transition_rate(0,1) = get_cell_definition("prey").phenotype.cycle.data.transition_rate(0,1) * 0.01; }
	else
	{ phenotype.cycle.data.transition_rate(0,1) = 0; }
	return; 
}

void prey_cycling_function( Cell* pCell , Phenotype& phenotype, double dt )
{
	static int signal_index = microenvironment.find_density_index( "prey signal" ); 
	
	double threshold = parameters.doubles("prey_quorom_threshold" ) + 1e-16 ; 
	double factor = (threshold - pCell->nearest_density_vector()[signal_index] )/threshold; 
	if( factor < 0 )
	{ factor = 0.0; } 
	
	phenotype.cycle.data.transition_rate(0,1) = get_cell_definition("prey").phenotype.cycle.data.transition_rate(0,1); 
	phenotype.cycle.data.transition_rate(0,1) *= factor; 
	
	return; 
}
*/
