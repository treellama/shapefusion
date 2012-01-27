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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <cmath>

#include "PhysicsElements.h"

BigEndianBuffer& PhysicsConstants::LoadObject(BigEndianBuffer& buffer)
{
	// everything is a 16.16 fixed, for whatever reason
	mMaximumForwardVelocity = buffer.ReadFixed();
	mMaximumBackwardVelocity = buffer.ReadFixed();
	mMaximumPerpendicularVelocity= buffer.ReadFixed();
	
	mAcceleration = buffer.ReadFixed();
	mDeceleration = buffer.ReadFixed();
	mAirborneDeceleration = buffer.ReadFixed();
	
	mGravitationalAcceleration = buffer.ReadFixed();
	mClimbingAcceleration = buffer.ReadFixed();
	mTerminalVelocity = buffer.ReadFixed();

	mExternalDeceleration = buffer.ReadFixed();

	mAngularAcceleration = buffer.ReadFixed();
	mAngularDeceleration = buffer.ReadFixed();
	mMaximumAngularVelocity = buffer.ReadFixed();
	mAngularRecenteringVelocity = buffer.ReadFixed();

	mFastAngularVelocity = buffer.ReadFixed();
	mFastAngularMaximum = buffer.ReadFixed();

	mMaximumElevation = buffer.ReadFixed();
	mExternalAngularDeceleration = buffer.ReadFixed();

	mStepDelta = buffer.ReadFixed();
	mStepAmplitude = buffer.ReadFixed();

	mRadius = buffer.ReadFixed();
	mHeight = buffer.ReadFixed();
	mDeadHeight = buffer.ReadFixed();
	mCameraHeight = buffer.ReadFixed();
	mSplashHeight = buffer.ReadFixed();

	mHalfCameraSeparation = buffer.ReadFixed();

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& PhysicsConstants::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteFixed(mMaximumForwardVelocity);
	buffer.WriteFixed(mMaximumBackwardVelocity);
	buffer.WriteFixed(mMaximumPerpendicularVelocity);

	buffer.WriteFixed(mAcceleration);
	buffer.WriteFixed(mDeceleration);
	buffer.WriteFixed(mAirborneDeceleration);
	
	buffer.WriteFixed(mGravitationalAcceleration);
	buffer.WriteFixed(mClimbingAcceleration);
	buffer.WriteFixed(mTerminalVelocity);

	buffer.WriteFixed(mExternalDeceleration);

	buffer.WriteFixed(mAngularAcceleration);
	buffer.WriteFixed(mAngularDeceleration);
	buffer.WriteFixed(mMaximumAngularVelocity);
	buffer.WriteFixed(mAngularRecenteringVelocity);

	buffer.WriteFixed(mFastAngularVelocity);
	buffer.WriteFixed(mFastAngularMaximum);

	buffer.WriteFixed(mMaximumElevation);
	buffer.WriteFixed(mExternalAngularDeceleration);

	buffer.WriteFixed(mStepDelta);
	buffer.WriteFixed(mStepAmplitude);
	
	buffer.WriteFixed(mRadius);
	buffer.WriteFixed(mHeight);
	buffer.WriteFixed(mDeadHeight);
	buffer.WriteFixed(mCameraHeight);
	buffer.WriteFixed(mSplashHeight);

	buffer.WriteFixed(mHalfCameraSeparation);

	return buffer;
}

BigEndianBuffer& AttackDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mType = buffer.ReadShort();
	mRepetitions = buffer.ReadShort();
	mError = buffer.ReadShort() * 360.0 / 512.0;
	mRange = buffer.ReadShort();
	mAttackShape = buffer.ReadShort();

	mDx = buffer.ReadShort();
	mDy = buffer.ReadShort();
	mDz = buffer.ReadShort();

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& AttackDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteShort(mType);
	buffer.WriteShort(mRepetitions);
	buffer.WriteShort(std::floor(mError * 512.0 / 360.0 + 0.5));
	buffer.WriteShort(mRange);
	buffer.WriteShort(mAttackShape);

	buffer.WriteShort(mDx);
	buffer.WriteShort(mDy);
	buffer.WriteShort(mDz);
	
	return buffer;
}

