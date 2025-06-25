/*

*/

#pragma once

#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>


#define ASSERT2(cond, action, msg, ...) \
	do { if (!(cond)) { printf("!! err, line %d: " msg "\n", __LINE__, ##__VA_ARGS__); \
	fflush(stdout); action; } } while(0)

#define ASSERT(cond, msg, ...) ASSERT2(cond, exit(1), msg, ##__VA_ARGS__)
