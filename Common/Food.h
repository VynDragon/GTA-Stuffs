#pragma once

#include <string>

typedef struct {
	std::string name;
	float		water;
	float		food;
} FoodType;

// length = 4

#define NUMBER_DIFFERENT_FOOD 4

static const FoodType foodTypes[] = {	{ "Canned Beans", 0.0f, 0.15f },
										{ "Water Bottle", 0.25f, 0.0f },
										{ "Canned PineApple", 0.02f, 0.08f },
										{ "Cola Can", 0.10f, 0.05f },
										{"", 0, 0},
									};