BigEndianBuffer& DamageDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mType = buffer.ReadShort();
	mFlags = buffer.ReadShort();

	mBase = buffer.ReadShort();
	mRandom = buffer.ReadShort();

	mScale = buffer.ReadFixed();
	
	mGoodData = true;
	return buffer;
}

BigEndianBuffer& DamageDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteShort(mType);
	buffer.WriteShort(mFlags);

	buffer.WriteShort(mBase);
	buffer.WriteShort(mRandom);

	buffer.WriteFixed(mScale);

	return buffer;
}

static unsigned short BuildCollection(unsigned short collection, unsigned short colorTable)
{
	if (collection == 31 && colorTable == 7) {
		return 0xffff;
	} else {
		return collection | (colorTable << 5);
	}
}

BigEndianBuffer& EffectDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mCollection = buffer.ReadUShort();
	mColorTable = (mCollection >> 5) & 0x7;
	mCollection &= 0x1f;
	mShape = buffer.ReadShort();
	mSoundPitch = buffer.ReadFixed();
	mFlags = buffer.ReadUShort();
	mDelay = buffer.ReadShort();
	mDelaySound = buffer.ReadShort();

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& EffectDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteUShort(BuildCollection(mCollection, mColorTable));
	buffer.WriteShort(mShape);
	buffer.WriteFixed(mSoundPitch);
	buffer.WriteUShort(mFlags);
	buffer.WriteShort(mDelay);
	buffer.WriteShort(mDelaySound);

	return buffer;
}

