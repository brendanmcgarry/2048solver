#include "Vector.h"


Vector::Vector(int direction) {
	if (direction == 0) {
		x = 0;
		y = -1;
	}
	else if (direction == 1) {
		x = 1;
		y = 0;
	}
	else if (direction == 2) {
		x = 0;
		y = 1;
	}
	else if (direction == 3) {
		x = -1;
		y = 0;
	}
}