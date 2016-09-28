#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <curses.h>

#define DELAY (20) // Millisecond delay between game updates 

bool game_over = false; //Setup game system when true, finish this game
bool update_screen = true; // Set to false to prevent screen update. 


// Call global variable 
	int lives = 10;
    int score = 0;
    int level = 1;
    int time1,time2;
	int d = 1;
	int min=0;
	int timer=0;

// Call sprite image	
char * msg_image =
/**/  "CAB202 Assignment 1 - Pong"
/**/  "  YeongJe Park            "
/**/  "      n8923108            "  
/**/  "Controls:                 "
/**/  "Arrow keys : move up/down "
/**/  "  h: show this help screen"
/**/  "  q:quit game             "
/**/  "  L: cycle levels         "  
/**/  "  Press a key to play...  ";

char * more_image =
/**/  "+--------------------------+"
/**/  "|                          |"
/**/  "|        Game Over         |"
/**/  "|     Play again (y/n)?    |"
/**/  "|                          |"
/**/  "+--------------------------+";

char * right_image =
/**/ "|"
/**/ "|"
/**/ "|"
/**/ "|"
/**/ "|"
/**/ "|"
/**/ "|" ;

char * ping_image = 
/**/ "0";

char * three_image =
/**/ " \\ | /  "
/**/ "  \\|/   "
/**/ "--   --  "
/**/ " /|\\   "
/**/ " / | \\  ";

sprite_id three;
sprite_id msg;
sprite_id more;
sprite_id right;
sprite_id left;
sprite_id ping;

// Show Game Over image and ask "Play again"
void draw_more(void)
{
	clear_screen();
   	more = sprite_create((screen_width()-30)/2 ,(screen_height()-6)/2,28,6,more_image);
   	sprite_draw(more);
    show_screen();	
}
// Show Help image
void draw_msg(void)
{
	int msg_x = (screen_width() - 26) / 2;
    int msg_y = (screen_height() - 9) / 2;
    msg = sprite_create(msg_x, msg_y, 26, 9, msg_image);
    sprite_draw(msg);
    show_screen();
	int key = getchar();

	if (key == 'q' )
	{
		draw_more();
		int key2 =getchar();
		
    	if (key2 =='n')
    	{
    	game_over=true;
     	}
		else if(key2 =='y') 
     	{
	    game_over =false;
		clear_screen();
    	}
	}

	else{
		game_over = false;
		clear_screen();
	}
}
//Show count time 3 > 2 > 1 (counting time is 0.3 second)
void draw_time(void)
{
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +1,    "+---------+" );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +2, "|         |" );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +3, "|    3    |" );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +4, "|         |" );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +5, "+---------+" );
	show_screen();
	timer_pause(0300);

	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +3, "|    2    |" );
	show_screen();
	timer_pause(0300);

	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +3, "|    1    |" );
	show_screen();
	timer_pause(0300);

    draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2+1,    "           " );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +2, "           " );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +3, "           " );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +4, "           " );
	show_screen();
	draw_string( (screen_width() - 11) / 2, (screen_height() - 5) / 2 +5, "           " );
	show_screen();
	ping = sprite_create((screen_width() - 11) / 2 +5, (screen_height() - 5) / 2+3 , 1, 1, ping_image);
}

