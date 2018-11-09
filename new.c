
// Author  - Manan Patel
// Assignment 2 CAB202 2017 
#include "lcd_model.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <macros.h>
#include <graphics.h>
#include <sprite.h>
#include <math.h>
#include "usb_serial.h"


#define W 8
#define H 9
#define zombie_w 9
#define zombie_h 7
#define key_w 8
#define key_h 3 
#define castle_w 80
#define castle_h 24
#define door_w 10
#define door_h 11
#define door2_w 10
#define door2_h 11
#define treasure_w 7
#define treasure_h 4
#define mapx_border_w 8
#define mapx_border_h 10000
#define mapy_border_w 20000
#define mapy_border_h 8


int minutes = 0;
int seconds = 0;
char buffer[20];

int level = -1 ;
int lives = 3 ;
int score = 0;
volatile uint8_t key_holder = 0; //key is not with hero
volatile uint8_t new_game = 0;
 int floor_setup = 1;
volatile int time_counter = 0;


void draw_double(uint8_t x, uint8_t y, double value, colour_t colour);
void draw_int( uint8_t x, uint8_t y, int value, colour_t colour );

uint8_t bitmap[] = {
 0b01000010,
 0b01111110,
 0b00111100,
 0b00011000,
 0b11111111,
 0b10100101,
 0b10111101,
 0b00100100,
 0b00100100,
  

};

uint8_t bitmap_zombie[] = {
       0b00111111,   0b00000000,
       0b01111111,   0b10000000,
       0b01001100,   0b10000000,
       0b01111111,   0b10000000,
       0b01111111,   0b10000000,
       0b01001100,   0b10000000,
       0b01001100,   0b10000000,
 };


uint8_t bitmap_key[] = {

       0b11100000,  
       0b10111111,  
       0b11100101,  
};



uint8_t bitmap_castle[] = {
 
            
  0b11111000,  0b01111100,  0b00111110,  0b01111100,  0b00111110,  0b01111100,  0b00111110,  0b01111100,  0b00111110,  0b00011111,  
  0b11001000,  0b01000100,  0b00100010,  0b01000100,  0b00100010,  0b01000100,  0b00100010,  0b01000100,  0b00100010,  0b00010011,
  0b11001111,  0b11000111,  0b11100011,  0b11000111,  0b11100011,  0b11000111,  0b11100011,  0b11000111,  0b11100011,  0b11110011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00011111,  0b10000000,  0b00001111,  0b11000000,  0b00000000,  0b11111100,  0b00000000,  0b01111110,  0b00000011,
  0b11000000,  0b00011111,  0b10000000,  0b00001111,  0b11000000,  0b00000000,  0b11111100,  0b00000000,  0b01111110,  0b00000011,
  0b11000000,  0b00011111,  0b10000000,  0b00001111,  0b11000000,  0b00000000,  0b11111100,  0b00000000,  0b01111110,  0b00000011,
  0b11000000,  0b00011111,  0b10000000,  0b00001111,  0b11000000,  0b00000000,  0b11111100,  0b00000000,  0b01111110,  0b00000011,
  0b11000000,  0b00011111,  0b10000000,  0b00001111,  0b11000000,  0b00000000,  0b11111100,  0b00000000,  0b01111110,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000011,
  0b11000000,  0b01111111,  0b10000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b11111111,  0b00000011,
  0b11000000,  0b01000000,  0b10000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b10000001,  0b00000011,
  0b11000000,  0b01000000,  0b10000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b10000001,  0b00000011,
  0b11000000,  0b01111111,  0b10000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b11111111,  0b00000011,
  0b11000000,  0b01000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b10000000,  0b00000011,                                    
  0b11000000,  0b01000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b00000000,  0b10000000,  0b00000011,
  0b11111111,  0b11111111,  0b11111111,  0b11111111,  0b11111111,  0b11111111,  0b11111111,  0b11111111,  0b11111111,  0b11111111,

  };

  uint8_t bitmap_door[] = {
  0b00001110,0b00000000,
  0b00111111,0b10000000,
  0b01110001,0b11000000,
  0b11100000,0b11000000,
  0b11000000,0b11000000,
  0b11000110,0b11000000,
  0b11000110,0b11000000,
  0b11000000,0b11000000,
  0b11000000,0b11000000,
  0b11000000,0b11000000,
  0b11000000,0b11000000,
};   

