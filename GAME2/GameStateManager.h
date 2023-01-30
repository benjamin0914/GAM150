#pragma once

typedef void(*FP)(void);
extern float g_dt;
extern char Roboto;
extern int current, previous, next;
extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();