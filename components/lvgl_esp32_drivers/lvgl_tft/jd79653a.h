/**
 * @file il3820.h
 *
 */

#ifndef JD79653A_H
#define JD79653A_H

#ifdef __cplusplus
extern "C"
{
#endif

void jd79653a_init();
void jd79653a_sleep();

void jd79653a_fb_set_full_color(uint8_t color);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JD79653A_H
