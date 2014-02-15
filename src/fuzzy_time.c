#include "pebble.h"
#include "num2words.h"

#define BUFFER_SIZE 24
	static GColor backcol=GColorBlack;
	static GColor textcol=GColorWhite;
static bool initseq = false;
static char* old;
static char* old2;
static char* old3;
static char* old3;
static char* old4;
static char* old_datum;
static char* battery_text="B:100%";
static char* conn_text="?";
static char* battconn;
static char* old_conn;
static char* old_batt;
static bool battload=false;
static struct CommonWordsData {
  TextLayer *label ;
  TextLayer *label2;
  TextLayer *label3;
  //TextLayer *label4;
  TextLayer *datum;
  TextLayer *conbatt;	
  TextLayer *conn;
	GBitmap *batticon,*bticon;
  BitmapLayer *battlayer;
  BitmapLayer *btlayer;	
  Window *window;
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char buffer3[BUFFER_SIZE];
  char buffer4[BUFFER_SIZE];
  //char* old;
  //char* old2;
  //char* old3;
  //char* old4;
  char date_text;
  char* batt;
	char* oldbatt;
} s_data;

static void handle_battery(BatteryChargeState charge_state) {
battery_text="100%";

  if (charge_state.is_charging) {
	 if (!battload) vibes_short_pulse();
    battery_text="Lade";
	  battload=true;
	  s_data.batticon=gbitmap_create_with_resource(RESOURCE_ID_ICON_BATT_LADE);
	  //bitmap_layer_set_bitmap(s_data.battlayer,s_data.batticon);
  } else {
	battload=false;
	  //if (charge_state.charge_percent>80)  s_data.batticon=gbitmap_create_with_resource(RESOURCE_ID_batt_100);
	  //if (charge_state.charge_percent>60)  s_data.batticon=gbitmap_create_with_resource(RESOURCE_ID_batt_80);
	   //if (charge_state.charge_percent>40)  s_data.batticon=gbitmap_create_with_resource(RESOURCE_ID_batt_60);
	   //if (charge_state.charge_percent>20)  s_data.batticon=gbitmap_create_with_resource(RESOURCE_ID_batt_40);
	   //if (charge_state.charge_percent<=20)  s_data.batticon=gbitmap_create_with_resource(RESOURCE_ID_batt_20);
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  	text_layer_set_text(s_data.conbatt, battery_text);
	 //bitmap_layer_set_bitmap(s_data.battlayer,s_data.batticon);
	//layer_set_hidden(bitmap_layer_get_layer(s_data.battlayer),false);
}
static void update_time(struct tm* t) {
	static char datum_text[] = "xx xx.xx.xxxx ************* (KW xx)";
	 strftime(datum_text,sizeof(datum_text), "%a %e.%m.%g KW%V", t);
  	fuzzy_time_to_words(t->tm_hour, t->tm_min, s_data.buffer, s_data.buffer2, s_data.buffer3, s_data.buffer4, BUFFER_SIZE);
		
		if (old!=s_data.buffer) text_layer_set_text(s_data.label, s_data.buffer);
	    if (old!=s_data.buffer) old=s_data.buffer;
	
		if (old2!=s_data.buffer2) text_layer_set_text(s_data.label2, s_data.buffer2);
	    if (old2!=s_data.buffer2) old2=s_data.buffer2;
	
		if (old3!=s_data.buffer3) text_layer_set_text(s_data.label3, s_data.buffer3);
	    if (old3!=s_data.buffer3) old3=s_data.buffer3;
  		
		//if (old4!=s_data.buffer4) text_layer_set_text(s_data.label4, s_data.buffer4);
		//if (old4!=s_data.buffer4) old4=s_data.buffer4;
  	 text_layer_set_text(s_data.datum, datum_text);
	
  		//if (datum_text!=old_datum) text_layer_set_text(s_data.datum, datum_text);
		if (datum_text!=old_datum) old_datum=datum_text;
	
	
	
		//s_data.oldbatt=s_data.batt;
	
}
static void handle_bluetooth(bool connected) {
	conn_text="?";
	if (connected) {
		snprintf(conn_text, sizeof(conn_text), "C");
	} else {
		snprintf(conn_text, sizeof(conn_text), "X");
	}
	  text_layer_set_text(s_data.conn, conn_text);
	if (initseq!=true) vibes_double_pulse();
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    update_time(tick_time);
  
}

static void do_init(void) {
	initseq=true;
  s_data.window = window_create();
  const bool animated = true;
  window_stack_push(s_data.window, animated);

  window_set_background_color(s_data.window, backcol);
  GFont font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TEXTFONT_24));
  GFont boldfont=fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BOLDFONT_32));
  GFont datumfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SMALLFONT_14));
  Layer *root_layer = window_get_root_layer(s_data.window);
  GRect frame = layer_get_frame(root_layer);
  s_data.battlayer = bitmap_layer_create(GRect(3, frame.size.h-60, frame.size.w-3, frame.size.h));
  s_data.btlayer = bitmap_layer_create(GRect(3,frame.size.h-60,frame.size.w-3,frame.size.h));
  s_data.label = text_layer_create(GRect(3, 3, frame.size.w-3, 38));
  s_data.label2 = text_layer_create(GRect(3,39,frame.size.w-3,74));
  s_data.label3 = text_layer_create(GRect(3,75,frame.size.w-3,110));
 // s_data.label4 = text_layer_create(GRect(0,105,frame.size.w,140));
  s_data.datum = text_layer_create(GRect(3,frame.size.h-30,frame.size.w-3,frame.size.h-15));
  s_data.conbatt = text_layer_create(GRect(3,frame.size.h-15,frame.size.w-3,frame.size.h));
  s_data.conn = text_layer_create(GRect(3,frame.size.h-15,frame.size.w-3,frame.size.h));
  text_layer_set_background_color(s_data.label, backcol);
  text_layer_set_background_color(s_data.label2,backcol);
  text_layer_set_background_color(s_data.label3,backcol);
  //text_layer_set_background_color(s_data.label4,GColorBlack);
  text_layer_set_background_color(s_data.datum,GColorClear);
  text_layer_set_background_color(s_data.conbatt,GColorClear);
  text_layer_set_background_color(s_data.conn,GColorClear);
  text_layer_set_text_color(s_data.label, textcol);
  text_layer_set_text_color(s_data.label2, textcol);
  text_layer_set_text_color(s_data.label3, textcol);
  //text_layer_set_text_color(s_data.label4, GColorWhite);
  text_layer_set_text_color(s_data.datum,textcol);
  text_layer_set_text_color(s_data.conbatt,textcol);
  text_layer_set_text_color(s_data.conn,textcol);

  text_layer_set_font(s_data.label, font);
	text_layer_set_font(s_data.label2, font);
	text_layer_set_font(s_data.label3, boldfont);
	//text_layer_set_font(s_data.label4, boldfont);
  text_layer_set_font(s_data.datum,datumfont);
  text_layer_set_font(s_data.conbatt,datumfont);
  text_layer_set_font(s_data.conn,datumfont);
  text_layer_set_text_alignment(s_data.label, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_data.label2, GTextAlignmentLeft);
	 text_layer_set_text_alignment(s_data.label3, GTextAlignmentLeft);
	 //text_layer_set_text_alignment(s_data.label4, GTextAlignmentLeft);
	 text_layer_set_text_alignment(s_data.datum, GTextAlignmentLeft);
	text_layer_set_text_alignment(s_data.conbatt, GTextAlignmentLeft);
	text_layer_set_text_alignment(s_data.conn, GTextAlignmentRight);
  layer_add_child(root_layer, text_layer_get_layer(s_data.label));
	layer_add_child(root_layer, text_layer_get_layer(s_data.label2));
	layer_add_child(root_layer, text_layer_get_layer(s_data.label3));
	//layer_add_child(root_layer, text_layer_get_layer(s_data.label4));
  layer_add_child(root_layer, text_layer_get_layer(s_data.datum));
	layer_add_child(root_layer, text_layer_get_layer(s_data.conbatt));
	layer_add_child(root_layer, text_layer_get_layer(s_data.conn));
	layer_add_child(root_layer, bitmap_layer_get_layer(s_data.battlayer));
	layer_add_child(root_layer,bitmap_layer_get_layer(s_data.btlayer));
 
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);
  
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
	battery_state_service_subscribe(&handle_battery);
 	bluetooth_connection_service_subscribe(&handle_bluetooth);
	handle_battery(battery_state_service_peek());
  handle_bluetooth(bluetooth_connection_service_peek());
	initseq=false;
}

static void do_deinit(void) {
	 tick_timer_service_unsubscribe();
	 battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  window_destroy(s_data.window);
  text_layer_destroy(s_data.label);
	text_layer_destroy(s_data.label2);
	text_layer_destroy(s_data.label3);
	gbitmap_destroy(s_data.batticon);
	gbitmap_destroy(s_data.bticon);
	//text_layer_destroy(s_data.label4);
  text_layer_destroy(s_data.datum);
	text_layer_destroy(s_data.conbatt);
	text_layer_destroy(s_data.conn);
	bitmap_layer_destroy(s_data.battlayer);
	bitmap_layer_destroy(s_data.btlayer);
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}
