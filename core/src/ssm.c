#include "ssm.h"
#include <math.h>
#include <string.h>

void SSM_Init(SSM_State *s, float initial_temp) {
    memset(s, 0, sizeof(SSM_State));
    s->x[0] = initial_temp;
    s->last_prediction = initial_temp;
    s->initialized = 1;
}

float SSM_Predict(SSM_State *s) {
    float x0_pred = s->x[0] + SSM_DT * s->x[1];
    float x1_pred = SSM_ALPHA * s->x[1];
    s->last_prediction = x0_pred;
    s->x[0] = x0_pred;
    s->x[1] = x1_pred;
    return x0_pred;
}

void SSM_Update(SSM_State *s, float actual_temp, float *out_error, float *out_accuracy_pct, float *out_rolling_mae) {
    float error = actual_temp - s->last_prediction;
    s->x[0] = actual_temp;
    s->x[1] = s->x[1] + 0.1f * error; // Correcting the trend

    s->err_buf[s->buf_idx] = fabsf(error);
    s->buf_idx = (s->buf_idx + 1) % SSM_WINDOW_SIZE;
    if (!s->buf_full && s->buf_idx == 0) s->buf_full = 1;

    int count = s->buf_full ? SSM_WINDOW_SIZE : s->buf_idx;
    float mae = 0.0f;
    for (int i = 0; i < count; i++) mae += s->err_buf[i];
    if (count > 0) mae /= (float)count;

    float acc = (1.0f - (fabsf(error) / 2.0f)) * 100.0f; // Window of 2.0°C
    if (acc < 0.0f) acc = 0.0f;

    *out_error = error;
    *out_accuracy_pct = acc;
    *out_rolling_mae = mae;
}