BigEndianBuffer& MonsterDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mCollection = buffer.ReadUShort();
	mColorTable = (mCollection >> 5) & 0x7;
	mCollection &= 0x1f;

	mVitality = buffer.ReadShort();
	mImmunities = buffer.ReadULong();
	mWeaknesses = buffer.ReadULong();
	mFlags = buffer.ReadULong();

	mClass = buffer.ReadLong();
	mFriends = buffer.ReadLong();
	mEnemies = buffer.ReadLong();

	mSoundPitch = buffer.ReadFixed();

	mActivationSound = buffer.ReadShort();
	mFriendlyActivationSound = buffer.ReadShort();
	mClearSound = buffer.ReadShort();
	mKillSound = buffer.ReadShort();
	mApologySound = buffer.ReadShort();
	mFriendlyFireSound = buffer.ReadShort();
	mFlamingSound = buffer.ReadShort();
	mRandomSound = buffer.ReadShort();
	mRandomSoundMask = buffer.ReadShort();

	mCarryingItemType = buffer.ReadShort();
	
	mRadius = buffer.ReadShort();
	mHeight = buffer.ReadShort();
	mPreferredHoverHeight = buffer.ReadShort();
	mMinimumLedgeDelta = buffer.ReadShort();
	mMaximumLedgeDelta = buffer.ReadShort();
	mExternalVelocityScale = buffer.ReadFixed();
	mImpactEffect = buffer.ReadShort();
	mMeleeImpactEffect = buffer.ReadShort();
	mContrailEffect = buffer.ReadShort();

	mHalfVisualArc = buffer.ReadShort();
	mHalfVerticalVisualArc = buffer.ReadShort();
	mVisualRange = buffer.ReadShort();
	mDarkVisualRange = buffer.ReadShort();
	mIntelligence = buffer.ReadShort();
	mSpeed = buffer.ReadShort();
	mGravity = buffer.ReadShort();
	mTerminalVelocity = buffer.ReadShort();
	mDoorRetryMask = buffer.ReadShort();
	mShrapnelRadius = buffer.ReadShort();
	
	mShrapnelDamage.LoadObject(buffer);
	if (!mShrapnelDamage.IsGood()) {
		return buffer;
	}

	mHitShapes = buffer.ReadShort();
	mHardDyingShape = buffer.ReadShort();
	mSoftDyingShape = buffer.ReadShort();
	mHardDeadShapes = buffer.ReadShort();
	mSoftDeadShapes = buffer.ReadShort();
	mStationaryShape = buffer.ReadShort();
	mMovingShape = buffer.ReadShort();
	mTeleportInShape = buffer.ReadShort();
	mTeleportOutShape = buffer.ReadShort();

	mAttackFrequency = buffer.ReadShort();
	mMeleeAttack.LoadObject(buffer);
	if (!mMeleeAttack.IsGood()) {
		return buffer;
	}

	mRangedAttack.LoadObject(buffer);
	if (!mRangedAttack.IsGood()) {
		return buffer;
	}

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& MonsterDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteUShort(BuildCollection(mCollection, mColorTable));

	buffer.WriteShort(mVitality);
	buffer.WriteULong(mImmunities);
	buffer.WriteULong(mWeaknesses);
	buffer.WriteULong(mFlags);

	buffer.WriteLong(mClass);
	buffer.WriteLong(mFriends);
	buffer.WriteLong(mEnemies);

	buffer.WriteFixed(mSoundPitch);

	buffer.WriteShort(mActivationSound);
	buffer.WriteShort(mFriendlyActivationSound);
	buffer.WriteShort(mClearSound);
	buffer.WriteShort(mKillSound);
	buffer.WriteShort(mApologySound);
	buffer.WriteShort(mFriendlyFireSound);
	buffer.WriteShort(mFlamingSound);
	buffer.WriteShort(mRandomSound);
	buffer.WriteShort(mRandomSoundMask);

	buffer.WriteShort(mCarryingItemType);
	
	buffer.WriteShort(mRadius);
	buffer.WriteShort(mHeight);
	buffer.WriteShort(mPreferredHoverHeight);
	buffer.WriteShort(mMinimumLedgeDelta);
	buffer.WriteShort(mMaximumLedgeDelta);
	buffer.WriteFixed(mExternalVelocityScale);
	buffer.WriteShort(mImpactEffect);
	buffer.WriteShort(mMeleeImpactEffect);
	buffer.WriteShort(mContrailEffect);

	buffer.WriteShort(mHalfVisualArc);
	buffer.WriteShort(mHalfVerticalVisualArc);
	buffer.WriteShort(mVisualRange);
	buffer.WriteShort(mDarkVisualRange);
	buffer.WriteShort(mIntelligence);
	buffer.WriteShort(mSpeed);
	buffer.WriteShort(mGravity);
	buffer.WriteShort(mTerminalVelocity);
	buffer.WriteShort(mDoorRetryMask);
	buffer.WriteShort(mShrapnelRadius);

	mShrapnelDamage.SaveObject(buffer);
	
	buffer.WriteShort(mHitShapes);
	buffer.WriteShort(mHardDyingShape);
	buffer.WriteShort(mSoftDyingShape);
	buffer.WriteShort(mHardDeadShapes);
	buffer.WriteShort(mSoftDeadShapes);
	buffer.WriteShort(mStationaryShape);
	buffer.WriteShort(mMovingShape);
	buffer.WriteShort(mTeleportInShape);
	buffer.WriteShort(mTeleportOutShape);

	buffer.WriteShort(mAttackFrequency);
	mMeleeAttack.SaveObject(buffer);
	mRangedAttack.SaveObject(buffer);

	return buffer;
}

