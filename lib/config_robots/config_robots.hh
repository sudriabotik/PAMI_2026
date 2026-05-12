# ifndef CONFIG_ROBOTS
# define CONFIG_ROBOTS

#include <Arduino.h>

//# define TEST_MODE
//# define PRINT_DISTANCES
//# define EVITEMENT 

# ifdef TEST_MODE
# define WAIT_SUBSTRACTION 40000
# endif
# ifndef TEST_MODE
# define WAIT_SUBSTRACTION 0
# endif

# define MATCH_TIME 100000 - WAIT_SUBSTRACTION

///////// define pin name /////////// 

const uint8_t sensorPinRight = A3;
const uint8_t sensorPinMidel = A2;
const uint8_t sensorPinLeft = A1;
const uint8_t servoPin = 17; //A0
const uint8_t tirette = 11; // D11
const uint8_t bouton_equipe = 10; // D10 low = blue vers le haut, hight= jaune ( Ã©quipe bleu par default)

#define DIR_X 3 // moteur gauche
#define STEP_X 6 // moteur gauche
#define DIR_Y 4 //moteur droit
#define STEP_Y 7 //moteur droit
#define ENABLE 8 // pin enable des driver moteur droite et gauche

///////////// fin pin name ////////////

////// chose which pami you want to use /////////////

# define PAMI_1 
//#define PAMI_2
//#define PAMI_3
//#define PAMI_4
//#define PAMI_5
//#define PAMI_6


///////// fin chose which pami you want to use /////////////

// position struct {x,y} in mm
typedef struct Point {
    float x; 
    float y; 
} Point;

extern Point waypoints[];
extern bool avoidance[];
extern float rpms[];
extern uint32_t numPoints;
extern unsigned long timereq[];
extern char equipe; // equipe bleu par default


#ifdef PAMI_1 

#define X_POSITION_START_ 2950.0
#define Y_POSITION_START_  1620.0
#define TETA_POSITION_START_ -90.0

#define MOTOR_RPM_ 200
#define MOTOR_ACCEL_ 1000
#define MOTOR_DECEL_ 1000
//#define MOTOR_DECEL_AVOIDEMENT 5000 // freinage brust s'il y a un obstacle
//#define MOTOR_DECEL_FINISH_ 500

#define MOTOR_ACCEL_DECEL_ROTATE_ 700 // 700

#define DIAMETRE_ROUE 59 //distance (2)
#define COEF_DROIT 1.0 //droit(1) diminuer - plus a gauche, augmenter - plus a droite 
#define ENTRE_AXE 92.5 //rotation (3)

#define ADD_DELAY_START 500.0 // after the 85sec we add a extra delay to avoid all the pami to start at the same time.
const int SUPERSTAR_FLOOR_THRESHOLD = 75; //The distance above which we consider there is no floor  // True when the superstar is doing the superstar-specific movements, used to change how avoidances are triggered


# define GLOBAL_WAIT 85000 - WAIT_SUBSTRACTION

# endif

#ifdef PAMI_2

#define X_POSITION_START_ 2850
#define Y_POSITION_START_  1620
#define TETA_POSITION_START_ -90

#define MOTOR_RPM_ 200
#define MOTOR_ACCEL_ 1000
#define MOTOR_DECEL_ 1000
//#define MOTOR_DECEL_AVOIDEMENT 5000 // freinage brust s'il y a un obstacle
//#define MOTOR_DECEL_FINISH_ 500

#define MOTOR_ACCEL_DECEL_ROTATE_ 700 // 700

#define DIAMETRE_ROUE 59.5
#define COEF_DROIT 0.99987
#define ENTRE_AXE 92.5 

#define ADD_DELAY_START 500.0 // after the 85sec we add a extra delay to avoid all the pami to start at the same time.
const int SUPERSTAR_FLOOR_THRESHOLD = 75; //The distance above which we consider there is no floor  // True when the superstar is doing the superstar-specific movements, used to change how avoidances are triggered

# define GLOBAL_WAIT 85000 - WAIT_SUBSTRACTION

# endif

#ifdef PAMI_3

#define X_POSITION_START_ 2550
#define Y_POSITION_START_  1920
#define TETA_POSITION_START_ -180

