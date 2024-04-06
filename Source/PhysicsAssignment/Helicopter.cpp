#include "Helicopter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"

AHelicopter::AHelicopter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(Mesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);
	SpringArm->TargetArmLength = 2000.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepWorldTransform);

	PhysicsThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Physics Thruster"));
	PhysicsThruster->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);
	
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(1);
	Mesh->SetAngularDamping(1);
	
	PhysicsThruster->SetRelativeRotation(FRotator(-90, 0, 0));
	PhysicsThruster->bAutoActivate = true;
	
	SpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AHelicopter::BeginPlay()
{
	Super::BeginPlay();

	// Fix aligning thruster and mesh to have the same center of mass. 
	FVector ThrusterCenterOfMassDifference = PhysicsThruster->GetComponentLocation() - Mesh->GetCenterOfMass();
	FRotator InvertedMeshRotation = Mesh->GetComponentRotation().GetInverse();
	FVector CenterOfMassActualOffset = InvertedMeshRotation.RotateVector(ThrusterCenterOfMassDifference);
	FVector CenterOfMassWantedOffset = FVector(CenterOfMassActualOffset.X, CenterOfMassActualOffset.Y, 0);
	Mesh->SetCenterOfMass(CenterOfMassWantedOffset, FName());
	
}

void AHelicopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHelicopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Mouse Right", this, &AHelicopter::MouseRight);
	PlayerInputComponent->BindAxis("Mouse Up", this, &AHelicopter::MouseUp);
	PlayerInputComponent->BindAxis("Move Up", this, &AHelicopter::MoveUp);
	PlayerInputComponent->BindAxis("Rotate Right", this, &AHelicopter::RotateRight);
	PlayerInputComponent->BindAxis("Tilt Forward", this, &AHelicopter::TiltForward);
	PlayerInputComponent->BindAxis("Tilt Right", this, &AHelicopter::TiltRight);
}

void AHelicopter::MouseRight(float Value)
{
	SpringArm->AddRelativeRotation(FRotator(0, Value, 0));
	auto Rotation = SpringArm->GetRelativeRotation();
	Rotation.Yaw = FMath::Clamp(Rotation.Yaw, -45 , 45);
	SpringArm->SetRelativeRotation(Rotation);
}

void AHelicopter::MouseUp(float Value)
{
	SpringArm->AddRelativeRotation(FRotator(Value, 0, 0));
	auto Rotation = SpringArm->GetRelativeRotation();
	Rotation.Pitch = FMath::Clamp(Rotation.Pitch, -45 , 45);
	SpringArm->SetRelativeRotation(Rotation);
}

void AHelicopter::MoveUp(float Value)
{
	float DesiredUpForce = Value * VariableUpForce + ConstantUpForce;
	PhysicsThruster->ThrustStrength = DesiredUpForce / GetActorUpVector().Z * Mesh->GetMass();
}

void AHelicopter::TiltForward(float Value)
{
	float DesiredAngle = Value * DesiredTiltAngle + Mesh->GetComponentRotation().Pitch;
	float ClampedValue = FMath::Clamp(DesiredAngle, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorRightVector() * ClampedValue * TiltingSpeed, FName(), true);
}

void AHelicopter::TiltRight(float Value)
{
	float DesiredAngle = Value * -DesiredTiltAngle + Mesh->GetComponentRotation().Roll;
	float ClampedValue = FMath::Clamp(DesiredAngle, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorForwardVector() * ClampedValue * TiltingSpeed, FName(), true);
}

void AHelicopter::RotateRight(float Value)
{
	Mesh->AddTorqueInDegrees(GetActorUpVector() * Value * YawRotationSpeed, FName(), true);
}

void AHelicopter::SetDesiredTiltAngle(float Value)
{
	DesiredTiltAngle = FMath::Clamp(Value, -60.f, 60.f);
}

float AHelicopter::GetDesiredTiltAngle()
{
	return DesiredTiltAngle;
}

void AHelicopter::SetTiltSpeedClampBound(float Value)
{
	TiltSpeedClampBound = Value > 0 ? Value : 0;
}

float AHelicopter::GetTiltSpeedClampBound()
{
	return TiltSpeedClampBound;
}

void AHelicopter::SetTiltingSpeed(float Value)
{
	TiltingSpeed = Value > 0 ? Value : 0;
}

float AHelicopter::GetTiltingSpeed()
{
	return TiltingSpeed;
}
