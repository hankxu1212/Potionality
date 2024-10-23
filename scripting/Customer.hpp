#pragma once

#include "Behaviour.hpp"

class SpriteLoader;

class Customer : public Behaviour
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;

	const char* getClassName() const override { return "Customer"; }

private:
};