#include <math.h>

#define DEF_CAM_X 0.0
#define DEF_CAM_Y 0.0
#define DEF_CAM_Z 50.0
#define CTRLPOINTS_TRAY_U 6
#define CTRLPOINTS_TRAY_V 4
#define CTRLPOINTS_GOURD_U 9
#define CTRLPOINTS_GOURD_V 8

const double PAN_STEP = 1.0;
const double ONE_STEP =  M_PI / 180.0;
const double FORTY_FIVE = ONE_STEP * 45.0;
const double SIN_ONE_STEP = sin(ONE_STEP);
const double COS_ONE_STEP = cos(ONE_STEP);
const double SIN_FORTY_FIVE = sin(FORTY_FIVE);
const double COS_FORTY_FIVE = cos(FORTY_FIVE);

const GLubyte tray_color[] = {224, 223, 219};
const GLubyte gourd_color[] = {255, 255, 255};

void init_gourd(void);
