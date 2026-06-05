// Game
#include "Constants.hpp"
#include "PlayerStateMachine.hpp"
// Engine
#include "Engine/Scene/GameObject.hpp"

namespace Game
{
    using Player::State::StateBase;
    using Player::State::StateType;

#pragma region StateBase
    bool StateBase::TryDigging() const noexcept
    {
        auto const worldPosition{m_dependencies.owner.GetWorldLocation()};
        return m_dependencies.grid.TryDigging(
        {
            static_cast<int32_t>(worldPosition.x),
            static_cast<int32_t>(worldPosition.y),
        });
    }
#pragma endregion StateBase

#pragma region Idle
    Player::State::Idle::Idle(Dependencies const& dependencies) noexcept
        : StateBase(dependencies){}

    void Player::State::Idle::OnEnter() noexcept
    {
        m_dependencies.animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)},
            1
        );
    }

    StateType Player::State::Idle::Update() noexcept
    {
        if (IsMoving()) return typeid(Walking);
        return std::nullopt;
    }
#pragma endregion Idle

#pragma region Walking
    Player::State::Walking::Walking(Dependencies const& dependencies) noexcept
        : StateBase(dependencies) {}

    void Player::State::Walking::OnEnter() noexcept
    {
        m_dependencies.animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)},
            2
        );
    }

    StateType Player::State::Walking::Update() noexcept
    {
        // Not moving -> idle
        if (auto& movementComponent{ m_dependencies.movementComponent };
            !movementComponent.IsMoving())
            return typeid(Idle);

        // Digging -> switching state
        if (TryDigging()) return typeid(Digging);

        return std::nullopt;
    }
#pragma endregion Walking

#pragma region Digging
    Player::State::Digging::Digging(Dependencies const& dependencies) noexcept
        : StateBase(dependencies)
        , m_pSDLRenderer{ Engine::Renderer::GetInstance().GetSDLRenderer() }
    {
        // Registering the tunnel digging render callback to the renderer
        Engine::Renderer::GetInstance().RegisterFunction(m_renderTunnelsFunction, Engine::Renderer::Layer::background);

        // TODO: Wrap it all up, the game should not touch SDL

        // Creating mask texture
        m_maskTexture = SDL_CreateTexture(m_pSDLRenderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET,
                                           static_cast<int32_t>(windowData.logicalDims.x),
                                           static_cast<int32_t>(windowData.logicalDims.y));

        // Initializing the mask with all white
        SDL_SetRenderTarget(m_pSDLRenderer, m_maskTexture);
        SDL_SetRenderDrawColor(m_pSDLRenderer, 255, 255, 255, 255);
        SDL_RenderClear(m_pSDLRenderer);
        SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
    }

    Player::State::Digging::~Digging() noexcept
    {
        SDL_DestroyTexture(m_maskTexture);// TODO: Use RAII wrapper for texture

        // Unregistering the tunnel digging render callback from the renderer
        Engine::Renderer::GetInstance().UnregisterFunction(m_renderTunnelsFunction);
    }

    void Player::State::Digging::RenderTunnels() const noexcept
    {
        // Rendering the tunnels
        SDL_SetTextureBlendMode(m_maskTexture, SDL_BLENDMODE_MOD);
        SDL_RenderTexture(m_pSDLRenderer, m_maskTexture, nullptr, nullptr);
    }

    void Player::State::Digging::Dig() const noexcept
    {
        // Digging a circle
        SDL_SetRenderTarget(m_pSDLRenderer, m_maskTexture);
        static auto constexpr offset{ 0.5f * glm::vec2{tileSideLength, tileSideLength} };
        static float constexpr halfTileSideLength{ 0.5f * tileSideLength };
        Engine::Renderer::GetInstance().RenderFilledCircle(m_dependencies.owner.GetWorldLocation() + offset, halfTileSideLength);
        SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
    }

    void Player::State::Digging::OnEnter() noexcept
    {
        // TODO: Change sprite sheet view

    }

    StateType Player::State::Digging::Update() noexcept
    {
        // Digging
        Dig();

        // Trying to switch state
        if (!(TryDigging() and IsMoving())) return typeid(Idle);
        return std::nullopt;
    }

#pragma endregion Digging

#pragma region StateMachine
    Player::StateMachine::StateMachine(State::Dependencies const& dependencies) noexcept
    : m_states([&dependencies]{
        // NOTE: Direct construction does not work since it requires copy contructors
        std::unordered_map<std::type_index, std::unique_ptr<StateBase>> states;
        states.emplace(typeid(State::Idle), std::make_unique<State::Idle>(dependencies));
        states.emplace(typeid(State::Walking), std::make_unique<State::Walking>(dependencies));
        states.emplace(typeid(State::Digging), std::make_unique<State::Digging>(dependencies));
        return states;
    }())
    , m_pCurrentState{ StatesAt(typeid(State::Idle)) }
    {
        m_pCurrentState->OnEnter();
    }

    void Player::StateMachine::Update() noexcept
    {
        TryChangingState(
            m_pCurrentState->Update()
        );
    }

    void Player::StateMachine::TryChangingState(StateType const stateType)
    {
        if (!stateType.has_value()) return;
        if (stateType.value() == typeid(*m_pCurrentState)) return;
        m_pCurrentState->OnExit();
        m_pCurrentState = StatesAt(stateType.value());
        m_pCurrentState->OnEnter();
    }
#pragma endregion StateMachine

}
