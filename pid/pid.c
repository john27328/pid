

void laser_1_pid(void)
{
		set_point_laser_1 = (S32)total_states.laser_1_setpoint;
		laser_1_temp = (S32)(y_approx_current_laser_1*10);
		prop_coef_laser_1 = total_states.prop_laser_1;
		integral_coef_laser_1 = total_states.integr_laser_1;
		dif_coef_laser_1 = (S32)(total_states.dif_laser_1*200.0);
		// Coefficient range : 0 - 100
		// Calculate delta error 
		err_laser_1 = laser_1_temp - set_point_laser_1; //Change mode - Using for cooling mode
	
		if(err_laser_1 > 0)
		{
			PID_LASER_1[0] = (prop_coef_laser_1 * err_laser_1*100)/(total_states.zone_laser_1*100); // Calculate proportional term 
			PID_LASER_1[0] = PID_LASER_1[0]*2600;
		}
		else PID_LASER_1[0] = 0;
	
		if(PID_LASER_1[0] > 240000)PID_LASER_1[0] = 240000;
		if(PID_LASER_1[0] < 5) PID_LASER_1[0] = 0; //limit proportional error

		sumError_laser_1+=err_laser_1;     
		if(sumError_laser_1 < sumError_min_laser_1) sumError_laser_1 = sumError_min_laser_1;
		if(sumError_laser_1 > sumError_max_laser_1) sumError_laser_1 = sumError_max_laser_1;    //limit integral  overflow
		PID_LASER_1[1] = integral_coef_laser_1 * sumError_laser_1; // Calculate integral term 

		PID_LASER_1[2] = dif_coef_laser_1 *(laser_1_temp - last_T_laser_1); //Calculate derivative term
		last_T_laser_1 = laser_1_temp;
		if(PID_LASER_1[2] > 240000)	PID_LASER_1[2] = 240000;
		if(PID_LASER_1[2] < -240000)	PID_LASER_1[2] = -240000;
		
		output_laser_1 = PID_LASER_1[0] + PID_LASER_1[1] + PID_LASER_1[2]; //Calculate new duty for Cooling Peltier Module 
	 
		if(output_laser_1 > output_limit_hi_laser_1) output_laser_1 = output_limit_hi_laser_1;   //limit overflow
		if(output_laser_1 < output_limit_low_laser_1)  output_laser_1 = output_limit_low_laser_1;  //to cool, cool and cool - not sleep - only for my system (normal=0)	
		
		// Output signal
		if((laser_1_thermistor_detected == __FALSE)||(total_states.regulator_enable ==__FALSE))LPC_PWM1->MR2 = 0;
		else LPC_PWM1->MR2 = (output_laser_1>>2); 
		LPC_PWM1->LER = 0x04;			// Update values. Will apply in next reset	
};


void laser_2_pid(void)
{
		set_point_laser_2 = (S32)total_states.laser_2_setpoint;
		laser_2_temp = (S32)(y_approx_current_laser_2*10);
		prop_coef_laser_2 = total_states.prop_laser_2;
		integral_coef_laser_2 = total_states.integr_laser_2;
		dif_coef_laser_2 = (S32)(total_states.dif_laser_2*200.0);
		// Coefficient range : 0 - 100
		// Calculate delta error 
		err_laser_2 = laser_2_temp - set_point_laser_2; //Change mode - Using for cooling mode
	
		if(err_laser_2 > 0)
		{
			PID_LASER_2[0] = (prop_coef_laser_2 * err_laser_2*100)/(total_states.zone_laser_2*100); // Calculate proportional term 
			PID_LASER_2[0] = PID_LASER_2[0]*2600;
		}
		else PID_LASER_2[0] = 0;
	
		if(PID_LASER_2[0] > 240000)PID_LASER_2[0] = 240000;
		if(PID_LASER_2[0] < 1) PID_LASER_2[0] = 0; //limit proportional error

		sumError_laser_2+=err_laser_2;     
		if(sumError_laser_2 < sumError_min_laser_2) sumError_laser_2 = sumError_min_laser_2;
		if(sumError_laser_2 > sumError_max_laser_2) sumError_laser_2 = sumError_max_laser_2;    //limit integral  overflow
		PID_LASER_2[1] = integral_coef_laser_2 * sumError_laser_2; // Calculate integral term 

		PID_LASER_2[2] = dif_coef_laser_2 *(laser_2_temp - last_T_laser_2); //Calculate derivative term
		last_T_laser_2 = laser_2_temp;
		if(PID_LASER_2[2] > 240000)	PID_LASER_2[2] = 240000;
		if(PID_LASER_2[2] < -240000)	PID_LASER_2[2] = -240000;
		
		output_laser_2 = PID_LASER_2[0] + PID_LASER_2[1] + PID_LASER_2[2]; //Calculate new duty for Cooling Peltier Module 
	 
		if(output_laser_2 > output_limit_hi_laser_2) output_laser_2 = output_limit_hi_laser_2;   //limit overflow
		if(output_laser_2 < output_limit_low_laser_2)  output_laser_2 = output_limit_low_laser_2;  //to cool, cool and cool - not sleep - only for my system (normal=0)	
		
		// Output signal
		if((laser_2_thermistor_detected == __FALSE)||(total_states.regulator_enable ==__FALSE))LPC_PWM1->MR1 = 0; 
		else LPC_PWM1->MR1 = (output_laser_2>>2); 
		LPC_PWM1->LER = 0x02;			// Update values. Will apply in next reset	

};