uint8_t bitmap_treasure[]= 
{

     0b00011000,
     0b00111100,
     0b01111100,
     0b11111110,

};


uint8_t mapx_border[] = {
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
};


uint8_t mapy_border[] = {
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
};

Sprite hero;
Sprite zombie;
Sprite key;
Sprite castle;
Sprite door ;
Sprite treasure ;
Sprite left_border;
Sprite right_border;
Sprite top_border;
Sprite bottom_border;



void draw_border_scrolling(void){
   
   sprite_draw(&top_border);
  sprite_draw(&left_border);
  sprite_draw(&bottom_border);
  sprite_draw(&right_border);
 
}


void draw_border(void) 
{
   
  draw_line(0, 0, LCD_X - 1, 0, FG_COLOUR);
  draw_line(0, LCD_Y- 1, LCD_X, LCD_Y- 1, FG_COLOUR);
  draw_line(0, 0, 0, LCD_Y-1, FG_COLOUR);
  draw_line(LCD_X-1, 0, LCD_X-1, LCD_Y-1, FG_COLOUR);
  
}

void name_number(void) 
{
   draw_string(14, 15, "Manan Patel ", FG_COLOUR);
  draw_string(19, 25, "n9839950", FG_COLOUR);

}

void setup_usb_serial( void );
void usb_serial_send_int(int value);
void usb_serial_send(char * message);

void moving_message(void)
{
  usb_serial_send("Location: (");
  usb_serial_send_int((int)hero.x);
  usb_serial_putchar(',');
  usb_serial_send_int((int)hero.y);
  usb_serial_send(") ");
  usb_serial_send("Score: ");
  usb_serial_send_int(score);
  usb_serial_send(" Lives: ");
  usb_serial_send_int(lives);
  usb_serial_send(" Level: ");
  usb_serial_send_int(level);
  usb_serial_send(")\r\n");

}

void setup_hero(void) {

  sprite_init(&hero, (LCD_X - 35), (LCD_Y - 12), W, H, bitmap);
}

void setup_zombie(void) {
  sprite_init(&zombie ,(LCD_X - 13),(LCD_Y - 12) , zombie_w, zombie_h, bitmap_zombie ); 
}

void setup_key(void) {
  sprite_init(&key ,(LCD_X - 77) , (LCD_Y - 23) , key_w , key_h , bitmap_key);

}

void setup_castle(void) {

  sprite_init(&castle , 2 ,0, castle_w , castle_h , bitmap_castle);
}

void setup_door(void) 
{
  sprite_init(&door ,LCD_X /2 - door_w + 5, LCD_Y/2 - door_h , door_w,  door_h , bitmap_door);
}

void setup_treasure(void)
{

  sprite_init(&treasure ,(LCD_X - 77) , (LCD_Y - 15) , treasure_w , treasure_h , bitmap_treasure);

}

void setup_border_scorlling(void)
{
  
  sprite_init(&left_border,-8,0,8,83,mapy_border);
  
  sprite_init(&top_border,-8,-8,143,8,mapx_border);
  
  sprite_init(&right_border,127,0,8,83,mapy_border);
  
  sprite_init(&bottom_border,-8,83,143,8,mapy_border);

}


