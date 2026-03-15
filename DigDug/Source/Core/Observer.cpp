#include "Core/Observer.h"
#include <algorithm>
#include <utility>

void DAE::Subject::AddObserver(Observer& observer) noexcept
{
    if (std::ranges::binary_search(m_pObservers, &observer)) return;
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
