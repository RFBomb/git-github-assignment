/**
 * Provides details about a specific book
 */
public record BookDetails(String Author, String Title, String ISBN)
{
    public org.json.JSONObject toJSON()
    {
        var obj = new org.json.JSONObject();
        obj.put("Author", Author);
        obj.put("Title", Title);
        obj.put("ISBN", ISBN);
        return obj;
    }

    public static BookDetails fromJSON(org.json.JSONObject obj)
    {
        return new BookDetails(obj.getString("Author"), obj.getString("Title"), obj.getString("ISBN"));
    }
}