// Draw ping(ball)
void draw_ping(void)
{
	int now = get_current_time();
	srand(now);
	int px_range = (screen_width()-11)/2+5;
	int py_range = (screen_height()-5)/2+3;
	ping = sprite_create(px_range,py_range,1,1,ping_image);
    sprite_draw(ping);
	
 //Setup movemonet of ping(ball)
	int angle = rand() % 180;
    sprite_turn_to(ping, 0.3, 0.1);
	sprite_draw(ping);
    sprite_turn(ping, angle);
	show_screen();
}
// Draw borader and state of game
void draw_border(void)
{
	int w = screen_width();
	int h = screen_height();

	draw_line(0,0,w-1,0,'*'); 
	draw_line(0,h-1,w-1,h-1,'*'); 
	draw_line(0,0,0,h-1,'*'); 
	draw_line(w-1,0,w-1,h-1,'*'); 

	draw_line(0,2,w-1,2,'*'); 
	draw_formatted(2, 1,"Lives = %d          * Score = %d          *Level = %d         * Time = %02d:%02d"
	, lives, score, level,min,timer);
    show_screen(); 
}
// Draw right paddle
void draw_right(void){
	int w = screen_width();
	int h = screen_height();

	if (h>=21)
	{
		int right_x = w-4 , right_y = h/2 -3;
	    right = sprite_create(right_x,right_y, 1, 7, right_image); // right_iamage
    	sprite_draw(right);
    	show_screen();
	}
	else if (h<21)
	{
		int a = (h-7)/2;
		right = sprite_create(w-4,h/2,1,a,right_image);
		sprite_draw(right);
    	show_screen();
	}	
}
// Draw movement of ping(ball)
void update_ping(void)
{
	sprite_step(ping);
	sprite_step(right);
	
	int x = round( sprite_x(ping) );
	int y = round( sprite_y(ping) );
    int ry = round(sprite_y(right));
	double dx = sprite_dx( ping );
	double dy = sprite_dy( ping );

	if ( x == 1  ) {
		dx = -dx;
	}
	if (( x == screen_width() - 1) )
	{
		lives = lives -1;
		draw_time();
	}

	if ( ( y == screen_height() - 1 ) )
	{	
	dy = -dy;
    }
	if ( y == 2) {
		dy = -dy;
	}
	if (x == screen_width()-4)
	{
		if (y == ry || y == ry +1 || y == ry +2 || y == ry +3 || y == ry +4 || y == ry +5 || y == ry +6 )
		{ 
			dx= -dx;
		    score = score+1;
		}
	}
	if ( dx != sprite_dx( ping ) || dy != sprite_dy( ping) )
	{
		sprite_back( ping );
		sprite_turn_to( ping, dx, dy );
	} 
}
// Draw movement of right paddle
void update_right(int key)
{
	int yr = round(sprite_y(right));
	
    
	if (('i' ==key|| KEY_UP == key) && yr > 3 )
		
	{
		sprite_move(right,0,-1);
	}
	
	else if (('k' ==key || KEY_DOWN == key) &&  yr < screen_height() - 8)
	{
		sprite_move(right,0,+1);
	}	
}
//Change level by 'L' key
void update_level(int key)
{
	if (key == 'L' && d ==1)
	{
		level = level+d;
		draw_time();
		draw_ping();	
	}
	if (level == 5)
	{ 
		level =4;
		d = -1;
	}	
    if (key == 'L' && d < 0)
	{
		level = level +d;
		draw_time();
		draw_ping();	
	}
	if (level ==1 && d==-1)
	{
		d = d*-1;
	}	
}
//Update Level 2
void update_le2(void)
{
	int h = screen_height();
	int x = round( sprite_x(ping) );
	int y = round( sprite_y(ping) );
	int left_x = 3 , left_y = y ;
	

	if (round( sprite_y(ping) ) >= screen_height()- 8)
	{
		left = sprite_create(left_x,h-8, 1, 7, right_image); //left
	    sprite_draw(left);
     	show_screen();
	}
	else 
	{
		left = sprite_create(left_x,left_y, 1, 7, right_image); //left
    	sprite_draw(left);
    	show_screen();
  	}

	sprite_step(left);

    int ry = round(sprite_y(left));
	double dx = sprite_dx( ping );
	double dy = sprite_dy( ping );

	
	if (x == 4)
	{
		if (y == ry || y == ry +1 || y == ry +2 || y == ry +3 || y == ry +4 || y == ry +5 || y == ry +6 )
		{ 
			dx= -dx;
		}
	}
	if ( dx != sprite_dx( ping ) || dy != sprite_dy( ping) )
	{
		sprite_back( ping );
		sprite_turn_to( ping, dx, dy );
	}	
}
//Update Level3
void le3(void)
{
	//int time33 = time3-time2;
	    
		three = sprite_create((screen_width()-8)/2 ,(screen_height()-5)/2,8,5,three_image);
    	sprite_draw(three);

     	int x = (screen_width()-8)/2+3;
     	int y = (screen_height()-5)/2+2;
    	double x_diff =  x- sprite_x(ping);
    	double y_diff =  y-sprite_y(ping);

	    double dist_squared = (x_diff*x_diff) + (y_diff*y_diff);

	 
    	double dist = sqrt(dist_squared);
    	double dx = (sprite_dx(ping));
    	double dy = (sprite_dy(ping));

		double GM = 0.9;
	
    	double a = GM/dist_squared;

	    dx =  dx +a;//(a * x_diff / dist);
	    dy =  dy + (a * y_diff / dist);
    	double v = sqrt(dx*dx+dy*dy) ;
    	if (v > 1)
    	{
     		dx =  dx/v;
    		dy =  dy/v;
    	}
		if (dist_squared < 1e-10)
    	{
    		dist_squared = 1e-10;
    	}
	    sprite_turn_to(ping,dx,dy);		
}
//Update Level4
void le4(void)
{
	int x = round( sprite_x(ping) );
	int y = round( sprite_y(ping) );
    
	double dx = sprite_dx( ping );
	double dy = sprite_dy( ping );
	
	if ((screen_width()/3) <= x && x <= (screen_width()-(screen_width()/3)))
	{
		if (y == screen_height()/3)
		{
			dy = -dy;
			draw_char(round(sprite_x(ping)),round(sprite_y(ping)),' ');
		}
	}
	if ((screen_width()/3) <= x && x <= (screen_width()-(screen_width()/3)))
	{
		if (y == (screen_height()/3)*2)
		{
			dy = -dy;
			draw_char(round(sprite_x(ping)),round(sprite_y(ping)),' ');
		}
	}
	
	if ( dx != sprite_dx( ping ) || dy != sprite_dy( ping) )
	{
		sprite_back( ping );
		sprite_turn_to( ping, dx, dy );
	}
}
// Setup game.
void setup(void) {
	time1 = get_current_time();
	draw_border();
	draw_right();
	draw_time();
	draw_ping();
}
//Process Game
void process( void ) {
 //Setup timer
	time2 = get_current_time();
	//timer = time2 - time1;
	if (timer<60)
	{
		timer = time2-time1;
	}
	if (timer ==60)
	{
		min =1;
	}
	if (timer>=60 && timer<120)
	{
		timer = time2-time1-60;
	}
	if (timer == 120)
	{
		min=2;
	}
	if (timer >=120 && timer<180)
	{
		timer = time2-time1-120;
	}
	if (timer==180)
	{
		min=3;
	}
	if (timer >=180 && timer<240)
	{
		timer = time2-time1-180;
	}
	if (timer==240)
	{
		min=4;
	}
	if (timer >=240 && timer<300)
	{
		timer = time2-time1-240;
	}
	if (timer==300)
	{
		min=5;
	}	
 //Setup Level by 'h' key
	int key = get_char();
	update_level(key);
	if (key =='h')
	{
		clear_screen();
		draw_msg();
	}
 //Show Game Over and ask 'one more game?'	
	if (lives==0)
	{
		clear_screen();
   	    more = sprite_create((screen_width()-30)/2 ,(screen_height()-6)/2,28,6,more_image);
   	    sprite_draw(more);
        show_screen();
    	int key2=getchar();
	
	    if (key2 =='y')
			{
			lives =10;
			clear_screen();
			}
		if (key2 =='n')
			{
			game_over=true;
			clear_screen();
			}
		else {
		clear_screen();
   	    more = sprite_create((screen_width()-30)/2 ,(screen_height()-6)/2,28,6,more_image);
   	    sprite_draw(more);
        show_screen();
		}
	}
 //Play game until disappear lives
	if (lives >0)
	{   
	// Do level 1
		update_right(key);
		clear_screen();
    	sprite_draw(right);
		draw_border();
    	update_ping();
		sprite_draw(ping);
	// Do level 2
		if (level ==2 )
	   { 
			update_le2();
    	}
	// Do level 3
		if (level ==3)
		{
		//	int time4 =get_current_time();
	//		int time3 = get_current_time();
		
			update_le2();
			if (timer >= 5)
			{
				le3();
			}
			
			
		}
	// Do level 4
		if (level ==4)
		{
			update_le2();
			draw_line((screen_width()/3),screen_height()/3,screen_width()-(screen_width()/3),screen_height()/3,'=');
        	draw_line((screen_width()/3),(screen_height()/3)*2,screen_width()-(screen_width()/3),(screen_height()/3)*2,'=');

			le4();
		}	
	}
}

void cleanup(void) {
	// STATEMENTS
}

// Program entry point.
int main( void ) {
	setup_screen();
	draw_msg();
	if (!game_over){	
		setup();
	    show_screen();
    	while ( ! game_over ) {
	    	process();
			if ( update_screen ) {
				show_screen();
     	    	}
  		    timer_pause( DELAY );
        }
  	}
return 0;

}