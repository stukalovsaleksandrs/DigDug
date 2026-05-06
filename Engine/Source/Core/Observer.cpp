#include "Core/Observer.h"
#include <algorithm>

/*******************************************
 * Observer
 *******************************************/

Engine::Observer::~Observer() noexcept
{
    // Unsubscribing the observer from the subjects
    for (auto const pSubject: m_pSubjects)
    {
        pSubject->RemoveObserver(*this);
    }
}

void Engine::Observer::AddSubject(Subject& subject) noexcept
{
    if (std::ranges::binary_search(m_pSubjects, &subject)) return;
    m_pSubjects.push_back(&subject);
}

/*******************************************
 * Subject
 *******************************************/

void Engine::Subject::BindObserver(Observer& observer) noexcept
{
    if (std::ranges::binary_search(m_pObservers, &observer)) return;
    observer.AddSubject(*this);
    m_pObservers.push_back(&observer);
}

void Engine::Subject::RemoveObserver(Observer& observer) noexcept
{
    auto& vec = m_pObservers;
    // NOTE: Just std::erase does not work for Emscripten
    vec.erase(std::remove(vec.begin(), vec.end(), &observer), vec.end());
}

Engine::Subject::~Subject() noexcept
{
    // Notifying all the observers that the subject got deleted
    NotifyObservers(m_subjectDeletedEvent);// The observer is actually deleted by this point:/
}

void Engine::Subject::NotifyObservers(Event const event) const noexcept
{
    for (auto const pObserver : m_pObservers) pObserver->OnNotify(event, *this);
}
