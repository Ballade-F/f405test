/*
 * ctrl_math.h
 *
 *  Created on: 2022年12月7日
 *      Author: Ballade_F
 */

#ifndef CTRL_CTRL_MATH_H_
#define CTRL_CTRL_MATH_H_

#include "math.h"

#define M_PIf       3.14159265358979323846f
#define DEG2RAD		0.0174532925f
#define RAD2DEG		57.29578f

#define MIN(a, b) 	(((a) < (b)) ? (a) : (b))
#define MAX(a, b) 	(((a) > (b)) ? (a) : (b))

float atan2_approx(float y, float x);
float acos_approx(float x);



#endif /* CTRL_CTRL_MATH_H_ */
