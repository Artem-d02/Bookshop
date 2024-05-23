#include "bookshop/bookshop.hpp"

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <algorithm>

const std::string name1 = "The last wish";
const std::string name2 = "Sword of Destiny";

const std::string autor = "Andrzej Sapkowski";

const int creationYear1 = 1993;
const int creationYear2 = 1992;

const double price1 = 30.99;
const double price2 = 35.99;

const std::string publishing = "SuperNova";

const std::string genre = "Adventure";

const size_t exCount1 = 100;
const size_t exCount2 = 50;

NBookshop::TBook book1(name1, autor, creationYear1, price1, publishing, genre, exCount1);
NBookshop::TBook book2(name2, autor, creationYear2, price2, publishing, genre, exCount2);
NBookshop::TShop shop;

TEST(BookShopTest, CheckBook)
{
  ASSERT_NO_THROW(NBookshop::TBook{});

  ASSERT_EQ(book1.Name(), name1);
  ASSERT_EQ(book1.Author(), autor);
  ASSERT_EQ(book1.CreationYear(), creationYear1);
  ASSERT_EQ(book1.Price(), price1);
  ASSERT_EQ(book1.Publishing(), publishing);
  ASSERT_EQ(book1.Genre(), genre);

  ASSERT_EQ(book1.ExemplarsInStock(), exCount1);

  const size_t deltaEx = 50;
  ASSERT_TRUE(book1.ChangeExemplarsInStock(deltaEx));
  ASSERT_EQ(book1.ExemplarsInStock(), exCount1 + deltaEx);

  const int invalidDeltaEx = -500;
  ASSERT_FALSE(book1.ChangeExemplarsInStock(invalidDeltaEx));
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
  ASSERT_NO_THROW(NBookshop::TOrder{});

  NBookshop::TOrder order(222, { { book1.ID(), 1 }, { book2.ID(), 1 } });

  ASSERT_EQ(order.Books().size(), 2);
  ASSERT_EQ(order.Books()[0].first, book1.ID());
  ASSERT_EQ(order.GetStatus(), NBookshop::TOrder::EStatus::NOT_CREATED);

  ASSERT_TRUE(order.ChangeStatus(NBookshop::TOrder::EStatus::PROCESSING));

  ASSERT_EQ(order.GetStatus(), NBookshop::TOrder::EStatus::PROCESSING);

  ASSERT_TRUE(order.ChangeRefundOption(NBookshop::TOrder::ERefundOptions::COURIER));
}

TEST(BookShopTest, checkShop) {
  ASSERT_TRUE(shop.AddBook(book1));
  ASSERT_TRUE(shop.AddBook(book2));

  ASSERT_TRUE(shop.AddBook(book2));

  ASSERT_TRUE(shop.HasBook(book1.ID()));

  ASSERT_EQ(shop.BookInfo(book1.ID()).ExemplarsInStock(), book1.ExemplarsInStock());

  const size_t book1Count = 10;
  const size_t book2Count = 5;
  NBookshop::TOrder validOrder(333, { { book1.ID(), book1Count }, { book2.ID(), book2Count } });

  ASSERT_TRUE(shop.MakeOrder(validOrder));
  ASSERT_EQ(shop.BookInfo(book1.ID()).ExemplarsInStock(), book1.ExemplarsInStock() - book1Count);

  const size_t tooManyBooks = 500;
  NBookshop::TOrder invalidOrder1(444, { { book1.ID(), tooManyBooks } }); //  too many books

  ASSERT_FALSE(shop.MakeOrder(invalidOrder1));
  ASSERT_EQ(shop.BookInfo(book1.ID()).ExemplarsInStock(), book1.ExemplarsInStock() - book1Count);

  NBookshop::TOrder invalidOrder2(555, { { 100500, 1 } }); //  book which is not in stock
  ASSERT_FALSE(shop.MakeOrder(invalidOrder2));

  ASSERT_TRUE(shop.HasOrder(validOrder.ID()));

  ASSERT_TRUE(shop.DeliverOrder(validOrder.ID()));
  ASSERT_FALSE(shop.DeliverOrder(invalidOrder1.ID()));

  ASSERT_TRUE(shop.RefundBook({ book2.ID(), 1 }));
  ASSERT_FALSE(shop.RefundBook({ 100500, 1 }));

  ASSERT_TRUE(shop.RefundOrder(validOrder.ID(), NBookshop::TOrder::ERefundOptions::SELF));

  ASSERT_FALSE(shop.RefundOrder(invalidOrder1.ID(), NBookshop::TOrder::ERefundOptions::SELF));

  ASSERT_TRUE(shop.Order(validOrder.ID()).GetStatus() == NBookshop::TOrder::EStatus::REFUNDED);
}

TEST(BookShopTest, CheckConsumer)
{
  const NBookshop::ui64 ConsumerID = 12345;
  NBookshop::TConsumer consumer(ConsumerID, &shop);

  ASSERT_EQ(consumer.ID(), ConsumerID);

  ASSERT_TRUE(consumer.MakeCart(1111));

  ASSERT_TRUE(consumer.AddBook(book1));

  ASSERT_TRUE(consumer.AddBook(book2));

  NBookshop::TBook noExistedBook;
  ASSERT_FALSE(consumer.AddBook(noExistedBook));

  ASSERT_EQ(consumer.BooksInCart().size(), 2);

  auto order1 = consumer.MakeOrder(1234567);

  ASSERT_EQ(consumer.BooksInCart().size(), 0);

  auto orderID = consumer.Orders()[0];
  auto& order1InShop = shop.Order(orderID);

  ASSERT_EQ(order1InShop.Books().size(), 2);

  ASSERT_TRUE(order1InShop.ChangeStatus(NBookshop::TOrder::EStatus::DONE));

  ASSERT_EQ(consumer.GetStatus(order1InShop.ID()), NBookshop::TOrder::EStatus::DONE);

  ASSERT_EQ(consumer.GetStatus(0), NBookshop::TOrder::EStatus::NOT_CREATED);

  ASSERT_TRUE(consumer.AddBook(book2));

  NBookshop::ui64 newOrderID = 54321;
  auto order2 = consumer.MakeOrder(newOrderID);

  auto& orders = consumer.Orders();
  ASSERT_EQ(orders.size(), 2);

  ASSERT_EQ(orders[1], newOrderID);

  auto ordForRefIt = std::find_if(orders.cbegin(), orders.cend(), [ID = newOrderID](const auto& curOrderID) { return curOrderID == ID; });
  ASSERT_NE(ordForRefIt, orders.cend());

  ASSERT_TRUE(consumer.Refund(*ordForRefIt, NBookshop::TOrder::ERefundOptions::SELF));
}

int main(int argc, char **argv)
{
  //  Start google tests
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}