void cryst_1_pid(void)
{
		set_point_cryst_1 = (S32)total_states.cryst_1_setpoint;
		cryst_1_temp = (S32)(y_approx_current_cryst_1*10);
		prop_coef_cryst_1 = total_states.prop_cryst_1;
		integral_coef_cryst_1 = total_states.integr_cryst_1;
		dif_coef_cryst_1 = (S32)(total_states.dif_cryst_1*200.0);
		// Coefficient range : 0 - 100
		// Calculate delta error 
		err_cryst_1 = cryst_1_temp - set_point_cryst_1; //Change mode - Using for cooling mode
	
		if(err_cryst_1 > 0)
		{
			PID_CRYST_1[0] = (prop_coef_cryst_1 * err_cryst_1*100)/(total_states.zone_cryst_1*100); // Calculate proportional term 
			PID_CRYST_1[0] = PID_CRYST_1[0]*2600;
		}
		else PID_CRYST_1[0] = 0;
	
		if(PID_CRYST_1[0] > 240000)PID_CRYST_1[0] = 240000;
		if(PID_CRYST_1[0] < 1) PID_CRYST_1[0] = 0; //limit proportional error

		sumError_cryst_1+=err_cryst_1;     
		if(sumError_cryst_1 < sumError_min_cryst_1) sumError_cryst_1 = sumError_min_cryst_1;
		if(sumError_cryst_1 > sumError_max_cryst_1) sumError_cryst_1 = sumError_max_cryst_1;    //limit integral  overflow
		PID_CRYST_1[1] = integral_coef_cryst_1 * sumError_cryst_1; // Calculate integral term 

		PID_CRYST_1[2] = dif_coef_cryst_1 *(cryst_1_temp - last_T_cryst_1); //Calculate derivative term
		last_T_cryst_1 = cryst_1_temp;
		if(PID_CRYST_1[2] > 240000)	PID_CRYST_1[2] = 240000;
		if(PID_CRYST_1[2] < -240000)	PID_CRYST_1[2] = -240000;
		
		output_cryst_1 = PID_CRYST_1[0] + PID_CRYST_1[1] + PID_CRYST_1[2]; //Calculate new duty for Cooling Peltier Module 
	 
		if(output_cryst_1 > output_limit_hi_cryst_1) output_cryst_1 = output_limit_hi_cryst_1;   //limit overflow
		if(output_cryst_1 < output_limit_low_cryst_1)  output_cryst_1 = output_limit_low_cryst_1;  //to cool, cool and cool - not sleep - only for my system (normal=0)	
		
		// Output signal
		if((cryst_1_thermistor_detected == __FALSE)||(total_states.regulator_enable ==__FALSE))LPC_PWM1->MR4 = 0; 
		else LPC_PWM1->MR4 = (output_cryst_1>>2); 
		LPC_PWM1->LER = 0x10;			// Update values. Will apply in next reset	
};

void cryst_2_pid(void)
{
		set_point_cryst_2 = (S32)total_states.cryst_2_setpoint;
		cryst_2_temp = (S32)(y_approx_current_cryst_2*10);
		prop_coef_cryst_2 = total_states.prop_cryst_2;
		integral_coef_cryst_2 = total_states.integr_cryst_2;
		dif_coef_cryst_2 = (S32)(total_states.dif_cryst_2*200.0);
		// Coefficient range : 0 - 100
		// Calculate delta error 
		err_cryst_2 = cryst_2_temp - set_point_cryst_2; //Change mode - Using for cooling mode
	
		if(err_cryst_2 > 0)
		{
			PID_CRYST_2[0] = (prop_coef_cryst_2 * err_cryst_2*100)/(total_states.zone_cryst_2*100); // Calculate proportional term 
			PID_CRYST_2[0] = PID_CRYST_2[0]*2600;
		}
		else PID_CRYST_2[0] = 0;
	
		if(PID_CRYST_2[0] > 240000)PID_CRYST_2[0] = 240000;
		if(PID_CRYST_2[0] < 1) PID_CRYST_2[0] = 0; //limit proportional error

		sumError_cryst_2+=err_cryst_2;     
		if(sumError_cryst_2 < sumError_min_cryst_2) sumError_cryst_2 = sumError_min_cryst_2;
		if(sumError_cryst_2 > sumError_max_cryst_2) sumError_cryst_2 = sumError_max_cryst_2;    //limit integral  overflow
		PID_CRYST_2[1] = integral_coef_cryst_2 * sumError_cryst_2; // Calculate integral term 

		PID_CRYST_2[2] = dif_coef_cryst_2 *(cryst_2_temp - last_T_cryst_2); //Calculate derivative term
		last_T_cryst_2 = cryst_2_temp;
		if(PID_CRYST_2[2] > 240000)	PID_CRYST_2[2] = 240000;
		if(PID_CRYST_2[2] < -240000)	PID_CRYST_2[2] = -240000;
		
		output_cryst_2 = PID_CRYST_2[0] + PID_CRYST_2[1] + PID_CRYST_2[2]; //Calculate new duty for Cooling Peltier Module 
	 
		if(output_cryst_2 > output_limit_hi_cryst_2) output_cryst_2 = output_limit_hi_cryst_2;   //limit overflow
		if(output_cryst_2 < output_limit_low_cryst_2)  output_cryst_2 = output_limit_low_cryst_2;  //to cool, cool and cool - not sleep - only for my system (normal=0)	
		
		// Output signal
		if((cryst_2_thermistor_detected == __FALSE)||(total_states.regulator_enable ==__FALSE))LPC_PWM1->MR3 = 0; 
		else LPC_PWM1->MR3 = (output_cryst_2>>2); 
		LPC_PWM1->LER = 0x08;			// Update values. Will apply in next reset	

};