void loading_screen(void){
  if (new_game == 1){
  for(int i = 3; i > 0;i--){
    //clear_screen();
    
    draw_string(2, 12, "Loading...", FG_COLOUR);
    draw_string(2,24, "Score = " , FG_COLOUR);
    draw_double( 40 ,24 , score , FG_COLOUR);
     draw_string ( 2 ,32 , "level = " , FG_COLOUR);
    draw_double(40 , 32 , level , FG_COLOUR) ;
    show_screen();
    _delay_ms(1000);
    clear_screen();
     level++;
     _delay_ms(100);

   
        
        show_screen();

  }
  }
  } 

void countDown(void){
  for(int i = 3; i > 0;i--){
    //clear_screen();
    draw_int(LCD_X/2, LCD_Y/2, i, FG_COLOUR);
    draw_string(8, 12, "Ready to Play?", FG_COLOUR);
    show_screen();
    _delay_ms(333);
    clear_screen();
    level = 1;

      }  

  }


int elapsed_time(){
  seconds = (time_counter * 65536.0 + TCNT1) * (1.0) /(8000000.0);
  return seconds;
}

ISR(TIMER1_OVF_vect){
  time_counter++;
  seconds = elapsed_time();
  if(seconds == 60){
    minutes ++;
    seconds = 0;
    time_counter = 0;
  }
}


int get_seconds(){
  return seconds;
}
int get_minutes(){
  return minutes;
}


void Rsetup_hero(void){
    int hx = rand() % (LCD_X - H - 1);
    int hy = rand() % (LCD_Y - W - 1);
    sprite_init(&hero, hx, hy, W, H, bitmap);
    sprite_draw(&hero);
    
}

void Rsetup_zombie(void){
    int mx = rand() % (LCD_X - zombie_w - 1);
    int my = rand() % (LCD_Y - zombie_h - 1);
    sprite_init(&zombie, mx, my, zombie_w, zombie_h, bitmap_zombie);
    sprite_draw(&zombie);
    
}

void Rsetup_key(void){
    int kx = rand() % (LCD_X - key_w - 1);
    int ky = rand() % (LCD_Y - key_h - 1);
    sprite_init(&key, kx, ky, key_w, key_h, bitmap_key);
    sprite_draw(&key);
    
}

void Rsetup_door(void){
    int dx = rand() % (LCD_X - door_w - 1);
    int dy = rand() % (LCD_Y - door_h - 1);
    sprite_init(&door, dx, dy, door_h, door_w, bitmap_door);
    sprite_draw(&door);
    
}

void Rsetup_treasure(void){
    int tx = rand() % (LCD_X - treasure_w - 1);
    int ty = rand() % (LCD_Y - treasure_h - 1);
    sprite_init(&treasure, tx, ty, treasure_w, treasure_h, bitmap_treasure);
  sprite_draw(&treasure);
   
}



void setup(void) {
  
  set_clock_speed(CPU_8MHz);
  lcd_init(LCD_DEFAULT_CONTRAST);
    if (floor_setup == 1)
        { 
          setup_hero();
         setup_castle();
          setup_door();
         setup_key();
         setup_zombie();
        }

    if (floor_setup == 2 )
    {   
         setup_hero();
     setup_door();
         setup_key();
         setup_zombie();
         setup_treasure();
    }   

    TCCR1A = 0;
    TCCR1B = 1;
    TIMSK1 = 1;
     sei();
    show_screen();

    CLEAR_BIT(DDRF, 5); // up
    CLEAR_BIT(DDRF, 6); // left_button
    CLEAR_BIT(DDRD, 1); // joy stick up
    CLEAR_BIT(DDRB, 7); // joy stick down
    CLEAR_BIT(DDRB, 1); // joy stick left
    CLEAR_BIT(DDRD, 0); // joy stick right 
    CLEAR_BIT(DDRB,0);

      
      } // END OF SETUP



