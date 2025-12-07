import org.json.JSONException;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class MemberCollection
{
    private final List<Member> members; // the collection of members

    public MemberCollection()
    {
        members = new ArrayList<>();
    }

    public Iterator<Member> iterator()
    {
        return members.iterator();
    }

    public void AddMember(String firstName, String lastName)
    {
        var member = new Member(members.size(), firstName, lastName);
        members.add(member);
        try
        {
            SaveToDisk();
        } catch (Exception e) {
            /**/
        }
    }

    public int Count()
    {
        return members.size();
    }

    private static final String DiskPath = ".\\Members.json";

    public void SaveToDisk()
    {
        try {
            final var json = new org.json.JSONObject();
            var array = new org.json.JSONArray();

            // Write members to disk
            for (Member member : members) {
                array.put(member.toJSON());
            }
            json.put("Members", array);

            Files.writeString(Path.of(DiskPath), json.toString(4));
        } catch (Exception e) {
            /**/
        }
    }

    public static MemberCollection LoadFromDisk()
    {
        var members = new MemberCollection();

        try {
            String content = Files.readString(Path.of(DiskPath));
            var obj = new org.json.JSONObject(content);

            // Members
            try {
                var details = obj.getJSONArray("Members");
                for (int i = 0; i < details.length(); i++) {
                    var item = details.getJSONObject(i);
                    var member = Member.fromJSON(item);
                    members.members.add(member);
                }
            } catch (JSONException j) {
                // ignore
            }
        } catch (IOException e) {
            return members;
        }
        return members;
    }
}
