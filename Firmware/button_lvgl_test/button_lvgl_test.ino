// Number for button pins
const int upBtn = 5;
const int downBtn = 11;
const int rightBtn = 7; 
const int leftBtn = 4;
const int homeBtn = 10;

// Variables for reading pushbutton status
int upBtnState = 0;
int downBtnState = 0;
int rightBtnState = 0;
int leftBtnState = 0;
int homeBtnState = 0;

//Variable for debouncing 
unsigned long lastISRmillis = 0L;

void ui_setup(){
  // Create a group and add widgets to it so they can be selected by the keypad
  lv_group_t *g = lv_group_create();
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
  lv_indev_set_read_cb(indev, keyboard_read);
  lv_indev_set_group(indev, g);
  
  // Assign the keypad input device to the group
  lv_indev_set_group(indev_keypad, g);

  // Create a button
  lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
  // ... add position, size, and event callback ...
  lv_obj_add_event_cb(btn1, button_event_cb, LV_EVENT_CLICKED, NULL);

  // Add the button to the group
  lv_group_add_obj(g, btn1);

  // Create another button
  lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
  // ... add position, size, and event callback ...
  lv_obj_add_event_cb(btn2, button_event_cb, LV_EVENT_CLICKED, NULL);

  // Add the second button to the group
  lv_group_add_obj(g, btn2);
}

void pin_setup(){
// Initialize button pins as input
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(rightBtn, INPUT);
  pinMode(leftBtn, INPUT);
  pinMode(homeBtn, INPUT);

  attachInterrupt(digitalPinToInterrupt(upBtn), button_pressed_up, FALLING);
  attachInterrupt(digitalPinToInterrupt(downBtn), button_pressed_down, FALLING);
  attachInterrupt(digitalPinToInterrupt(rightBtn), button_pressed_right, FALLING);
  attachInterrupt(digitalPinToInterrupt(leftBtn), button_pressed_left, FALLING);
  attachInterrupt(digitalPinToInterrupt(homeBtn), button_pressed_home, FALLING);
}

/* TODO */
void keyboard_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if(key_pressed()) {
    data->key = my_last_key();            /* Get the last pressed or released key */
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

/* TODO */
void loop(){
  uint8_t my_last_key(void)
  {
    if (upBtnState == LOW && digitalRead(upBtn)) {
      upBtnState = digitalRead(upBtn);
      lv_indev_add_event_cb(indev,)
    }
  }

  upBtnState = digitalRead(upBtn);
  downBtnState = digitalRead(downBtn);
  rightBtnState = digitalRead(rightBtn);
  leftBtnState = digitalRead(leftBtn);
  homeBtnState = digitalRead(homeBtn);
  

}

void button_pressed_up(void){
  if (millis() > lastISRmillis + 50)//50 mS debounce time
  {
    upBtnState = 1;//set flag
    lastISRmillis = millis();
    button_event_cb(*btn1);
  }
}
void button_pressed_down(void){
  if (millis() > lastISRmillis + 50)//50 mS debounce time
  {
    downBtnState = 1;//set flag
    lastISRmillis = millis();
    button_event_cb(*btn1);
  }
}
void button_pressed_right(void){
  if (millis() > lastISRmillis + 50)//50 mS debounce time
  {
    rightBtnState = 1;//set flag
    lastISRmillis = millis();
    button_event_cb(*btn1);
  }
}
void button_pressed_left(void){
  if (millis() > lastISRmillis + 50)//50 mS debounce time
  {
    leftBtnState = 1;//set flag
    lastISRmillis = millis();
    button_event_cb(*btn1);
  }
}
void button_pressed_home(void){
  if (millis() > lastISRmillis + 50)//50 mS debounce time
  {
    homeBtnState = 1;//set flag
    lastISRmillis = millis();
    button_event_cb(*btn1);
  }
}
/* TODO */
void button_event_cb(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);

  /* FOCUS ON CLICKING!!! */
  if(code == LV_EVENT_CLICKED) {  // Select button was pressed, change screen based on button selection
    
  }

  /* LATER PROBLEMS */
  if(code == LV_EVENT_FOCUSED) {  // Scroll onto a button -> turn button dark
    
  }

  if(code == LV_EVENT_DEFOCUSED) {  // Scroll off of a button -> turn button light
    
  }
}