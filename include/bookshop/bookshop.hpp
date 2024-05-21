#pragma once

#include <sys/types.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace NBookshop {
    using ui64 = u_int64_t;
    static ui64 BookIDCounter{0};
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
        size_t& ExemplarsInStock();
        ui64 ID() const;
    };

    class TCart final {
        ui64 ID_{};
        std::vector<ui64> Books_;
    public:
        explicit TCart(ui64 cartId = CartIDCounter++);
        bool AddBook(const TBook& book);
        const std::vector<ui64>& BooksInCart() const;
    };

    class TOrder final {
    public:
        enum class TStatus {
            NOT_CREATED,
            PROCESSING,
            DELIVERING,
            DONE,
            REFUNDED
        };
        TOrder() = default;
        explicit TOrder(ui64 ID, const std::vector<std::pair<ui64, size_t>>& books);    //  { bookID : count}
        TStatus GetStatus() const;
        bool ChangeStatus(TStatus newStatus);
        const std::vector<std::pair<ui64, size_t>>& Books() const;
        ui64 ID() const;
    private:
        TStatus Status_{TStatus::NOT_CREATED};
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
        bool RefundBook(ui64 bookID);
        TBook& BookInfo(ui64 bookID);
        TOrder& Order(ui64 orderID);
    };

    class TConsumer {
    public:
        enum class RefundOptions {
            COURIER,
            SELF,
            NOT_DELIVERED
        };
    private:
        ui64 ID_{};
        TCart Cart_;
        std::vector<ui64> Orders_;
    public:
        TConsumer() = default;
        virtual ~TConsumer() = default;
        explicit TConsumer(ui64 ID);
        bool MakeCart();
        bool AddBook(const TBook& book);
        TOrder MakeOrder(ui64 orderID);
        TOrder::TStatus GetStatus(ui64 orderID);
        bool Refund(ui64 orderID, RefundOptions refOpt);
        ui64 ID() const;
        const std::vector<ui64>& BooksInCart() const;
        std::vector<ui64>& Orders();
    };

}