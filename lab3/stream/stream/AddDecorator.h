#include <utility>
#include <memory>

template <typename Functionality, typename... Args>
auto AddFunc(const Args&... args)
{
	return [=](auto&& b) {
		return std::make_unique<Functionality>(std::forward<decltype(b)>(b), args...);
	};
}


template <typename Component, typename Decorator>
auto operator<<(Component&& component, const Decorator& decorate)
{
	return decorate(std::forward<Component>(component));
}