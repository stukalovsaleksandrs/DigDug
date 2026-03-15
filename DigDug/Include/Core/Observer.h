#ifndef OBSERVER_H
#define OBSERVER_H
#include <cstddef>
#include <cstdint>
#include <functional>

namespace DAE
{
    /*******************************************
     * Event
     *******************************************/

    template <int length> struct SDBMHash
    {
        consteval static unsigned int _Calculate(const char* const text, unsigned int& value) {
            const unsigned int character = SDBMHash<length - 1>::_Calculate(text, value);
            value = character + (value << 6) + (value << 16) - value;
            return text[length - 1];
        }
        consteval static unsigned int Calculate(const char* const text) {
            unsigned int value = 0;
            const auto character = _Calculate(text, value);
            return character + (value << 6) + (value << 16) - value;
        }
    };
    template <> struct SDBMHash<1> {
        consteval static int _Calculate(const char* const text, unsigned int& ) { return text[0]; }
    };
    template <size_t N> consteval unsigned int MakeSDBMHash(const char (&text)[N]) {
        return SDBMHash<N - 1>::Calculate(text);
    }

    struct EventArgument{};

    using EventId = uint32_t;

    struct Event
    {
        EventId const id;
        static constexpr uint8_t maxArgumentCount{ 8 };
        uint8_t argumentCount;
        // TODO: Use memory pool
        std::array<EventArgument, maxArgumentCount> arguments{};

        explicit Event(EventId const in_id, uint8_t const in_argumentCount = 0)
            : id{in_id}
            , argumentCount{in_argumentCount}
        {}
    };

    enum class CommonEvents : EventId
    {
        SubjectDeleted = MakeSDBMHash("SubjectDeleted")
    };

    /*******************************************
     * Observer
     *******************************************/

    class GameObject;
    class Subject;
    class Observer
    {
    public:
        virtual ~Observer() noexcept = default;

        // Executed when a delegate is fired
        virtual void OnNotify(Event event, Subject const& caller) noexcept = 0;
    };

    /*******************************************
     * Subject
     *******************************************/

    class Subject
    {
    public:
        void AddObserver(Observer& observer) noexcept;
        void RemoveObserver(Observer& observer) noexcept;

        virtual ~Subject() noexcept = default;

    protected:
        void NotifyObservers(Event event) const noexcept;
        std::vector<Observer*> m_pObservers;
        // std::unordered_map<Event, std::vector<std::function<void()>>> m_EventToFunctions;

    };

}

#endif
