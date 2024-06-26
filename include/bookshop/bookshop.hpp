#pragma once

#include <sys/types.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace NBookshop {
    using ui64 = u_int64_t;
    static ui64 CartIDCounter{0};

    class TBook final {
        ui64 ID_;
        std::string Name_;
        std::string Author_;
        int CreationYear_;
        double Price_;
        std::string Publishing_;
        std::string Genre_;
        size_t ExemplarsInStock_;
    public:
        TBook() = default;
        TBook(
            const std::string& name,
            const std::string& author,
            int creationYear,
            double price,
            const std::string& publishing,
            const std::string& genre,
            size_t exemplarsInStock
        );
        const std::string& Name() const;
        const std::string& Author() const;
        int CreationYear() const;
        double Price() const;
        const std::string& Publishing() const;
        const std::string& Genre() const;
        size_t ExemplarsInStock() const;
        bool ChangeExemplarsInStock(int delta);
        ui64 ID() const;
    };

    class TCart final {
        ui64 ID_{};
        std::vector<ui64> Books_;
    public:
        explicit TCart(ui64 cartId = CartIDCounter++);
        bool AddBook(const TBook& book);
        const std::vector<ui64>& BooksInCart() const;
        void Clear();
    };

    class TOrder final {
    public:
        enum class EStatus {
            NOT_CREATED,
            PROCESSING,
            DELIVERING,
            DONE,
            REFUNDED
        };
        enum class ERefundOptions {
            COURIER,
            SELF,
            NOT_DELIVERED,
            NO_REFUND
        };
        TOrder() = default;
        explicit TOrder(ui64 ID, const std::vector<std::pair<ui64, size_t>>& books);    //  { bookID : count}
        EStatus GetStatus() const;
        bool ChangeStatus(EStatus newStatus);
        bool ChangeRefundOption(ERefundOptions refOpt);
        const std::vector<std::pair<ui64, size_t>>& Books() const;
        ui64 ID() const;
    private:
        EStatus Status_{EStatus::NOT_CREATED};
        ERefundOptions RefundOptions_{ERefundOptions::NO_REFUND};
        std::vector<std::pair<ui64, size_t>> Books_;
        ui64 ID_{};
    };

    class TShop final {
        std::unordered_map<ui64, TBook> BooksInStock_;
        std::unordered_map<ui64, TOrder> Orders_;
    public:
        TShop() = default;
        bool AddBook(const TBook& book);
        bool MakeOrder(const TOrder& newOrder);
        bool DeliverOrder(ui64 orderID);
        bool RefundBook(const std::pair<ui64, size_t>& bookInfo);
        bool RefundOrder(ui64 orderID, TOrder::ERefundOptions refOpt);
        bool HasBook(ui64 bookID) const;
        TBook& BookInfo(ui64 bookID);
        bool HasOrder(ui64 orderID) const;
        TOrder& Order(ui64 orderID);
    };

    class TConsumer final {
    private:
        ui64 ID_{};
        TCart Cart_;
        std::vector<ui64> Orders_;
        TShop* Shop_;
    public:
        TConsumer(ui64 ID, TShop* shop);
        bool MakeCart(ui64 cartID);
        bool AddBook(const TBook& book);
        TOrder MakeOrder(ui64 orderID);
        TOrder::EStatus GetStatus(ui64 orderID);
        bool Refund(ui64 orderID, TOrder::ERefundOptions refOpt);
        ui64 ID() const;
        const std::vector<ui64>& BooksInCart() const;
        std::vector<ui64>& Orders();
    };

}