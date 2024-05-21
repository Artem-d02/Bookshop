#include "bookshop/bookshop.hpp"
#include <exception>

namespace NBookshop {

    TBook::TBook(
            const std::string& name,
            const std::string& author,
            int creationYear,
            double price,
            const std::string& publishing,
            const std::string& genre,
            size_t exemplarsInStock
    )
        :   Name_(name),
            Author_(author),
            CreationYear_(creationYear),
            Price_(price),
            Publishing_(publishing),
            Genre_(genre),
            ExemplarsInStock_(exemplarsInStock),
            ID_(BookIDCounter++)
    {}

    const std::string& TBook::Name() const {
        return Name_;
    }

    const std::string& TBook::Author() const {
        return Author_;
    }

    int TBook::CreationYear() const {
        return CreationYear_;
    }

    double TBook::Price() const {
        return Price_;
    }

    const std::string& TBook::Publishing() const {
        return Publishing_;
    }

    const std::string& TBook::Genre() const {
        return Genre_;
    }

    size_t TBook::ExemplarsInStock() const {
        return ExemplarsInStock_;
    }

    bool TBook::ChangeExemplarsInStock(int delta) {
        if (delta < 0) {
            if (std::abs(delta) > ExemplarsInStock_) {
                return false;
            }
        }
        ExemplarsInStock_ += delta;
        return true;
    }

    ui64 TBook::ID() const {
        return ID_;
    }

    TCart::TCart(ui64 cartID)
        : ID_(cartID)
    {}

    bool TCart::AddBook(const TBook& book) {
        try {
            Books_.push_back(book.ID());
            return true;
        }
        catch (const std::exception& e) {
            return false;
        }
    }

    const std::vector<ui64>& TCart::BooksInCart() const {
        return Books_;
    }

    TOrder::TOrder(ui64 ID, const std::vector<std::pair<ui64, size_t>>& books)
        : Books_(books), ID_(ID)
    {}

    TOrder::TStatus TOrder::GetStatus() const {
        return Status_;
    }

    bool TOrder::ChangeStatus(TStatus newStatus) {
        try {
            Status_ = newStatus;
            return true;
        }
        catch (const std::exception& e) {
            return false;
        }
    }

    const std::vector<std::pair<ui64, size_t>>& TOrder::Books() const {
        return Books_;
    }

    ui64 TOrder::ID() const {
        return ID_;
    }

    TConsumer::TConsumer(ui64 ID)
        : ID_(ID)
    {}

    bool TConsumer::MakeCart() {
        return true;
    }

    bool TConsumer::AddBook(const TBook& book) {
        return true;
    }

    TOrder TConsumer::MakeOrder(ui64 orderID) {
        
    }
    
    TOrder::TStatus TConsumer::GetStatus(ui64 orderID) {
        return {};
    }
    
    bool TConsumer::Refund(ui64 orderID, RefundOptions refOpt) {
        return true;
    }

    ui64 TConsumer::ID() const {
        return ID_;
    }

    const std::vector<ui64>& TConsumer::BooksInCart() const {
        return Cart_.BooksInCart();
    }

    std::vector<ui64>& TConsumer::Orders() {
        return Orders_;
    }

    bool TShop::AddBook(const TBook& book) {
        try {
            auto it = BooksInStock_.find(book.ID());
            if (it != BooksInStock_.end()) {
                return it->second.ChangeExemplarsInStock(book.ExemplarsInStock());
            }
            else {
                BooksInStock_.emplace(book.ID(), book);
            }
            return true;
        }
        catch (const std::exception& e) {
            return false;
        }
    }

    bool TShop::MakeOrder(const TOrder& newOrder) {
        //  Check books in stock
        for (const auto& [bookID, count] : newOrder.Books()) {
            if (BooksInStock_.find(bookID) == BooksInStock_.end()) {
                return false;
            }
            if (BooksInStock_[bookID].ExemplarsInStock() < count) {
                return false;
            }
        }

        //  If there are enough books
        for (const auto& [bookID, count] : newOrder.Books()) {
            BooksInStock_[bookID].ChangeExemplarsInStock(-count);
        }

        Orders_.emplace(newOrder.ID(), newOrder);

        return true;
    }

    bool TShop::DeliverOrder(ui64 orderID) {
        if (Orders_.find(orderID) == Orders_.end()) {
            return false;
        }
        Orders_[orderID].ChangeStatus(TOrder::TStatus::DONE);
        return true;
    }

    bool TShop::RefundBook(ui64 bookID) {
        return true;
    }

    bool TShop::HasBook(ui64 bookID) {
        return BooksInStock_.find(bookID) != BooksInStock_.end();
    }

    TBook& TShop::BookInfo(ui64 bookID) {
        return BooksInStock_[bookID];
    }

    TOrder& TShop::Order(ui64 orderID) {

    }
}