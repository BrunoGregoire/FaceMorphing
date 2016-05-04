#include "KeypointsManager.h"

int KeypointsManager::GetSymmetric(int keypoint)
{
    if(keypoint==8)
        return -1;
    else if(keypoint<=16)
        return 16-keypoint;
    else if(keypoint<=26)
        return 43-keypoint;
    else if(keypoint<=30)
        return -1;
    else if(keypoint==33)
        return -1;
    else if(keypoint<=35)
        return 66-keypoint;
    else if(keypoint<=39)
        return 81-keypoint;
    else if(keypoint<=41)
        return 87-keypoint;
    else if (keypoint<=45)
        return 81-keypoint;
    else if(keypoint<=47)
        return 87-keypoint;
    else if (keypoint==51)
        return -1;
    else if(keypoint<=54)
        return 102-keypoint;
    else if(keypoint==57)
        return -1;
    else if(keypoint<=59)
        return 114-keypoint;
    else if(keypoint<=62)
        return keypoint+3;
    else if(keypoint<=65)
        return keypoint-3;

    return -1;
}