void hero_movement (void) 
{
   int x = round(hero.x);
  int y = round(hero.y);

  int door_x = LCD_X /2 - door_w + 5 ;


  if (floor_setup == 1)
  {
     if (round (hero.y < castle_h && key_holder == 0))
   
    {
      hero.y = castle_h ;
    }     

  if ((round(hero.x) <= door_x) && (round(hero.y) < castle_h) ) 
  {
          hero.y = castle_h ;
  } 

  if ((round(hero.x) >= door_x ) && ((round(hero.y) < castle_h))) 
  {
    hero.y = castle_h ;
  } 
  }


  if ( x < 0 || x + W >= LCD_X ) hero.dx = 0;
  if ( y < 0 || y + H >= LCD_Y ) hero.dy = 0;
  if (x < 1) {
    hero.x = 1;
    hero.dx = 0;
  }

  if ((hero.x + W) >= LCD_X - 1) {
    hero.x = LCD_X - W -1;
    hero.dx = 0;
  }

  if (hero.y < 1){
    hero.y = 1;
    hero.dy = 0;
  }

  if((hero.y + H)>= LCD_Y - 1) {
    hero.y = LCD_Y - H-1;
    hero.dy = 0;
  }


  if(BIT_IS_SET(PIND,1)){
    hero.dx = 0;
    hero.y -= 1;
  } 

  if(BIT_IS_SET(PINB,7)){
    hero.dx = 0;
    hero.y += 1;
  } 

  if(BIT_IS_SET(PINB,1)){
    hero.x -= 1;
  } 

  if(BIT_IS_SET(PIND,0)){
    hero.x += 1;
  } 


  if (usb_serial_available()){
    int c = usb_serial_getchar();
    if ( c == 'a' ) hero.x = -1;
    if ( c == 'd' ) hero.x = +1;
    if ( c == 's' ) hero.y = +1;
    if ( c == 'w' ) hero.y = -1;
  }

  moving_message();
} // END OF HERO MOVEMENT 


typedef enum { false, true } bool; 

 
  bool collisionmosture(void) {
  bool collided = true;
  
  
    int hx = round(hero.x);
  int hy = round(hero.y);

 
  int hr = hx + W - 1;
  int hb = hy + H - 1;
  
  int mx = round(zombie.x);
  int my = round(zombie.y);
  int mr = mx + zombie_w - 1;
  int mb = my + zombie_h - 1;
  
  if (hr < mx ) { collided = false; }
  else if (hb < my ) { collided = false; }
  
  else if (mr < hx ) { collided = false; }
    else if (mb < hy ) { collided = false; }
  
  return collided;
} // END OF COLLISION DETECTION WITH MONSTER


bool collision_key(void) {
  bool collided = true;
  
  
    int hx = round(hero.x);
  int hy = round(hero.y);

 
  int hr = hx + W - 1;
  int hb = hy + H - 1;
  
  int kx = round(key.x);
  int ky = round(key.y);
  int er = kx + key_w - 1;
  int eb = ky + key_h - 1;
  
  if (hr < kx ) { collided = false; }
  else if (hb < ky ) { collided = false; }
  
  else if (er < hx ) { collided = false; }
    else if (eb < hy ) { collided = false; }
  
  return collided;

} // END OF COLLISION DETECTION WITH KEY


bool collision_door(void) {
 
  bool collided = true;
  
  int hx = round(hero.x);
  int hy = round(hero.y);
  int hr = hx + W - 1;
  int hb = hy + H - 1;
  
  int tx = round(door.x);
  int ty = round(door.y);
  int tr = tx + door_w - 1;
  int tb = ty + door_h - 1;
  
  if (hr < tx ) { collided = false; }
  else if (hb < ty ) { collided = false; }
  
  else if (tr < hx ) { collided = false; }
    else if (tb < hy ) { collided = false; }
  
  return collided;

} //END OF COLLISION WITH DOOR


