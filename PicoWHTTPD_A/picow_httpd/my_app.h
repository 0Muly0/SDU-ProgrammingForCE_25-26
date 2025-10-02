#ifndef MY_APP_H
#define MY_APP_H

// 7. Defined a header which declares the my_app function 
#ifdef __cplusplus
extern "C" {
#endif

// New main function to substitute old main
void my_app(bool debug_flag);

#ifdef __cplusplus
}
#endif
#endif