import org.json.JSONObject;

import java.util.Date;

public record CheckoutRecord(int MemberId, int BookId, Date CheckoutDate, Date CheckInDate)
{

    public JSONObject toJSON()
    {
        JSONObject obj = new JSONObject();
        obj.put("MemberId", MemberId);
        obj.put("BookId", BookId);
        obj.put("CheckoutDate", CheckoutDate.toInstant().toEpochMilli());
        obj.put("CheckInDate", CheckInDate.toInstant().toEpochMilli());
        return obj;
    }

    public static CheckoutRecord fromJSON(JSONObject obj)
    {
        int memberId = obj.getInt("MemberId");
        int bookId = obj.getInt("BookId");
        long checkout = obj.getLong("CheckoutDate");
        long checkin = obj.getLong("CheckInDate");
        var checkoutDate = new Date(checkout);
        var checkinDate = new Date(checkin);
        return new CheckoutRecord(memberId, bookId, checkoutDate, checkinDate);
    }

}
