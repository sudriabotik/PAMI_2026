#include "config_robots.hh"

char equipe = 'B'; // Ã©quipe bleu par default

#ifdef PAMI_1 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2950,1500},
    {2935,1250},
    {2925,1200},
    {2000,1200},
    {1500,100},
};

int avoidance[] = {AVOID_NOTHING, AVOID_MIDDLE, AVOID_MIDDLE, AVOID_MIDDLE, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0, 0, 0};
uint32_t numPoints = 5;

# endif 

#ifdef PAMI_2 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2845,1500},
    {2835,1400},
    {1500,800},
};

int avoidance[] = {AVOID_NOTHING, AVOID_MIDDLE, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0};

uint32_t numPoints = 3;

# endif 

#ifdef PAMI_3

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2540,1895},
    {2500,1880},
    {2500,1700},
    // EXIT THE ZONE
    {2500,1450},
    {1750,1450},
    
};


int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING, AVOID_NOTHING, AVOID_MIDDLE, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0, 85000, 0};

uint32_t numPoints = 5;

# endif



#ifdef PAMI_4 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2690,1895},
    {2650,1880},
    {2650,1700},
    // EXIT THE ZONE
    {2600,1300},
    {2200,800},
    
};


int avoidance[] = {AVOID_NOTHING, AVOID_NOTHING, AVOID_NOTHING, AVOID_MIDDLE, AVOID_ALL};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0, 85000, 0};

uint32_t numPoints = 5;

# endif

#ifdef PAMI_5

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2845,1500},
    {2835,1400},
    {1500,800},
  
};

int avoidance[] = {AVOID_NOTHING,true,true,true};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
unsigned long timereq[] = {0, 0, 0, 0, 0};
uint32_t numPoints = 2;

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