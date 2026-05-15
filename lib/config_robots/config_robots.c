#include "config_robots.hh"

char equipe = 'B'; // Ã©quipe bleu par default

#ifdef PAMI_1 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2475,1500},
    //{2475,900},
    //{2200,1100},
    {2200,900},
};

int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING };
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_  };
unsigned long timereq[] = {0, 0};
uint32_t numPoints = 2;

# endif 

#ifdef PAMI_2 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2675,1450},
    {1850,1450}, //1880
};

int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING};
float rpms[] = {MOTOR_RPM_, 60};
unsigned long timereq[] = {0, 0};

uint32_t numPoints = 2;

# endif 

#ifdef PAMI_3

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2575,800},
    {2800,800},
};


int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0};

uint32_t numPoints = 2;

# endif



#ifdef PAMI_4 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2500,1750},
    {2500,1740},
    // EXIT ZONE
    {2250,500},
    {1550,850}
    
    
};


int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING, AVOID_ALL, AVOID_NOTHING};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0,0, 85500, 0};

uint32_t numPoints = 4;

# endif

#ifdef PAMI_5
 
// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2650,1650},
    {2650,1600},
    // EXIT ZONE
    {2650,1500},
    {2300,450},
    {2300,190},
  
};
//int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING, AVOID_MIDDLE, AVOID_ALL}; -> PREMIERE OPTION
int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING,AVOID_NOTHING, AVOID_NOTHING, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_,MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 87000,0, 0, 0};
uint32_t numPoints = 5;
 
# endif

#ifdef PAMI_6

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {500,1600 },
    {650,1170},
   { 1840,1170},
    {1860,1350},
  
};

bool avoidance[] = {false,true,true,true};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
uint32_t numPoints = 4;

# endif