BigEndianBuffer& ProjectileDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mCollection = buffer.ReadUShort();
	mColorTable = (mCollection >> 5) & 0x7;
	mCollection &= 0x1f;
	mShape = buffer.ReadShort();
	
	mDetonationEffect = buffer.ReadShort();
	mMediaDetonationEffect = buffer.ReadShort();
	mContrailEffect = buffer.ReadShort();
	mTicksBetweenContrails = buffer.ReadShort();
	mMaximumContrails = buffer.ReadShort();
	mMediaProjectilePromotion = buffer.ReadShort();
	
	mRadius = buffer.ReadShort();
	mAreaOfEffect = buffer.ReadShort();
	mDamage.LoadObject(buffer);
	if (!mDamage.IsGood()) {
		return buffer;
	}

	mFlags = buffer.ReadULong();

	mSpeed = buffer.ReadShort();
	mMaximumRange = buffer.ReadShort();
	
	mSoundPitch = buffer.ReadFixed();
	mFlybySound = buffer.ReadShort();
	mReboundSound = buffer.ReadShort();

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& ProjectileDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteUShort(BuildCollection(mCollection, mColorTable));
	buffer.WriteShort(mShape);

	buffer.WriteShort(mDetonationEffect);
	buffer.WriteShort(mMediaDetonationEffect);
	buffer.WriteShort(mContrailEffect);
	buffer.WriteShort(mTicksBetweenContrails);
	buffer.WriteShort(mMaximumContrails);
	buffer.WriteShort(mMediaProjectilePromotion);

	buffer.WriteShort(mRadius);
	buffer.WriteShort(mAreaOfEffect);
	mDamage.SaveObject(buffer);
	
	buffer.WriteULong(mFlags);

	buffer.WriteShort(mSpeed);
	buffer.WriteShort(mMaximumRange);

	buffer.WriteFixed(mSoundPitch);
	buffer.WriteShort(mFlybySound);
	buffer.WriteShort(mReboundSound);

	return buffer;
}

BigEndianBuffer& TriggerDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mRoundsPerMagazine = buffer.ReadShort();
	mAmmunitionType = buffer.ReadShort();
	mTicksPerRound = buffer.ReadShort();
	mRecoveryTicks = buffer.ReadShort();
	mChargingTicks = buffer.ReadShort();
	mRecoilMagnitude = buffer.ReadShort();

	mFiringSound = buffer.ReadShort();
	mClickSound = buffer.ReadShort();
	mChargingSound = buffer.ReadShort();
	mShellCasingSound = buffer.ReadShort();
	mReloadingSound = buffer.ReadShort();
	mChargedSound = buffer.ReadShort();

	mProjectileType = buffer.ReadShort();
	mThetaError = buffer.ReadShort();
	mDx = buffer.ReadShort();
	mDz = buffer.ReadShort();
	mShellCasingType = buffer.ReadShort();
	mBurstCount = buffer.ReadShort();

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& TriggerDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteShort(mRoundsPerMagazine);
	buffer.WriteShort(mAmmunitionType);
	buffer.WriteShort(mTicksPerRound);
	buffer.WriteShort(mRecoveryTicks);
	buffer.WriteShort(mChargingTicks);
	buffer.WriteShort(mRecoilMagnitude);
	
	buffer.WriteShort(mFiringSound);
	buffer.WriteShort(mClickSound);
	buffer.WriteShort(mChargingSound);
	buffer.WriteShort(mShellCasingSound);
	buffer.WriteShort(mReloadingSound);
	buffer.WriteShort(mChargedSound);
	
	buffer.WriteShort(mProjectileType);
	buffer.WriteShort(mThetaError);
	buffer.WriteShort(mDx);
	buffer.WriteShort(mDz);
	buffer.WriteShort(mShellCasingType);
	buffer.WriteShort(mBurstCount);

	return buffer;
}

