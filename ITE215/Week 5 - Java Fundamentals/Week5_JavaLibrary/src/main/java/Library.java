import java.io.IOException;

///
///  The library class manages the Books and Members
///
public class Library
{
    private static MemberCollection _members = MemberCollection.LoadFromDisk();
    private static BookCollection _books = BookCollection.LoadFromDisk();
    private static CheckoutHistory _history = CheckoutHistory.LoadFromDisk();

    public static MemberCollection Members() { return _members; }
    public static BookCollection Books() { return _books; }
    public static CheckoutHistory History() { return _history; }

    public static void SaveLibraryData()
    {
        try {
            _books.SaveToDisk();
            _members.SaveToDisk();
            _history.SaveToDisk();
        } catch (IOException e) {
            /**/
        }
    }

    public static void LoadLibraryData()
    {
        _books = BookCollection.LoadFromDisk();
        _members = MemberCollection.LoadFromDisk();
        _history = CheckoutHistory.LoadFromDisk();
    }
}
