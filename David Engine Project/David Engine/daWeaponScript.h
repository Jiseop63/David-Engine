#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class WeaponScript : public Script
	{
	public:
		WeaponScript();
		virtual ~WeaponScript();

		virtual void Update() override;

		void SetTextures(std::shared_ptr<Texture> first, std::shared_ptr<Texture> second)
		{
			mFirstTexture = first; mSecondTexture = second;
		}

		void WeaponAttack();


		void PlayAnimation();
		void ChangeSlotTexture();

		void AttackLogic();

	protected:
		GameObject*		mEffectObject;
		Animator*		mAnimator;
		MeshRenderer*	mRenderer;

		// 투사체 풀 만들기

	protected:
		std::shared_ptr<Texture> mFirstTexture;
		std::shared_ptr<Texture> mSecondTexture;

	private:
		bool mWeaponAttacked;
	};
}