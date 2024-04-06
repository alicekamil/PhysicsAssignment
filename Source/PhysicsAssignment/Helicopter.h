#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Helicopter.generated.h"

UCLASS()
class PHYSICSASSIGNMENT_API AHelicopter : public APawn
{
	GENERATED_BODY()

public:

	AHelicopter();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* PhysicsThruster;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void MouseRight(float Value);

	UFUNCTION()
	void MouseUp(float Value);

	UFUNCTION()
	void MoveUp(float Value);

	UFUNCTION()
	void RotateRight(float Value);

	UFUNCTION()
	void TiltForward(float Value);

	UFUNCTION()
	void TiltRight(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Helicopter Movement")
	float VariableUpForce = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Helicopter Movement")
	float ConstantUpForce = 970;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Helicopter Movement")
	float YawRotationSpeed = 60;

	UFUNCTION(BlueprintSetter)
	void SetDesiredTiltAngle(float Value);

	UFUNCTION(BlueprintGetter)
	float GetDesiredTiltAngle();

	UFUNCTION(BlueprintSetter)
	void SetTiltSpeedClampBound(float Value);

	UFUNCTION(BlueprintGetter)
	float GetTiltSpeedClampBound();

	UFUNCTION(BlueprintSetter)
	void SetTiltingSpeed(float Value);

	UFUNCTION(BlueprintGetter)
	float GetTiltingSpeed();


private:
	UPROPERTY(EditAnywhere, BlueprintSetter = SetDesiredTiltAngle, BlueprintGetter = GetDesiredTiltAngle, 
				Category = "Helicopter Movement", meta = (ClampMin = -60, ClampMax = 60))
	float DesiredTiltAngle = 30;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltSpeedClampBound, BlueprintGetter = GetTiltSpeedClampBound,
				Category = "Helicopter Movement", meta = (ClampMin = 0))
	float TiltSpeedClampBound = 20;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltingSpeed, BlueprintGetter = GetTiltingSpeed,
				Category = "Helicopter Movement", meta = (ClampMin = 0))
	float TiltingSpeed = 5;

public:	

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