BigEndianBuffer& WeaponDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mItemType = buffer.ReadShort();
	mPowerupType = buffer.ReadShort();
	mWeaponClass = buffer.ReadShort();
	mFlags = buffer.ReadShort();
	
	mFiringLightIntensity = buffer.ReadFixed();
	mFiringIntensityDecayTicks = buffer.ReadShort();

	mIdleHeight = buffer.ReadFixed();
	mBobAmplitude = buffer.ReadFixed();
	mKickHeight = buffer.ReadFixed();
	mReloadHeight = buffer.ReadFixed();
	mIdleWidth = buffer.ReadFixed();
	mHorizontalAmplitude = buffer.ReadFixed();

	mCollection = buffer.ReadUShort();
	mColorTable = (mCollection >> 5) & 0x7;
	mCollection &= 0x1f;
	
	mIdleShape = buffer.ReadShort();
	mFiringShape = buffer.ReadShort();
	mReloadingShape = buffer.ReadShort();
	buffer.ReadShort(); // unused
	mChargingShape = buffer.ReadShort();
	mChargedShape = buffer.ReadShort();

	mReadyTicks = buffer.ReadShort();
	mAwaitReloadTicks = buffer.ReadShort();
	mLoadingTicks = buffer.ReadShort();
	mFinishLoadingTicks = buffer.ReadShort();
	mPowerupTicks = buffer.ReadShort();

	mPrimaryTrigger.LoadObject(buffer);
	if (!mPrimaryTrigger.IsGood()) {
		return buffer;
	}
	
	mSecondaryTrigger.LoadObject(buffer);
	if (!mSecondaryTrigger.IsGood()) {
		return buffer;
	}

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& WeaponDefinition::SaveObject(BigEndianBuffer& buffer) const
{
	buffer.WriteShort(mItemType);
	buffer.WriteShort(mPowerupType);
	buffer.WriteShort(mWeaponClass);
	buffer.WriteShort(mFlags);
	
	buffer.WriteFixed(mFiringLightIntensity);
	buffer.WriteShort(mFiringIntensityDecayTicks);

	buffer.WriteFixed(mIdleHeight);
	buffer.WriteFixed(mBobAmplitude);
	buffer.WriteFixed(mKickHeight);
	buffer.WriteFixed(mReloadHeight);
	buffer.WriteFixed(mIdleWidth);
	buffer.WriteFixed(mHorizontalAmplitude);

	buffer.WriteUShort(BuildCollection(mCollection, mColorTable));

	buffer.WriteShort(mIdleShape);
	buffer.WriteShort(mFiringShape);
	buffer.WriteShort(mReloadingShape);
	buffer.WriteShort(-1); // unused
	buffer.WriteShort(mChargingShape);
	buffer.WriteShort(mChargedShape);

	buffer.WriteShort(mReadyTicks);
	buffer.WriteShort(mAwaitReloadTicks);
	buffer.WriteShort(mLoadingTicks);
	buffer.WriteShort(mFinishLoadingTicks);
	buffer.WriteShort(mPowerupTicks);

	mPrimaryTrigger.SaveObject(buffer);
	mSecondaryTrigger.SaveObject(buffer);

	return buffer;
}

std::ostream& operator<<(std::ostream& s, const PhysicsConstants& constants)
{
	s << "Maximum Forward Velocity: " << constants.mMaximumForwardVelocity << std::endl;
	s << "Maximum Backward Velocity: " << constants.mMaximumBackwardVelocity << std::endl;
	s << "Maximum Perpendicular Velocity: " << constants.mMaximumPerpendicularVelocity << std::endl;
	s << "Acceleration: " << constants.mAcceleration << std::endl;
	s << "Deceleration: " << constants.mDeceleration << std::endl;
	s << "Airborne Deceleration: " << constants.mAirborneDeceleration << std::endl;
	s << "Gravitational Acceleration: " << constants.mGravitationalAcceleration << std::endl;
	s << "Climbing Acceleration: " << constants.mClimbingAcceleration << std::endl;
	s << "Terminal Velocity: " << constants.mTerminalVelocity << std::endl;
	s << "External Deceleration: " << constants.mExternalDeceleration << std::endl;
	s << "Angular Acceleration: " << constants.mAngularAcceleration << std::endl;
	s << "Angular Deceleration: " << constants.mAngularDeceleration << std::endl;
	s << "Maximum Angular Velocity: " << constants.mMaximumAngularVelocity << std::endl;
	s << "Angular Recentering Velocity: " << constants.mAngularRecenteringVelocity << std::endl;
	s << "Fast Angular Velocity: " << constants.mFastAngularVelocity << std::endl;
	s << "Fast Angular Maximum: " << constants.mFastAngularMaximum << std::endl;
	s << "Maximum Elevation: " << constants.mMaximumElevation << std::endl;
	s << "Eternal Angular Deceleration: " << constants.mExternalAngularDeceleration << std::endl;
	s << "Step Delta: " << constants.mStepDelta << std::endl;
	s << "Step Amplitude: " << constants.mStepAmplitude << std::endl;
	s << "Radius: " << constants.mRadius << std::endl;
	s << "Height: " << constants.mHeight << std::endl;
	s << "Dead Height: " << constants.mDeadHeight << std::endl;
	s << "Camera Height: " << constants.mCameraHeight << std::endl;
	s << "Splash Height: " << constants.mSplashHeight << std::endl;
	s << "Half Camera Separation: " << constants.mHalfCameraSeparation << std::endl;
	return s;
}