bool collision_treasure(void) {
 
  bool collided = true;
  
  int hx = round(hero.x);
  int hy = round(hero.y);
  int hr = hx + W - 1;
  int hb = hy + H - 1;
  
  int ax = round(treasure.x);
  int ay = round(treasure.y);
  int ar = ax + treasure_w - 1;
  int ab = ay + treasure_h - 1;
  
  if (hr < ax ) { collided = false; }
  else if (hb < ay ) { collided = false; }
  
  else if (ar < hx ) { collided = false; }
    else if (ab < hy ) { collided = false; }
  
  return collided;

} //END OF COLLISION WITH DOOR

bool border_collision(Sprite * sprite1 ,Sprite * sprite2){
  bool collided = true;

  int sprite1_x = round(sprite1->x);
  int sprite1_y = round(sprite1->y);
  int sprite1_r = sprite1_x + sprite1->width - 1;
  int sprite1_b = sprite1_y + sprite1->height - 1;

  int sprite2_x = round(sprite2->x);
  int sprite2_y = round(sprite2->y);
  int sprite2_r = sprite2_x + sprite2->width - 1;
  int sprite2_b = sprite2_y + sprite2->height - 1;

  if (sprite1_r < sprite2_x + 1) collided = false;
  if (sprite1_b < sprite2_y - 1) collided = false;

  if (sprite2_r + 1 < sprite1_x) collided = false;
  if (sprite2_b - 1 < sprite1_y) collided = false;

  return collided;
}


void status_screen(){

  while(BIT_IS_SET(PINB, 0)){
    int sec = get_seconds();
    int min = get_minutes();
    clear_screen();
    sprintf(buffer,"Time: %.2d:%.2d",min,sec);
    draw_string(8,0,buffer,FG_COLOUR);
    sprintf(buffer,"Score: %d",score);
    draw_string(8,10,buffer,FG_COLOUR);
    sprintf(buffer,"Lives: %d",lives);
    draw_string(8,20,buffer,FG_COLOUR);
    sprintf(buffer,"Floor: %d",level);
    draw_string(8,30,buffer,FG_COLOUR);
    show_screen();
}
}




void map_scorlling(void){
 
  if(level > 4){
    int dx = 0, dy = 0;
    hero.x = LCD_X/2 - W - 1;
    hero.y = LCD_Y/2 - H - 1;

    
    if ( BIT_IS_SET(PIND, 1) ) dy = 1; // Up
    if ( BIT_IS_SET(PINB, 7) ) dy = -1; // Down
    if ( BIT_IS_SET(PINB, 1) ) dx = 1; // Left
    if ( BIT_IS_SET(PIND, 0) ) dx = -1; // Right

    
    if(border_collision(&hero,&top_border) && BIT_IS_SET(PIND, 1))
    {
      dy = -1;
    }
    if(border_collision(&hero,&bottom_border) && BIT_IS_SET(PINB, 7))
    {
      dy = 1;
    }
    if(border_collision(&hero,&left_border) && BIT_IS_SET(PINB, 1))
    {
      dx = -1;
    }
    if(border_collision(&hero,&right_border) && BIT_IS_SET(PIND, 0))
    {
      dx = 1;
    }




    if( dx || dy ){
      if(key_holder == 0)
      {
        key.x += dx;
        key.y += dy;
      }

      door.x += dx;
      door.y += dy;
      zombie.x += dx;
      zombie.y += dy;
      treasure.x += dx;
      treasure.y += dy;
      left_border.x += dx;
      left_border.y += dy;
      top_border.x += dx;
      top_border.y += dy;
      right_border.x += dx;
      right_border.y += dy;
      bottom_border.x += dx;
      bottom_border.y += dy;
      sprite_draw(&key);
      sprite_draw(&door);
      sprite_draw(&treasure);
      sprite_draw(&zombie);
      
      draw_border_scrolling();
      show_screen();
    }
  }
  else return; 
}



