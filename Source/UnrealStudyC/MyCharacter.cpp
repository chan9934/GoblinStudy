// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "Components/Widgetcomponent.h"
#include "MyHpWidget.h"
#include "MyStatComponent.h"
#include "MyAIController.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SM(TEXT("SkeletalMesh'/Game/UndeadPack/EnemyGoblin/Mesh/SM_EnemyGoblin.SM_EnemyGoblin'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 130.f));

	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));
	static ConstructorHelpers::FClassFinder<UAnimInstance>AI(TEXT("AnimBlueprint'/Game/Animation/MyAnimBlueprint.MyAnimBlueprint_C'"));
	if (AI.Succeeded())
	{

		GetMesh()->SetAnimInstanceClass(AI.Class);

	}
	static ConstructorHelpers::FClassFinder<UMyHpWidget>WC(TEXT("WidgetBlueprint'/Game/UI/WBP_UI.WBP_UI_C'"));
	if (WC.Succeeded())
	{
		HpBar->SetWidgetClass(WC.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}
	/*FName WeaponSocket(TEXT("hand_R_Socket"));
	if(GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh>SW(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hambone/SK_Blunt_Hambone.SK_Blunt_Hambone'"));
		if (SW.Succeeded())
		{
			Weapon->SetSkeletalMesh(SW.Object);
		}

	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}*/
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//FName WeaponSocket(TEXT("hand_R_Socket"));
	//auto MyWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);

	//if (MyWeapon)
	//{
	//	MyWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	//}

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
	AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
	HpBar->InitWidget();
	auto HpWidget = Cast<UMyHpWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(Stat);
	}
	Stat->OnDead.AddUObject(this, &AMyCharacter::DeadBinding);

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMyCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
}

void AMyCharacter::UpDown(float Value)
{
	if (Value == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::LeftRight(float Value)
{
	if (Value == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::Attack()
{
	if (IsAttacking)
	{
		return;
	}

	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToSection(AttackIndex);

	AttackIndex = (AttackIndex + 1) % 4;


	IsAttacking = true;
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimInstance->GetDeadMontage())
	{
		if (IsAttacking == true)
		{
			UE_LOG(LogTemp, Log, TEXT("Destroy"));
			Destroy();
		}
	}
	
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	float AttackRange = 100.f;
	float AttackRadius = 50.f;
	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * AttackRange, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("HitAcotr %s"), *HitResult.Actor->GetName());
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;

	float HalfHeight = AttackRange * 0.5f + AttackRadius;

	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();

	FColor DrawColor;
	if (bResult)
	{
		DrawColor = FColor::Green;
	}
	else
	{
		DrawColor = FColor::Red;
	}
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 2.f);

}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	auto MyActor = Cast<AMyCharacter>(DamageCauser);
	if (MyActor)
	{
		UE_LOG(LogTemp, Log, TEXT("Damage is %d"), &MyActor->AttackIndex);
		auto MyWeapon = Cast<AMyWeapon>(Weapon);
	

			if (MyActor->CharacterGrab == true)
			{
				DamageAmount *= 2.5f;
			}

		
		Stat->OnAttacked(DamageAmount);

	}
	return DamageAmount;
}

void AMyCharacter::DeadBinding()
{
	
	AnimInstance->PlayDeadMontage();
}

