#include <math.h>

#define DEF_CAM_X 0.0
#define DEF_CAM_Y 0.0
#define DEF_CAM_Z 15.0
#define CTRLPOINTS_TRAY_U 6
#define CTRLPOINTS_TRAY_V 4

const double PAN_STEP = 1.0;
const double ONE_STEP =  M_PI / 260.0;
const double SIN_ONE_STEP = sin(ONE_STEP);
const double COS_ONE_STEP = cos(ONE_STEP);

const GLubyte tray_color[] = {224, 223, 219};
