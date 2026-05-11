#include "config_robots.hh"

char equipe = 'B'; // Ã©quipe bleu par default

#ifdef PAMI_1 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2950,1500},
    {2940,1300},
    {2925,1200},
    {2500,1200},
    {1500,100},
};

bool avoidance[] = {true,true,true,true,true};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
uint32_t numPoints = 5;

# endif 

#ifdef PAMI_2 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2845,1500},
    {2835,1400},
    {1500,800},
    
};

bool avoidance[] = {false,false, false};
float rpms[] = {MOTOR_RPM_,MOTOR_RPM_, MOTOR_RPM_};

uint32_t numPoints = 3;

# endif 

#ifdef PAMI_3

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2950,1500},
    {2940,1300},
    {2925,1200},
    {2500,1200},
    {2300,100},
};


bool avoidance[] = {false,false,false,false,false};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};

uint32_t numPoints = 5;

# endif



#ifdef PAMI_4 

// waypoints and avoidance need to have the same length
Point waypoints[] = {
    {2845,1400},
    {2835,1300},
    {2200,800},

};

bool avoidance[] = {false,false,false};
float rpms[] = {MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_, MOTOR_RPM_};
uint32_t numPoints = 3;

# endif

#ifdef PAMI_5

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