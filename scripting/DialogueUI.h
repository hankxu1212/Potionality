#pragma once

#include "Behaviour.hpp"

class SpriteRenderer;

class DialogueUI : public Behaviour
{
public:
	inline static DialogueUI* Instance;

	DialogueUI() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;

	void StartConversation(const std::string& newText);
	void ContinueConversation();
	bool CanContinue();

	void Enable();
	void Disable();

	virtual const char* getClassName() const override { return "DialogueUI"; }

private:
	SpriteRenderer* spriteRenderer;
	std::string currentText;
	
	float timeSinceUpdate;
	const float animationPeriod = 0.1;
	bool isInAnimation = false;
	uint32_t charactersShown = 0;

	bool canContinue = true;
};