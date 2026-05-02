#ifndef SE_OBSERVER_H
#define SE_OBSERVER_H

// Standard
#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>

#include "Engine/Utils/Constants.h"

namespace Engine
{
    /*******************************************
     * Event
     *******************************************/

    struct Event
    {
        EventId const id;
        // static constexpr uint8_t maxArgumentCount{ 8 };
        // uint8_t argumentCount;

        explicit Event(EventId const in_id)
            : id{in_id}
            // , argumentCount{in_argumentCount}
        {}
    };

    enum class CommonEvents : EventId
    {
        SubjectDeleted = std::to_underlying(EventType::SubjectDeleted),
    };

    /*******************************************
     * Observer
     *******************************************/

    class GameObject;
    class Subject;
    class Observer
    {
    public:
        virtual ~Observer() noexcept;

        // Executed when a delegate is fired
        virtual void OnNotify(Event event, Subject const& caller) noexcept = 0;
        void AddSubject(Subject& subject) noexcept;

    protected:
        std::vector<Subject*> m_pSubjects;

    };

    /*******************************************
     * Subject
     *******************************************/

    class Subject
    {
    public:
        virtual ~Subject() noexcept;
        void BindObserver(Observer& observer) noexcept;
        void RemoveObserver(Observer& observer) noexcept;

        void NotifyObservers(Event event) const noexcept;

    private:
        Event const m_subjectDeletedEvent{
            std::to_underlying(CommonEvents::SubjectDeleted)
        };

        std::vector<Observer*> m_pObservers;
        // std::unordered_map<Event, std::vector<std::function<void()>>> m_EventToFunctions;

    };

}

#endif// SE_OBSERVER_H
