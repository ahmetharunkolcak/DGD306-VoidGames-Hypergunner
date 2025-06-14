// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetContainable.generated.h"

class UTexture2D;

UINTERFACE(MinimalAPI)
class UWidgetContainable : public UInterface {
	GENERATED_BODY()
};

class VOIDGAMESHYPERGUNNER_API IWidgetContainable {
	GENERATED_BODY()

	public:
		virtual void SetInGameCharacterImage(UTexture2D* ImageToSet, const bool bIsForLeftPlayer) = 0;
};
