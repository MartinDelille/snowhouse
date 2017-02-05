#include <deque>
#include <list>
#include <set>

#include "tests.h"

using namespace snowhouse;

static const char* ExpectedActual = "\nActual: [ 1, 2, 3, 5, 8 ]";

static void insert_numbers(std::multiset<int>& container)
{
  container.insert(1);
  container.insert(2);
  container.insert(3);
  container.insert(5);
  container.insert(8);
}

static void insert_numbers(std::set<int>& container)
{
  container.insert(1);
  container.insert(2);
  container.insert(3);
  container.insert(5);
  container.insert(8);
}

#if __cplusplus >= 201103L
#include <forward_list>

static void insert_numbers(std::forward_list<int>& container)
{
  container.push_front(8);
  container.push_front(5);
  container.push_front(3);
  container.push_front(2);
  container.push_front(1);
}
#endif

template<typename T>
static void insert_numbers(T& container)
{
  container.push_back(1);
  container.push_back(2);
  container.push_back(3);
  container.push_back(5);
  container.push_back(8);
}

template<typename T>
void TestHasAll(const T& container)
{
  it("handles All()");
  {
    AssertThat(container, Has().All().GreaterThan(1).Or().LessThan(4));
  }

  it("handles failing All()");
  {
    AssertTestFails(AssertThat(container, Has().All().GreaterThan(4)), std::string("Expected: all greater than 4") + ExpectedActual);
  }

  it("handles invalid expression after All()");
  {
    AssertTestFails(AssertThat(container, Has().All().Not()), "The expression contains a not operator without any operand");
  }

  it("handles no expression after All()");
  {
    AssertTestFails(AssertThat(container, Has().All()), "The expression after \"all\" operator does not yield any result");
  }
}

#if __cplusplus >= 201103L
template<>
void TestHasAll(const std::forward_list<int>&)
{
  // The constraint is size-based but there is no size() method available
}
#endif

template<typename T>
void TestLength(const T& container)
{
  it("handles HasLength()");
  {
    AssertThat(container, HasLength(5));
  }

  it("handles failing HasLength()");
  {
    AssertTestFails(AssertThat(container, HasLength(7)), std::string("of length 7") + ExpectedActual);
  }

  it("handles Is().OfLength()");
  {
    AssertThat(container, Is().OfLength(5));
  }

  it("handles failing Is().OfLength()");
  {
    AssertTestFails(AssertThat(container, Is().OfLength(7)), std::string("of length 7") + ExpectedActual);
  }
}

#if __cplusplus >= 201103L
template<>
void TestLength(const std::forward_list<int>&)
{
  // There is no size() method available
}
#endif

template<typename T>
void TestEmpty(const T& container)
{
  it("handles IsEmpty()");
  {
    T is_empty;

    AssertThat(is_empty, IsEmpty());
  }

  it("handles failing IsEmpty()");
  {
    AssertTestFails(AssertThat(container, IsEmpty()), "of length 0");
  }

  it("handles Is().Empty()");
  {
    T is_empty;

    AssertThat(is_empty, Is().Empty());
  }

  it("handles failing Is().Empty()");
  {
    AssertTestFails(AssertThat(container, Is().Empty()), "of length 0");
  }
}

#if __cplusplus >= 201103L
template<>
void TestEmpty(const std::forward_list<int>&)
{
  // The constraint is size-based but there is no size() method available
}
#endif

template<typename T>
void SequenceContainerActual()
{
  T container;
  container.clear();
  insert_numbers(container);

  TestHasAll(container);

  it("handles AtLeast()");
  {
    AssertThat(container, Has().AtLeast(1).LessThan(5));
  }

  it("handles failing AtLeast()");
  {
    AssertTestFails(AssertThat(container, Has().AtLeast(2).LessThan(2)), std::string("Expected: at least 2 less than 2") + ExpectedActual);
  }

  it("handles Exactly()");
  {
    AssertThat(container, Has().Exactly(1).EqualTo(3));
  }

  it("handles failing Exactly()");
  {
    AssertTestFails(AssertThat(container, Has().Exactly(2).EqualTo(3)), std::string("Expected: exactly 2 equal to 3") + ExpectedActual);
  }

  it("handles AtMost()");
  {
    AssertThat(container, Has().AtMost(1).EqualTo(5));
  }

  it("handles failing AtMost()");
  {
    AssertTestFails(AssertThat(container, Has().AtMost(1).EqualTo(3).Or().EqualTo(5)), std::string("Expected: at most 1 equal to 3 or equal to 5") + ExpectedActual);
  }

  it("handles None()");
  {
    AssertThat(container, Has().None().EqualTo(666));
  }

  it("handles failing None()");
  {
    AssertTestFails(AssertThat(container, Has().None().EqualTo(5)), std::string("Expected: none equal to 5") + ExpectedActual);
  }

  it("handles Contains()");
  {
    AssertThat(container, Contains(3));
  }

  it("detects failing Contains()");
  {
    AssertTestFails(AssertThat(container, Contains(99)), std::string("contains 99") + ExpectedActual);
  }

  it("handles Is().Containing()");
  {
    AssertThat(container, Is().Containing(3));
  }

  it("detects failing Is().Containing()");
  {
    AssertTestFails(AssertThat(container, Is().Containing(99)), std::string("contains 99") + ExpectedActual);
  }

  TestLength(container);

  TestEmpty(container);

  it("handles EqualsContainer()");
  {
    std::list<int> expected;
    expected.assign(container.begin(), container.end());

    AssertThat(container, EqualsContainer(expected));
  }

  it("handles failing EqualsContainer()");
  {
    const int e[] = {4, 2, 4};
    std::list<int> expected(e, e + sizeof(e) / sizeof(e[0]));

    AssertTestFails(AssertThat(container, EqualsContainer(expected)), "Expected: [ 4, 2, 4 ]");
  }

  it("handles Is().EqualToContainer()");
  {
    std::list<int> expected;
    expected.assign(container.begin(), container.end());

    AssertThat(container, Is().EqualToContainer(expected));
  }

  it("handles failing Is().EqualToContainer()");
  {
    const int e[] = {4, 2, 4};
    std::list<int> expected(e, e + sizeof(e) / sizeof(e[0]));

    AssertTestFails(AssertThat(container, Is().EqualToContainer(expected)), "Expected: [ 4, 2, 4 ]");
  }
}

void SequenceContainerTests()
{
  describe("Sequence containers (std::vector)");
  SequenceContainerActual<std::vector<int> >();

  describe("Sequence containers (std::list)");
  SequenceContainerActual<std::list<int> >();

  describe("Sequence containers (std::deque)");
  SequenceContainerActual<std::deque<int> >();

  describe("Sequence containers (std::set)");
  SequenceContainerActual<std::set<int> >();

  describe("Sequence containers (std::multiset)");
  SequenceContainerActual<std::multiset<int> >();

#if __cplusplus >= 201103L
  describe("Sequence containers (std::forward_list)");
  SequenceContainerActual<std::forward_list<int> >();
#endif
}