std::ostream& operator<<(std::ostream& s, const AttackDefinition& attack)
{
	s << "Type: " << attack.mType << std::endl;
	s << "Repetitions: " << attack.mRepetitions << std::endl;
	s << "Error: " << attack.mError << std::endl;
	s << "Range: " << attack.mRange << std::endl;
	s << "Attack Shape: " << attack.mAttackShape << std::endl;
	s << "dx: " << attack.mDx << std::endl;
	s << "dy: " << attack.mDy << std::endl;
	s << "dz: " << attack.mDz << std::endl;

	return s;
}

std::ostream& operator<<(std::ostream& s, const DamageDefinition& damage)
{
	s << "Type: " << damage.mType << std::endl;
	s << "Flags: " << damage.mFlags << std::endl;
	s << "Base: " << damage.mBase << std::endl;
	s << "Random: " << damage.mRandom << std::endl;
	s << "Scale: " << damage.mScale << std::endl;

	return s;
}

std::ostream& operator<<(std::ostream& s, const EffectDefinition& effect)
{
	s << "Collection: " << effect.mCollection << std::endl;
	s << "Color Table: " << effect.mColorTable << std::endl;
	s << "Shape: " << effect.mShape << std::endl;
	s << "Sound Pitch: " << effect.mSoundPitch << std::endl;
	s << "Flags: " << effect.mFlags << std::endl;
	s << "Delay: " << effect.mDelay << std::endl;
	s << "Delay Sound: " << effect.mDelaySound << std::endl;

	return s;
}

