// Engine
#include "Utils/Utils.hpp"
#include "Sound/SDLSoundService.hpp"
// Third-party
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"
// Standard
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#pragma region Impl
class Engine::SDLSoundService::Impl final
{
public:
#pragma region Messages
    struct Message
    {
        virtual ~Message() noexcept = default;
    };

    struct LoadMessage final : Message
    {
        explicit LoadMessage(SoundId const _id, std::string_view const _path) noexcept : Message(), id{ _id }, path{ _path }{}
        SoundId id;
        std::string path;
    };

    struct PlayMessage final : Message
    {
        explicit PlayMessage(SoundData const& data) noexcept : Message(), data{ data }{}
        SoundData data;
    };

    struct StopMessage final : Message{};

    struct VolumeMessage final : Message
    {
        explicit VolumeMessage(float const volume) noexcept : Message(), volume{ volume }{}
        float volume;
    };
#pragma endregion Messages

    Impl()
    {
        // 1. Initializing SDL_mixer
        Utils::Check(MIX_Init(), "MIX_Init failed");

        // 2. Creating a mixer instance
        m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        Utils::Check(m_pMixer,"Mixer creation failed");

        // 3. Creating a sound effect track
        m_pTrack = MIX_CreateTrack(m_pMixer);
        Utils::Check(m_pTrack,"Sound effect track creation failed");
    }

    ~Impl()
    {
        // Destroying audio objects
        for (auto* pAudio : m_pSounds)
        {
            MIX_DestroyAudio(pAudio);
        }

        // Stopping playback globally
        MIX_StopTrack(m_pTrack, 0);

        // Destroying track first
        MIX_DestroyTrack(m_pTrack);

        // Destroying mixer
        MIX_DestroyMixer(m_pMixer);

        // Shutting the service down
        MIX_Quit();
    }
    Impl(Impl&&) noexcept = delete;
    Impl& operator=(Impl&&) noexcept = delete;
    Impl(Impl const&) noexcept = delete;
    Impl& operator=(Impl const&) noexcept = delete;

    [[nodiscard]] SoundId LoadSound(std::string_view const path) noexcept
    {
        m_pSounds.push_back(nullptr);// Will get populated in another thread
        SoundId const soundId{ static_cast<SoundId>(m_pSounds.size() - 1) };
        PushMessage(std::make_unique<LoadMessage>(soundId, path));
        return soundId;
    }

    void PlaySound(SoundData const& data) noexcept
    {
        PushMessage(std::make_unique<PlayMessage>(data));
    }

    void StopAllSounds() noexcept
    {
        PushMessage(std::make_unique<StopMessage>());
    }

    void SetVolume(float const volume) noexcept
    {
        PushMessage(std::make_unique<VolumeMessage>(volume));
    }

private:
    MIX_Mixer* m_pMixer{};
    MIX_Track* m_pTrack{};

    std::unordered_map<SoundId, uint32_t> m_idToIdx{};
    std::vector<MIX_Audio*> m_pSounds{};

    // Ring buffer
    std::array<std::unique_ptr<Message>, 6> m_messageQueue{};
    uint32_t m_head{}, m_tail{};
    std::mutex m_messageQueueMutex{};
    std::condition_variable_any m_messageCV{};

    void PushMessage(std::unique_ptr<Message>&& pMessage) noexcept
    {
        assert((m_tail + 1) % m_messageQueue.size() != m_head);
        std::scoped_lock const lock{m_messageQueueMutex};
        m_messageQueue.at(m_tail) = std::move(pMessage);
        m_tail = (m_tail + 1) % m_messageQueue.size();
        m_messageCV.notify_one();
    }

    void _LoadSound(LoadMessage const& message)
    {
        MIX_Audio* const pAudio{ MIX_LoadAudio(m_pMixer, message.path.data(), true) };
        Utils::Check(pAudio, std::format("Failed to load {}", message.path));
        m_pSounds.at(message.id) = pAudio;
    }

    void _PlaySound(SoundData const& data) const noexcept
    {
        // TODO: Manage looping sounds
        MIX_SetTrackAudio(m_pTrack, m_pSounds.at(data.id));
        MIX_PlayTrack(m_pTrack, 0);
    }

    [[nodiscard]] bool HasMessages() const noexcept
    {
        return m_tail != m_head;
    }

    void ProcessMessage(Message const& message)
    {
        // NOTE: I know it's unoptimized to do it all the time,
        // but I'm already doodling around here for too long
        if (auto* const pLoadMessage{ dynamic_cast<LoadMessage const*>(&message)})
        {
            _LoadSound(*pLoadMessage);
        }
        else if (auto* const pPlayMessage{ dynamic_cast<PlayMessage const*>(&message)})
        {
            _PlaySound(pPlayMessage->data);
        }
        else if (dynamic_cast<StopMessage const*>(&message))
        {
            MIX_StopTrack(m_pTrack, 10);
        }
        else if (auto* const pVolumeMessage{ dynamic_cast<VolumeMessage const*>(&message)})
        {
            MIX_SetMixerGain(m_pMixer, pVolumeMessage->volume);
        }
        else
        {
            throw std::runtime_error{"Unknown sound message type"};
        }
    }

    void ProcessMessages(std::stop_token const stopToken) noexcept
    {
        // 1. Creating a lock to reuse later
        std::unique_lock messageLock{m_messageQueueMutex};
        while (true)
        {
            // 2. Waiting for the message buffer to have something
            // NOTE: Automatically unlocks while waiting and locks once awoken
            m_messageCV.wait(messageLock, stopToken, [&]{ return HasMessages(); });
            // 2.1. Cancelling the execution if stop was requested while waiting
            if (stopToken.stop_requested()) return;

            // 3. Messages available, processing the oldest
            // 3.1. Retrieving the oldest message
            std::unique_ptr pMessage{ std::move(m_messageQueue.at(m_head)) };
            // 3.2. Incrementing the head
            // NOTE: No need to check if head is at the tail since it's checked above
            m_head = (m_head + 1) % m_messageQueue.size();
            // 3.3. Unlocking the mutex to allow new messages come in during playback
            messageLock.unlock();
            // 3.4. Processing the message
            // TODO: Process looping sounds
            ProcessMessage(*pMessage.get());
            // 3.5. Locking the mutex back since wait() requires it
            messageLock.lock();
        }
    }

    std::jthread m_soundThread{ &Impl::ProcessMessages, this };

};
#pragma endregion Impl

#pragma region SDLSoundService
Engine::SDLSoundService::SDLSoundService() noexcept
    : m_pImpl{ std::make_unique<Impl>() }{}

// Destructor requires the implementation class to be defined, see:
// https://stackoverflow.com/questions/34072862/why-is-error-invalid-application-of-sizeof-to-an-incomplete-type-using-uniqu/34073093
// https://cpppatterns.com/patterns/pimpl.html
Engine::SDLSoundService::~SDLSoundService() noexcept = default;

Engine::SoundId Engine::SDLSoundService::LoadSound(std::string_view const path)
{
    return m_pImpl->LoadSound(path);
}

void Engine::SDLSoundService::PlaySound(SoundData const& soundData) noexcept
{
    m_pImpl->PlaySound(soundData);
}

void Engine::SDLSoundService::StopAllSounds() noexcept
{
    m_pImpl->StopAllSounds();
}

void Engine::SDLSoundService::SetVolume(float const volume) noexcept
{
    m_pImpl->SetVolume(volume);
}
#pragma endregion SDLSoundService
