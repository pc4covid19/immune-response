#include "./internal_viral_response.h" 

using namespace PhysiCell; 

std::string internal_virus_response_version = "0.2.0"; 

Submodel_Information internal_virus_response_model_info; 

void internal_virus_response_model_setup( void )
{
	// set up the model 
		// set version info 
	internal_virus_response_model_info.name = "internal viral response"; 
	internal_virus_response_model_info.version = internal_virus_response_version; 
		// set functions 
	internal_virus_response_model_info.main_function = NULL; 
	internal_virus_response_model_info.phenotype_function = internal_virus_response_model; 
	internal_virus_response_model_info.mechanics_function = NULL; 
	
		// what microenvironment variables do you expect? 
		// what custom data do I need? 
	internal_virus_response_model_info.cell_variables.push_back( "max_infected_apoptosis_rate" ); 
	internal_virus_response_model_info.cell_variables.push_back( "max_apoptosis_half_max" ); 
	internal_virus_response_model_info.cell_variables.push_back( "apoptosis_hill_power" ); 	
	
		// register the submodel  
	internal_virus_response_model_info.register_model();	
		// set functions for the corresponding cell definition 
		
//	pCD = find_cell_definition( "lung epithelium" ); 
//	pCD->functions.update_phenotype = epithelium_submodel_info.phenotype_function;
//	pCD->functions.custom_cell_rule = epithelium_submodel_info.mechanics_function;
	
	return; 
}

void internal_virus_response_model( Cell* pCell, Phenotype& phenotype, double dt )
{
	static Cell_Definition* pCD = find_cell_definition( "lung epithelium" ); 
	
	// bookkeeping -- find microenvironment variables we need

	static int nV_external = microenvironment.find_density_index( "virion" ); 
	static int nA_external = microenvironment.find_density_index( "assembled virion" ); 
	
	static int nV_internal = pCell->custom_data.find_variable_index( "virion" ); 
	static int nA_internal = pCell->custom_data.find_variable_index( "assembled_virion" ); 
	
	// actual model goes here 

	// now, set apoptosis rate 
	
	static int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "apoptosis" );
	// phenotype.death.rates[apoptosis_model_index] = 
	
	// base death rate (from cell line)
	double base_death_rate = 
		pCD->phenotype.death.rates[apoptosis_model_index]; 
	
	// additional death rate from infectoin  
	double additional_death_rate = pCell->custom_data["max_infected_apoptosis_rate"] ; 
	
	
	double v = pCell->custom_data[nA_internal] / 
		pCell->custom_data["max_apoptosis_half_max"] ; 
	v = pow( v, pCell->custom_data["apoptosis_hill_power"] ); 
	
	double effect = v / (1.0+v); 
	additional_death_rate *= effect; 
	phenotype.death.rates[apoptosis_model_index] = base_death_rate + additional_death_rate; 
	
	// if we're infected, secrete a chemokine for the immune model
	static int nAV = pCell->custom_data.find_variable_index( "assembled_virion" ); 	
	double AV = pCell->custom_data[nAV]; 
	
	static int nP = pCell->custom_data.find_variable_index( "viral_protein"); 
	double P = pCell->custom_data[nP];

	static int chemokine_index = microenvironment.find_density_index( "chemokine" ); 
	
