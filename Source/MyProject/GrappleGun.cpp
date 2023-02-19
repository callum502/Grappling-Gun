// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleGun.h"

// Sets default values for this component's properties
UGrappleGun::UGrappleGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UGrappleGun::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UGrappleGun::m_Init(UCableComponent* cable, ACharacter* character, APlayerCameraManager* camera_manager)
{
	m_Cable = cable;
	m_Character = character;
	m_Camera_Manager = camera_manager;
}

void UGrappleGun::m_Release_Swing()
{
	
	m_Swinging = false;
	m_Attached = false;
	m_AttachedIsPullable = false;
	m_Cable->SetHiddenInGame(true);
	m_Cable->EndLocation = FVector(0, 0, 0);
}

void UGrappleGun::m_Swing()
{
	if (m_Hook_Point.Z > m_Character->GetActorLocation().Z && !m_Character->GetCharacterMovement()->IsMovingOnGround())
	{
		float dot;
		FVector direction = UKismetMathLibrary::Normal( m_Character->GetActorLocation() - m_Hook_Point);
		dot = UKismetMathLibrary::Dot_VectorVector(m_Out_Hit_Normal, direction);
		
		if (dot>0)
		{
			if (m_Character->GetCharacterMovement()->IsFalling())
			{
				float dot2;
				dot2 = UKismetMathLibrary::Dot_VectorVector(m_Character->GetVelocity(), m_Character->GetActorLocation() - m_Hook_Point);
				FVector Force;
				Force = dot2 * UKismetMathLibrary::Normal(m_Hook_Point - m_Character->GetActorLocation());
				m_Character->GetCharacterMovement()->AddForce(Force*2);
				m_Character->GetCharacterMovement()->AirControl = 0.3;
			}
		}
		else
		{
			m_Release_Swing();
			m_Character->LaunchCharacter(FVector(0,0,1000),false,false);
		}
	}
	else
	{
		m_Release_Swing();
	}
}

void UGrappleGun::m_Fire()
{
	TArray<AActor*> actorsToIgnore;
	FVector startPoint = m_Character->GetActorLocation();
	FVector endpoint = (m_Camera_Manager->GetActorForwardVector() * 10000.0) + startPoint;
	FHitResult OutHit;
	if(UKismetSystemLibrary::LineTraceSingle(this, startPoint, endpoint, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, OutHit, true))
	{
		m_Attached = true;
		m_AttachedIsPullable = OutHit.GetComponent()->ComponentHasTag("Pullable");
		m_Attached_Object = OutHit.GetComponent();
		m_Hook_Point = OutHit.Location;
		m_Out_Hit_Normal = OutHit.Normal;
	}
}

void UGrappleGun::m_Update_Cable()
{
	m_Cable->SetHiddenInGame(false);
	FVector direction = m_Character->GetActorLocation() - m_Hook_Point;
	m_Cable->CableLength = direction.Size()-300;
	m_Cable->EndLocation = m_Character->GetMesh()->GetSocketTransform("Swing", RTS_Actor).GetLocation();;
	if (m_AttachedIsPullable)
	{
		m_Hook_Point = Cast<UPrimitiveComponent>(m_Attached_Object)->GetComponentLocation();
	}
	m_Cable->SetWorldLocation(m_Hook_Point);
}

void UGrappleGun::m_Update()
{
	if (m_Attached)
	{
		m_Update_Cable();
		if (m_Swinging)
		{
			m_Swing();
		}
		else
		{
			float distance = UKismetMathLibrary::Vector_GetAbsMax(m_Character->GetActorLocation() - m_Hook_Point);
			if (m_AttachedIsPullable)
			{
				if (m_Pulling)
				{
					m_Pull();
				}
			}
			else if (distance>100)
			{
				m_Zip();
			}
			else
			{
				m_Release_Zip();
			}
		}
	}
}

void UGrappleGun::m_Release_Zip()
{
	m_Swinging = false;
	m_Attached = false;
	m_Zipping = false;
	m_Character->GetCharacterMovement()->GravityScale = 1;
	m_Character->GetCharacterMovement()->GroundFriction = 8;
	m_Character->JumpMaxCount = 1;
	m_Cable->EndLocation = FVector(0, 0, 0);
	m_Cable->SetHiddenInGame(true);
	m_Character->GetCharacterMovement()->Velocity = FVector(0,0,0);
	if (0.9 > m_Out_Hit_Normal.Z)
	{
		m_Character->LaunchCharacter(FVector(0,0,1000),false,false);
	}
}

void UGrappleGun::m_Pull()
{
	FVector force;
	force = UKismetMathLibrary::Normal(m_Character->GetActorLocation() - m_Hook_Point)* m_Pull_Force;
	Cast<UPrimitiveComponent>(m_Attached_Object)->AddForce(force);
}

void UGrappleGun::m_Zip()
{
	if (!m_Zipping)
	{
		m_Zipping = true;
		m_Swinging = false;
		m_Character->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
		m_Character->GetCharacterMovement()->GravityScale = 0;
		m_Character->JumpMaxCount = 0;
		m_Character->GetCharacterMovement()->GroundFriction = 0;
	}
	//apply force
	FVector force;
	force = UKismetMathLibrary::Normal(m_Hook_Point - m_Character->GetActorLocation()) * m_Zip_Force;
	m_Character->GetCharacterMovement()->AddForce(force);
}

void UGrappleGun::m_Left_Click()
{
	if (m_AttachedIsPullable)
	{
		m_Pulling = true;
	}
	else if (!m_Zipping)
	{
		m_Swinging = true;
		m_Fire();
	}
}

void UGrappleGun::m_Left_Release()
{
	if (m_Swinging)
	{
		m_Release_Swing();
	}
	else
	{
		m_Pulling = false;
	}

}

void UGrappleGun::m_Right_Click()
{
	if (!m_Swinging)
	{
		if (m_AttachedIsPullable)
		{
			m_Release_Swing();
		}
		else
		{
			m_Fire();
		}
	}
}

// Called every frame
void UGrappleGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}