std::ostream& operator<<(std::ostream& s, const MonsterDefinition& monster)
{
	s << "mCollection: " << monster.mCollection << std::endl;
	s << "mColorTable: " << monster.mColorTable << std::endl;

	s << "mVitality: " << monster.mVitality << std::endl;
	s << "mImmunities: " << monster.mImmunities << std::endl;
	s << "mWeaknesses: " << monster.mWeaknesses << std::endl;
	s << "mFlags: " << monster.mFlags << std::endl;

	s << "mClass: " << monster.mClass << std::endl;
	s << "mFriends: " << monster.mFriends << std::endl;
	s << "mEnemies: " << monster.mEnemies << std::endl;
	
	s << "mSoundPitch: " << monster.mSoundPitch << std::endl;
	
	s << "mActivationSound: " << monster.mActivationSound << std::endl;
	s << "mFriendlyActivationSound: " << monster.mFriendlyActivationSound << std::endl;
	s << "mClearSound: " << monster.mClearSound << std::endl;
	s << "mKillSound: " << monster.mKillSound << std::endl;
	s << "mApologySound: " << monster.mApologySound << std::endl;
	s << "mFriendlyFireSound: " << monster.mFriendlyFireSound << std::endl;
	s << "mFlamingSound: " << monster.mFlamingSound << std::endl;
	s << "mRandomSound: " << monster.mRandomSound << std::endl;
	s << "mRandomSoundMask: " << monster.mRandomSoundMask << std::endl;

	s << "mCarryingItemType: " << monster.mCarryingItemType << std::endl;
	
	s << "mRadius: " << monster.mRadius << std::endl;
	s << "mHeight: " << monster.mHeight << std::endl;
	s << "mPreferredHoverHeight: " << monster.mPreferredHoverHeight << std::endl;
	s << "mMinimumLedgeDelta: " << monster.mMinimumLedgeDelta << std::endl;
	s << "mMaximumLedgeDelta: " << monster.mMaximumLedgeDelta << std::endl;
	s << "mExternalVelocityScale: " << monster.mExternalVelocityScale << std::endl;
	s << "mImpactEffect: " << monster.mImpactEffect << std::endl;
	s << "mMeleeImpactEffect: " << monster.mMeleeImpactEffect << std::endl;
	s << "mContrailEffect: " << monster.mContrailEffect << std::endl;

	s << "mHalfVisualArc: " << monster.mHalfVisualArc << std::endl;
	s << "mHalfVerticalVisualArc: " << monster.mHalfVerticalVisualArc << std::endl;
	s << "mVisualRange: " << monster.mVisualRange << std::endl;
	s << "mDarkVisualRange: " << monster.mDarkVisualRange << std::endl;
	s << "mIntelligence: " << monster.mIntelligence << std::endl;
	s << "mSpeed: " << monster.mSpeed << std::endl;
	s << "mGravity: " << monster.mGravity << std::endl;
	s << "mTerminalVelocity: " << monster.mTerminalVelocity << std::endl;
	s << "mDoorRetryMask: " << monster.mDoorRetryMask << std::endl;
	s << "mShrapnelRadius: " << monster.mShrapnelRadius << std::endl;
	
	s << "mShrapnelDamage: " << std::endl;
	s << monster.mShrapnelDamage << std::endl;

	s << "mHitShapes: " << monster.mHitShapes << std::endl;
	s << "mHardDyingShape: " << monster.mHardDyingShape << std::endl;
	s << "mSoftDyingShape: " << monster.mSoftDyingShape << std::endl;
	s << "mHardDeadShapes: " << monster.mHardDeadShapes << std::endl;
	s << "mSoftDeadShapes: " << monster.mSoftDeadShapes << std::endl;
	s << "mStationaryShape: " << monster.mStationaryShape << std::endl;
	s << "mMovingShape: " << monster.mMovingShape << std::endl;
	s << "mTeleportInShape: " << monster.mTeleportInShape << std::endl;
	s << "mTeleportOutShape: " << monster.mTeleportOutShape << std::endl;

	s << "mAttackFrequency: " << monster.mAttackFrequency << std::endl;
	s << "mMeleeAttack: " << std::endl;
	s << monster.mMeleeAttack << std::endl;
	s << "mRangedAttack: " << std::endl;
	s << monster.mRangedAttack << std::endl;

	return s;
}

std::ostream& operator<<(std::ostream& s, const ProjectileDefinition& projectile)
{
	s << "mCollection: " << projectile.mCollection << std::endl;
	s << "mColorTable: " << projectile.mColorTable << std::endl;
	s << "mShape: " << projectile.mShape << std::endl;
	
	s << "mDetonationEffect: " << projectile.mDetonationEffect << std::endl;
	s << "mMediaDetonationEffect: " << projectile.mMediaDetonationEffect << std::endl;
	s << "mContrailEffect: " << projectile.mContrailEffect << std::endl;
	s << "mTicksBetweenContrails: " << projectile.mTicksBetweenContrails << std::endl;
	s << "mMaximumContrails: " << projectile.mMaximumContrails << std::endl;
	s << "mMediaProjectilePromotion: " << projectile.mMediaProjectilePromotion << std::endl;

	s << "mRadius: " << projectile.mRadius << std::endl;
	s << "mAreaOfEffect: " << projectile.mAreaOfEffect << std::endl;
	s << "mDamage" << std::endl;
	s << projectile.mDamage;
	
	s << "mFlags: " << projectile.mFlags << std::endl;

	s << "mSpeed: " << projectile.mSpeed << std::endl;
	s << "mMaximumRange: " << projectile.mMaximumRange << std::endl;

	s << "mSoundPitch: " << projectile.mSoundPitch << std::endl;
	s << "mFlybySound: " << projectile.mFlybySound << std::endl;
	s << "mReboundSound: " << projectile.mReboundSound << std::endl;

	return s;
}