/* old 
	if( P > 0.001 )
	{
		phenotype.secretion.secretion_rates[chemokine_index] = 
			pCell->custom_data[ "infected_cell_chemokine_secretion_rate" ];
		phenotype.secretion.saturation_densities[chemokine_index] = 1.0; 		
	}
	else
	{
		phenotype.secretion.secretion_rates[chemokine_index] = 0.0;
	}
*/	
	
	// if I am dead, make sure to still secrete the chemokine 
	
	// static int chemokine_index = microenvironment.find_density_index( "chemokine" ); 
	// static int nP = pCell->custom_data.find_variable_index( "viral_protein"); 
	// double P = pCell->custom_data[nP];
	
	// static int nAV = pCell->custom_data.find_variable_index( "assembled_virion" ); 
	// double AV = pCell->custom_data[nAV]; 
	static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
		
	static int nR = pCell->custom_data.find_variable_index( "viral_RNA");
	double R = pCell->custom_data[nR];
	
	if( R >= 1.00 - 1e-16 ) 
	{
		pCell->custom_data["infected_cell_chemokine_secretion_activated"] = 1.0; 
	}

	if( pCell->custom_data["infected_cell_chemokine_secretion_activated"] > 0.1 && phenotype.death.dead == false )
	{
		double rate = AV; 
		rate /= pCell->custom_data["max_apoptosis_half_max"];
		if( rate > 1.0 )
		{ rate = 1.0; }
		rate *= pCell->custom_data[ "infected_cell_chemokine_secretion_rate" ];

		phenotype.secretion.secretion_rates[chemokine_index] = rate; 
		phenotype.secretion.saturation_densities[chemokine_index] = 1.0;

		// (Adrianne) adding pro-inflammatory cytokine secretion by infected cells
		pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = pCell->custom_data["activated_cytokine_secretion_rate"]/10;
	}
	/*
	// (Adrianne) check whether the cell is undergoing pyroptosis and if so, evalute the pyropotosis model
	if( pCell->custom_data["cell_pyroptosis_flag"]==1 && phenotype.death.dead == false )
	{
		std::cout<<AV<<" "<<pCell->custom_data["max_apoptosis_half_max"]/2<<std::endl;
		std::cout<<"here2"<<std::endl;
		pyroptosis_cascade( pCell, phenotype, dt ); 
	};
	
	// (Adrianne) pyroptosis cascade in the cell is initiated if assembled virions > max_apoptosis_half_max*3/2 - this choice is currently arbitrary and needs to be discussed in more detail
	if( AV >  pCell->custom_data["max_apoptosis_half_max"]/100 && phenotype.death.dead == false )
	{
		std::cout<<AV<<" "<<pCell->custom_data["max_apoptosis_half_max"]/2<<std::endl;
		std::cout<<"here"<<std::endl;
		pCell->custom_data["cell_pyroptosis_flag"]=1; // (Adrianne) pyroptosis cascade initiated
		pyroptosis_cascade( pCell, phenotype, dt );
	}
	*/
	return; 
}

