using System;
using System.Data;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;

namespace ECommerceData
{
    internal class Program
    {
        private static ECommerceSystem data = ECommerceSystem.LoadFromDisk();
        private static Customer? Customer;
        private static Cart? Cart;

        private static string ReadLine()
        {
            string? input;
            while (string.IsNullOrWhiteSpace(input = Console.ReadLine())) { }
            return input;
        }

        private static int GetInt(int min, int max)
        {
            int value;
            while (true)
            {
                while (!int.TryParse(ReadLine(), out value)) { }
                if (value < min || value > max) { Console.WriteLine($"Invalid input, expected between {min} - {max}"); continue; } 
                return value;
            }
        }

        static void Main(string[] args)
        {
            while (true)
            {
                Console.Clear();
                Console.WriteLine("======================================");
                Console.WriteLine("         E-Commerce System Menu       ");
                Console.WriteLine("======================================");
                if (Customer is not null)
                {
                    Console.WriteLine($" Customer : {Customer.EmailAddress}");
                    Console.WriteLine("======================================");
                }    
                Console.WriteLine("1. Add New Product");
                Console.WriteLine("2. Lookup Customer");
                Console.WriteLine("3. Register New Customer");
                Console.WriteLine("4. Search Products");
                Console.WriteLine("5. Place Order");
                Console.WriteLine("6. Generate Invoice");
                Console.WriteLine("7. List Customer Orders");
                Console.WriteLine("8. Save System Data");
                Console.WriteLine("9. Load System Data");
                Console.WriteLine("0. Exit");
                Console.WriteLine("======================================");
                Console.Write("Select an option: ");

                string choice = ReadLine();
                HandleMainMenuChoice(choice);
            }
        }

        private static void HandleMainMenuChoice(string? choice)
        {
            switch (choice)
            {
                case "1":
                    Menu_AddProduct();
                    data.SaveToDisk();
                    break;

                case "2":
                case "3":
                    Menu_GetCustomer(choice == "3", true);
                    data.SaveToDisk();
                    break;

                case "4":
                    Menu_SearchProducts();
                    data.SaveToDisk();
                    break;

                case "5":
                    Menu_PlaceOrder();
                    data.SaveToDisk();
                    break;

                case "6":
                    Menu_GenerateInvoice();
                    break;
                    
                case "7":
                    Menu_ListCustomerOrders();
                    break;

                case "8":
                    Menu_SaveData();
                    break;

                case "9":
                    Menu_LoadData();
                    break;

                case "0":
                    data.SaveToDisk();
                    Environment.Exit(0);
                    break;

                default:
                    Console.WriteLine("Invalid input. Press Enter to continue...");
                    Console.ReadLine();
                    break;
            }
        }

        // ------------------ Search Submenu ------------------
        
        private static void Menu_AddProduct()
        {
            Console.Clear();
            Console.WriteLine("[Add New Product to database]");
            
            Console.WriteLine("Enter the Product Description");
            var descrip = ReadLine();

            Console.WriteLine("Enter the Product Category");
            var category = ReadLine();

            
            while(true)
            {
                Console.WriteLine("Enter the Product Price");
                var sPrice = ReadLine();
                if (Double.TryParse(sPrice, out var price))
                {
                    data.AddProduct(descrip, category, price);
                    break;
                }
                Console.WriteLine("Invalid Price");
            }               
        }

        private static Customer GetCustomer()
        {
            while (Customer is null)
            {
                Menu_GetCustomer(false, false);
            }
            return Customer;
        }
        private static Cart GetCart()
        {
            while (Cart is null)
            {
                Cart = data.GetCustomerCart(Customer?.Id ?? -1);
            }
            return Cart;
        }

