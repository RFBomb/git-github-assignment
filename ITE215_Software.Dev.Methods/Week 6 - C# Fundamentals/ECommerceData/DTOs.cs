using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace ECommerceData
{
    public record Customer(int Id, string EmailAddress) { }

    public record Product(int Id, string Description, string Category, double Price) { }

    /// <summary>
    /// Represents a line item on an order
    /// </summary>
    public record LineItem(Product Product, int Quantity) { public double GetSubTotal() => Product.Price * Quantity; }

    public interface IOrder
    {
        Guid OrderId { get; }
        int CustomerId { get; }
        IEnumerable<LineItem> LineItems { get; }
        double GetSubTotal() => LineItems.Select(p => p.Product.Price * p.Quantity).Sum();
    }

    /// <summary>
    /// Represents a Cart that the user can populate with items
    /// </summary>
    /// <param name="CartId"></param>
    public record Cart(Guid CartId) : IOrder
    {
        private readonly Dictionary<Product, int> cart = [];
        private int customerId = -1;

        internal Cart() : this(Guid.NewGuid()) { }

        [JsonConstructor]
        internal Cart(Guid CartId, int CustomerId, IEnumerable<LineItem> lineItems) : this(CartId)
        {
            customerId = CustomerId;
            foreach (var item in lineItems)
                cart[item.Product] = item.Quantity;
        }


        // allow setting the customerID, but not changing it
        public int CustomerId
        {
            get => customerId;
            set
            {
                if (customerId < 0 && value >= 0)
                    customerId = value;
                else if (value >= 0 && customerId != value)
                    throw new InvalidOperationException("Cannot change the owner of a cart");
            }
        }

        Guid IOrder.OrderId => this.CartId;

        public IEnumerable<LineItem> LineItems => cart.Select(kvp => new LineItem(kvp.Key, kvp.Value));

        public double GetSubTotal() => cart.Select(p => p.Key.Price * p.Value).Sum();

        public void UpdateCart(Product product, int quantityToAdd)
        {
            bool found = cart.TryGetValue(product, out int value);
            int newValue = value + quantityToAdd;
            if (newValue > 0)
                cart[product] = newValue;
            else if (found)
                cart.Remove(product);
        }

        public void RemoveFromCart(Product product) => cart.Remove(product);
        public void SetQuantity(Product product, int quantity) => cart[product] = quantity;
    }

    /// <summary>
    /// Represents an order that has been placed
    /// </summary>
    /// <param name="OrderId"></param>
    /// <param name="CustomerId"></param>
    public record Order(Guid OrderId, int CustomerId, DateTime OrderDate, IEnumerable<LineItem> LineItems) : IOrder
    {
        internal Order(Cart cart) : this(cart.CartId, cart.CustomerId, DateTime.UtcNow, cart.LineItems) { }
    }
}
