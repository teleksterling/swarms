//world.h
#include "data.h"

enum statusEnum {RUNNING, PAUSED, STEP};

class Cworld
{
 private:
	vec o;
	statusEnum status;
	double l, w, h;

 public:
 	int view;
	int listID;
	int speed;  //milliseconds timer delay

	void drawMe(void);
	vec origin(void);
	statusEnum getStatus(void);
	void flipStatus(void);
	void pause(void) {status = PAUSED;}
	void step(void) {status = STEP;}
	void run(void) {status = RUNNING;}
	
	Cworld(void);

};
