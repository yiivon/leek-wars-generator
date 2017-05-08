#ifndef SRC_EFFECT_EFFECTRELATIVESHIELD_HPP_
#define SRC_EFFECT_EFFECTRELATIVESHIELD_HPP_

#include "Effect.hpp"

class EffectRelativeShield: public Effect {
public:
	EffectRelativeShield();
	virtual ~EffectRelativeShield();

	virtual void apply(Fight* fight) override;
	virtual int getEffectValue() const override;
};

#endif
