#pragma once

namespace My {
    void SetSrand();
    int Random(int min, int max);
    float RandomF(float min, float max, int num);
    int Clamp(int val, int max, int min);
}