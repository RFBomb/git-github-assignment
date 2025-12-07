using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace ECommerceData
{
    /// <summary>
    /// The ECommerse Management System
    /// </summary>
    public partial class ECommerceSystem
    {
        [JsonSourceGenerationOptions(AllowTrailingCommas =true, WriteIndented = true)]
        [JsonSerializable(typeof(ECommerceSystem))]
        public partial class SerializerContext : JsonSerializerContext { }

        [JsonInclude] private HashSet<Product> Products { get; init; } = [];
        [JsonInclude] private Dictionary<string, Customer> Customers { get; init; } = [];
        [JsonInclude] private List<Order> OrderHistory { get; init; } = [];         // placed orders
        [JsonInclude] private Dictionary<int, Cart> Carts { get; init; } = [];     // active carts, where key = customerId

        #region < Product Management >

        public void AddProduct(string? descrip, string? category, double price)
        {
            Products.Add(new Product(Products.Count, descrip!, category!, price));
        }

        public IEnumerable<Product> GetProducts() => Products;

        #endregion

        #region < Customers >

        public Customer RegisterCustomer(string emailAddress)
        {
            // pretend we also do email validation here
            if (Customers.TryGetValue(emailAddress.ToLowerInvariant(), out _))
                throw new ArgumentException("Email Address already Taken");
            var cust = new Customer(Customers.Count + 1, emailAddress);
            Customers[cust.EmailAddress.ToLowerInvariant()] = cust;
            return cust;
        }
        public Customer? LoginCustomer(string emailAddress)
        {
            Customers.TryGetValue(emailAddress.ToLowerInvariant(), out var result);
            return result;
        }

        public Cart GetCustomerCart(Customer customer) => GetCustomerCart(customer.Id);
        public Cart GetCustomerCart(int customerId = -1)
        {
            if (Carts.TryGetValue(customerId, out var cart)) return cart;
            cart = new Cart(Guid.NewGuid()) { CustomerId = customerId };
            if (customerId >= 0) Carts[customerId] = cart;
            return cart;
        }
        
        public IEnumerable<Order> GetCustomerOrders(Customer customer) => GetCustomerOrders(customer.Id);
        public IEnumerable<Order> GetCustomerOrders(int customerId) => OrderHistory.Where(o => o.CustomerId == customerId);

        public Cart PlaceOrder(Cart cart, int customerId)
        {
            // Validation
            ArgumentNullException.ThrowIfNull(cart);
            ArgumentOutOfRangeException.ThrowIfLessThan(customerId, 0, nameof(customerId));
            cart.CustomerId = customerId;

            // generate a new cart for the dictionary to avoid adding items to this cart
            var newCart = new Cart() { CustomerId = customerId };
            Carts[customerId] = newCart;

            // convert the cart to an order history
            OrderHistory.Add(new Order(cart));

            // return the new (empty) cart
            return newCart; 
        }

        #endregion

        /// <summary>
        /// Prints invoices to text files
        /// </summary>
        public void PrintInvoice(IOrder order, Customer? customer)
        {
            if (order == null) return;
            //Directory.CreateDirectory(".\\Invoices\\");
            //var fp = Path.Combine(".\\Invoices\\", order.OrderId.ToString());
            //using var stream = File.OpenWrite(fp);
            //using var writer = new StreamWriter(stream, UTF8Encoding.UTF8);
            

            customer ??= Customers.Where(c => c.Value.Id == order.CustomerId).Select(s => s.Value).FirstOrDefault();
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine($"-------------------------------------------------");
            Console.WriteLine($"Customer ID: {customer?.EmailAddress}");
            if (order is Order o)
            {
                Console.WriteLine($"Order ID   : {order.OrderId}");
                Console.WriteLine($"Order Date : {o.OrderDate}");
            }
            else // is a cart
            {
                Console.WriteLine($"Cart ID : {order.OrderId}");
                Console.WriteLine($"Date    : {DateTime.UtcNow}");
            }
            Console.WriteLine($"-------------------------------------------------");
            Console.WriteLine();
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"{"Description",-20}\t{"Qty", 10}\tCost");
            Console.ForegroundColor = ConsoleColor.White;
            double total = 0;
            foreach (var item in order.LineItems)
            {
                double subTotal = item.GetSubTotal();
                total += subTotal;
                Console.WriteLine($"{item.Product.Description,-20}\t{item.Quantity, 10}\t{subTotal:f2}");
            }
            Console.WriteLine();
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine($"-------------------------------------------------");
            Console.WriteLine($"\t\t\t Subtotal : ${total:f2}");
            Console.WriteLine($"-------------------------------------------------");
            Console.ForegroundColor = ConsoleColor.White;
            //writer.Flush();
            //writer.Close();
            //writer.Dispose();
            //stream.Dispose();
        }

        

        private const string fp = ".\\ECommerceData.json";

        public void SaveToDisk()
        {
            Directory.CreateDirectory(Path.GetDirectoryName(Path.GetFullPath(fp))!);
            using var outStream = File.OpenWrite(fp);
            JsonSerializer.Serialize(outStream, this, SerializerContext.Default.ECommerceSystem);
        }

        public static ECommerceSystem LoadFromDisk()
        {
            if (File.Exists(fp))
            {
                using var reader = File.OpenRead(fp);
                return JsonSerializer.Deserialize(reader, SerializerContext.Default.ECommerceSystem) ?? new();
            }
            else
            {
                return new ECommerceSystem();
            }
        }

    }
}
