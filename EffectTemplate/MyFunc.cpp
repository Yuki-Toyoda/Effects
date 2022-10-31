#include "Myfunc.h"
#include <time.h>
#include <stdlib.h>
#include <cmath>

namespace My {
    void SetSrand(){
        srand(static_cast<unsigned int>(time(NULL)));
    }

    int Random(int min, int max){
        return min + static_cast<int>(rand() * static_cast<float>(max - min + 1.0f) / static_cast<float>(1.0f + RAND_MAX));
    }

    float RandomF(float min, float max,int num) {
        int Num = pow(10, num);
        return static_cast<float>(Random(static_cast<int>(max * Num), static_cast<int>(min * Num)) * (1.0f / static_cast<float>(Num)));
    }

    int Clamp(int val, int max, int min){
        if(val < min){
            return min;
        }
        if(max < val){
            return max;
        }
        return val;
    }
}