        private static void Menu_GetCustomer(bool registerNew, bool getCart)
        {
            Console.Clear();
            if (registerNew) { Console.WriteLine("[Register New Customer]"); }
            else { Console.WriteLine("[Lookup Customer]"); }

            Customer = null;
            if (getCart) Cart = null;
            while (true)
            {
                Console.WriteLine("Enter email address");
                string input = ReadLine();
                if (Regex.IsMatch(input, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"))
                {
                    if (!registerNew)
                    {
                        Customer = data.LoginCustomer(input);
                        if (Customer == null)
                        {
                            Console.WriteLine("Customer not found. Register them? (y/n) :");
                            switch(ReadLine())
                            {
                                case "y":
                                case "Y":
                                    Customer = data.RegisterCustomer(input);
                                    break;
                                case "n":
                                case "N":
                                if (getCart) Cart = data.GetCustomerCart();
                                    return;
                                default:
                                    Console.WriteLine("Invalid response, customer not registered.");
                                    Console.ReadLine();
                                    break;
                            }
                        }
                    }
                    else
                    {
                        try
                        {
                            Customer = data.RegisterCustomer(input);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine($"Error: {e.Message}");
                            Console.ReadLine();
                        }
                    }
                    if (getCart && Customer is not null) Cart = data.GetCustomerCart(Customer);
                    return;
                }
                else
                {
                    Console.WriteLine("Invalid Email Address.");
                    Console.ReadLine();
                }
            }
        }

        private static void Menu_ListCustomerOrders()
        {
            Console.Clear();
            Console.WriteLine("[List All Orders For Customer]");
            var cust = GetCustomer();
            var orders = data.GetCustomerOrders(cust).ToArray();
            bool any = false;
            int index = 1;
            foreach(var item in orders)
            {
                if (!any)
                {
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine($"{"#", -5} | {"ORDER DATE", -23} | ORDER ID");
                    Console.WriteLine($"---------------------------------------------------------------------------");
                    Console.ForegroundColor = ConsoleColor.White;
                }
                Console.WriteLine($"{index, -5} | {item.OrderDate, -23} | {item.OrderId}");
                any = true;
                index++;
            }

            if (any)
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine($"---------------------------------------------------------------------------");
                Console.ForegroundColor = ConsoleColor.White;
                Console.WriteLine("\n\nSelect an order number (left column) to view it, or 0 to return to main menu: ");
                int order = GetInt(0, index);
                if (order == 0) return;
                data.PrintInvoice(orders[order - 1], cust);
            }
            else
            {
                Console.WriteLine("No Orders");
            }
            Console.ReadLine();
        }

        private static void Menu_SaveData()
        {
            Console.Clear();
            Console.WriteLine("[Save System Data]");
            data.SaveToDisk();
            Console.WriteLine("Data Saved Successfully");
            Console.ReadLine();
        }

        private static void Menu_LoadData()
        {
            Console.Clear();
            Console.WriteLine("[Load System Data]");
            data = ECommerceSystem.LoadFromDisk();
            Console.WriteLine("Data Loaded Successfully");
            Console.ReadLine();
        }

        private static void Menu_PlaceOrder()
        {
            Console.Clear();
            Console.WriteLine("[Place Order]");
            if (Cart is null || Cart.LineItems.Any() == false)
            {
                Console.WriteLine("No items in cart");
            }
            else
            {
                data.PrintInvoice(Cart, Customer);
                bool place = false;
                while (!place)
                {
                    Console.WriteLine();
                    Console.WriteLine("Place Order? (y/n) :");
                    switch (ReadLine())
                    {
                        case "y":
                        case "Y":
                            place = true;
                            break;
                        case "n":
                        case "N":
                            return;
                        default:
                            Console.WriteLine("Invalid response");
                            break;
                    }
                }

                var cust = GetCustomer();
                if (cust is not null)
                {
                    Cart = data.PlaceOrder(Cart, cust.Id);
                    Console.WriteLine("Order placed. Press 'Enter' to return.");
                }
                else
                {
                    Console.WriteLine("Order not placed. Press 'Enter' to return.");
                }
            }
            Console.ReadLine();
        }

        private static void Menu_GenerateInvoice()
        {
            Console.Clear();
            Console.WriteLine("[Generate Invoice]");
            if (Cart is null || Cart.LineItems.Any() == false)
            {
                Console.WriteLine("No items in cart");
                Console.WriteLine();
            }
            else
            {
                data.PrintInvoice(Cart, Customer);
            }
            Console.WriteLine("Press Enter to return...");
            Console.ReadLine();
        }

        private static void Menu_SearchProducts()
        {
            while (true)
            {
                Console.Clear();
                Console.WriteLine("====== Product Search ======");
                Console.WriteLine("1. Display All");
                Console.WriteLine("2. Search by Name");
                Console.WriteLine("3. Search by Category");
                Console.WriteLine("4. Back to Main Menu");
                Console.Write("Select an option: ");

                string? choice = Console.ReadLine();

                switch (choice)
                {
                    case "1":
                        PrintSearchResults(data.GetProducts().ToArray());
                        break;
                    case "2":
                        Menu_SearchByName();
                        break;

                    case "3":
                        Menu_SearchByCategory();
                        break;

                    case "4":
                        return;

                    default:
                        Console.WriteLine("Invalid input. Press Enter to continue...");
                        Console.ReadLine();
                        break;
                }
            }
        }

        private static void Menu_SearchByName()
        {
            Console.Clear();
            Console.WriteLine("[Search Products By Name]");
            var search = Console.ReadLine();
            var results = data.GetProducts().Where(p => p.Description.Contains(search, StringComparison.InvariantCultureIgnoreCase)).ToArray();
            PrintSearchResults(results);
            Console.ReadLine();
        }

        private static void Menu_SearchByCategory()
        {
            Console.Clear();
            Console.WriteLine("[Search Products By Category]");
            var search = Console.ReadLine();
            var results = data.GetProducts().Where(p => p.Category.Contains(search, StringComparison.InvariantCultureIgnoreCase)).ToArray();
            PrintSearchResults(results);
            Console.ReadLine();
        }

        private static void PrintSearchResults(IEnumerable<Product> products)
        {
            bool any = false;
            Console.WriteLine();
            Console.Write("Results : ");
            int index = 0;
            foreach (var p in products)
            {
                if (!any) Console.WriteLine();
                Console.WriteLine($"ID = {p.Id} -- ${p.Price} -- {p.Description} [{p.Category}]");
                any = true;
            }

            if (any)
            {
                Console.WriteLine("Enter a product ID to add it to your cart (-1 to return): ");
                int order = GetInt(-1, products.Max(n => n.Id));
                if (order == -1) return;
                var item = products.FirstOrDefault(n => n.Id == order);
                if (item is null)
                {
                    Console.WriteLine("\nProduct ID not found. Press 'Enter' to continue.");
                    Console.ReadLine();
                    return;
                }
                else
                {
                    GetCart().UpdateCart(item, 1);
                    Console.WriteLine("\nProduct added to cart. Press 'Enter' to continue.");
                    Console.ReadLine();
                }
                if (order == 0) return;
            }
            else
            {
                Console.WriteLine("No Matches Found");
            }
        }
    }
}
