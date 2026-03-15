#ifndef OBSERVER_H
#define OBSERVER_H

namespace DAE
{
    class GameObject;
    class Observer
    {
    public:
        virtual ~Observer() noexcept {}
        Observer(Observer const&) noexcept = delete;
        Observer(Observer&&) noexcept = delete;
        Observer& operator=(Observer const&) noexcept = delete;
        Observer& operator=(Observer&&) noexcept = delete;

        virtual void OnNotify(GameObject const& gameObject, Event event) noexcept = 0;
    };

}

#endif
