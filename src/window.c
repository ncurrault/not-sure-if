#include "window.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_fry_image;
static GFont s_res_gothic_18_bold;
static BitmapLayer *s_bitmaplayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_PLATFORM_BASALT
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_fry_image = gbitmap_create_with_resource(RESOURCE_ID_FRY_IMAGE);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_fry_image);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(-1, 130, 146, 27));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text_color(s_textlayer_2, GColorWhite);
  text_layer_set_text(s_textlayer_2, "or 00:00 PM");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_2, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 2, 141, 42));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "Not sure if 00:00 AM");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_1);
  gbitmap_destroy(s_res_fry_image);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_window(void) {
  initialise_ui();
	
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_window(void) {
  window_stack_remove(s_window, true);
}

// This function is (lovingly) ripped off from the "Simplicity" watchface
char * get_time_str (struct tm * tick_time)
{
	// Need to be static because it's used by the system later.
	static char time_text[] = "00:00";
	
	char * time_format = "%I:%M";
	
	strftime(time_text, sizeof(time_text), time_format, tick_time);

	// Kludge to handle lack of non-padded hour format string
	// for twelve hour clock.
	if (time_text[0] == '0')
	{
		//memmove(time_text, &time_text[1], sizeof(time_text) - 1);
		time_text[0] = ' ';
	}

	return time_text;
}

void handle_minute_tick(struct tm * tick_time, TimeUnits units_changed)
{
	char * to_print = "Not sure if 00:00 AM";
	char * clock_time = get_time_str(tick_time);
	
	for (int i=0;i<5;i++)
	{
		to_print[12+i] = clock_time[i]; // The '00:00' starts at 12
	}
	text_layer_set_text(s_textlayer_1,to_print);
	
	char * to_print_bottom = "or 00:00 PM";
	for (int i=0;i<5;i++)
	{
		to_print_bottom[3+i] = clock_time[i]; // The '00:00' starts at 3
	}
	text_layer_set_text(s_textlayer_2, to_print_bottom);
	
}

void handle_init()
{
	show_window();
	
	time_t* theCurrentTime = malloc(sizeof(*theCurrentTime));
	time(theCurrentTime);
	handle_minute_tick(localtime(theCurrentTime), MINUTE_UNIT);
	
	tick_timer_service_subscribe(MINUTE_UNIT,handle_minute_tick);	
}
void handle_deinit()
{
	//destroy_ui();
	tick_timer_service_unsubscribe();
}
int main(void)
{
	handle_init();
	app_event_loop();
	handle_deinit();
}