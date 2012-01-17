/*
 * This file is part of ShapeFusion (Copyright 2000 Tito Dal Canton)
 *
 * ShapeFusion is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ShapeFusion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ShapeFusion; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef PHYSICSELEMENTS_H
#define PHYSICSELEMENTS_H

#include "../BigEndianBuffer.h"
#include <iostream>

class PhysicsElement
{
private:
	bool mVerboseLoading;

protected:
	// So that subclasses can change their status
	bool mGoodData;

public:
	PhysicsElement(bool verbose) : mVerboseLoading(verbose), mGoodData(false) {}
	~PhysicsElement() { }

	bool IsGood() const { return mGoodData; }
	bool IsVerbose() const { return mVerboseLoading; }
};

class PhysicsConstants : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const PhysicsConstants&);
private:
	double mMaximumForwardVelocity;
	double mMaximumBackwardVelocity;
	double mMaximumPerpendicularVelocity;

	double mAcceleration;
	double mDeceleration;
	double mAirborneDeceleration;
	
	double mGravitationalAcceleration;
	double mClimbingAcceleration;
	double mTerminalVelocity;

	double mExternalDeceleration;

	double mAngularAcceleration;
	double mAngularDeceleration;
	double mMaximumAngularVelocity;
	double mAngularRecenteringVelocity;

	double mFastAngularVelocity;
	double mFastAngularMaximum;

	double mMaximumElevation;
	double mExternalAngularDeceleration;

	double mStepDelta;
	double mStepAmplitude;
	
	double mRadius;
	double mHeight;
	double mDeadHeight;
	double mCameraHeight;
	double mSplashHeight;

	double mHalfCameraSeparation;
public:
	// accessors
	double GetMaximumForwardVelocity() const { return mMaximumForwardVelocity; }
	double GetMaximumBackwardVelocity() const { return mMaximumBackwardVelocity; }
	double GetMaximumPerpendicularVelocity() const { return mMaximumPerpendicularVelocity; }

	double GetAcceleration() const { return mAcceleration; }
	double GetDeceleration() const { return mDeceleration; }
	double GetAirborneDeceleration() const { return mAirborneDeceleration; }

	double GetGravitationalAcceleration() const { return mGravitationalAcceleration; }
	double GetClimbingAcceleration() const { return mClimbingAcceleration; }
	double GetTerminalVelocity() const { return mTerminalVelocity; }

	double GetExternalDeceleration() const { return mExternalDeceleration; }

	double GetAngularAcceleration() const { return mAngularAcceleration; }
	double GetAngularDeceleration() const { return mAngularDeceleration; }
	double GetMaximumAngularVelocity() const { return mMaximumAngularVelocity; }
	double GetAngularRecenteringVelocity() const { return mAngularRecenteringVelocity; }


	double GetFastAngularVelocity() const { return mFastAngularVelocity; }
	double GetFastAngularMaximum() const { return mFastAngularMaximum; }

	double GetMaximumElevation() const { return mMaximumElevation; }
	double GetExternalAngularDeceleration() const { return mExternalAngularDeceleration; }

	double GetStepDelta() const { return mStepDelta; }
	double GetStepAmplitude() const { return mStepAmplitude; }
	
	double GetRadius() const { return mRadius; }
	double GetHeight() const { return mHeight; }
	double GetDeadHeight() const { return mDeadHeight; }
	double GetCameraHeight() const { return mCameraHeight; }
	double GetSplashHeight() const { return mSplashHeight; }

	double GetHalfCameraSeparation() const { return mHalfCameraSeparation; }


	PhysicsConstants(bool verbose = false) : PhysicsElement(verbose) { }
	~PhysicsConstants() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);

	static const int kSize = 104;
};

class AttackDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const AttackDefinition&);
private:
	short mType;
	short mRepetitions;
	double mError;
	short mRange;
	short mAttackShape;
	short mDx, mDy, mDz;

public:
	// accessors
	short GetType() { return mType; }
	short GetRepetitions() { return mRepetitions; }
	double GetError() { return mError; }
	short GetRange() { return mRange; }
	short GetShape() { return mAttackShape; }
	short GetDx() { return mDx; }
	short GetDy() { return mDy; }
	short GetDz() { return mDz; }

	// mutators
	void SetType(short v) { mType = v; }
	void SetRepetitions(short v) { mRepetitions = v; }
	void SetError(double d) { mError = d; }
	void SetRange(short v) { mRange = v; }
	void SetShape(short v) { mAttackShape = v; }
	void SetDx(short v) { mDx = v; }
	void SetDy(short v) { mDy = v; }
	void SetDz(short v) { mDz = v; }
	
	AttackDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~AttackDefinition() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);
};

class DamageDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const DamageDefinition&);
private:
	short mType;
	short mFlags;
	short mBase;
	short mRandom;
	double mScale;

	enum {
		kAlienDamage = 0x01
	};

public:
	// accessors
	short GetType() { return mType; }
	short GetBase() { return mBase; }
	bool GetAlien() { return mFlags & kAlienDamage; }
	short GetRandom() { return mRandom; }
	double GetScale() { return mScale; }

	// mutators
	void SetType(short v) { mType = v; }
	void SetAlien(bool b) { 
		if (b) mFlags |= kAlienDamage;
		else mFlags &= ~kAlienDamage; 
	}

	void SetBase(short v) { mBase = v; }
	void SetRandom(short v) { mRandom = v; }
	void SetScale(double d) { mScale = d; }

	DamageDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~DamageDefinition() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);
};

class EffectDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const EffectDefinition&);
private:
	unsigned short mCollection;
	unsigned short mColorTable;
	short mShape;
	double mSoundPitch;
	unsigned short mFlags;
	short mDelay;
	short mDelaySound;

	enum {
		kEndWhenAnimationLoops = 0x0001,
		kEndWhenTransferAnimationLoops = 0x0002,
		kSoundOnly = 0x0004,
		kMakeTwinVisible = 0x0008,
		kMediaEffect = 0x0010
	};

public:
	// accessors
	unsigned short GetCollection() { return mCollection; }
	unsigned short GetColorTable() { return mColorTable; }
	short GetShape() { return mShape; }
	double GetSoundPitch() { return mSoundPitch; }
	short GetDelay() { return mDelay; }
	short GetDelaySound() { return mDelaySound; }

	bool GetEndWhenAnimationLoops() { return mFlags & kEndWhenAnimationLoops; }
	bool GetEndWhenTransferAnimationLoops() { return mFlags & kEndWhenTransferAnimationLoops; }
	bool GetSoundOnly() { return mFlags & kSoundOnly; }
	bool GetMediaEffect() { return mFlags & kMediaEffect; }

	// mutators
	void SetCollection(unsigned short v) { mCollection = v; }
	void SetColorTable(unsigned short v) { mColorTable = v; }
	void SetShape(short v) { mShape = v; }
	void SetSoundPitch(double v) { mSoundPitch = v; }
	void SetDelay(short v) { mDelay = v; }
	void SetDelaySound(short v) { mDelaySound = v; }

	void SetEndWhenAnimationLoops(bool v) { 
		if (v) mFlags |= kEndWhenAnimationLoops;
		else mFlags &= ~kEndWhenAnimationLoops;
	}

	void SetEndWhenTransferAnimationLoops(bool v) { 
		if (v) mFlags |= kEndWhenTransferAnimationLoops;
		else mFlags &= ~kEndWhenTransferAnimationLoops;
	}
	
	void SetSoundOnly(bool v) { 
		if (v) mFlags |= kSoundOnly;
		else mFlags &= ~kSoundOnly;
	}

	void SetMediaEffect(bool v) { 
		if (v) mFlags |= kMediaEffect;
		else mFlags &= ~kMediaEffect;
	}

	EffectDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~EffectDefinition() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);

	static const int kSize = 14;
};	

class MonsterDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const MonsterDefinition&);
private:
	unsigned short mCollection;
	unsigned short mColorTable;

	short mVitality;
	unsigned long mImmunities;
	unsigned long mWeaknesses;
	unsigned long mFlags;

	long mClass;
	long mFriends;
	long mEnemies;
	
	double mSoundPitch;
	
	short mActivationSound;
	short mFriendlyActivationSound;
	short mClearSound;
	short mKillSound;
	short mApologySound;
	short mFriendlyFireSound;
	short mFlamingSound;
	short mRandomSound;
	short mRandomSoundMask;

	short mCarryingItemType;
	
	short mRadius;
	short mHeight;
	short mPreferredHoverHeight;
	short mMinimumLedgeDelta;
	short mMaximumLedgeDelta;
	double mExternalVelocityScale;
	short mImpactEffect;
	short mMeleeImpactEffect;
	short mContrailEffect;

	short mHalfVisualArc;
	short mHalfVerticalVisualArc;
	short mVisualRange;
	short mDarkVisualRange;
	short mIntelligence;
	short mSpeed;
	short mGravity;
	short mTerminalVelocity;
	short mDoorRetryMask;
	short mShrapnelRadius;
	
	DamageDefinition mShrapnelDamage;

	// sequence IDs
	short mHitShapes;
	short mHardDyingShape;
	short mSoftDyingShape;
	short mHardDeadShapes;
	short mSoftDeadShapes;
	short mStationaryShape;
	short mMovingShape;
	short mTeleportInShape;
	short mTeleportOutShape;

	short mAttackFrequency;
	AttackDefinition mMeleeAttack;
	AttackDefinition mRangedAttack;

public:
	// accessors
	unsigned short GetCollection() { return mCollection; }
	unsigned short GetColorTable() { return mColorTable; }

	short GetVitality() { return mVitality; }
	bool GetImmunity(int index) { return mImmunities & (1 << index); }
	bool GetWeakness(int index) { return mWeaknesses & (1 << index); }
	bool GetFlag(int index) { return mFlags & (1 << index); }

	long GetClass() { return mClass; }
	bool GetFriend(int index) { return mFriends & (1 << index); }
	bool GetEnemy(int index) { return mEnemies & (1 << index); }

	double GetSoundPitch() { return  mSoundPitch; }

	short GetActivationSound() { return  mActivationSound; }
	short GetFriendlyActivationSound() { return  mFriendlyActivationSound; }
	short GetClearSound() { return  mClearSound; }
	short GetKillSound() { return  mKillSound; }
	short GetApologySound() { return  mApologySound; }
	short GetFriendlyFireSound() { return  mFriendlyFireSound; }
	short GetFlamingSound() { return  mFlamingSound; }
	short GetRandomSound() { return  mRandomSound; }
	short GetRandomSoundMask() { return  mRandomSoundMask; }

	short GetCarryingItemType() { return mCarryingItemType; }

	short GetRadius() { return mRadius; }
	short GetHeight() { return mHeight; }
	short GetPreferredHoverHeight() { return mPreferredHoverHeight; }
	short GetMinimumLedgeDelta() { return mMinimumLedgeDelta; }
	short GetMaximumLedgeDelta() { return mMaximumLedgeDelta; }
	double GetExternalVelocityScale() { return mExternalVelocityScale; }
	short GetImpactEffect() { return mImpactEffect; }
	short GetMeleeImpactEffect() { return mMeleeImpactEffect; }
	short GetContrailEffect() { return mContrailEffect; }

	short GetVisualRange() { return mVisualRange; }
	short GetDarkVisualRange() { return mDarkVisualRange; }
	short GetIntelligence() { return mIntelligence; }
	short GetSpeed() { return mSpeed; }
	short GetGravity() { return mGravity; }
	short GetTerminalVelocity() { return mTerminalVelocity; }
	short GetDoorRetryMask() { return mDoorRetryMask; }
	short GetShrapnelRadius() { return mShrapnelRadius; }

	DamageDefinition* GetShrapnelDamage() { return &mShrapnelDamage; }

	short GetHitShapes() { return  mHitShapes; }
	short GetHardDyingShape() { return  mHardDyingShape; }
	short GetSoftDyingShape() { return  mSoftDyingShape; }
	short GetHardDeadShapes() { return  mHardDeadShapes; }
	short GetSoftDeadShapes() { return  mSoftDeadShapes; }
	short GetStationaryShape() { return  mStationaryShape; }
	short GetMovingShape() { return  mMovingShape; }
	short GetTeleportInShape() { return  mTeleportInShape; }
	short GetTeleportOutShape() { return  mTeleportOutShape; }

	short GetAttackFrequency() { return mAttackFrequency; }
	AttackDefinition* GetMeleeAttack() { return &mMeleeAttack; }
	AttackDefinition* GetRangedAttack() { return &mRangedAttack; }

	// mutators
	void SetCollection(unsigned short v) { mCollection = v; }
	void SetColorTable(unsigned short v) { mColorTable = v; }

	void SetVitality(short v) { mVitality = v; }
	void SetImmunity(int index, bool b) { 
		if (b) mImmunities |= (1 << index);
		else mImmunities &= ~(1 << index);
	}

	void SetWeakness(int index, bool b) { 
		if (b) mWeaknesses |= (1 << index);
		else mWeaknesses &= ~(1 << index);
	}

	void SetFlag(int index, bool b) { 
		if (b) mFlags |= (1 << index);
		else mFlags &= ~(1 << index);
	}

	void SetClass(long v) { mClass = v; }

	void SetFriend(int index, bool b) { 
		if (b) mFriends |= (1 << index);
		else mFriends &= ~(1 << index);
	}

	void SetEnemy(int index, bool b) { 
		if (b) mEnemies |= (1 << index);
		else mEnemies &= ~(1 << index);
	}

	void SetSoundPitch(double d) { mSoundPitch = d; }

	void SetRadius(short v) { mRadius = v; }
	void SetHeight(short v) { mHeight = v; }
	void SetPreferredHoverHeight(short v) { mPreferredHoverHeight = v; }
	void SetMinimumLedgeDelta(short v) { mMinimumLedgeDelta = v; }
	void SetMaximumLedgeDelta(short v) { mMaximumLedgeDelta = v; }
	void SetExternalVelocityScale(double d) { mExternalVelocityScale = d; }
	void SetImpactEffect(short v) { mImpactEffect = v; }
	void SetMeleeImpactEffect(short v) { mMeleeImpactEffect = v; }
	void SetContrailEffect(short v) { mContrailEffect = v; }

	void SetVisualRange(short v) { mVisualRange = v; }
	void SetDarkVisualRange(short v) { mDarkVisualRange = v; }
	void SetIntelligence(short v) { mIntelligence = v; }
	void SetSpeed(short v) { mSpeed = v; }
	void SetGravity(short v) { mGravity = v; }
	void SetTerminalVelocity(short v) { mTerminalVelocity = v; }
	void SetDoorRetryMask(short v) { mDoorRetryMask = v; }
	void SetShrapnelRadius(short v) { mShrapnelRadius = v; }


	void SetActivationSound(short v) { mActivationSound = v; }
	void SetFriendlyActivationSound(short v) { mFriendlyActivationSound = v; }
	void SetClearSound(short v) { mClearSound = v; }
	void SetKillSound(short v) { mKillSound = v; }
	void SetApologySound(short v) { mApologySound = v; }
	void SetFriendlyFireSound(short v) { mFriendlyFireSound = v; }
	void SetFlamingSound(short v) { mFlamingSound = v; }
	void SetRandomSound(short v) { mRandomSound = v; }
	void SetRandomSoundMask(short v) { mRandomSoundMask = v; }

	void SetCarryingItemType(short v) { mCarryingItemType = v; }

	void SetHitShapes(short v) { mHitShapes = v; }
	void SetHardDyingShape(short v) { mHardDyingShape = v; }
	void SetSoftDyingShape(short v) { mSoftDyingShape = v; }
	void SetHardDeadShapes(short v) { mHardDeadShapes = v; }
	void SetSoftDeadShapes(short v) { mSoftDeadShapes = v; }
	void SetStationaryShape(short v) { mStationaryShape = v; }
	void SetMovingShape(short v) { mMovingShape = v; }
	void SetTeleportInShape(short v) { mTeleportInShape = v; }
	void SetTeleportOutShape(short v) { mTeleportOutShape = v; }

	void SetAttackFrequency(short v) { mAttackFrequency = v; }

	MonsterDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~MonsterDefinition() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);

	static const int kSize = 156;
};

class ProjectileDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const ProjectileDefinition&);
private:
	unsigned short mCollection;
	unsigned short mColorTable;
	short mShape;
	
	short mDetonationEffect;
	short mMediaDetonationEffect;
	short mContrailEffect;
	short mTicksBetweenContrails;
	short mMaximumContrails;
	short mMediaProjectilePromotion;

	short mRadius;
	short mAreaOfEffect;
	DamageDefinition mDamage;
	
	unsigned long mFlags;

	short mSpeed;
	short mMaximumRange;

	double mSoundPitch;
	short mFlybySound;
	short mReboundSound;

public:
	unsigned short GetCollection() { return mCollection; }
	unsigned short GetColorTable() { return mColorTable; }
	short GetShape() { return mShape; }

	short GetDetonationEffect() { return mDetonationEffect; }
	short GetMediaDetonationEffect() { return mMediaDetonationEffect; }
	short GetContrailEffect() { return mContrailEffect; }
	short GetTicksBetweenContrails() { return mTicksBetweenContrails; }
	short GetMaximumContrails() { return mMaximumContrails; }
	short GetMediaProjectilePromotion() { return mMediaProjectilePromotion; }

	short GetRadius() { return mRadius; }
	short GetAreaOfEffect() { return mAreaOfEffect; }

	DamageDefinition* GetDamage() { return &mDamage; }

	bool GetFlag(int flag_index) { return mFlags & (1 << flag_index); }

	short GetSpeed() { return mSpeed; }
	short GetMaximumRange() { return mMaximumRange; }

	double GetSoundPitch() { return mSoundPitch; }
	short GetFlybySound() { return mFlybySound; }
	short GetReboundSound() { return mReboundSound; }

	ProjectileDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~ProjectileDefinition() { }
	
	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);
	
	static const int kSize = 48;
};

class TriggerDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const TriggerDefinition&);
private:
	short mRoundsPerMagazine;
	short mAmmunitionType;
	short mTicksPerRound;
	short mRecoveryTicks;
	short mChargingTicks;
	short mRecoilMagnitude;
	
	short mFiringSound;
	short mClickSound;
	short mChargingSound;
	short mShellCasingSound;
	short mReloadingSound;
	short mChargedSound;
	
	short mProjectileType;
	short mThetaError;
	short mDx;
	short mDz;
	short mShellCasingType;
	short mBurstCount;
public:
	short GetRoundsPerMagazine() { return mRoundsPerMagazine; }
	short GetAmmunitionType() { return mAmmunitionType; }
	short GetTicksPerRound() { return mTicksPerRound; }
	short GetRecoveryTicks() { return mRecoveryTicks; }
	short GetChargingTicks() { return mChargingTicks; }
	short GetRecoilMagnitude() { return mRecoilMagnitude; }
	
	short GetFiringSound() { return mFiringSound; }
	short GetClickSound() { return mClickSound; }
	short GetChargingSound() { return mChargingSound; }
	short GetShellCasingSound() { return mShellCasingSound; }
	short GetReloadingSound() { return mReloadingSound; }
	short GetChargedSound() { return mChargedSound; }
	
	short GetProjectileType() { return mProjectileType; }
	short GetThetaError() { return mThetaError; }
	short GetDx() { return mDx; }
	short GetDz() { return mDz; }
	short GetShellCasingType() { return mShellCasingType; }
	short GetBurstCount() { return mBurstCount; }

	TriggerDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~TriggerDefinition() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);
};

class WeaponDefinition : public PhysicsElement
{
	friend std::ostream& operator<<(std::ostream&, const WeaponDefinition&);
private:
	short mItemType;
	short mPowerupType;
	short mWeaponClass;
	short mFlags;
	
	double mFiringLightIntensity;
	short mFiringIntensityDecayTicks;

	double mIdleHeight;
	double mBobAmplitude;
	double mKickHeight;
	double mReloadHeight;
	double mIdleWidth;
	double mHorizontalAmplitude;

	unsigned short mCollection;
	unsigned short mColorTable;
	
	short mIdleShape;
	short mFiringShape;
	short mReloadingShape;
	// short unused
	short mChargingShape;
	short mChargedShape;

	short mReadyTicks;
	short mAwaitReloadTicks;
	short mLoadingTicks;
	short mFinishLoadingTicks;
	short mPowerupTicks;

	TriggerDefinition mPrimaryTrigger;
	TriggerDefinition mSecondaryTrigger;
public:
	short GetItemType() { return mItemType; }
	short GetPowerupType() { return mPowerupType; }
	short GetWeaponClass() { return mWeaponClass; }
	bool GetFlag(int index) { return mFlags & (1 << index); }
	
	double GetFiringLightIntensity() { return mFiringLightIntensity; }
	short GetFiringIntensityDecayTicks() { return mFiringIntensityDecayTicks; }

	double GetIdleHeight() { return mIdleHeight; }
	double GetBobAmplitude() { return mBobAmplitude; }
	double GetKickHeight() { return mKickHeight; }
	double GetReloadHeight() { return mReloadHeight; }
	double GetIdleWidth() { return mIdleWidth; }
	double GetHorizontalAmplitude() { return mHorizontalAmplitude; }

	unsigned short GetCollection() { return mCollection; }
	unsigned short GetColorTable() { return mColorTable; }
	
	short GetIdleShape() { return mIdleShape; }
	short GetFiringShape() { return mFiringShape; }
	short GetReloadingShape() { return mReloadingShape; }
	short GetChargingShape() { return mChargingShape; }
	short GetChargedShape() { return mChargedShape; }

	short GetReadyTicks() { return mReadyTicks; }
	short GetAwaitReloadTicks() { return mAwaitReloadTicks; }
	short GetLoadingTicks() { return mLoadingTicks; }
	short GetFinishLoadingTicks() { return mFinishLoadingTicks; }
	short GetPowerupTicks() { return mPowerupTicks; }

	TriggerDefinition* GetPrimaryTrigger() { return &mPrimaryTrigger; }
	TriggerDefinition* GetSecondaryTrigger() { return &mSecondaryTrigger; }

	WeaponDefinition(bool verbose = false) : PhysicsElement(verbose) { }
	~WeaponDefinition() { }

	BigEndianBuffer& SaveObject(BigEndianBuffer& buffer) const;
	BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);

	static const int kSize = 134;
};

#endif
