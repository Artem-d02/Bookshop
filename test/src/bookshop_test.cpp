#include "bookshop/bookshop.hpp"

#include <gtest/gtest.h>
#include <vector>

NBookshop::TBook book1("The last wish", "Andrzej Sapkowski", 1993, 30.99, "SuperNova", "Adventure", 100);
NBookshop::TBook book2("Sword of Destiny", "Andrzej Sapkowski", 1992, 35.99, "SuperNova", "Adventure", 50);

TEST(BookShopTest, CheckBook)
{
  ASSERT_EQ(book1.Name(), "The last wish");
}

TEST(BookShopTest, CheckCart)
{
  NBookshop::TCart cart(1111);
  cart.AddBook(book1.ID());
  cart.AddBook(book2.ID());
  ASSERT_EQ(cart.BooksInCart().size(), 2);
  ASSERT_EQ(cart.BooksInCart()[0], book1.ID());
  ASSERT_EQ(cart.BooksInCart()[1], book2.ID());
}

TEST(BookShopTest, CheckOrder)
{
  std::vector<NBookshop::ui64> bookIDs = { book1.ID(), book2.ID() };
  NBookshop::TOrder order(222, bookIDs);

  ASSERT_EQ(order.Books().size(), 2);
  ASSERT_EQ(order.Books()[0], book1.ID());
  ASSERT_EQ(order.GetStatus(), NBookshop::TOrder::TStatus::NOT_CREATED);

  order.ChangeStatus(NBookshop::TOrder::TStatus::PROCESSING);

  ASSERT_EQ(order.GetStatus(), NBookshop::TOrder::TStatus::PROCESSING);
}

TEST(BookShopTest, CheckConsumer)
{
  NBookshop::TConsumer consumer(12345);

  consumer.MakeCart();
  consumer.AddBook(book1.ID());
  consumer.AddBook(book2.ID());

  ASSERT_EQ(consumer.BooksInCart().size(), 2);

  consumer.MakeOrder(1234567);

  ASSERT_EQ(consumer.BooksInCart().size(), 0);

  auto order1 = consumer.Orders()[0];

  ASSERT_EQ(order1.Books().size(), 2);

  order1.ChangeStatus(NBookshop::TOrder::TStatus::DONE);

  ASSERT_EQ(consumer.GetStatus(order1.ID()), NBookshop::TOrder::TStatus::DONE);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}