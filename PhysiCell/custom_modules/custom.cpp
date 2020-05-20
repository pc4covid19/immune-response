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
	
	if( parameters.bools("predators_eat_prey") == true )
	{ get_cell_definition("predator").functions.custom_cell_rule = predator_hunting_function; }

	if( parameters.bools("predators_cycle_if_big") == true )
	{ get_cell_definition("predator").functions.update_phenotype = predator_cycling_function; }

	if( parameters.bools("prey_quorom_effect") == true )
	{ get_cell_definition("prey").functions.update_phenotype = prey_cycling_function; }
		
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
	
	// place prey 
	
	for( int n = 0 ; n < parameters.ints("number_of_prey") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( get_cell_definition("prey") ); 
		pC->assign_position( position );
	}
	
	// place predators 
	
	for( int n = 0 ; n < parameters.ints("number_of_predators") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( get_cell_definition("predator") ); 
		pC->assign_position( position );
	}	
	
	return; 
}

std::vector<std::string> immune_coloring_function( Cell* pCell )
{
	static int epithelial_type = get_cell_definition( "?" ).type; 
	static int CD8_Tcell_type = get_cell_definition( "CD8_Tcell" ).type; 
	
/*	
	
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = false_cell_coloring_cytometry(pCell); 
	
	// color live prey 
		
	if( pCell->phenotype.death.dead == false && pCell->type == prey_type )
	{
		 output[0] = parameters.strings("prey_color");  
		 output[2] = parameters.strings("prey_color");  
	}
	
	// color live predators 

	if( pCell->phenotype.death.dead == false && pCell->type == predator_type )
	{
		 output[0] = parameters.strings("predator_color");  
		 output[2] = parameters.strings("predator_color");  
	}
	
	return output; 
}


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
