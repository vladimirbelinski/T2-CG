#include <math.h>

#define DEF_CAM_X 0.0
#define DEF_CAM_Y 0.0
#define DEF_CAM_Z 50.0
#define CTRLPOINTS_TRAY_U 6
#define CTRLPOINTS_TRAY_V 4
#define CTRLPOINTS_GOURD_U 9
#define CTRLPOINTS_GOURD_V 9
#define CTRLPOINTS_PUDDING_U 7
#define CTRLPOINTS_PUDDING_V 9

const double PAN_STEP = 1.0;
const double ONE_STEP =  M_PI / 180.0;
const double SIN_ONE_STEP = sin(ONE_STEP);
const double COS_ONE_STEP = cos(ONE_STEP);

const double FOURTY_FIVE = ONE_STEP * 45.0;
const double SIN_FOURTY_FIVE = sin(FOURTY_FIVE);
const double COS_FOURTY_FIVE = cos(FOURTY_FIVE);

const double NINETY = ONE_STEP * 90.0;
const double SIN_NINETY = sin(NINETY);
const double COS_NINETY = cos(NINETY);

const GLubyte tray_color[] = {224, 223, 219};
const GLfloat gourd_color[] = {1.0, 1.0, 1.0, 0.5};
const GLfloat pudding_color[] = {1.0, 1.0, 1.0, 0.5};

void init_gourd(void);
