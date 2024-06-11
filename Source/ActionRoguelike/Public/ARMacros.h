#pragma once
#include "Engine/Engine.h"

#define PrintText(_text) GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT(_text)))
#define PrintString(_string) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, _string)
#define PrintStringInfinite(_string) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, _string)
#define PrintFName(_text) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, _text.ToString())
#define PrintFloat(_float) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%f"), _float))
#define PrintInt(_int) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%d"), _int))
#define PrintBool(_bool) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), _bool ? TEXT("TRUE") : TEXT("FALSE")))
#define PrintVector(_vector) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%s"), *_vector.ToString()))
#define PrintRotator(_rotator) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%s"), *_rotator.ToString()))