void monster_move(void)
{

  new_game = 0;
  float mdx = hero.x - zombie.x;
  float mdy = hero.y - zombie.y;
  float dist = sqrt(mdx * mdx + mdy * mdy);
  mdx = mdx * 0.2/dist;
  mdy = mdy * 0.2/dist;
  // Move monster
  zombie.x += mdx;
  zombie.y += mdy;
  sprite_draw(&zombie);

  //show_screen();
}




 
void process (void){

      hero_movement();

 


              
      if (lives == 0 )
    {   
       lcd_clear();
       clear_screen();
       draw_string(20,10,"Game over!",FG_COLOUR);
     draw_string(20,20,"Score: ",FG_COLOUR);
     draw_int(50,20,score,FG_COLOUR);
     draw_string(20,30,"Floor: ",FG_COLOUR);
      draw_int(50,30,level,FG_COLOUR);
       show_screen();

       if (BIT_IS_SET(PINF, 5) || BIT_IS_SET(PINF, 6) )
       {
          clear_screen();
          level = -1 ;
          lives = 3 ;


          if (level == -1){
     draw_border();
     name_number();
     show_screen();
     level ++; ;
     }

   if (level == 0)
   {
    clear_screen();
   if (BIT_IS_SET(PINF, 5) || BIT_IS_SET(PINF, 6) )
    {countDown();}

    }
    
  if (level == 1)  
  {
         draw_border();
         sprite_draw(&hero);
         sprite_draw(&castle);
         sprite_draw(&zombie);
         sprite_draw(&door);
         sprite_draw(&key);
         monster_move();
         show_screen(); 

       clear_screen();
        if (collisionmosture())
    {
      setup();
      usb_serial_send("Player is killed by monster \n");
       lives -- ;
       key_holder = 0;
    } 
 
   }


   if (collision_key())
   {
    key_holder = 1 ;   // key is with hero
    usb_serial_send("Player picked up the key\n");
   }


  
   if(key_holder == 1)
   { 
        key.x = hero.x - key_w;
        key.y = hero.y ;
        draw_border();
        sprite_draw(&key);
        sprite_draw(&hero);
        sprite_draw(&castle);
        sprite_draw(&zombie);
        sprite_draw(&door);
        show_screen();

   }

     if (collision_door () && key_holder == 1 && new_game == 0) 
        {
          key_holder = 0;
        
        clear_screen();
        new_game = 1; 
        loading_screen();
        usb_serial_send("Player moved to level ");
        usb_serial_send_int(level);
        usb_serial_send("\n");
       
        clear_screen();

        if (level >= 2 ) {
        
        
        usb_serial_send("Player moved to level ");
        usb_serial_send_int(level);
        key_holder = 0;
        Rsetup_hero();
        Rsetup_treasure();
        setup_border_scorlling();
        Rsetup_door();
        Rsetup_key();
        Rsetup_zombie();
        
        
        floor_setup = 2;
      }
        }
        
    if (collisionmosture())
    {
      setup();
      usb_serial_send("Player is killed by monster \n");
       lives -- ;
       key_holder = 0;
    }

    if (level >= 2 ) 
    {
        
        floor_setup = 2;
          
         clear_screen();
        new_game = 0 ;
        draw_border();
         sprite_draw(&door);    
         sprite_draw(&key);
         draw_border_scrolling();
        sprite_draw(&hero);
        map_scorlling();
       sprite_draw(&treasure);
         
        castle.x = -100;
        castle.y = -199;
     
       
        monster_move();
        
           if (collision_treasure())
           {
                 treasure.x = -100;
                 treasure.y = -100;
                 score += 10;
                 usb_serial_send("Player picked up treasure\n");

           } 
        sprite_draw(&zombie);
        show_screen();
        
      
     }

       }

       show_screen();
       show_screen();

    }        

if (!lives == 0){
   if (level == -1){
     draw_border();
     name_number();
     show_screen();
     level ++; ;
     }

   if (level == 0)
   {
    clear_screen();
   if (BIT_IS_SET(PINF, 5) || BIT_IS_SET(PINF, 6) )
    {countDown();}

    }
    
  if (level == 1)  
  {
         draw_border();
         sprite_draw(&hero);
         sprite_draw(&castle);
         sprite_draw(&zombie);
         sprite_draw(&door);
         sprite_draw(&key);
         monster_move();
         show_screen(); 

       clear_screen();
        if (collisionmosture())
    {
      setup();
      usb_serial_send("Player is killed by monster \n");
       lives -- ;
       key_holder = 0;
    } 
 
   }


   if (collision_key())
   {
    key_holder = 1 ;   // key is with hero
    usb_serial_send("Player picked up the key\n");
   }


  
   if(key_holder == 1)
   { 
        key.x = hero.x - key_w;
        key.y = hero.y ;
        draw_border();
        sprite_draw(&key);
        sprite_draw(&hero);
        sprite_draw(&castle);
        sprite_draw(&zombie);
        sprite_draw(&door);
        show_screen();

   }

     if (collision_door () && key_holder == 1 && new_game == 0) 
        {
          key_holder = 0;
        
        clear_screen();
        new_game = 1; 
        loading_screen();
        usb_serial_send("Player move to level ");
        usb_serial_send_int(level);
        usb_serial_send("\n");
       
        clear_screen();

        if (level >= 2 ) {
        
        
        usb_serial_send("Player move to level ");
        usb_serial_send_int(level);
        key_holder = 0;
        Rsetup_hero();
        Rsetup_treasure();
        setup_border_scorlling();
        Rsetup_door();
        Rsetup_key();
        Rsetup_zombie();
        
        
        floor_setup = 2;
      }
        }
        
    if (collisionmosture())
    {
      setup();
      usb_serial_send("Player is killed by monster \n");
       lives -- ;
       key_holder = 0;
    }

    if (level >= 2 ) 
    {
        
        floor_setup = 2;
          
         clear_screen();
        new_game = 0 ;
        draw_border();
         sprite_draw(&door);    
         sprite_draw(&key);
         draw_border_scrolling();
        sprite_draw(&hero);
        map_scorlling();
       sprite_draw(&treasure);
         
        castle.x = -100;
        castle.y = -199;
     
       
        monster_move();
        
           if (collision_treasure())
           {
                 treasure.x = -100;
                 treasure.y = -100;
                 score += 10;
                 usb_serial_send("Player picked up treasure\n");

           } 
        sprite_draw(&zombie);
        show_screen();
        
      
     }
     

     status_screen();   
     
   }


        
       
}




