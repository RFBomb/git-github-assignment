///
public record Member(int MemberId, String FirstName, String LastName)
{
    public org.json.JSONObject toJSON()
    {
        var json = new org.json.JSONObject();
        json.put("MemberId", MemberId);
        json.put("FirstName", FirstName);
        json.put("LastName", LastName);
        return json;
    }

    public static Member fromJSON(org.json.JSONObject json)
    {
        var MemberId = json.getInt("MemberId");
        var FirstName = json.getString("FirstName");
        var LastName = json.getString("LastName");
        return new Member(MemberId, FirstName, LastName);
    }
}
