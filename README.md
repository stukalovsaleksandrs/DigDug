# Dig Dug

A game engine and a game that are gradually developed during the Programming 4 course at Howest Digital and Entertainment in the academic year 2025/2026.

Dig Dug is a 1982 maze video game developed and published by Namco for Japanese and European arcades; it was distributed by Atari, Inc. in North America. The player digs underground tunnels to attack enemies in each level, by either inflating them to bursting or crushing them underneath rocks <sup>[Source](https://en.wikipedia.org/wiki/Dig_Dug)</sup>.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/cmake.yml/badge.svg)](https://github.com/stukalovsaleksandrs/DigDug/cmake/actions)
[![Build Status](https://github.com/avadae/minigin/actions/workflows/emscripten.yml/badge.svg)](https://github.com/stukalovsaleksandrs/DigDug/emscripten/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/stukalovsaleksandrs/DigDug/releases/latest)

# State machine
The states are represented as classes implementing the following interface:
```c++
// Engine/Include/Engine/State.h
class IState;
using pState = std::unique_ptr<IState>;
class IState
{
public:
    virtual ~IState() = default;
    virtual pState Update() noexcept = 0;
    virtual void OnEnter() noexcept = 0;
    virtual void OnExit() noexcept = 0;
};
```
Currently, the only use case is player state machine, and it involves 2 states: `Idle` and `Walking`(see `PlayerStateMachine.h`):

```c++
// Game/Include/PlayerStateMachine.h
class Idle final : public Engine::IState
{
public:
    explicit Idle(Dependencies const&) noexcept;
    void OnEnter() noexcept override;
    void OnExit() noexcept override{}
    Engine::pState Update() noexcept override;

private:
    Dependencies m_dependencies;

};

class Walking final : public Engine::IState
{
public:
    explicit Walking(Dependencies const&) noexcept;
    void OnEnter() noexcept override;
    void OnExit() noexcept override{}
    Engine::pState Update() noexcept override;

private:
    Dependencies m_dependencies;

};
```
The current state machine is managed by the `PlayerStateMachine` class. It manages current state initialization, update and transition logic:
```c++
// Game/Source/PlayerStateMachine.cpp
Player::StateMachine::StateMachine(State::Dependencies const& dependencies) noexcept
    : m_pCurrentState{ std::make_unique<State::Idle>(dependencies) }
{
    m_pCurrentState->OnEnter();
}

void Player::StateMachine::Update() noexcept
{
    TryChangingState(
        m_pCurrentState->Update()
    );
}

void Player::StateMachine::TryChangingState(Engine::pState pState)
{
    if (!pState) return;
    if (typeid(*pState) == typeid(*m_pCurrentState)) return;
    m_pCurrentState->OnExit();
    m_pCurrentState = std::move(pState);
    m_pCurrentState->OnEnter();
}
```
The next in the line is Digging. It will be added soon, together with the digging mechanic.
***
Sprites ripped by JDASTER64
