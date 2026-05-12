#ifndef SSM_H
#define SSM_H

#define SSM_WINDOW_SIZE   20      // Rolling MAE depth
#define SSM_DT            0.1f    // 100ms internal step
#define SSM_ALPHA         0.95f   // Trend decay factor

typedef struct {
    float x[2];                   // State: [temp, trend]
    float err_buf[SSM_WINDOW_SIZE];
    int   buf_idx;
    int   buf_full;
    float last_prediction;
    int   initialized;
} SSM_State;

void  SSM_Init(SSM_State *s, float initial_temp);
float SSM_Predict(SSM_State *s);
void  SSM_Update(SSM_State *s, float actual_temp, float *out_error, float *out_accuracy_pct, float *out_rolling_mae);

#endif
