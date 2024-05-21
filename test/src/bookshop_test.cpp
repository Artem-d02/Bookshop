#include "bookshop/bookshop.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

NBookshop::TBook book1("The last wish", "Andrzej Sapkowski", 1993, 30.99, "SuperNova", "Adventure", 100);
NBookshop::TBook book2("Sword of Destiny", "Andrzej Sapkowski", 1992, 35.99, "SuperNova", "Adventure", 50);
NBookshop::TShop shop;

TEST(BookShopTest, CheckBook)
{
  ASSERT_EQ(book1.Name(), "The last wish");
  ASSERT_EQ(book1.ExemplarsInStock(), 100);

  book1.ChangeExemplarsInStock(50);
  ASSERT_EQ(book1.ExemplarsInStock(), 150);
}

TEST(BookShopTest, CheckCart)
{
  NBookshop::TCart cart(1111);
  cart.AddBook(book1);
  cart.AddBook(book2);
  ASSERT_EQ(cart.BooksInCart().size(), 2);
  ASSERT_EQ(cart.BooksInCart()[0], book1.ID());
  ASSERT_EQ(cart.BooksInCart()[1], book2.ID());
}

TEST(BookShopTest, CheckOrder)
{
  NBookshop::TOrder order(222, { { book1.ID(), 1 }, { book2.ID(), 1 } });

  ASSERT_EQ(order.Books().size(), 2);
  ASSERT_EQ(order.Books()[0].first, book1.ID());
  ASSERT_EQ(order.GetStatus(), NBookshop::TOrder::TStatus::NOT_CREATED);

  ASSERT_TRUE(order.ChangeStatus(NBookshop::TOrder::TStatus::PROCESSING));

  ASSERT_EQ(order.GetStatus(), NBookshop::TOrder::TStatus::PROCESSING);
}

TEST(BookShopTest, checkShop) {
  ASSERT_TRUE(shop.AddBook(book1));
  ASSERT_TRUE(shop.AddBook(book2));

  ASSERT_TRUE(shop.HasBook(book1.ID()));

  ASSERT_EQ(shop.BookInfo(book1.ID()).ExemplarsInStock(), book1.ExemplarsInStock());

  const size_t book1Count = 10;
  const size_t book2Count = 5;
  NBookshop::TOrder validOrder(333, { { book1.ID(), book1Count }, { book2.ID(), book2Count } });

  ASSERT_TRUE(shop.MakeOrder(validOrder));
  ASSERT_EQ(shop.BookInfo(book1.ID()).ExemplarsInStock(), book1.ExemplarsInStock() - book1Count);

  const size_t tooManyBooks = 500;
  NBookshop::TOrder invalidOrder(444, { { book1.ID(), tooManyBooks } }); //  too many books

  ASSERT_FALSE(shop.MakeOrder(invalidOrder));
  ASSERT_EQ(shop.BookInfo(book1.ID()).ExemplarsInStock(), book1.ExemplarsInStock() - book1Count);

  ASSERT_TRUE(shop.HasOrder(validOrder.ID()));

  ASSERT_TRUE(shop.DeliverOrder(validOrder.ID()));
  ASSERT_FALSE(shop.DeliverOrder(invalidOrder.ID()));

  ASSERT_TRUE(shop.RefundBook({ book2.ID(), 1 }));

  ASSERT_TRUE(shop.RefundOrder(validOrder.ID()));

  ASSERT_TRUE(shop.Order(validOrder.ID()).GetStatus() == NBookshop::TOrder::TStatus::REFUNDED);
}

TEST(BookShopTest, CheckConsumer)
{
  NBookshop::TConsumer consumer(12345, &shop);

  ASSERT_TRUE(consumer.MakeCart(1111));

  ASSERT_TRUE(consumer.AddBook(book1));

  ASSERT_TRUE(consumer.AddBook(book2));

  ASSERT_EQ(consumer.BooksInCart().size(), 2);

  auto order1 = consumer.MakeOrder(1234567);

  ASSERT_EQ(consumer.BooksInCart().size(), 0);

  auto orderID = consumer.Orders()[0];
  auto& order1InShop = shop.Order(orderID);

  
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}