std::ostream& operator<<(std::ostream& s, const TriggerDefinition& trigger)
{
	s << "mRoundsPerMagazine: " << trigger.mRoundsPerMagazine << std::endl;
	s << "mAmmunitionType: " << trigger.mAmmunitionType << std::endl;
	s << "mTicksPerRound: " << trigger.mTicksPerRound << std::endl;
	s << "mRecoveryTicks: " << trigger.mRecoveryTicks << std::endl;
	s << "mChargingTicks: " << trigger.mChargingTicks << std::endl;
	s << "mRecoilMagnitude: " << trigger.mRecoilMagnitude << std::endl;
	
	s << "mFiringSound: " << trigger.mFiringSound << std::endl;
	s << "mClickSound: " << trigger.mClickSound << std::endl;
	s << "mChargingSound: " << trigger.mChargingSound << std::endl;
	s << "mShellCasingSound: " << trigger.mShellCasingSound << std::endl;
	s << "mReloadingSound: " << trigger.mReloadingSound << std::endl;
	s << "mChargedSound: " << trigger.mChargedSound << std::endl;
	
	s << "mProjectileType: " << trigger.mProjectileType << std::endl;
	s << "mThetaError: " << trigger.mThetaError << std::endl;
	s << "mDx: " << trigger.mDx << std::endl;
	s << "mDz: " << trigger.mDz << std::endl;
	s << "mShellCasingType: " << trigger.mShellCasingType << std::endl;
	s << "mBurstCount: " << trigger.mBurstCount << std::endl;

	return s;
}

std::ostream& operator<<(std::ostream& s, const WeaponDefinition& weapon)
{
	s << "mItemType: " << weapon.mItemType << std::endl;
	s << "mPowerupType: " << weapon.mPowerupType << std::endl;
	s << "mWeaponClass: " << weapon.mWeaponClass << std::endl;
	s << "mFlags: " << weapon.mFlags << std::endl;
	
	s << "mFiringLightIntensity: " << weapon.mFiringLightIntensity << std::endl;
	s << "mFiringIntensityDecayTicks: " << weapon.mFiringIntensityDecayTicks << std::endl;

	s << "mIdleHeight: " << weapon.mIdleHeight << std::endl;
	s << "mBobAmplitude: " << weapon.mBobAmplitude << std::endl;
	s << "mKickHeight: " << weapon.mKickHeight << std::endl;
	s << "mReloadHeight: " << weapon.mReloadHeight << std::endl;
	s << "mIdleWidth: " << weapon.mIdleWidth << std::endl;
	s << "mHorizontalAmplitude: " << weapon.mHorizontalAmplitude << std::endl;

	s << "mCollection: " << weapon.mCollection << std::endl;
	s << "mColorTable: " << weapon.mColorTable << std::endl;
	
	s << "mIdleShape: " << weapon.mIdleShape << std::endl;
	s << "mFiringShape: " << weapon.mFiringShape << std::endl;
	s << "mReloadingShape: " << weapon.mReloadingShape << std::endl;
	s << "mChargingShape: " << weapon.mChargingShape << std::endl;
	s << "mChargedShape: " << weapon.mChargedShape << std::endl;

	s << "mReadyTicks: " << weapon.mReadyTicks << std::endl;
	s << "mAwaitReloadTicks: " << weapon.mAwaitReloadTicks << std::endl;
	s << "mLoadingTicks: " << weapon.mLoadingTicks << std::endl;
	s << "mFinishLoadingTicks: " << weapon.mFinishLoadingTicks << std::endl;
	s << "mPowerupTicks: " << weapon.mPowerupTicks << std::endl;

	s << "mPrimaryTrigger: " << weapon.mPrimaryTrigger << std::endl;
	s << "mSecondaryTrigger: " << weapon.mSecondaryTrigger << std::endl;

	return s;
}
