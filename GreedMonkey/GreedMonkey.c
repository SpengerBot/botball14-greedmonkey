void start_position();
void watch_for_cube();
void go_for_cube();
void camera_fix();
void calibrate();
void cube_is_near();

// Created on Mo März 24 2014
//Motors
#define Motor_Left 2
#define Motor_Right 0
#define Motor_Up 1

//Servos
#define	Servo_Left 3
#define Servo_Right 1
#define Servo_Back 0

#define Servo_Back_Up 2000
#define Servo_Back_Down 0
#define Servo_Left_Open 1250
#define Servo_Left_Closed 375
#define Servo_Right_Open 1000
#define Servo_Right_Closed 2030

//Button
#define Button_Up 14

//Camera Channel
#define Yellow_Channel 0

//Motor Werte
#define up_Motor_up_speed 100
#define up_Motor_down_speed -30

//Klauen Werte
int klaue_down = 0;
int klaue_orange=0;
int klaue_yellow=0;
int klaue_start=0;

//start 48500
//down 58000
void main()
{
	calibrate();
	enable_servos();
	start_position();
	watch_for_cube();
	go_for_cube();
	cube_is_near();
}

void calibrate(){
	
	printf("Please press the claibrate Button");
	set_b_button_text("calibrate");
	msleep(500);
	while(!b_button()){}
	display_clear();
	printf("calibrating");
	motor(Motor_Up,up_Motor_down_speed);
	while(!digital(Button_Up)){}
	ao();
	motor(Motor_Up,up_Motor_up_speed);
	while(digital(Button_Up)){}
	ao();
	clear_motor_position_counter(Motor_Up);
	klaue_down=get_motor_position_counter(Motor_Up);
	printf("Motor Position: %d", klaue_down);
	move_to_position(Motor_Up,100,10000);
	
}

void start_position(){
	set_servo_position(Servo_Back, Servo_Back_Up);
	motor(Motor_Left,100);
	motor(Motor_Right,25);
	msleep(2500);
	ao();
	set_servo_position(Servo_Left, Servo_Left_Open);
	set_servo_position(Servo_Right, Servo_Right_Open);
}
void go_for_cube()
{
	camera_fix();
	printf("echo: %d\n",get_object_bbox(0,0).width);
	while(get_object_bbox(0,0).width < 50){
		camera_update();
		printf("search: %d\n",get_object_center(0,0).x);
		if(get_object_center(0,0).x > 100)
		{
			printf("it's right\n");
			motor(Motor_Left,5);
			motor(Motor_Right,-5);
		}
		else if(get_object_center(0,0).x < 60)
		{
			printf("it's left\n");
			motor(Motor_Left,-5);
			motor(Motor_Right,5);
		}
		else
		{
			printf("drive\n");
			motor(Motor_Left,5);
			motor(Motor_Right,5);
		}
	}
}
void watch_for_cube(){
	camera_open_at_res(0);
	display_clear();
	camera_update();
	while(get_object_count(0)<1){
		camera_update();
	}
	while(get_object_center(0,0).x > 100 || get_object_center(0,0).x < 60){
		camera_update();
		printf("Column: %d\n",get_object_center(0,0).x);
		if(get_object_center(0,0).x > 100)
		{
			motor(Motor_Left,5);
			motor(Motor_Right,-5);
		}
		if(get_object_center(0,0).x < 60)
		{
			motor(Motor_Left,-5);
			motor(Motor_Right,5);
		}
	}
	printf("Last Col: %d\n",get_object_center(0,0).x);
	ao();
	
}
void cube_is_near(){
	camera_fix();
	//for(get_object_bbox(0,0));
}

void camera_fix(){
	double seconds_now=seconds()+3;
	while(seconds() < seconds_now){
		camera_update();
	}
}

