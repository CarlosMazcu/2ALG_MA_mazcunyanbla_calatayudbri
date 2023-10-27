// common_def.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__ 1

#define VERBOSE_

typedef enum {
  kErrorCode_Ok = 0,
  kErrorCode_Null = -1,
  kErrorCode_Memory = -2,
  kErrorCode_NodeNull = -10, 
  kErrorCode_SrcNull = -11,
  kErrorCode_BytesZero = -12,
  kErrorCode_DataNull = -13,
} ErrorCode;

#endif // __COMMON_DEF_H__