// Sara and Fiona's code for pyroptosis
void pyroptosis_cascade( Cell* pCell, Phenotype& phenotype, double dt )
{
	// (Sara and Fiona) Pyroptosis code starts here.

	// Intracellular components
	double nfkb_n = pCell->custom_data.find_variable_index( "nuclear_NFkB" ); 
	double nlrp3_i = pCell->custom_data.find_variable_index( "inactive_NLRP3" ); 
	double nlrp3_a = pCell->custom_data.find_variable_index( "active_NLRP3" ); 
	double nlrp3_b = pCell->custom_data.find_variable_index( "bound_NLRP3" ); 
	double asc_b = pCell->custom_data.find_variable_index( "bound_ASC" );
	double caspase1_b = pCell->custom_data.find_variable_index( "bound_caspase1" );
	double gsdmd_c = pCell->custom_data.find_variable_index( "cleaved_gasderminD" );
	double il_1b_p = pCell->custom_data.find_variable_index( "pro_IL_1b" );
	double il_1b_c = pCell->custom_data.find_variable_index( "cytoplasmic_IL_1b" );
	double il_1b_e = pCell->custom_data.find_variable_index( "external_IL_1b" );
	double il_18_c = pCell->custom_data.find_variable_index( "cytoplasmic_IL_18" );
	double il_18_e = pCell->custom_data.find_variable_index( "external_IL_18" );
	double volume_c = pCell->custom_data.find_variable_index( "cytoplasmic_volume" );
	//Not needed components (can be implicitely found via conservation laws)
	//static double nfkb_c = pCell->custom_data.find_variable_index( "cytoplasmic_NFkB_fraction" ); 
	//static double asc_f = pCell->custom_data.find_variable_index( "free_ASC" );
	//static double caspase1_f = pCell->custom_data.find_variable_index( "free_caspase1" );
	//static double gsdmd_uc = pCell->custom_data.find_variable_index( "uncleaved_gasderminD" );
	//static double il_18_e = pCell->custom_data.find_variable_index( "external_IL_1b" );

	// Rate constants
	double k_nfkb_ctn = pCell->custom_data.find_variable_index( "rate_NFkB_cytoplasm_to_nucleus" ); 
	double k_nfkb_ntc = pCell->custom_data.find_variable_index( "rate_NFkB_nucleus_to_cytoplasm" );
	double k_nlrp3_ita = pCell->custom_data.find_variable_index( "rate_NLRP3_incactive_to_active" );
	double k_nlrp3_atb = pCell->custom_data.find_variable_index( "rate_NLRP3_active_to_bound" );
	double k_asc_ftb = pCell->custom_data.find_variable_index( "rate_ASC_free_to_bound" );
	double k_c1_ftb = pCell->custom_data.find_variable_index( "rate_caspase1_free_to_bound" );
	double k_il1b_cte = pCell->custom_data.find_variable_index( "rate_Il1b_cytoplasmic_to_external" );
	double k_il18_cte = 1;//pCell->custom_data.find_variable_index( "rate_Il18_cytoplasmic_to_external" );
	double k_vol_c = 1;// pCell->custom_data.find_variable_index( "rate_pyroptosis_volume_increase" );
	// Decay constants
	double d_nlrp3 = pCell->custom_data.find_variable_index( "decay_NLRPR_inactive_and_active" );
	double d_il = pCell->custom_data.find_variable_index( "decay_IL1b" );
	// Hill function rates
	double a_nlrp3 = pCell->custom_data.find_variable_index( "rate_constant_NLRP3_production" );
	double a_il1b_p = pCell->custom_data.find_variable_index( "rate_constant_IL1b_production" );
	double a_gsdmd = pCell->custom_data.find_variable_index( "rate_constant_GSDMD_cleavage" );
	double a_il1b_c = pCell->custom_data.find_variable_index( "rate_constant_Il1b_cleavage" );
	double a_il18 = pCell->custom_data.find_variable_index( "rate_constant_Il18_cleavage" );
	double hm_nfkb = pCell->custom_data.find_variable_index( "halfmax_NFkB_transcription" );
	
	//std::cout<<pCell->custom_data.find_variable_index( "halfmax_caspase1_cleavage" )<<std::endl;
	//double hm_c1 = 1;//pCell->custom_data.find_variable_index( "halfmax_caspase1_cleavage" );
	double hm_c1 = pCell->custom_data.find_variable_index("halfmax_caspase1_cleavage");
	double hex_nfkb = pCell->custom_data.find_variable_index( "hillexponent_NFkB_transcription" );
	double hex_c1 = pCell->custom_data.find_variable_index( "hillexponent_caspase1_cleavage" );
	// Total concentrations (let's have them all as 1 now and compute fractions)
	//static double tot_nfkb = pCell->custom_data.find_variable_index( "total_NFkB_concentration" );	
	//static double tot_asc = pCell->custom_data.find_variable_index( "total_ASC_concentration" );
	//static double tot_c1 = pCell->custom_data.find_variable_index( "total_caspase1_concentration" );
	//static double tot_gsdmd = pCell->custom_data.find_variable_index( "total_GSDMD_concentration" );
	//static double tot_il18 = pCell->custom_data.find_variable_index( "total_IL18_concentration" );


	// System of pyroptosis equations starts here

	//If the inflammsome base is formed set F_ib = 0. 
	double F_ib = 1;
	if( pCell->custom_data[nlrp3_b] >= 1)
	{F_ib=0;}

	//Update nuclear NFkB
	double dNFkB_n = dt * ( F_ib*k_nfkb_ctn*(1-pCell->custom_data[nfkb_n]) - k_nfkb_ntc*pCell->custom_data[nfkb_n]);
	pCell->custom_data[nfkb_n] += dNFkB_n; 

	//Set Hill function 1
	double hill_nfkb = (pow(pCell->custom_data[nfkb_n],hex_nfkb))/(pow(hm_nfkb,hex_nfkb)+pow(pCell->custom_data[nfkb_n],hex_nfkb));
    
	//Update NLRP3 (inactive, active and bound)
	double dNLRP3_i = dt * ( a_nlrp3*hill_nfkb - (k_nlrp3_ita + d_nlrp3)*pCell->custom_data[nlrp3_i] ); 
	pCell->custom_data[nlrp3_i] += dNLRP3_i;

	double dNLRP3_a = dt * (k_nlrp3_ita*pCell->custom_data[nlrp3_i] - (k_nlrp3_atb*F_ib + d_nlrp3)*pCell->custom_data[nlrp3_a]);
	pCell->custom_data[nlrp3_a] += dNLRP3_a;

	double dNLRP3_b = dt * k_nlrp3_atb * F_ib * pCell->custom_data[nlrp3_a];
	pCell->custom_data[nlrp3_b] += dNLRP3_b; 

	//Update bound ASC
	double dASC_b = dt * k_asc_ftb * (1-F_ib) * pCell->custom_data[nlrp3_b]*(1-pCell->custom_data[asc_b]);
	pCell->custom_data[asc_b] += dASC_b; 

	//Update bound caspase1
	double dCasp1 = dt * k_c1_ftb * pCell->custom_data[asc_b]*(1-pCell->custom_data[caspase1_b]);
	pCell->custom_data[caspase1_b] += dCasp1; 

	//Set Hill function 2
	double hill_caspase1 = (pow(pCell->custom_data[caspase1_b],hex_c1))/(pow(hm_c1,hex_c1)+pow(pCell->custom_data[caspase1_b],hex_c1));

	//Update cleaved GSDMD
	double dGSDMD_c = dt * a_gsdmd * hill_caspase1 * (1-pCell->custom_data[gsdmd_c]);
	pCell->custom_data[gsdmd_c] += dGSDMD_c;

	//Set G function
	double g_gsdmd = pCell->custom_data[gsdmd_c];

	//Update IL1b (pro, cytoplasmic, external)	We want to relate this to secreted cytokine IL1b
	double dIL1b_p = dt * (a_il1b_p*hill_nfkb - a_il1b_c*hill_caspase1*pCell->custom_data[il_1b_p] - d_il*pCell->custom_data[il_1b_p]);
	pCell->custom_data[il_1b_p] += dIL1b_p;

	double dIL1b_c = dt * (a_il1b_c*hill_caspase1*pCell->custom_data[il_1b_p] - k_il1b_cte*g_gsdmd*pCell->custom_data[il_1b_c] - d_il*pCell->custom_data[il_1b_c]);
	pCell->custom_data[il_1b_c] += dIL1b_c;

	double dIL1b_e = dt * (k_il1b_cte*g_gsdmd*pCell->custom_data[il_1b_c]);
	pCell->custom_data[il_1b_e] += dIL1b_e; //We want to relate this to secreted cytokine IL1b

	//Update IL18 (cytoplasmic, external)
	double dIL18_c = dt * (a_il18*hill_caspase1*(1-pCell->custom_data[il_18_c]-pCell->custom_data[il_18_e]) - k_il18_cte*g_gsdmd*pCell->custom_data[il_18_c]  );
	pCell->custom_data[il_18_c] += dIL18_c;

	double dIL18_e = dt * k_il18_cte*g_gsdmd*pCell->custom_data[il_18_c];
	pCell->custom_data[il_18_e] += dIL18_e;//We want to relate this to secreted cytokine IL18

	//Update cytoplasmic volume
	double dVol_c = dt * k_vol_c * g_gsdmd * pCell->custom_data[volume_c];
	pCell->custom_data[volume_c] += dVol_c;//We want to relate this to cell cytoplasm volume
		
	// (Adrianne) update cell pro-inflammatory secretion rate based on IL18 secretion rate - need to double check unit conversion
	static int proinflammatory_cytokine_index = microenvironment.find_density_index( "pro-inflammatory cytokine");
	pCell->phenotype.secretion.secretion_rates[proinflammatory_cytokine_index] = pCell->custom_data["activated_cytokine_secretion_rate"]+k_il18_cte*g_gsdmd*pCell->custom_data[il_18_c];
	
	// (Adrianne) finding homeostatic cell volume for lung epithelium cells
	static Cell_Definition* pCD = find_cell_definition( "lung epithelium" ); 
	std::cout<<pCell->custom_data[volume_c]<<" "<<1.5*pCD->phenotype.volume.total<<std::endl;
	// (Adrianne) if cell volume modelled by the ODE system exceeds 1.5*homeotastatic volume, then the cell dies
	if( pCell->custom_data[volume_c]>1.5*pCD->phenotype.volume.total)
	{
		// (Adrianne) cell lyses
		std::cout<<"cell pyroptoses"<<std::endl;
		pCell->lyse_cell(); // (Adrianne) for now using built in lyse_cell function, but may be that we need to build a better model for this
		pCell->functions.update_phenotype = NULL; 
	}

	return;
}
