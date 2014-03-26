void main2()
{
	camera_open_at_res(0);
	display_clear();
	camera_update();
	while(1){
		display_clear();
		camera_update();
		//printf("get_object_center(0,0).x: %d\n",get_object_center(0,0).x);
		printf("get_object_bbox(0,0).x: %d\n",get_object_bbox(0,0).width);
		msleep(200);
	}
}
