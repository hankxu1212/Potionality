#pragma once

#include "Behaviour.hpp"

class SpriteLoader;

class Potion : public Behaviour
{
public:
	// Global potion instance (change to list?)
	inline static Potion* Instance;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;

	const char* getClassName() const override { return "Potion"; }
};
