#include "Core/Observer.h"
#include <algorithm>

DAE::Observer::~Observer() noexcept
{
    // Unsubscribing the observer from the subjects
    for (auto const pSubject: m_pSubjects)
    {
        pSubject->RemoveObserver(*this);
    }
}

void DAE::Observer::AddSubject(Subject& subject) noexcept
{
    if (std::ranges::binary_search(m_pSubjects, &subject)) return;
    m_pSubjects.push_back(&subject);
}

void DAE::Subject::AddObserver(Observer& observer) noexcept
{
    if (std::ranges::binary_search(m_pObservers, &observer)) return;
    observer.AddSubject(*this);
    m_pObservers.push_back(&observer);
}

void DAE::Subject::RemoveObserver(Observer& observer) noexcept
{
    std::erase(m_pObservers, &observer);
}

void DAE::Subject::NotifyObservers(Event const event) const noexcept
{
    for (auto const pObserver : m_pObservers) pObserver->OnNotify(event, *this);
}
