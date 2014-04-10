void wait_for_light_save(int light_port_)
{
	int l_on_, l_off_, l_mid_, OK=0;

	while (!OK) {
		cbc_display_clear();
		cbc_printf (0,0,"CALIBRATE: sensor port #%d", light_port_);
		sleep(1.); sleep(1.);

		cbc_printf(0,1,"  press <-- when light on");
		while(!left_button()){/*sensor value when light is on*/
			cbc_printf(0,2,"  value is %d, bright = low   ",l_on_=analog10 (light_port_)); 
			msleep(50);
		}

		cbc_printf(0,1,"  light on value is = %d        ", l_on_);
		sleep(1.);

		cbc_printf(0,2,"  press --> when light off             ");
		while(!right_button()){	/*sensor value when light is off*/
			cbc_printf(0,3,"   value is %d, dark = high   ",l_off_=analog10 (light_port_)); 
			msleep(50);
		}

		cbc_printf(0,2,"  light off value is = %d         ", l_off_);
		sleep(1.);

		cbc_printf(0,3,"                              ");

		if((l_off_-l_on_)>=120){ /*bright = small values */
			OK=1;
			l_mid_=(l_on_+l_off_)/2;
			cbc_printf(0,4,"Good Calibration!");
			cbc_printf(0,6,"Diff = %d:  WAITING",l_off_-l_on_);
			int on = 0;									//New Code to avoid flashlight starting!
			while(analog10(light_port_)>l_mid_&on){		//New Code to avoid flashlight starting!
				cbc_printf(0,7,"Value = %d; Threshold = %d   ",analog10(light_port_),l_mid_);
				msleep(25);
				if(analog10(light_port_)>l_mid_) {		//New Code to avoid flashlight starting!
					on = 1;								//New Code to avoid flashlight starting!
				}										//New Code to avoid flashlight starting!
			}
			cbc_printf(0,6,"Going!                      ");
			cbc_printf(0,7,"Value = %d; Threshold = %d   ",analog10(light_port_),l_mid_);
		}
		else{
			cbc_printf(0,6,"BAD CALIBRATION");
		}
	}
}
