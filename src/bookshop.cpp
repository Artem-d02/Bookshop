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

    size_t& TBook::ExemplarsInStock() {
        return ExemplarsInStock_;
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
        return true;
    }

    bool TShop::MakeOrder(const TOrder& newOrder) {
        return true;
    }

    bool TShop::DeliverOrder(ui64 orderID) {
        return true;
    }

    bool TShop::RefundBook(ui64 bookID) {
        return true;
    }

    TBook& TShop::BookInfo(ui64 bookID) {
        
    }

    TOrder& TShop::Order(ui64 orderID) {

    }
}