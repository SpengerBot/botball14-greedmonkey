#include <kovan/kovan.h>
 
void wait_for_light_save(int light_port_)
{
	int xBut, l_on_, l_off_, l_mid_, OK = 0;
	xBut = get_extra_buttons_visible();
	set_extra_buttons_visible(0);
	set_a_button_text("-");
	set_c_button_text("-");
	set_analog_pullup(light_port_, 1);
	while (!OK) {
		set_b_button_text("Light is On");
		display_clear();
		display_printf (0, 0, "CALIBRATE: sensor port #%d", light_port_);
		display_printf(0, 1, "   press button when light is on");
		while(b_button_clicked() == 0) {
			l_on_ = analog (light_port_);
			display_printf(0,1,"   light on value is = %d        ", l_on_);
			msleep(50);
		}
		l_on_ = analog(light_port_); /* sensor value when light is on */
 
		set_b_button_text("Light is Off");
 
		display_printf(0,1,"   light on value is = %d        ", l_on_);
		msleep(200);
 
		display_printf(0,2,"   press button when light off");
		while(b_button_clicked() == 0) {
			l_off_ = analog(light_port_);
			display_printf(0,3,"   light off value is = %d         ", l_off_);
			msleep(50);
		}
		l_off_ = analog(light_port_); /* sensor value when light is off */
 
		display_printf(0,3,"   light off value is = %d         ", l_off_);
		msleep(200);
 
		if((l_off_ - l_on_) >= 60) { /* bright = small values */
			OK = 1;
			l_mid_ = (l_on_ + l_off_) / 3;
			display_printf(0, 5, "Good Calibration!");
			display_printf(0, 7, "Diff = %d:  WAITING", l_off_ - l_on_);
			int start = 0;
			while(!start){
				while(analog(light_port_) > l_mid_);
				msleep(250);
				if(analog(light_port_) < l_mid_){
					start = 1;
				}
			}
			
		} else {
			display_printf(0,7,"BAD CALIBRATION");
		}
	}
	set_extra_buttons_visible(xBut);
	set_a_button_text("A");
	set_b_button_text("B");
	set_c_button_text("C");
}