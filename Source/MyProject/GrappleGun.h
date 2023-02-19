// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ArrowComponent.h"
#include "C:\Program Files\Epic Games\UE_4.26\Engine\Plugins\Runtime\CableComponent\Source\CableComponent\Classes\CableComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SceneComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GrappleGun.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class MYPROJECT_API UGrappleGun : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrappleGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//functions
	UFUNCTION(BlueprintCallable)
		void m_Init(UCableComponent* cable, ACharacter* character, APlayerCameraManager* camera_manager);
	UFUNCTION(BlueprintCallable)
		void m_Release_Swing();

	UFUNCTION(BlueprintCallable)
		void m_Swing();

	UFUNCTION(BlueprintCallable)
		void m_Fire();

	UFUNCTION(BlueprintCallable)
		void m_Update_Cable();

	UFUNCTION(BlueprintCallable)
		void m_Update();

	UFUNCTION(BlueprintCallable)
		void m_Release_Zip();

	UFUNCTION(BlueprintCallable)
		void m_Pull();

	UFUNCTION(BlueprintCallable)
		void m_Zip();

	UFUNCTION(BlueprintCallable)
		void m_Left_Click();

	UFUNCTION(BlueprintCallable)
		void m_Left_Release();

	UFUNCTION(BlueprintCallable)
		void m_Right_Click();

	//variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_Pull_Force = 500000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_Zip_Force = 200000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_Hook_Point;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_Out_Hit_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Attached;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_AttachedIsPullable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Swinging;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Pulling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Zipping;

	//components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* m_Attached_Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCableComponent* m_Cable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACharacter* m_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerCameraManager* m_Camera_Manager;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
