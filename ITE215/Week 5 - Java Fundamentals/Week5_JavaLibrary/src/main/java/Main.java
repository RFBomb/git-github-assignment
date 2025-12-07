import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Main
{
    private final static Scanner scanner = new Scanner(System.in);


    //TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
    void main()
    {
        int choice = -1;

        while (choice != 0) {
            printMenu();
            System.out.print("Enter your choice: ");
            try {
                choice = Integer.parseInt(GetNextLine());
            } catch (Exception e) {
                choice = -1;
            }

            switch (choice) {
                case 1:
                    addBookMenu();
                    break;
                case 2:
                    registerMemberMenu();
                    break;
                case 3:
                    issueBookMenu();
                    break;
                case 4:
                    returnBookMenu();
                    break;
                case 5:
                    searchBooksMenu();
                    break;
                case 6:
                    listIssuedBooksMenu();
                    break;
                case 7:
                    saveLibraryMenu();
                    break;
                case 8:
                    loadLibraryMenu();
                    break;
                case 9:
                    System.out.println("Printing Inventory");
                    for(var item : Library.Books().ListInventory())
                    {
                        System.out.println(item.toString());
                    }
                    break;
                case 0:
                    System.out.println("Exiting Library System...");
                    break;
                default:
                    System.out.println("Invalid choice. Please try again.\n");
            }
        }
    }

    private void printMenu()
    {
        System.out.println("\n===== LIBRARY MENU =====");
        System.out.println("1. Add New Book");
        System.out.println("2. Register New Member");
        System.out.println("3. Issue Book");
        System.out.println("4. Return Book");
        System.out.println("5. Search Books");
        System.out.println("6. List All Issued Books");
        System.out.println("7. Save Library Data");
        System.out.println("8. Load Library Data");
        System.out.println("0. Exit");
    }


    private static String GetNextLine()
    {
        while (true) {
            String value = scanner.nextLine();
            if (value.isBlank())
                continue;
            return value;
        }
    }

    private static int GetInteger(String message, int min, int max)
    {
        while (true) {
            System.out.print(message);
            String value =GetNextLine();
            try {
                var i =  Integer.parseInt(value);
                if (i >= min && i <= max) {
                    return i;
                }
                System.out.print("Invalid input - Expected a number between " + min + " - " + max + ". Please try again.\n");
            } catch (Exception e) {
                // try again
            }
        }
    }



// ---------------- MENU HANDLERS ------------------

    private void addBookMenu()     {
        System.out.println("\n== Add New Book ==");
        System.out.print("Enter title: ");
        String title = GetNextLine();

        System.out.print("Enter author: ");
        String author = GetNextLine();

        System.out.print("Enter ISBN: ");
        String isbn = GetNextLine();

        System.out.print("Enter the quantity: ");
        int qty = scanner.nextInt();

        try {
            Library.Books().AddBook(author,title, isbn, qty);
            System.out.println("Book added.\n");
        }catch (Exception e){ /* do nothing */ }
    }

    private void registerMemberMenu()
    {
        System.out.println("\n== Register New Member ==");
        System.out.print("Enter member first name: ");
        String fname = GetNextLine();

        System.out.print("Enter member last name: ");
        String lname = GetNextLine();

        Library.Members().AddMember(fname, lname);

        System.out.println("Member registered.\n");
    }

    private void issueBookMenu()
    {
        System.out.println("\n== Issue Book ==");
        var memberId = GetInteger("Enter Member ID: ", 0, Library.Members().Count() - 1);

        var bookId = GetInteger("Enter book's Inventory ID: ", 0, Library.Books().Count() - 1);
        var book = Library.Books().GetInventoryIdForCheckout(bookId);
        try {
            book.Checkout(memberId);
            Library.Books().SaveToDisk();
            System.out.println("Book issued.\n");
        } catch (LibraryException e) {
            System.out.println(e.getMessage());
        } catch (IOException io) {
            /*ignore*/
        }
    }

    private void returnBookMenu()
    {
        System.out.println("\n== Return Book ==");
        var bookId = GetInteger("Enter book's Inventory ID: ", 0, Library.Books().Count()-1);
        var book = Library.Books().GetInventoryIdForCheckout(bookId);
        try{
            Library.History().AddToLog(book.Return());
            System.out.println("Book returned.\n");
            Library.Books().SaveToDisk();
            Library.History().SaveToDisk();
        }
        catch(LibraryException e){ System.out.println(e.getMessage()); }
        catch (IOException e){/**/}
    }

    private void searchBooksMenu()
    {
        System.out.println("\n== Search Books ==");
        System.out.println("1. Search by Title");
        System.out.println("2. Search by Author");
        System.out.println("3. Search by ISBN");

        int option = GetInteger("Enter choice: ", 0, 3);
        List<BookDetails> books = null;
        switch (option) {
            case 1:
                System.out.print("Enter title: ");
                books = Library.Books().SearchTitle(GetNextLine());
                break;
            case 2:
                System.out.print("Enter author: ");
                books = Library.Books().SearchAuthor(GetNextLine());
                break;
            case 3:
                System.out.print("Enter ISBN: ");
                var result = Library.Books().SearchISBN(GetNextLine());
                if (result != null)
                {
                    books = new ArrayList<>();
                    books.add(result);
                }
                break;
            default:
                System.out.println("Invalid option.");
        }

        if (books != null && !books.isEmpty()) {
            for (var item : books) {
                System.out.printf("\"%s\" - %s - [%s]%n", item.Title(), item.Author(), item.ISBN());
            }
        }
        else
        {
            System.out.println("No matches found.");
        }

        System.out.println();
    }

    private void listIssuedBooksMenu()
    {
        System.out.println("\n== Books Currently Issued ==");
        System.out.println("[MemberID] - [InventoryID] - [CheckoutDate] - [ISBN]");
        for (var item : Library.Books().ListIssuedBooks())
        {
            System.out.printf("[%d] - [%d] - [%s] - [%s]\n", item.MemberID(), item.Id, item.CheckoutDate(), item.ISBN);
        }

    }

    private void saveLibraryMenu()
    {
        System.out.println("\n== Save Library Data ==");
        try {
            Library.SaveLibraryData();
            System.out.println("Library saved.\n");
        }catch (Exception e)
        {
            System.out.println("Library failed to save.\n");
        }
    }

    private void loadLibraryMenu()
    {
        System.out.println("\n== Load Library Data ==");
        Library.LoadLibraryData();
        System.out.println("Library loaded.\n");
    }

}