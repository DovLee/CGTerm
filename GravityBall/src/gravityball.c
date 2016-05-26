#include "gravityball.h"

static int screen_width;
static int screen_height;

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *ball;
	Evas_Object *hole;
	float velocity_x;
	float velocity_y;
} appdata_s;

static void
win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	elm_exit();
}

static Eina_Bool
keydown_cb(void *data , int type , void *event)
{
	appdata_s *ad = data;
	Ecore_Event_Key *ev = event;
	if (!strcmp(ev->keyname, KEY_END)) {
		/* Let window go to hide state. */
		elm_win_lower(ad->win);

		return ECORE_CALLBACK_DONE;
	}

	return ECORE_CALLBACK_PASS_ON;
}

static void
start_game(appdata_s *ad)
{
	int random = rand();
	srand(time(NULL));
	/* Move the hole to random position */
	evas_object_move(ad->hole, random %(screen_width - BALL_DIAMETER), random%(screen_height - BALL_DIAMETER) );
	/* Moving the ball to the center of the screen and show object */
	evas_object_move(ad->ball, screen_width /2 - BALL_DIAMETER/2, screen_height/2 - BALL_DIAMETER/2);
}

static void
add_rolling_resistance(float *velocity)
{
	if( fabs(  *velocity * (1 - RESISTANCE) ) >= 0 ){
		*velocity *= (1 - RESISTANCE);
	} else {
		*velocity = 0;
	}
}

static void
set_current_velocity(float *velocity, float acceleration,int position, int min_position, int max_position )
{
	if( (position - *velocity) <= (max_position - BALL_DIAMETER) && (position - *velocity) >= min_position){
		*velocity += acceleration;
	} else {
		*velocity = -1* ELASTICITY * (*velocity);
	}
}

static void
accelerometer_cb(sensor_h sensor, sensor_event_s *event, void *data)
{
	Evas_Coord x, y, w, h, hole_x, hole_y;
	appdata_s * ad = (appdata_s *)data;
	evas_object_geometry_get(ad->ball, &x, &y, &w, &h);

	/* Impact of resistance coefficient */
	add_rolling_resistance( &ad->velocity_x );
	add_rolling_resistance( &ad->velocity_y );

	/* Calculation of current  ball speed */
	set_current_velocity( &ad->velocity_x, (float)event->values[0], x, 0, screen_width );
	set_current_velocity( &ad->velocity_y, -1*(float)event->values[1], y, 0, screen_height);

	evas_object_move(ad->ball,  x - ad->velocity_x ,  y - ad->velocity_y);

	/* Restart the game after entering the hole */
	evas_object_geometry_get(ad->hole, &hole_x, &hole_y, NULL, NULL);
	if( (x < hole_x + ACCURACY && x > hole_x - ACCURACY)  && (y < hole_y + ACCURACY && y > hole_y - ACCURACY)){
		start_game(ad);
	}

}

static int
register_accelerometer_callback(appdata_s *ad)
{
	int error;
	bool supported;
	sensor_h accelerometer;
	sensor_listener_h accelerationListener;

	error = sensor_is_supported( SENSOR_ACCELEROMETER, &supported );
	if(error != SENSOR_ERROR_NONE && supported){
		return error;
	}

	error = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &accelerometer);
	if(error != SENSOR_ERROR_NONE){
		return error;
	}

	error = sensor_create_listener( accelerometer, &accelerationListener);
	if(error != SENSOR_ERROR_NONE){
		return error;
	}

	error = sensor_listener_set_event_cb( accelerationListener, ACCELEROMETER_INTERVAL_MS, accelerometer_cb, ad );
	if(error != SENSOR_ERROR_NONE){
		return error;
	}

	error = sensor_listener_start( accelerationListener );

	return SENSOR_ERROR_NONE;
}

static void
create_base_gui(appdata_s *ad)
{
	char buf[PATH_MAX];

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_wm_desktop_layout_support_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);
	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);

	Evas_Object *background = elm_bg_add(ad->win);
	elm_win_resize_object_add(ad->win, background);
	snprintf(buf, sizeof(buf), "%s/bg.png", app_get_resource_path());
	elm_bg_file_set(background, buf, NULL);
	evas_object_size_hint_weight_set(background, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	/* Get window dimensions */
	elm_win_screen_size_get (ad->win, NULL, NULL, &screen_width, &screen_height);

	/* add hole to window */
	ad->hole = evas_object_image_filled_add(evas_object_evas_get(ad->win));
	snprintf(buf, sizeof(buf), "%s/hole.png", app_get_resource_path());
	evas_object_image_file_set(ad->hole, buf, NULL);
	evas_object_resize(ad->hole, BALL_DIAMETER, BALL_DIAMETER);

	/* add ball to window */
	ad->ball = evas_object_image_filled_add(evas_object_evas_get(ad->win));
	snprintf(buf, sizeof(buf), "%s/ball.png", app_get_resource_path());
	evas_object_image_file_set(ad->ball, buf, NULL);
	evas_object_resize(ad->ball, BALL_DIAMETER, BALL_DIAMETER);

	/* show evas objects */
	evas_object_show(background);
	evas_object_show(ad->hole);
	evas_object_show(ad->ball);

	/* initialize ball velocity */
	ad->velocity_x = 0;
	ad->velocity_y = 0;

	start_game(ad);

	register_accelerometer_callback(ad);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, keydown_cb, NULL);
}

static bool
app_create(void *data)
{
	appdata_s *ad = data;
	create_base_gui(ad);
	return true;
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

int
main(int argc, char *argv[])
{
	appdata_s *ad = {0,};
	app_event_callback_s event_callback;

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = NULL;
	event_callback.language_changed = NULL;

	event_callback.low_memory = NULL;
	event_callback.low_battery = NULL;
	event_callback.device_orientation = NULL;
	event_callback.region_format_changed = NULL;

	memset(&ad, 0x0, sizeof(appdata_s));

	int ret = app_efl_main(&argc, &argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
	    // LOGE("app_efl_main() is failed. err = %d", ret);
	}

	return ret;
}