#define MOTOR_RPM_ 200
#define MOTOR_ACCEL_ 500
#define MOTOR_DECEL_ 500
//#define MOTOR_DECEL_AVOIDEMENT 5000 // freinage brust s'il y a un obstacle
//#define MOTOR_DECEL_FINISH_ 500

#define MOTOR_ACCEL_DECEL_ROTATE_ 700 // 700

#define DIAMETRE_ROUE 59.87
#define COEF_DROIT 1.0008 // 1.0
#define ENTRE_AXE 90.39

#define ADD_DELAY_START 0.0 // after the 85sec we add a extra delay to avoid all the pami to start at the same time. 
const int SUPERSTAR_FLOOR_THRESHOLD = 75; //The distance above which we consider there is no floor  // True when the superstar is doing the superstar-specific movements, used to change how avoidances are triggered

# define GLOBAL_WAIT 40000 - WAIT_SUBSTRACTION

# endif

#ifdef PAMI_4

#define X_POSITION_START_ 2750
#define Y_POSITION_START_  1920
#define TETA_POSITION_START_ -180

#define MOTOR_RPM_ 200
#define MOTOR_ACCEL_ 1000
#define MOTOR_DECEL_ 1000
//#define MOTOR_DECEL_AVOIDEMENT 5000 // freinage brust s'il y a un obstacle
//#define MOTOR_DECEL_FINISH_ 500

#define MOTOR_ACCEL_DECEL_ROTATE_ 700 // 700

#define DIAMETRE_ROUE 58.93
#define COEF_DROIT 1.003
#define ENTRE_AXE 92.5

#define ADD_DELAY_START 2000.0 // after the 85sec we add a extra delay to avoid all the pami to start at the same time.
const int SUPERSTAR_FLOOR_THRESHOLD = 75; //The distance above which we consider there is no floor  // True when the superstar is doing the superstar-specific movements, used to change how avoidances are triggered

# define GLOBAL_WAIT 45000 - WAIT_SUBSTRACTION

# endif // PAMI_4


#ifdef PAMI_5

#define X_POSITION_START_ 50
#define Y_POSITION_START_  1920
#define TETA_POSITION_START_ -90

#define MOTOR_RPM_ 150
#define MOTOR_ACCEL_ 1400
#define MOTOR_DECEL_ 3000
//#define MOTOR_DECEL_AVOIDEMENT 5000 // freinage brust s'il y a un obstacle
//#define MOTOR_DECEL_FINISH_ 500

#define MOTOR_ACCEL_DECEL_ROTATE_ 700 // 700

#define DIAMETRE_ROUE 61.0
#define COEF_DROIT 1.0
#define ENTRE_AXE 97.8

#define ADD_DELAY_START 0.0 // after the 85sec we add a extra delay to avoid all the pami to start at the same time.
const int SUPERSTAR_FLOOR_THRESHOLD = 75; //The distance above which we consider there is no floor  // True when the superstar is doing the superstar-specific movements, used to change how avoidances are triggered

# ifdef TEST_MODE
# define GLOBAL_WAIT 2000
# endif
# ifndef TEST_MODE
# define GLOBAL_WAIT 85000//85000
# endif

# endif // PAMI_5

#ifdef PAMI_6

#define X_POSITION_START_ 70.0
#define Y_POSITION_START_  1600
#define TETA_POSITION_START_ 0.0

#define MOTOR_RPM_ 150
#define MOTOR_ACCEL_ 1400
#define MOTOR_DECEL_ 3000
//#define MOTOR_DECEL_AVOIDEMENT 5000 // freinage brust s'il y a un obstacle
//#define MOTOR_DECEL_FINISH_ 500

#define MOTOR_ACCEL_DECEL_ROTATE_ 700 // 700

#define DIAMETRE_ROUE 61.0
#define COEF_DROIT 1.0
#define ENTRE_AXE 97.8

#define ADD_DELAY_START 0.0 // after the 85sec we add a extra delay to avoid all the pami to start at the same time.
const int SUPERSTAR_FLOOR_THRESHOLD = 75; //The distance above which we consider there is no floor  // True when the superstar is doing the superstar-specific movements, used to change how avoidances are triggered

# ifdef TEST_MODE
# define GLOBAL_WAIT 2000
# endif
# ifndef TEST_MODE
# define GLOBAL_WAIT 85000//85000
# endif

# endif // PAMI_6


# endif //CONFIG_ROBOTS