// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	InitPhysicsHandle();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateTargetLocationAndRotation();
}

bool UGrabber::Grab()
{
	if(PhysicsHandle == nullptr)
	{
		return false;
	}
	
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	if(FHitResult Hit; GetWorld()->SweepSingleByChannel(
		Hit,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(ApproximationRange)))
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			Hit.GetComponent(),
			NAME_None,
			Hit.ImpactPoint,
			GetComponentRotation());
		HoldDistance = FVector::Distance(GetComponentLocation(), Hit.ImpactPoint);
		return true;
	}
	else
	{
		return false;
	}
}

void UGrabber::Release()
{
	if(PhysicsHandle == nullptr || PhysicsHandle->GetGrabbedComponent() == nullptr)
	{
		return;
	}
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::InitPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing physics handle"));
	}
}

void UGrabber::UpdateTargetLocationAndRotation()
{
	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}
