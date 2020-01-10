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

//main program definitions
//TODO? put constants from main program here? maybe

//Body definitions
#define CHROMOSSOMES_SIZE 8
#define STANDARD_RADIUS 0.30f
#define STANDARD_POWER 1.0f
#define LINEAR_DAMPING 0.25f //TODO check sleep threshold
#define ANGULAR_DAMPING 0.05f
//#define LINEAR_DAMPING 0.5f
//#define ANGULAR_DAMPING 0.05f
#define BODY_RESOLUTION 32
#define BORDER_SIZE 0.2f

//Filter definitions
#define FISH_BODY_CATEGORY 0x0001
#define FISH_NEAR_SENSOR_CATEGORY 0x0002
#define RING_CATEGORY 0x0004
#define SEX_SEED_CATEGORY 0x0008
#define ASEX_SEED_CATEGORY 0x0010

//Seed definitions
#define SEED_RADIUS 0.10f
#define SEED_RESOLUTION 32

//Ring definitions
#define RING_RESOLUTION 32

//Angles definitions
#define SIN_15 0.2588190451
#define COS_15 0.96592582628f
#define SIN_30 0.5f
#define COS_30 0.86602540378f

#define SUM_SIN(SA, CA, SB, CB) (SA*CB + SB*CA)
#define SUM_COS(SA, CA, SB, CB) (CA*CB - SA*SB)
