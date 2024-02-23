// common_def.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__ 1

#define VERBOSE_

typedef enum
{
  kErrorCode_Ok = 0,
  kErrorCode_Null = -1,
  kErrorCode_Memory = -2,
  kErrorCode_NodeNull = -10,
  kErrorCode_SrcNull = -11,
  kErrorCode_BytesZero = -12,
  kErrorCode_DataNull = -13,
  kErrorCode_SizeZero = -14,
  kErrorCode_SizeMismatch = -15,
  kErrorCode_OpsNull = -16,
  kErrorCode_VectorNull = -20,
  kErrorCode_StorageNull = -21,
  kErrorCode_VectorFull = -22,
  kErrorCode_VectorEmpty = -23,
  kErrorCode_PositionMismatch = -24,
  kErrorCode_NotEnoughCapacity = -25,
  kErrorCode_ListNull = -30,
  kErrorCode_FirstNull = -31,
  kErrorCode_InvalidIndex = -32,
  kErrorCode_ListEmpty = -33,
}ErrorCode;

#endif // __COMMON_DEF_H__