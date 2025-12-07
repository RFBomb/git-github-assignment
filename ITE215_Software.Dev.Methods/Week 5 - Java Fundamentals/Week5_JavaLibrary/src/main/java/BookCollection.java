import org.json.JSONException;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class BookCollection
{
    private final List<Book> Inventory; // collection of all inventoried books
    private final HashMap<String, BookDetails> books; // Table of book details
    private final HashMap<String, List<Book>> InventoryMap; // table of inventory, uses ISBN hash to quickly locate all books in inventory

    private int numberOfBooks;

    public BookCollection()
    {
        books = new HashMap<>();
        Inventory = new ArrayList<>();
        InventoryMap = new HashMap<>();
    }

    ///  Adds a book to the coll
    public void AddBook(String author, String title, String isbn, int quantity) throws IOException
    {
        books.putIfAbsent(isbn, new BookDetails(author, title, isbn));
        InventoryMap.putIfAbsent(isbn, new ArrayList<>());

        for (int i = 0; i < quantity; i++)
            AddToInventory(isbn);
        SaveToDisk();
    }

    private void AddToInventory(String ISBN)
    {
        var item = new Book(Inventory.size(), ISBN);
        Inventory.add(item);
        InventoryMap.get(ISBN).add(item);
    }

    ///  Add a book to the inventory by its ISBN.
    /// ISBN must already be defined in the system.
    public void AddBook(String ISBN)
    {
        if (books.containsKey(ISBN)) {
            AddToInventory(ISBN);
        } else {
            // throw
        }
        try{
        SaveToDisk();} catch (IOException e){/**/}
    }

    public int Count()
    {
        return Inventory.size();
    }

    /// Gets Book object by its ID #
    public Book GetInventoryIdForCheckout(int id)
    {
        return Inventory.get(id);
    }

    public List<Book> ListIssuedBooks()
    {
        return Inventory.stream()
                .filter(Book::IsCheckedOut)
                .toList();
    }

    public List<Book> ListInventory()
    {
        return Inventory.stream()
                .toList();
    }

    ///  Get the book details for a specified
    public BookDetails SearchISBN(String isbn)
    {
        return books.get(isbn);
    }

    ///  Returns an iterator to get all books matching a specific title
    /// yield return would be great here... but the language doesn't support it
    public List<BookDetails> SearchTitle(String title)
    {
        var uTitle = title.toUpperCase();
        return  books.values().stream()
                .filter(bookDetails -> bookDetails.Title().toUpperCase().contains(uTitle))
                .toList();
    }

    ///  Returns an iterator to get all books matching a specific author
    /// yield return would be great here...
    public List<BookDetails> SearchAuthor(String author){
        var uAuthor = author.toUpperCase();
        return  books.values().stream()
                .filter(bookDetails -> bookDetails.Author().toUpperCase().contains(uAuthor))
                .toList();
    }

    private static final String DiskPath = ".\\BookInventory.json";

    public void SaveToDisk() throws IOException
    {
        final var json = new org.json.JSONObject();
        var array = new org.json.JSONArray();

        // Write BookDetails to disk
        for  (BookDetails book : books.values())
        {
            array.put(book.toJSON());
        }
        json.put("ISBNs", array);

        // write Inventory Items to disk
        array = new org.json.JSONArray();
        for (Book book : Inventory)
        {
            array.put(book.toJSON());
        }
        json.put("Inventory", array);

        Files.writeString(Path.of(DiskPath), json.toString(4));
    }

    public static BookCollection LoadFromDisk()
    {
        BookCollection bookCollection = new BookCollection();
        try
        {
            String content = Files.readString(Path.of(DiskPath));
            var obj = new org.json.JSONObject(content);

            // ISBNS
            try {
                var details = obj.getJSONArray("ISBNs");
                for (int i = 0; i < details.length(); i++) {
                    var item = details.getJSONObject(i);
                    var bd = BookDetails.fromJSON (item);
                    bookCollection.books.put(bd.ISBN(), bd);
                    bookCollection.InventoryMap.put(bd.ISBN(), new ArrayList<>());
                }
            } catch (JSONException j) {
                // ignore
            }

            // Inventory
            try {
                var inv = obj.getJSONArray("Inventory");
                for (int i = 0; i < inv.length(); i++) {
                    var item = inv.getJSONObject(i);
                    var book = Book.fromJSON(item);
                    bookCollection.Inventory.add(book);
                    var coll = bookCollection.InventoryMap.get(book.ISBN);
                    if (coll == null) {
                        bookCollection.books.put(book.ISBN, new BookDetails("", "", book.ISBN));
                        coll = new ArrayList<>();
                        bookCollection.InventoryMap.put(book.ISBN, coll);
                    }
                    coll.add(book);
                }
            } catch (JSONException j) {
                // ignore
            }
        }
        catch (IOException e)
        {
            return bookCollection;
        }
        return bookCollection;
    }
}
