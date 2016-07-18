#include <pebble.h>

Window *mainWindow;
TextLayer *valueLayer;
int currentValue = 0;
int totalValue = 0;
char valueString[1][40];

typedef enum LastAction {
  NONE,
  UP,
  DOWN
} LastAction;

LastAction lastAction = NONE;


void setup_text_layer(Window *window) {
  valueLayer = text_layer_create(GRect(0, 60, 144, 48));
  text_layer_set_text_alignment(valueLayer, GTextAlignmentCenter);
  text_layer_set_font(valueLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(valueLayer, "0");
  layer_add_child(window_get_root_layer(window), 
                  text_layer_get_layer(valueLayer));
}

void destroy_text_layer() {
  text_layer_destroy(valueLayer);
}

void update_text_value() {
  APP_LOG(APP_LOG_LEVEL_INFO, "current value updated to %d, total = %d", currentValue, totalValue);
  snprintf(valueString[0], sizeof(valueString[0]), "%d", currentValue + totalValue);
  text_layer_set_text(valueLayer, valueString[0]);
}

void increment() {
  if (currentValue < -1) {
    currentValue /= 2;
  } else if (currentValue == -1) {
    currentValue = 0;
  } else if (currentValue == 0) { 
    currentValue = 1;
  } else {
    currentValue *= 2;
  }
}

void decrement() {
  if (currentValue < 0) {
    currentValue *= 2;
  } else if (currentValue == 0) {
    currentValue = -1;
  } else if (currentValue == 1) {
    currentValue = 0;
  } else {
    currentValue /= 2;
  }
}

void increment_next() {
  totalValue += currentValue;
  currentValue = 1;
}

void decrement_next() {
  totalValue += currentValue;
  currentValue = -1;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  totalValue = 0;
  currentValue = 0;
  update_text_value();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  switch (lastAction) {
    case NONE: increment(); break;
    case UP: increment(); break;
    case DOWN: increment_next(); break;
  }
  lastAction = UP;
  update_text_value();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  switch (lastAction) {
    case NONE: decrement(); break;
    case DOWN: decrement(); break;
    case UP: decrement_next(); break;
  }
  lastAction = DOWN;
  update_text_value();
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
  
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

void main_window_load(Window *window) {
  setup_text_layer(window);
}

void main_window_unload(Window *window) {
  destroy_text_layer();
}
  

void main_window_create() {
  mainWindow = window_create();
  window_set_window_handlers(mainWindow, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_click_config_provider(mainWindow, click_config_provider);
}

void main_window_destroy() {
  window_destroy(mainWindow);
}

int main() {
  main_window_create();
  window_stack_push(mainWindow, true);
  app_event_loop();
  main_window_destroy();
}
