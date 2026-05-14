#include "config_robots.hh"

char equipe = 'B'; // Ã©quipe bleu par default

#ifdef PAMI_1 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2475,1500},
    {2475,1100},
    {2200,800},
};

int avoidance[] = {AVOID_NOTHING, AVOID_ALL, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0};
uint32_t numPoints = 3;

# endif 

#ifdef PAMI_2 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2675,1800},
    {2450,1450},
    {1750,1450},
};

int avoidance[] = {AVOID_NOTHING, AVOID_MIDDLE, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0};

uint32_t numPoints = 3;

# endif 

#ifdef PAMI_3

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2575,1600},
    {2450,800},
    {2900,800},
};


int avoidance[] = {AVOID_NOTHING, AVOID_MIDDLE, AVOID_MIDDLE};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0};

uint32_t numPoints = 3;

# endif



#ifdef PAMI_4 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2450,1750},
    {2450,1700},
    // EXIT ZONE
    {2450,1400},
    {1500,1000},
    
    
};


int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING, AVOID_ALL, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 86000, 0, 0};

uint32_t numPoints = 4;

# endif

#ifdef PAMI_5

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2600,1650},
    {2600,1640},
    // EXIT ZONE
    {2600,1400},
    {2300,100},
  
};

int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING, AVOID_MIDDLE, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 85000, 0, 0};
uint32_t numPoints = 4;

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