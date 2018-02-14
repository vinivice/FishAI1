#pragma once

//Input struct
struct Readings
{
	//near sensor reading
	GLuint nearSensorReadings;
	//eyes readings
	GLfloat distanceObjectSeem[4];
	b2Vec3 colorObjectSeem[4];
	//TODO add other infos like flags and color for example
};

//Output struct
struct Commands
{
	bool lPropulsor, rPropulsor, lReverse, rReverse;
	bool eat, sexReproduce, asexReproduce;

};

//Body definitions
#define CHROMOSSOMES_SIZE 7
#define STANDARD_RADIUS 0.30f
#define STANDARD_POWER 1.0f
#define LINEAR_DAMPING 0.25f
#define ANGULAR_DAMPING 0.05f

//Filter definirions
#define FISH_BODY_CATEGORY 0x0001
#define FISH_NEAR_SENSOR_CATEGORY 0x0002
#define RING_CATEGORY 0x0004
#define SEX_SEED_CATEGORY 0x0008
#define ASEX_SEED_CATEGORY 0x0010

//Seed definitions
#define SEED_RADIUS 0.15f

