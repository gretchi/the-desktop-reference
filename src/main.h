#pragma once

#include <Arduino.h>
#include <sys/time.h>

void display_seconds();
void init_tks();
bool set_clock();
void display_qrcode(char *, int, int);
void disp_7seg(int);
String get_era(const struct tm *timeinfo);
