#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "Control.h" 

void drawBouncingBall();

int main(int argc, char **argv)
{
 	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* redraw_timer = NULL;

	if(	!al_init() || 
		!al_init_primitives_addon() || 
		!al_init_font_addon() || 
		!al_init_ttf_addon() ||
		!al_install_mouse() || 
		!al_install_keyboard()) 
	{
 		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
 
	display = al_create_display(640, 480);
	if(!display) 
	{
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
 
	event_queue = al_create_event_queue();
	if(!event_queue) 
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	bool running = true;
	bool redraw = false;
	redraw_timer = al_create_timer(ALLEGRO_BPS_TO_SECS(60));

	al_register_event_source(event_queue, al_get_timer_event_source(redraw_timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	alGui::Theme defaultTheme;
	alGui::Theme::SetCurrentTheme(&defaultTheme);

	alGui::Control screen (0, 0, 640, 480);
	alGui::Panel panel ("options", 200, 481);
	alGui::ScrollPanel scroll(150, 250, 500);
	alGui::Button btn ("End Game");

	panel.onClick([](const ALLEGRO_EVENT& event) -> bool { fprintf(stdout, "PANEL CLICK ONLY\n"); return false; });
	panel.onMouseEnter([](const ALLEGRO_EVENT& event) -> bool { fprintf(stdout, "Panel Mouse Enter\n"); return true; });
	panel.onMouseLeave([](const ALLEGRO_EVENT& event) -> bool { fprintf(stdout, "Panel Mouse Leave\n"); return true; });
	btn.onClick([&running](const ALLEGRO_EVENT& event) -> bool { running = false; return true; });

	screen.attachControl(&panel);
	panel.attachControl(&scroll, 30, 100);
	scroll.attachControl(&btn, alGui::PLACE_RELATIVE_BOTLEFT, 10, 50);
	
	screen.onClick([](const ALLEGRO_EVENT& event) -> bool { fprintf(stdout, "SCREEN CLICK\n"); return true; });

	al_start_timer(redraw_timer); 
	while(running)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		switch(event.type)
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;

			case ALLEGRO_EVENT_KEY_DOWN:
				running = (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE);
				break;

			case ALLEGRO_EVENT_TIMER:
				redraw = true;
				break;
		}

		screen.handleEvent(event);

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));

			drawBouncingBall();
			screen.draw();

 			al_flip_display();
		}
	}
 
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(redraw_timer);
 
	return 0;
}



int x = 50;
int y = 50;
int xspeed = 2;
int yspeed = 2;
void drawBouncingBall()
{
	if (x < 0 && xspeed < 0) xspeed =- xspeed;
	if (x > 640 && xspeed > 0) xspeed =- xspeed;
	if (y < 0 && yspeed < 0) yspeed =- yspeed;
	if (y > 480 && yspeed > 0) yspeed =- yspeed;

	x += xspeed;
	y += yspeed;

	al_draw_filled_circle(x, y, 15, al_map_rgb(90, 104, 200));
}
