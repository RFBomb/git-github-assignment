import java.time.Instant;

import static java.lang.String.format;

/// Ties an ISBN to an inventory ID
public class Book
{
    ///  constructor for new books
    public Book(int id, String isbn)
    {
        Id = id;
        ISBN = isbn;
        _memberID = -1;
    }

    ///  constructor for JSON
    private Book(int id, String isbn, int memberID, java.util.Date checkoutDate)
    {
        Id = id;
        ISBN = isbn;
        _memberID = memberID;
    }

    /// The memberID that has the book checked out
    private int _memberID;
    private java.util.Date _checkoutDate;

    /// The Inventory ID of the book
    public final int Id;

    /// The ISBN of the book
    public final String ISBN;

    ///  Returns true if the book was checked out by a member, otherwise false
    public boolean IsCheckedOut() { return _memberID >= 0;}

    // returns the checkout date as a string
    public String CheckoutDate()
    {
        if  (IsCheckedOut() && _checkoutDate != null)
        {
            return _checkoutDate.toString();
        }
        return "Not checked out";
    }

    /// Get the member id of the person that checked the book out
    public int MemberID() { return _memberID; }

    /// Checks out the book
    public void Checkout(int memberID) throws  LibraryException
    {
        if (this._memberID >= 0)
        {
            throw new LibraryException("Cannot check out a book that is already checked out");
        }
        if (memberID >= 0) {
            this._memberID = memberID;
            this._checkoutDate = java.util.Date.from(Instant.now());
        }
    }
    /// Returns this book (clearing the IsCheckedOut flag and the MemberID)
    public CheckoutRecord Return() throws LibraryException
    {
        if (this._memberID <= 0 || this._checkoutDate == null)
        {
            throw new LibraryException("Cannot return a book that was never checked out.");
        }
        var rec = new CheckoutRecord(_memberID, Id, _checkoutDate, java.util.Date.from(Instant.now()));
        this._memberID = -1;
        this._checkoutDate = null;
        return rec;
    }


    public org.json.JSONObject toJSON()
    {
        org.json.JSONObject jSONObject = new org.json.JSONObject();
        jSONObject.put("id", Id);
        jSONObject.put("isbn", ISBN);
        jSONObject.put("memberID", MemberID());
        if (_checkoutDate != null)
            jSONObject.put("checkoutDate", _checkoutDate.toInstant().toEpochMilli());
        else
            jSONObject.put("checkoutDate", -1);
        return jSONObject;
    }

    public static Book fromJSON(org.json.JSONObject obj)
    {
        var id = obj.getInt("id");
        var isbn = obj.getString("isbn");
        var memberID = obj.getInt("memberID");
        long  checkoutDate = obj.getLong("checkoutDate");
        java.util.Date cDate;
        if (checkoutDate >= 0)
        {
            cDate = java.util.Date.from(Instant.ofEpochMilli(checkoutDate));
        }
        else {
            cDate = null;
        }
        return new Book(id, isbn, memberID, cDate);
    }

    public String toString()
    {
        return format("id: %d, ISBN: %s, checkedOutBy: %d, checkoutDate: %s:", Id, ISBN, _memberID, _checkoutDate);
    }
}