int main(void) {
  
  setup();
  setup_usb_serial();

      for ( ;; ) {
    process();
    _delay_ms(10);

  }
} // END OF MAIN

// -------------------------------------------------
// Helper functions.
// -------------------------------------------------


void draw_double(uint8_t x, uint8_t y, double value, colour_t colour) {
  snprintf(buffer, sizeof(buffer), "%2.0f", value);
  draw_string(x, y, buffer, colour);
}
 
void draw_int(uint8_t x, uint8_t y, int value, colour_t colour) {
  snprintf(buffer, sizeof(buffer), "%d", value );
  draw_string(x, y, buffer, colour);
}

void setup_usb_serial(void) {
  
  lcd_init(LCD_DEFAULT_CONTRAST);
  draw_string(12, 12, "Connect USB...", FG_COLOUR);
  show_screen();

  usb_init();

  while ( !usb_configured() || !usb_serial_get_control()) {

  }

  clear_screen();
  for(int i = 0; i < 3 ; i++)
  {
    draw_string(12, 12, "How are you!", FG_COLOUR);
    _delay_ms(500);
    show_screen();
    clear_screen();
  }
  _delay_ms(500);
}

void usb_serial_send_int(int value) {
  static char buffer[8];
  snprintf(buffer, sizeof(buffer), "%d", value);
  usb_serial_send( buffer );
}

void usb_serial_send(char * message) {
 
  usb_serial_write((uint8_t *) message, strlen(message));
}