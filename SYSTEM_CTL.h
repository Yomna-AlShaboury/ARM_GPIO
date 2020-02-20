/*
 * SYSTEM_CTL.h
 *
 *  Created on: Feb 20, 2020
 *      Author: yssh
 */

#ifndef SYSTEM_CTL_H_
#define SYSTEM_CTL_H_

#include "std_types.h"

#define GPIOHBCTL *((volatile u32*)(0x400FE000 + 0x06C))
#define RCGCGPIO  *((volatile u32*)(0x400FE000 + 0x608))



#endif /* SYSTEM_CTL_H_ */
