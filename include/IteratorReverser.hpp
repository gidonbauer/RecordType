#ifndef RT_ITERATOR_REVERSER_HPP_
#define RT_ITERATOR_REVERSER_HPP_

#include <iterator>

namespace RT {

// - Iterator reverser class -----------------------------------------------------------------------
template <BidirContainerType Container>
class IteratorReverser {
  Container* m_container;

 public:
  constexpr IteratorReverser(Container& c) noexcept
      : m_container(&c) {}

  [[nodiscard]] constexpr auto begin() noexcept { return std::rbegin(*m_container); }
  [[nodiscard]] constexpr auto end() noexcept { return std::rend(*m_container); }
  [[nodiscard]] constexpr auto begin() const noexcept { return std::crbegin(*m_container); }
  [[nodiscard]] constexpr auto end() const noexcept { return std::crend(*m_container); }

  [[nodiscard]] constexpr auto cbegin() const noexcept { return std::crbegin(*m_container); }
  [[nodiscard]] constexpr auto cend() const noexcept { return std::crend(*m_container); }

  [[nodiscard]] constexpr auto rbegin() noexcept { std::begin(*m_container); }
  [[nodiscard]] constexpr auto rend() noexcept { std::end(*m_container); }
  [[nodiscard]] constexpr auto rbegin() const noexcept { std::cbegin(*m_container); }
  [[nodiscard]] constexpr auto rend() const noexcept { std::cend(*m_container); }

  [[nodiscard]] constexpr auto crbegin() const noexcept { std::cbegin(*m_container); }
  [[nodiscard]] constexpr auto crend() const noexcept { std::cend(*m_container); }
};

}  // namespace RT

#endif  // RT_ITERATOR_REVERSER_HPP_